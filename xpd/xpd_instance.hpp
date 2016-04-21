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
        //! @param nins The number of inputs.
        //! @param nouts The number of outputs.
        //! @param samplerate The sample rate.
        //! @param nsamples The number of samples to process.
        void prepare(const int nins, const int nouts, const int samplerate, const int nsamples) xpd_noexcept;
        
        //! @brief Performs the digital signal processing chain of the instance.
        //! @param nsamples The number of samples to process.
        //! @param nins The number of inputs.
        //! @param inputs The input matrix.
        //! @param nouts The number of outputs.
        //! @param outputs The output matrix.
        void perform(int nsamples, const int nins, const sample** inputs, const int nouts, sample** outputs) xpd_noexcept;
        
        //! @brief Releases the digital signal processing chain of the instance.
        void release() xpd_noexcept;
        
        //! @brief Sends a message through a tie.
        //! @param name The tie that will pass the vector of atoms.
        //! @param selector The selector.
        //! @param atoms The vector of atoms.
        //! @todo preallocate the list;
        void send(tie name, symbol selector, std::vector<atom> const& atoms) const;
        
        //! @brief Sends a midi event.
        //! @param event The midi event to send.
        void send(midi::event const& event) const;
        
        //! @brief Sends a post to the console.
        //! @param post The console post to send.
        void send(console::post const& post) const;
        
#define LCOV_EXCL_START
        //! @brief Receives a message from a tie.
        //! @param name The tie that received the vector of atoms.
        //! @param selector The selector.
        //! @param atoms The vector of atoms.
        virtual void receive(tie name, symbol selector, std::vector<atom> const& atoms) {}
        
        //! @brief Receives a midi event.
        //! @param event The received midi event.
        virtual void receive(midi::event const& event) {}
        
        //! @brief Receives a post from the console.
        //! @param post The console post received.
        virtual void receive(console::post const& post) {};
#define LCOV_EXCL_STOP
        
        //! @brief Binds the instance to a tie.
        //! @param name The tie to bind to the instance.
        void bind(tie name);
        
        //! @brief Unbinds the instance from a tie.
        //! @param name The tie to unbind from the instance.
        void unbind(tie name);
        
    private:
        instance(instance const& other) xpd_delete_f;
        instance& operator=(instance const& other) xpd_delete_f;
        struct internal;
        internal* m_ptr;
    };

}

#endif // XPD_INSTANCE_HPP
