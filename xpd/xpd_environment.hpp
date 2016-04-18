/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_ENVIRONMENT_HPP
#define XPD_ENVIRONMENT_HPP

#include "PdTypes.hpp"

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
        static unsigned int get_major_version() noexcept;
        
        //! @brief Gets the minor version of Pure Data.
        static unsigned int get_minor_version() noexcept;
        
        //! @brief Gets the bug version of Pure Data.
        static unsigned int get_bug_version() noexcept;
        
        //! @brief Adds a path the search path.
        static void searchpath_add(std::string const& path) noexcept;
        
        //! @brief Clears all the search path.
        static void searpath_clear() noexcept;
        
    private:
        
        //! @brief The Pure Data contructor.
        environment() noexcept;
        
        //! @brief The Pure Data destructor.
        ~environment() noexcept;
        
        //! @brief Gets the singleton of Pure Data.
        static environment& get() noexcept;
        
        //! @brief Locks Pure Data.
        static void lock() noexcept;
        
        //! @brief Unlocks Pure Data.
        static void unlock() noexcept;
        
        mutex        m_mutex;
        friend class instance;
    };
}

#endif // XPD_ENVIRONMENT_HPP
