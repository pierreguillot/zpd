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
        inline static xpd_constexpr cpd_tie* gettie(tie const& tie) xpd_noexcept {return static_cast<cpd_tie*>(tie.ptr);}
        inline static xpd_constexpr tie createtie(void *ptr) xpd_noexcept {return tie(ptr);}
        inline static xpd_constexpr cpd_symbol* getsymbol(symbol const& symbol) xpd_noexcept {return static_cast<cpd_symbol*>(symbol.ptr);}
        inline static xpd_constexpr symbol createsymbol(void *ptr) xpd_noexcept {return symbol(ptr);}
    };
    
    struct instance::internal
    {
    public:        
        cpd_instance object;
        instance*  ref;
        c_hook_console console;
        c_hook_midi midi;
        c_hook_message message;
        
        static internal* allocate(instance* _ref)
        {
            internal* ptr = (internal *)cpd_instance_new(sizeof(internal));
            if(ptr)
            {
                ptr->ref = _ref;
                ptr->console.m_log   = (c_hook_print)instance::internal::m_log_hook;
                ptr->console.m_post  = (c_hook_print)instance::internal::m_post_hook;
                ptr->console.m_error = (c_hook_print)instance::internal::m_error_hook;
                ptr->console.m_fatal = (c_hook_print)instance::internal::m_fatal_hook;
                
                ptr->midi.m_noteon           = (c_hook_noteon)instance::internal::m_noteon;
                ptr->midi.m_controlchange    = (c_hook_controlchange)instance::internal::m_controlchange;
                ptr->midi.m_programchange    = (c_hook_programchange)instance::internal::m_programchange;
                ptr->midi.m_pitchbend        = (c_hook_pitchbend)instance::internal::m_pitchbend;
                ptr->midi.m_aftertouch       = (c_hook_aftertouch)instance::internal::m_aftertouch;
                ptr->midi.m_polyaftertouch   = (c_hook_polyaftertouch)instance::internal::m_polyaftertouch;
                ptr->midi.m_byte             = (c_hook_byte)instance::internal::m_byte;
                
                ptr->message.m_bang          = (c_hook_bang)instance::internal::m_bang;
                ptr->message.m_float         = (c_hook_float)instance::internal::m_float;
                ptr->message.m_symbol        = (c_hook_symbol)instance::internal::m_symbol;
                ptr->message.m_gpointer      = (c_hook_gpointer)instance::internal::m_gpointer;
                ptr->message.m_list          = (c_hook_list)instance::internal::m_list;
                ptr->message.m_anything      = (c_hook_anything)instance::internal::m_anything;
                
                cpd_instance_set_hook_console(reinterpret_cast<cpd_instance*>(ptr), &ptr->console);
                cpd_instance_set_hook_midi(reinterpret_cast<cpd_instance*>(ptr), &ptr->midi);
                
            }
            return ptr;
        }
        
        static void m_bind(instance::internal* instance, cpd_tie* tie)
        {
            cpd_instance_bind(reinterpret_cast<cpd_instance *>(instance), tie, &instance->message);
        }
        
        static void m_unbind(instance::internal* instance, cpd_tie* tie)
        {
            cpd_instance_unbind(reinterpret_cast<cpd_instance *>(instance), tie);
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
        
        
        
        
        static void m_bang(instance::internal* instance, cpd_tie* tie)
        {
            instance->ref->receive(smuggler::createtie(tie), symbol::bang_s, std::vector<atom>());
        }
        
        static void m_float(instance::internal* instance, cpd_tie* tie, cpd_float f)
        {
            instance->ref->receive(smuggler::createtie(tie), symbol::float_s, std::vector<atom>(1, f));
        }
        
        static void m_symbol(instance::internal* instance, cpd_tie* tie, cpd_symbol* s)
        {
            instance->ref->receive(smuggler::createtie(tie), symbol::symbol_s, std::vector<atom>(1, smuggler::createsymbol(s)));
        }
        
        static void m_gpointer(instance::internal* instance, cpd_tie* tie, cpd_gpointer *g)
        {
            ;
        }
        
        static void m_list(instance::internal* instance, cpd_tie* tie, cpd_list *list)
        {
            std::vector<atom> vec(cpd_list_get_size(list));
            for(size_t i = 0; i < vec.size(); ++i)
            {
                if(cpd_list_get_type(list, i) == Z_FLOAT)
                {
                    vec[i] = cpd_list_get_float(list, i);
                }
                else if(cpd_list_get_type(list, i) == Z_SYMBOL)
                {
                    vec[i] = smuggler::createsymbol(cpd_list_get_symbol(list, i));
                }
            }
            instance->ref->receive(smuggler::createtie(tie), symbol::list_s, vec);
        }
        
        static void m_anything(instance::internal* instance, cpd_tie* tie, cpd_symbol *s, cpd_list *list)
        {
            std::vector<atom> vec(cpd_list_get_size(list));
            for(size_t i = 0; i < vec.size(); ++i)
            {
                if(cpd_list_get_type(list, i) == Z_FLOAT)
                {
                    vec[i] = cpd_list_get_float(list, i);
                }
                else if(cpd_list_get_type(list, i) == Z_SYMBOL)
                {
                    vec[i] = smuggler::createsymbol(cpd_list_get_symbol(list, i));
                }
            }
            instance->ref->receive(smuggler::createtie(tie), smuggler::createsymbol(s), vec);
        }
    };
    
    // ==================================================================================== //
    //                                          INSTANCE                                    //
    // ==================================================================================== //
    
    instance::instance()
    {
        environment::lock();
        

        m_ptr = internal::allocate(this);
        environment::unlock();
#define LCOV_EXCL_START
        if(!m_ptr)
        {
            throw "can't allocate instance.";
        }
#define LCOV_EXCL_STOP
    }
    
    instance::~instance() xpd_noexcept
    {
        release();
        environment::lock();
        cpd_instance_free(reinterpret_cast<cpd_instance *>(m_ptr));
        environment::unlock();
    }
    
    
    patch instance::load(std::string const& name, std::string const& path)
    {
        int todo;
        patch p;
        void* ptr = xpd_nullptr;
        environment::lock();
        cpd_instance_set(reinterpret_cast<cpd_instance *>(m_ptr));
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
            p = patch(ptr, size_t(cpd_patch_get_dollarzero(reinterpret_cast<cpd_patch *>(ptr))));
        }
        environment::unlock();
        return p;
    }
    
    void instance::close(patch& p)
    {
        int todo;
        environment::lock();
        cpd_instance_set(reinterpret_cast<cpd_instance *>(m_ptr));
        cpd_patch_free(reinterpret_cast<cpd_patch *>(p.m_ptr));
        environment::unlock();
    }
    
    
    int instance::samplerate() const xpd_noexcept
    {
        return cpd_instance_get_samplerate(reinterpret_cast<cpd_instance *>(m_ptr));
    }
    
    void instance::prepare(const int nins, const int nouts, const int samplerate, const int nsamples) xpd_noexcept
    {
        environment::lock();
        cpd_instance_dsp_prepare(reinterpret_cast<cpd_instance *>(m_ptr), nins, nouts, samplerate, nsamples);
        environment::unlock();
    }

    void instance::perform(int nsamples, const int nins, const sample** inputs, const int nouts, sample** outputs) xpd_noexcept
    {
        environment::lock();
        cpd_instance_dsp_perform(reinterpret_cast<cpd_instance *>(m_ptr), nsamples, nins, inputs, nouts, outputs);
        environment::unlock();
    }
    
    void instance::release() xpd_noexcept
    {
        environment::lock();
        cpd_instance_dsp_release(reinterpret_cast<cpd_instance *>(m_ptr));
        environment::unlock();
    }
    
    
    
    void instance::send(console::post const& post) const
    {
        int todo_set_instance;
        environment::lock();
        cpd_instance_set(reinterpret_cast<cpd_instance *>(m_ptr));
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
    
    void instance::send(tie name, symbol selector, std::vector<atom> const& atoms) const
    {
        int todo_set_instance;
        environment::lock();
        cpd_instance_set(reinterpret_cast<cpd_instance *>(m_ptr));
        if(selector == symbol::bang_s)
        {
            cpd_messagesend_bang(smuggler::gettie(name));
        }
        else if(selector == symbol::float_s)
        {
            cpd_messagesend_float(smuggler::gettie(name), atoms[0]);
        }
        else if(selector == symbol::symbol_s)
        {
            cpd_messagesend_symbol(smuggler::gettie(name), smuggler::getsymbol(atoms[0]));
        }
        else if(selector == symbol::list_s)
        {
            cpd_list* list = cpd_list_create(atoms.size());
            for(size_t i = 0; i < atoms.size(); ++i)
            {
                if(atoms[i].type() == atom::float_t)
                {
                    cpd_list_set_float(list, i, float(atoms[i]));
                }
                else if(atoms[i].type() == atom::symbol_t)
                {
                    cpd_list_set_symbol(list, i, smuggler::getsymbol(atoms[i]));
                }
            }
            cpd_messagesend_list(smuggler::gettie(name), list);
            cpd_list_free(list);
        }
        else
        {
            cpd_list* list = cpd_list_create(atoms.size());
            for(size_t i = 0; i < atoms.size(); ++i)
            {
                if(atoms[i].type() == atom::float_t)
                {
                    cpd_list_set_float(list, i, float(atoms[i]));
                }
                else if(atoms[i].type() == atom::symbol_t)
                {
                    cpd_list_set_symbol(list, i, smuggler::getsymbol(atoms[i]));
                }
            }
            cpd_messagesend_anything(smuggler::gettie(name), smuggler::getsymbol(selector), list);
            cpd_list_free(list);
        }
        environment::unlock();
    }
    
    void instance::send(midi::event const& event) const
    {
        int todo_set_instance;
        environment::lock();
        cpd_instance_set(reinterpret_cast<cpd_instance *>(m_ptr));
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
            cpd_midisend_aftertouch(event.channel(), event.value());
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
    
    void instance::bind(tie name)
    {
        reinterpret_cast<internal *>(m_ptr)->m_bind(reinterpret_cast<internal *>(m_ptr), smuggler::gettie(name));
    }
    
    void instance::unbind(tie name)
    {
        reinterpret_cast<internal *>(m_ptr)->m_unbind(reinterpret_cast<internal *>(m_ptr), smuggler::gettie(name));
    }
}



