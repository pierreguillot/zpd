/*
// Copyright (c) 2015-2016 Pierre Guillot.
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
#include <iostream>

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
        cpd_instance      object;
        instance*         ref;
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

        
        
        static void func_midi(instance::internal* instance, cpd_midi_event event)
        {
            instance->ref->receive(midi::event(midi::event::type_t(event.type), event.data1, event.data2, event.data3));
        }
        
        
        static void func_message(instance::internal* instance, cpd_message message)
        {
            std::vector<atom> vec(message.list.size);
            for(size_t i = 0; i < vec.size(); ++i)
            {
                if(cpd_list_get_type(&message.list, i) == CPD_FLOAT)
                {
                    vec[i] = cpd_list_get_float(&message.list, i);
                }
                else if(cpd_list_get_type(&message.list, i) == CPD_SYMBOL)
                {
                    vec[i] = smuggler::createsymbol(cpd_list_get_symbol(&message.list, i));
                }
            }
            instance->ref->receive(smuggler::createtie(message.tie), smuggler::createsymbol(message.selector), vec);
        }
        
        static void func_post(instance::internal* instance, cpd_post post)
        {
            instance->ref->receive(console::post(console::level(post.level), std::string(post.text)));
        }
    };
    
    // ==================================================================================== //
    //                                          INSTANCE                                    //
    // ==================================================================================== //
    
    instance::instance()
    {
        m_ptr = internal::allocate(this);
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
        cpd_instance_free(reinterpret_cast<cpd_instance *>(m_ptr));
    }
    
    
    patch instance::load(std::string const& name, std::string const& path)
    {
        patch p;
        void* ptr = xpd_nullptr;
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
        return p;
    }
    
    void instance::close(patch& p)
    {
        cpd_instance_patch_close(reinterpret_cast<cpd_instance *>(m_ptr), reinterpret_cast<cpd_patch *>(p.m_ptr));
    }
    
    
    int instance::samplerate() const xpd_noexcept
    {
        return cpd_instance_get_samplerate(reinterpret_cast<cpd_instance *>(m_ptr));
    }
    
    void instance::prepare(const int nins, const int nouts, const int samplerate, const int nsamples) xpd_noexcept
    {
        cpd_instance_dsp_prepare(reinterpret_cast<cpd_instance *>(m_ptr), nins, nouts, samplerate, nsamples);
    }

    void instance::perform(int nsamples, const int nins, const sample** inputs, const int nouts, sample** outputs) xpd_noexcept
    {
        cpd_instance_dsp_perform(reinterpret_cast<cpd_instance *>(m_ptr), nsamples, nins, inputs, nouts, outputs);
    }
    
    void instance::release() xpd_noexcept
    {
        cpd_instance_dsp_release(reinterpret_cast<cpd_instance *>(m_ptr));
    }
    
    
    
    void instance::send(console::post const& post) const
    {
        cpd_post cpost;
        cpost.level = static_cast<cpd_postlevel>(post.type);
        cpost.text  = post.text.c_str();
        cpd_instance_post_send(reinterpret_cast<cpd_instance *>(m_ptr), cpost);
    }
    
    void instance::send(tie name, symbol selector, std::vector<atom> const& atoms) const
    {
        cpd_message cmess;
        cmess.tie       = smuggler::gettie(name);
        cmess.selector  = smuggler::getsymbol(selector);
        cpd_list_init(&cmess.list, atoms.size());
        if(cmess.list.size == atoms.size())
        {
            for(size_t i = 0; i  < cmess.list.size; ++i)
            {
                if(atoms[i].type() == atom::float_t)
                {
                    cpd_list_set_float(&cmess.list, i, float(atoms[i]));
                }
                else if(atoms[i].type() == atom::symbol_t)
                {
                    cpd_list_set_symbol(&cmess.list, i, smuggler::getsymbol(symbol(atoms[i])));
                }
            }
            cpd_instance_message_send(reinterpret_cast<cpd_instance *>(m_ptr), cmess);
        }
    }
    
    void instance::send(midi::event const& event) const
    {
        cpd_midi_event cevent;
        cevent.type     = static_cast<cpd_midi_type>(event.type());
        cevent.data1    =  event.data1();
        cevent.data2    =  event.data2();
        cevent.data3    =  event.data3();
        cpd_instance_midi_send(reinterpret_cast<cpd_instance *>(m_ptr), cevent);
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



