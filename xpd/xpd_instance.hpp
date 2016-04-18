/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_INSTANCE_HPP
#define XPD_INSTANCE_HPP

#include "xpd_environment.hpp"
#include "xpd_console.hpp"
#include <vector>
#include <set>

namespace xpd
{
    class patch;
    // ==================================================================================== //
    //                                          INSTANCE                                    //
    // ==================================================================================== //
    
    //! @brief The instance manages a set of patches.
    //! @details The instance is a wrapper for the Pure Data's native instance.
    //! With the default constructor, the instance won't be initialized. The instance has some
    //! kind of smart pointer behavior so when an instance object is no more useful the object
    //! is deleted.
    class instance : private smuggler
    {
    public:
        
        //! @brief The constructor for an empty instance.
        //! @details Creates an instance that can be used as an empty reference inside
        //! another class.
        instance() noexcept;
        
        //! @brief The destructor.
        //! @details The instance will be destroyed if no other copy exists.
        virtual ~instance() noexcept;
        
        //! @brief Gets the sample rate of the instance.
        int get_sample_rate() const noexcept;
        
        
        //! @brief Sends a message the console.
        void send(console::message const& mess) noexcept;
        
        //! @brief Sends a message the console.
        virtual void receive(console::message const& mess) {};
        
        class listener
        {
        public:
            inline constexpr listener() {}
            inline virtual ~listener() {}
            virtual void patch_created(patch* p) = 0;
            virtual void patch_deleted(patch* p) = 0;
        };
        
        void listener_add(listener* listener);
        void listener_remove(listener* listener);
        
    protected:
        
        //! @brief Prepares the digital signal processing chain of the instance.
        void dsp_prepare(const int nins, const int nouts, const int samplerate, const int nsamples) noexcept;
        
        //! @brief Performs the digital signal processing chain of the instance.
        //! @details You should locks the instance to ensure thread safety.
        void dsp_perform(int nsamples, const int nins, const float** inputs, const int nouts, float** outputs) noexcept;
        
        //! @brief Releases the digital signal processing chain of the instance.
        void dsp_release() noexcept;
        
        
        //! @brief Sends anything.
        void send(tie name, symbol s, std::vector<atom> const& atoms) const;
        
        //! @brief Receives anything.
        virtual void receive(tie tie, symbol s, std::vector<atom> const& atoms) {}
        
    private:
        instance(instance const& other) = delete;
        instance& operator=(instance const& other) = delete;
        struct internal;
        
        static symbol   m_sym_bang;
        static symbol   m_sym_float;
        static symbol   m_sym_symbol;
        static symbol   m_sym_list;
        static symbol   m_sym_gpointer;
        
        internal*           m_ptr;
        std::set<listener*> m_listeners;
    };

}

#endif // XPD_INSTANCE_HPP
