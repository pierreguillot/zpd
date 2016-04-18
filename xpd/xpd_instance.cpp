/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "xpd_instance.hpp"
#include "PdPatch.hpp"

extern "C"
{
#include "../cpd/cpd.h"
}

namespace xpd
{
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
                ptr->s_bang     = symbol("bang");
                ptr->s_float    = symbol("float");
                ptr->s_symbol   = symbol("symbol");
                ptr->s_list     = symbol("list");
                ptr->s_gpointer = symbol("gpointer");
                cpd_instance_set_hook_console(reinterpret_cast<c_instance*>(ptr), &console);
                cpd_instance_set_hook_midi(reinterpret_cast<c_instance*>(ptr), &midi);
                
            }
            return ptr;
        }
        
        static void m_post_hook(instance::internal* instance, const char *s)
        {
            instance->ref->receiveConsolePost(std::string(s));
        }
        
        static void m_log_hook(instance::internal* instance, const char *s)
        {
            instance->ref->receiveConsoleLog(std::string(s));
        }
        
        static void m_error_hook(instance::internal* instance, const char *s)
        {
            instance->ref->receiveConsoleError(std::string(s));
        }
        
        static void m_fatal_hook(instance::internal* instance, const char *s)
        {
            instance->ref->receiveConsoleFatal(std::string(s));
        }
        

        
        
        static void m_noteon(instance::internal* instance, int port, int channel, int pitch, int velocity)
        {
            instance->ref->receiveMidiNoteOn(channel, pitch, velocity);
        }
        
        static void m_controlchange(instance::internal* instance, int port, int channel, int control, int value)
        {
            instance->ref->receiveMidiControlChange(channel, control, value);
        }
        
        static void m_programchange(instance::internal* instance, int port, int channel, int value)
        {
            instance->ref->receiveMidiProgramChange(channel, value);
        }
        
        static void m_pitchbend(instance::internal* instance, int port, int channel, int value)
        {
            instance->ref->receiveMidiPitchBend(channel, value);
        }
        
        static void m_aftertouch(instance::internal* instance, int port, int channel, int value)
        {
            instance->ref->receiveMidiAfterTouch(channel, value);
        }
        
        static void m_polyaftertouch(instance::internal* instance, int port, int channel, int pitch, int value)
        {
            instance->ref->receiveMidiPolyAfterTouch(channel, pitch, value);
        }
        
        static void m_byte(instance::internal* instance, int port, int value)
        {
            instance->ref->receiveMidiByte(port, value);
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
    }
    
    instance::~instance() noexcept
    {
        dsp_release();
        lock();
        cpd_instance_free(reinterpret_cast<c_instance *>(m_ptr));
        unlock();
    }
    
    void instance::dsp_prepare(const int nins, const int nouts, const int samplerate, const int nsamples) noexcept
    {
        lock();
        cpd_instance_dsp_prepare(reinterpret_cast<c_instance *>(m_ptr), nins, nouts, samplerate, nsamples);
        unlock();
    }

    void instance::dsp_perform(int nsamples, const int nins, const float** inputs, const int nouts, float** outputs) noexcept
    {
        cpd_instance_dsp_perform(reinterpret_cast<c_instance *>(m_ptr), nsamples, nins, inputs, nouts, outputs);
    }
    
    void instance::dsp_release() noexcept
    {
        lock();
        cpd_instance_dsp_release(reinterpret_cast<c_instance *>(m_ptr));
        unlock();
    }
    
    
    
    
    
    void instance::sendConsolePost(std::string const& message) noexcept
    {
        lock();
        cpd_console_post(message.c_str());
        unlock();
    }
    
    void instance::sendConsoleLog(std::string const& message) noexcept
    {
        lock();
        cpd_console_log(message.c_str());
        unlock();
    }
    
    void instance::sendConsoleError(std::string const& message) noexcept
    {
        lock();
        cpd_console_error(message.c_str());
        unlock();
    }
    
    void instance::sendConsoleFatal(std::string const& message) noexcept
    {
        lock();
        cpd_console_fatal(message.c_str());
        unlock();
    }
    
    
    void instance::send(tie name, symbol s, std::vector<atom> const& atoms) const
    {
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
    }
    
    void instance::sendMidiNote(int channel, int pitch, int velocity) const
    {
        cpd_midisend_noteon(channel, pitch, velocity);
    }
    
    void instance::sendMidiControlChange(int channel, int controller, int value) const
    {
        cpd_midisend_controlchange(channel, controller, value);
    }
    
    void instance::sendMidiProgramChange(int channel, int value) const
    {
        cpd_midisend_programchange(channel, value);
    }
    
    void instance::sendMidiPitchBend(int channel, int value) const
    {
        cpd_midisend_pitchbend(channel, value);
    }
    
    void instance::sendMidiAfterTouch(int channel, int value) const
    {
        cpd_midisend_aftertouch(channel, value);
    }
    
    void instance::sendMidiPolyAfterTouch(int channel, int pitch, int value) const
    {
        cpd_midisend_polyaftertouch(channel, pitch, value);
    }
    
    void instance::sendMidiByte(int port, int byte) const
    {
        cpd_midisend_byte(port, byte);
    }
    
    void instance::sendMidiSysEx(int port, int byte) const
    {
        cpd_midisend_sysex(port, byte);
    }
    
    void instance::sendMidiSysRealtime(int port, int byte) const
    {
        cpd_midisend_sysrealtimein(port, byte);
    }
    
    void instance::lock() noexcept
    {
        environment::lock();
        cpd_instance_set(reinterpret_cast<c_instance *>(m_ptr));
    }
    
    void instance::unlock() noexcept
    {
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



