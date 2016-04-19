/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "xpd_instance.hpp"
#include "xpd_patch.hpp"

extern "C"
{
#include "../cpd/cpd.h"
}

namespace xpd
{
    class smuggler
    {
    public:
        ~smuggler() xpd_noexcept {}
    public:
        inline static xpd_constexpr void const* gettie(tie const& tie) xpd_noexcept {return tie.ptr;}
        inline static xpd_constexpr tie createtie(void *ptr) xpd_noexcept {return tie(ptr);}
        inline static xpd_constexpr void const* getsymbol(symbol const& symbol) xpd_noexcept {return symbol.ptr;}
        inline static xpd_constexpr symbol createsymbol(void *ptr) xpd_noexcept {return symbol(ptr);}
    };
    
    struct instance::internal : public smuggler
    {
    public:        
        c_instance object;
        instance*  ref;
        
        static internal* allocate(instance* _ref)
        {
            c_hook_console console;
            console.m_log   = (c_hook_print)instance::internal::m_log_hook;
            console.m_post  = (c_hook_print)instance::internal::m_post_hook;
            console.m_error = (c_hook_print)instance::internal::m_error_hook;
            console.m_fatal = (c_hook_print)instance::internal::m_fatal_hook;
            
            c_hook_midi midi;
            midi.m_noteon           = (c_hook_noteon)instance::internal::m_noteon;
            midi.m_controlchange    = (c_hook_controlchange)instance::internal::m_controlchange;
            midi.m_programchange    = (c_hook_programchange)instance::internal::m_programchange;
            midi.m_pitchbend        = (c_hook_pitchbend)instance::internal::m_pitchbend;
            midi.m_aftertouch       = (c_hook_aftertouch)instance::internal::m_aftertouch;
            midi.m_polyaftertouch   = (c_hook_polyaftertouch)instance::internal::m_polyaftertouch;
            midi.m_byte             = (c_hook_byte)instance::internal::m_byte;
            
            internal* ptr = (internal *)cpd_instance_new(sizeof(internal));
            if(ptr)
            {
                ptr->ref = _ref;
                cpd_instance_set_hook_console(reinterpret_cast<c_instance*>(ptr), &console);
                cpd_instance_set_hook_midi(reinterpret_cast<c_instance*>(ptr), &midi);
                
            }
            return ptr;
        }
        
        static void m_post_hook(instance::internal* instance, const char *s)
        {
            instance->ref->receive(console::post(console::normal, std::string(s)));
        }
        
        static void m_log_hook(instance::internal* instance, const char *s)
        {
            instance->ref->receive(console::post(console::log, std::string(s)));
        }
        
        static void m_error_hook(instance::internal* instance, const char *s)
        {
            instance->ref->receive(console::post(console::error, std::string(s)));
        }
        
        static void m_fatal_hook(instance::internal* instance, const char *s)
        {
            instance->ref->receive(console::post(console::fatal, std::string(s)));
        }
        

        
        
        static void m_noteon(instance::internal* instance, int port, int channel, int pitch, int velocity)
        {
            instance->ref->receive(midi::event::note(channel, pitch, velocity));
        }
        
        static void m_controlchange(instance::internal* instance, int port, int channel, int control, int value)
        {
            instance->ref->receive(midi::event::control_change(channel, control, value));
        }
        
        static void m_programchange(instance::internal* instance, int port, int channel, int value)
        {
            instance->ref->receive(midi::event::program_change(channel, value));
        }
        
        static void m_pitchbend(instance::internal* instance, int port, int channel, int value)
        {
            instance->ref->receive(midi::event::pitch_bend(channel, value));
        }
        
        static void m_aftertouch(instance::internal* instance, int port, int channel, int value)
        {
            instance->ref->receive(midi::event::after_touch(channel, value));
        }
        
        static void m_polyaftertouch(instance::internal* instance, int port, int channel, int pitch, int value)
        {
            instance->ref->receive(midi::event::poly_after_touch(channel, pitch, value));
        }
        
