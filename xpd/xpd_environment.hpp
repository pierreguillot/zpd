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
    class environment
    {
    public:
        //! @brief Initializes the environment.
        static void initialize() xpd_noexcept;
        
        //! @brief Clears the environment.
        static void  clear() xpd_noexcept;
        
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
    };
}

#endif // XPD_ENVIRONMENT_HPP
