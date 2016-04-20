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

namespace xpd
{
    class patch;
    // ==================================================================================== //
    //                                          INSTANCE                                    //
    // ==================================================================================== //
    
    //! @brief The instance is the main interface to communicate within the xpd environment.
    //! @details The instance manages the posts to the console, the midi events, the messages
    //! and the digital signal processing. It is also the interface to load and delete patches.
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
        
    protected:
        
        //! @brief Loads a patch.
        patch load(std::string const& name, std::string const& path);
        
        //! @brief Closes a patch.
        void close(patch& p);
        
        //! @brief Prepares the digital signal processing chain of the instance.
        void prepare(const int nins, const int nouts, const int samplerate, const int nsamples) xpd_noexcept;
        
        //! @brief Performs the digital signal processing chain of the instance.
        //! @details You should locks the instance to ensure thread safety.
        void perform(int nsamples, const int nins, const float** inputs, const int nouts, float** outputs) xpd_noexcept;
        
        //! @brief Releases the digital signal processing chain of the instance.
        void release() xpd_noexcept;
        
        //! @brief Sends a midi event.
        void send(midi::event const& event) const;
        
        //! @brief Sends a message through a tie.
        void send(tie name, symbol s, std::vector<atom> const& atoms) const;
        
        //! @brief Sends a post to the console.
        void send(console::post const& post) xpd_noexcept;
        
#define LCOV_EXCL_START
        //! @brief Receives a message from a tie.
        virtual void receive(tie tie, symbol s, std::vector<atom> const& atoms) {}
        
        //! @brief Receives a midi event.
        virtual void receive(midi::event const& event) {}
        
        //! @brief Receives a post from the console.
        virtual void receive(console::post const& post) {};
#define LCOV_EXCL_STOP
        
        //! @brief Binds the instance to a tie.
        void bind(tie name);
        
        //! @brief Unbinds the instance from a tie.
        void unbind(tie name);
        
    private:
        instance(instance const& other) xpd_delete_f;
        instance& operator=(instance const& other) xpd_delete_f;
        struct internal;
        internal* m_ptr;
    };

}

#endif // XPD_INSTANCE_HPP
