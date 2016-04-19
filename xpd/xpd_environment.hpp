/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_ENVIRONMENT_HPP
#define XPD_ENVIRONMENT_HPP

#include "xpd_mutex.hpp"
#include <string>

namespace xpd
{
    // ==================================================================================== //
    //                                      ENVIRONMENT                                     //
    // ==================================================================================== //
    
    //! @brief The environment manages xpd global variables and methods.
    //! @details The environment is an unique opaque object used by the instances to ensure
    //! thread safety. You should never have to deal with it expect for the few public
    //! methods.
    class environment
    {
    public:
        //! @brief Gets the major version of Pure Data.
        static unsigned int version_major() xpd_noexcept;
        
        //! @brief Gets the minor version of Pure Data.
        static unsigned int version_minor() xpd_noexcept;
        
        //! @brief Gets the bug version of Pure Data.
        static unsigned int version_bug() xpd_noexcept;
        
        //! @brief Adds a path the search path.
        static void searchpath_add(std::string const& path) xpd_noexcept;
        
        //! @brief Clears all the search path.
        static void searpath_clear() xpd_noexcept;
        
    private:
        
        //! @brief The environment contructor.
        environment() xpd_noexcept;
        
        //! @brief The environment destructor.
        ~environment() xpd_noexcept;
        
        //! @brief Gets the singleton of the environment.
        static environment& get() xpd_noexcept;
        
        //! @brief Locks the environment.
        static void lock() xpd_noexcept;
        
        //! @brief Unlocks the environment.
        static void unlock() xpd_noexcept;
        
        mutex        m_mutex;
        friend class instance;
    };
}

#endif // XPD_ENVIRONMENT_HPP