        static void m_byte(instance::internal* instance, int port, int value)
        {
            instance->ref->receive(midi::event::byte(value));
        }
        
        
        
        
        static void m_bang(instance::internal* instance, c_tie* tie)
        {
            instance->ref->receive(createtie(tie), instance::m_sym_bang, std::vector<atom>());
        }
        
        static void m_float(instance::internal* instance, c_tie* tie, c_float f)
        {
            instance->ref->receive(createtie(tie), instance::m_sym_float, std::vector<atom>(1, f));
        }
        
        static void m_symbol(instance::internal* instance, c_tie* tie, c_symbol* s)
        {
            symbol sy(createsymbol(s));
            instance->ref->receive(createtie(tie), instance::m_sym_symbol, std::vector<atom>(1, sy));
        }
        
        static void m_gpointer(instance::internal* instance, c_tie* tie, c_gpointer *g)
        {
            instance->ref->receive(createtie(tie), instance::m_sym_gpointer, std::vector<atom>());
        }
        
        static void m_list(instance::internal* instance, c_tie* tie, c_list *list)
        {
            std::vector<atom> vec(cpd_list_get_size(list));
            for(size_t i = 0; i < vec.size(); ++i)
            {
                if(cpd_list_get_type(list, i) == Z_FLOAT)
                {
                    vec[i] = cpd_list_get_float(list, i);
                }
                else if(cpd_list_get_type(list, i) == Z_FLOAT)
                {
                    vec[i] = createsymbol(cpd_list_get_symbol(list, i));
                }
            }
#ifdef _XPD_CPP11_NOSUPPORT_
            instance->ref->receive(createtie(tie), instance::m_sym_list, vec);
#else
            instance->ref->receive(createtie(tie), instance::m_sym_list, std::move(vec));
#endif
        }
        
        static void m_anything(instance::internal* instance, c_tie* tie, c_symbol *s, c_list *list)
        {
            std::vector<atom> vec(cpd_list_get_size(list));
            for(size_t i = 0; i < vec.size(); ++i)
            {
                if(cpd_list_get_type(list, i) == Z_FLOAT)
                {
                    vec[i] = cpd_list_get_float(list, i);
                }
                else if(cpd_list_get_type(list, i) == Z_FLOAT)
                {
                    vec[i] = createsymbol(cpd_list_get_symbol(list, i));
                }
            }
#ifdef _XPD_CPP11_NOSUPPORT_
            instance->ref->receive(createtie(tie), createsymbol(s), vec);
#else
            instance->ref->receive(createtie(tie), createsymbol(s), std::move(vec));
#endif
        }
    };
    
    // ==================================================================================== //
    //                                          INSTANCE                                    //
    // ==================================================================================== //
    
    symbol   instance::m_sym_bang;
    symbol   instance::m_sym_float;
    symbol   instance::m_sym_symbol;
    symbol   instance::m_sym_list;
    symbol   instance::m_sym_gpointer;
    
    instance::instance()
    {
        environment::lock();
        m_sym_bang      = symbol("bang");
        m_sym_float     = symbol("float");
        m_sym_symbol    = symbol("symbol");
        m_sym_list      = symbol("list");
        m_sym_gpointer  = symbol("gpointer");
        m_ptr = internal::allocate(this);
        environment::unlock();
        if(!m_ptr)
        {
            throw "can't allocate instance.";
        }
    }
    
    instance::~instance() xpd_noexcept
    {
        release();
        environment::lock();
        cpd_instance_free(reinterpret_cast<c_instance *>(m_ptr));
        environment::unlock();
    }
    
    
    patch* instance::load(std::string const& name, std::string const& path)
    {
        int todo;
        patch* p = xpd_nullptr;
        void* ptr = xpd_nullptr;
        environment::lock();
        cpd_instance_set(reinterpret_cast<c_instance *>(m_ptr));
        if(path.empty())
        {
            ptr = cpd_patch_new(name.c_str(), xpd_nullptr);
        }
        else
        {
            ptr = cpd_patch_new(name.c_str(), path.c_str());
        }
        
        if(ptr)
        {
            p = new patch(ptr, name, path, cpd_patch_get_dollarzero(reinterpret_cast<c_patch *>(m_ptr)));
        }
        environment::unlock();
        return p;
    }
    
