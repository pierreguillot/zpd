/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_OBJECT_HPP
#define XPD_OBJECT_HPP

#include "xpd_patch.hpp"

namespace xpd
{
    //! @brief The object.
    //! @details In fact this just a description of an object.
    class object
    {
    public:
        
        //! @brief The destructor.
        inline virtual ~object() {};
        
        //! @brief Gets the text of the object.
        inline std::string text() const {return m_text;}
        
        //! @brief Gets the x position of the object.
        inline constexpr int x() const noexcept {return m_x;}
        
        //! @brief Gets the y position of the object.
        inline constexpr int y() const noexcept {return m_y;}
        
        //! @brief Gets the width of the object.
        inline constexpr int width() const noexcept {return m_width;}
        
        //! @brief Gets the height of the object.
        inline constexpr int height() const noexcept {return m_height;}
        
    protected:
        
        object(object const& other) = delete;
        object& operator=(object const& other) = delete;
        
        object(patch const* patch, void* ptr) noexcept;
        std::string m_text;
        int         m_x;
        int         m_y;
        int         m_width;
        int         m_height;
        friend class patch;
    };
}

#endif // XPD_OBJECT_HPP
