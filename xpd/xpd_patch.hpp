/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_PATCH_HPP
#define XPD_PATCH_HPP

#include "xpd_instance.hpp"

namespace xpd
{
    class object;
    // ==================================================================================== //
    //                                          PATCH                                       //
    // ==================================================================================== //
    
    //! @brief The patch is the interface that manages objects.
    //! @details The patch is can only be valid if it has been created by an instance. The
    //! interactions with a patch are limited to its retrieve some basic information.
    //! Nevertheless, the patch allows to retrieve its objects.
    class patch
    {
    public:
        //! @brief The default constructor.
        //! @details Allocates an invalid patch.
        inline xpd_constexpr patch() xpd_noexcept : m_ptr(xpd_nullptr), m_unique_id(0ul) {};
        
        //! @brief The copy constructor.
        inline xpd_constexpr patch(patch const& other) xpd_noexcept : m_ptr(other.m_ptr), m_unique_id(other.m_unique_id) {}
        
        //! @brief The copy operator.
        inline patch& operator=(patch const& other) xpd_noexcept {m_ptr = other.m_ptr; m_unique_id = other.m_unique_id; return *this;}
        
        //! @brief The destructor.
        inline ~patch() xpd_noexcept {};
        
        //! @brief Checks the validity of the patch.
        //! @return true if the patch if valid, otherwise false.
        inline xpd_constexpr operator bool() const xpd_noexcept {return static_cast<bool>(m_ptr);}
        
        //! @brief Gets the file's name.
        std::string name() const;
        
        //! @brief Gets the file's path.
        std::string path() const;
        
        //! @brief Gets the id of the patch.
        inline xpd_constexpr size_t unique_id() const xpd_noexcept {return m_unique_id;}
        
        //! @brief Gets the patch position.
        int x() const xpd_noexcept;
        
        //! @brief Gets the patch position.
        int y() const xpd_noexcept;
        
        //! @brief Gets the patch position.
        int width() const xpd_noexcept;
        
        //! @brief Gets the patch position.
        int height() const xpd_noexcept;
        
        //! @brief Gets the objects from the patch.
        std::vector<object> objects() const xpd_noexcept;
    private:
        
        inline xpd_constexpr patch(void* ptr, size_t uid) xpd_noexcept : m_ptr(ptr), m_unique_id(uid) {}
        
        void*  m_ptr;
        size_t m_unique_id;
        friend class instance;
    };
}

#endif // XPD_PATCH_HPP