    void instance::close(patch& p)
    {
        int todo;
        environment::lock();
        cpd_instance_set(reinterpret_cast<c_instance *>(m_ptr));
        cpd_patch_free(reinterpret_cast<c_patch *>(p.m_ptr));
        delete &p;
        environment::unlock();
    }
    
    
    int instance::samplerate() const xpd_noexcept
    {
        return cpd_instance_get_samplerate(reinterpret_cast<c_instance *>(m_ptr));
    }
    
    void instance::prepare(const int nins, const int nouts, const int samplerate, const int nsamples) xpd_noexcept
    {
        environment::lock();
        cpd_instance_dsp_prepare(reinterpret_cast<c_instance *>(m_ptr), nins, nouts, samplerate, nsamples);
        environment::unlock();
    }

    void instance::perform(int nsamples, const int nins, const float** inputs, const int nouts, float** outputs) xpd_noexcept
    {
        environment::lock();
        cpd_instance_dsp_perform(reinterpret_cast<c_instance *>(m_ptr), nsamples, nins, inputs, nouts, outputs);
        environment::unlock();
    }
    
    void instance::release() xpd_noexcept
    {
        environment::lock();
        cpd_instance_dsp_release(reinterpret_cast<c_instance *>(m_ptr));
        environment::unlock();
    }
    
    
    
    void instance::send(console::post const& post) xpd_noexcept
    {
        int todo_set_instance;
        environment::lock();
        cpd_instance_set(reinterpret_cast<c_instance *>(m_ptr));
        if(post.type == console::error)
        {
            cpd_console_error(post.text.c_str());
        }
        else if(post.type == console::fatal)
        {
            cpd_console_fatal(post.text.c_str());
        }
        else if(post.type == console::normal)
        {
            cpd_console_post(post.text.c_str());
        }
        else
        {
            cpd_console_log(post.text.c_str());
        }
        environment::unlock();
    }
    
    void instance::send(tie name, symbol s, std::vector<atom> const& atoms) const
    {
        int todo_set_instance;
        environment::lock();
        cpd_instance_set(reinterpret_cast<c_instance *>(m_ptr));
        c_list* list = cpd_list_create(atoms.size());
        for(size_t i = 0; i < atoms.size(); ++i)
        {
            if(atoms[i].type() == atom::float_t)
            {
                cpd_list_set_float(list, i, float_t(atoms[i]));
            }
            else if(atoms[i].type() == atom::symbol_t)
            {
                cpd_list_set_float(list, i, symbol(atoms[i]));
            }
        }
        cpd_messagesend_anything(reinterpret_cast<c_tie const *>(smuggler::gettie(name)),
                                 reinterpret_cast<c_symbol const *>(smuggler::getsymbol(s)), list);
        
        environment::unlock();
    }
    
    void instance::send(midi::event const& event) const
    {
        int todo_set_instance;
        environment::lock();
        cpd_instance_set(reinterpret_cast<c_instance *>(m_ptr));
        if(event.type() == midi::event::note_t)
        {
            cpd_midisend_noteon(event.channel(), event.pitch(), event.velocity());
        }
        else if(event.type() == midi::event::control_change_t)
        {
            cpd_midisend_controlchange(event.channel(), event.controler(), event.value());
        }
        else if(event.type() == midi::event::program_change_t)
        {
            cpd_midisend_programchange(event.channel(), event.program());
        }
        else if(event.type() == midi::event::pitch_bend_t)
        {
            cpd_midisend_pitchbend(event.channel(), event.bend());
        }
        else if(event.type() == midi::event::after_touch_t)
        {
            cpd_midisend_pitchbend(event.channel(), event.value());
        }
        else if(event.type() == midi::event::poly_after_touch_t)
        {
            cpd_midisend_polyaftertouch(event.channel(), event.pitch(), event.value());
        }
        else
        {
            cpd_midisend_byte(0, event.value());
        }
        environment::unlock();
    }
    
    void instance::listener_add(listener& listener)
    {
        m_listeners.insert(&listener);
    }
    
    void instance::listener_remove(listener& listener)
    {
        m_listeners.erase(&listener);
    }
}



