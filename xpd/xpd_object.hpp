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
    //! @details The object is can only be valid if it has been created by a patch. The
    //! interactions with an object are limited, you can only retrieve some basic informations.
    class object
    {
    public:
        //! @brief The default constructor.
        //! @details Allocates an invalid object.
        inline xpd_constexpr object() xpd_noexcept : m_patch(xpd_nullptr), m_ptr(xpd_nullptr) {}
        
        //! @brief The copy constructor.
        inline xpd_constexpr object(object const& other) xpd_noexcept : m_patch(other.m_patch), m_ptr(other.m_ptr) {}
        
        //! @brief The copy operator.
        inline object& operator=(object const& other) xpd_noexcept {m_patch = other.m_patch; m_ptr = other.m_ptr; return *this;}
        
        //! @brief The destructor.
        inline virtual ~object() {};
        
        //! @brief Checks the validity of the object.
        //! @return true if the object if valid, otherwise false.
        inline xpd_constexpr operator bool() const xpd_noexcept {return (m_patch != xpd_nullptr) && (m_ptr != xpd_nullptr);}
        
        //! @brief Gets the name of the object.
        std::string name() const;
        
        //! @brief Gets the text of the object.
        std::string text() const;
        
        //! @brief Gets the x position of the object.
        int x() const xpd_noexcept;
        
        //! @brief Gets the y position of the object.
        int y() const xpd_noexcept;
        
        //! @brief Gets the width of the object.
        int width() const xpd_noexcept;
        
        //! @brief Gets the height of the object.
        int height() const xpd_noexcept;
        
    protected:
        
        inline xpd_constexpr object(void const* patch, void* ptr) xpd_noexcept : m_patch(patch), m_ptr(ptr) {}
        
        void const* m_patch;
        void*       m_ptr;
        friend class patch;
    };
}

#endif // XPD_OBJECT_HPP
