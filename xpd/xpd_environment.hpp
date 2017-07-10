/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_ENVIRONMENT_INCLUDE_HPP
#define XPD_ENVIRONMENT_INCLUDE_HPP

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
        
        //! @brief Gets the current font name of Pure Data.
        static std::string font_name();
        
        //! @brief Gets the current font weight of Pure Data.
        static std::string font_weight();
        
        //! @brief GGets the current font size of Pure Data.
        static unsigned int font_size();
    };
}

#endif // XPD_ENVIRONMENT_INCLUDE_HPP
