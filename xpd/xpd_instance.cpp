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
#include "../cpd/cpd_midi.h"
#include "../cpd/cpd_message.h"
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
        
        static internal* allocate(instance* _ref)
        {
            internal* ptr = (internal *)cpd_instance_new(sizeof(internal));
            if(ptr)
            {
                ptr->ref = _ref;
                cpd_instance_post_sethook(reinterpret_cast<cpd_instance*>(ptr), (cpd_hook_post)func_post);
                cpd_instance_midi_sethook(reinterpret_cast<cpd_instance*>(ptr), (cpd_midi_hook)func_midi);
                
            }
            return ptr;
        }
        
        static void m_bind(instance::internal* instance, cpd_tie* tie)
        {
            cpd_instance_bind(reinterpret_cast<cpd_instance *>(instance), tie, (cpd_message_hook)func_message);
        }
        
        static void m_unbind(instance::internal* instance, cpd_tie* tie)
        {
            cpd_instance_unbind(reinterpret_cast<cpd_instance *>(instance), tie);
        }
        
        static void m_normal_hook(instance::internal* instance, const char *s)
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
        

        
        
        static void func_midi(instance::internal* instance, cpd_midi_event event)
        {
            instance->ref->receive(midi::event(midi::event::type_t(event.type), event.data1, event.data2, event.data3));
        }
        
        
        static void func_message(instance::internal* instance, cpd_tie* tie, cpd_symbol* s, cpd_list const* list)
        {
            std::vector<atom> vec(cpd_list_get_size(list));
            for(size_t i = 0; i < vec.size(); ++i)
            {
                if(cpd_list_get_type(list, i) == CPD_FLOAT)
                {
                    vec[i] = cpd_list_get_float(list, i);
                }
                else if(cpd_list_get_type(list, i) == CPD_SYMBOL)
                {
                    vec[i] = smuggler::createsymbol(cpd_list_get_symbol(list, i));
                }
            }
            instance->ref->receive(smuggler::createtie(tie), smuggler::createsymbol(s), vec);
        }
        
        static void func_post(instance::internal* instance, cpd_post post)
        {
            int todo;
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
        patch p;
        void* ptr = xpd_nullptr;
        environment::lock();
        if(path.empty())
        {
            ptr = cpd_instance_patch_load(reinterpret_cast<cpd_instance *>(m_ptr), name.c_str(), xpd_nullptr);
        }
        else
        {
            ptr = cpd_instance_patch_load(reinterpret_cast<cpd_instance *>(m_ptr), name.c_str(), path.c_str());
        }
        
        if(ptr)
        {
            int todo;
            p = patch(ptr, size_t(cpd_patch_get_dollarzero(reinterpret_cast<cpd_patch *>(ptr))));
        }
        environment::unlock();
        return p;
    }
    
    void instance::close(patch& p)
    {
        environment::lock();
        cpd_instance_patch_close(reinterpret_cast<cpd_instance *>(m_ptr), reinterpret_cast<cpd_patch *>(p.m_ptr));
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
        environment::lock();
        cpd_instance_post_send(reinterpret_cast<cpd_instance *>(m_ptr),
                               (cpd_post){static_cast<cpd_postlevel>(post.type), post.text.c_str()});
        environment::unlock();
    }
    
    void instance::send(tie name, symbol selector, std::vector<atom> const& atoms) const
    {
        int for_the_moment;
        cpd_instance_message_send(reinterpret_cast<cpd_instance *>(m_ptr), (cpd_message){smuggler::gettie(name), smuggler::getsymbol(selector), (cpd_list){0, NULL}});
    }
    
    void instance::send(midi::event const& event) const
    {
        cpd_instance_midi_send(reinterpret_cast<cpd_instance *>(m_ptr),
                               (cpd_midi_event){cpd_midi_type(event.type()), event.data1(), event.data2(), event.data3()});
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



