/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "xpd_instance.hpp"
#include "Pdpatch.hpp"

extern "C"
{
#include "../cpd/cpd.h"
}

namespace xpd
{
    
    //! @brief The smuggler is optimized for internal use.
    //! @details The class doesn't break the efficiency of creation of some type, but you
    //! should use it if and only if you know what you do.
    class smuggler
    {
    public:
        ~smuggler() noexcept {}
    public:
        inline static constexpr void const* gettie(tie const& tie) noexcept {return tie.ptr;}
        inline static constexpr tie createtie(void *ptr) noexcept {return tie(ptr);}
        inline static constexpr void const* getsymbol(symbol const& symbol) noexcept {return symbol.ptr;}
        inline static constexpr symbol createsymbol(void *ptr) noexcept {return symbol(ptr);}
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
            instance->ref->receive(console::post{console::level::normal, std::string(s)});
        }
        
        static void m_log_hook(instance::internal* instance, const char *s)
        {
            instance->ref->receive(console::post{console::level::log, std::string(s)});
        }
        
        static void m_error_hook(instance::internal* instance, const char *s)
        {
            instance->ref->receive(console::post{console::level::error, std::string(s)});
        }
        
        static void m_fatal_hook(instance::internal* instance, const char *s)
        {
            instance->ref->receive(console::post{console::level::fatal, std::string(s)});
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
            instance->ref->receive(createtie(tie), instance::m_sym_bang, std::vector<atom>{});
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
            instance->ref->receive(createtie(tie), instance::m_sym_list, vec);
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
            instance->ref->receive(createtie(tie), createsymbol(s), vec);
        }
    };
    
    // ==================================================================================== //
    //                                          INSTANCE                                    //
    // ==================================================================================== //
    
    instance::instance() noexcept
    {
        environment::lock();
        m_ptr = internal::allocate(this);
        environment::unlock();
        if(!m_ptr)
        {
            throw std::runtime_error("can't allocate instance.");
        }
        int todo;
        /*
        ptr->s_bang     = symbol("bang");
        ptr->s_float    = symbol("float");
        ptr->s_symbol   = symbol("symbol");
        ptr->s_list     = symbol("list");
        ptr->s_gpointer = symbol("gpointer");
         */
    }
    
    instance::~instance() noexcept
    {
        dsp_release();
        environment::lock();
        cpd_instance_free(reinterpret_cast<c_instance *>(m_ptr));
        environment::unlock();
    }
    
    void instance::dsp_prepare(const int nins, const int nouts, const int samplerate, const int nsamples) noexcept
    {
        environment::lock();
        cpd_instance_dsp_prepare(reinterpret_cast<c_instance *>(m_ptr), nins, nouts, samplerate, nsamples);
        environment::unlock();
    }

    void instance::dsp_perform(int nsamples, const int nins, const float** inputs, const int nouts, float** outputs) noexcept
    {
        environment::lock();
        cpd_instance_dsp_perform(reinterpret_cast<c_instance *>(m_ptr), nsamples, nins, inputs, nouts, outputs);
        environment::unlock();
    }
    
    void instance::dsp_release() noexcept
    {
        environment::lock();
        cpd_instance_dsp_release(reinterpret_cast<c_instance *>(m_ptr));
        environment::unlock();
    }
    
    
    
    void instance::send(console::post const& post) noexcept
    {
        int todo_set_instance;
        environment::lock();
        if(post.type == console::level::error)
        {
            cpd_console_error(post.text.c_str());
        }
        else if(post.type == console::level::fatal)
        {
            cpd_console_fatal(post.text.c_str());
        }
        else if(post.type == console::level::normal)
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
        if(atoms.empty())
        {
            if(s || s  == m_sym_bang)
            {
                cpd_messagesend_bang(reinterpret_cast<c_tie const *>(gettie(name)));
            }
        }
        else
        {
            
        }
        c_list* list = cpd_list_create(atoms.size());
        int todo;
        //cpd_messagesend_anything(reinterpret_cast<c_tie const *>(gettie(name)),
        //                          reinterpret_cast<c_symbol const *>(getsymbol(s)),
        //                          reinterpret_cast<c_list const *>(getvector(list)));
        
        environment::unlock();
    }
    
    void instance::send(midi::event const& event) const
    {
        int todo_set_instance;
        environment::lock();
        if(event.get_type() == midi::event::type::note)
        {
            cpd_midisend_noteon(event.get_channel(), event.get_pitch(), event.get_velocity());
        }
        else if(event.get_type() == midi::event::type::control_change)
        {
            cpd_midisend_controlchange(event.get_channel(), event.get_controler(), event.get_value());
        }
        else if(event.get_type() == midi::event::type::program_change)
        {
            cpd_midisend_programchange(event.get_channel(), event.get_program());
        }
        else if(event.get_type() == midi::event::type::pitch_bend)
        {
            cpd_midisend_pitchbend(event.get_channel(), event.get_bend());
        }
        else if(event.get_type() == midi::event::type::after_touch)
        {
            cpd_midisend_pitchbend(event.get_channel(), event.get_value());
        }
        else if(event.get_type() == midi::event::type::poly_after_touch)
        {
            cpd_midisend_polyaftertouch(event.get_channel(), event.get_pitch(), event.get_value());
        }
        else
        {
            cpd_midisend_byte(0, event.get_value());
        }
        environment::unlock();
    }
    
    int instance::get_sample_rate() const noexcept
    {
        return cpd_instance_get_samplerate(reinterpret_cast<c_instance *>(m_ptr));
    }
    
    void instance::listener_add(listener* listener)
    {
        assert("instance listener is nulltr" && listener);
        m_listeners.insert(listener);
    }
    
    void instance::listener_remove(listener* listener)
    {
        assert("instance listener is nulltr" && listener);
        m_listeners.erase(listener);
    }
}



