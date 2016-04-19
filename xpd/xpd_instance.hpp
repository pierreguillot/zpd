/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_INSTANCE_HPP
#define XPD_INSTANCE_HPP

#include "xpd_tie.hpp"
#include "xpd_symbol.hpp"
#include "xpd_atom.hpp"
#include "xpd_midi.hpp"
#include "xpd_console.hpp"
#include "xpd_environment.hpp"

#include <vector>
#include <set>
#include <cassert>

namespace xpd
{
    class patch;
    // ==================================================================================== //
    //                                          INSTANCE                                    //
    // ==================================================================================== //
    
    //! @brief The instance is the main interface to communicate within the xpd environment.
    //! @details The instance manages the posts to the console, the midi events, the messages
    //! through tie and the digital signal processing. It is also the interface to load and
    //! delete patches.
    //! @todo set the instance at each call of method
    class instance
    {
    public:
        
        //! @brief The constructor for an empty instance.
        //! @details Creates an instance that can be used as an empty reference inside
        //! another class.
        instance();
        
        //! @brief The destructor.
        //! @details The instance will be destroyed if no other copy exists.
        virtual ~instance() xpd_noexcept;
        
        //! @brief Gets the sample rate of the instance.
        int samplerate() const xpd_noexcept;
        
        //! @brief The instance listener.
        //! @details The instance listener id notified when a pacth is created or deleted.
        class listener
        {
        public:
           inline xpd_constexpr listener() {}
            inline virtual ~listener() {}
            virtual void patch_loaded(patch* p) = 0;
            virtual void patch_deleted(patch* p) = 0;
        };
        
        //! @brief Adds a listener to the instance.
        void listener_add(listener& listener);
        
        //! @brief Removes a listener from the instance.
        void listener_remove(listener& listener);
        
        //! @brief Loads a patch.
        patch* load(std::string const& name, std::string const& path);
        
        //! @brief Closes a patch.
        void close(patch& p);
        
        //! @brief Prepares the digital signal processing chain of the instance.
        void prepare(const int nins, const int nouts, const int samplerate, const int nsamples) xpd_noexcept;
        
        //! @brief Performs the digital signal processing chain of the instance.
        //! @details You should locks the instance to ensure thread safety.
        void perform(int nsamples, const int nins, const float** inputs, const int nouts, float** outputs) xpd_noexcept;
        
        //! @brief Releases the digital signal processing chain of the instance.
        void release() xpd_noexcept;
        
        //! @brief Sends a post to the console.
        void send(console::post const& post) xpd_noexcept;
        
        //! @brief Receives a post from the console.
        virtual void receive(console::post post) {};
        
        //! @brief Sends a message through a tie.
        void send(tie name, symbol s, std::vector<atom> const& atoms) const;
        
        //! @brief Receives a message from a tie.
        virtual void receive(tie tie, symbol s, std::vector<atom> atoms) {}
        
        //! @brief Sends a midi event.
        void send(midi::event const& event) const;
        
        //! @brief Receives a midi event.
        virtual void receive(midi::event event) {}
        
    private:
        instance(instance const& other) xpd_delete_f;
        instance& operator=(instance const& other) xpd_delete_f;
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
