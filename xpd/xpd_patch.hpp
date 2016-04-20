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
    //                                          PATCHER                                     //
    // ==================================================================================== //
    
    //! @brief The patch.
    //! @details The instance is a wrapper for the Pure Data's native patch.
    //! With the default constructor, the patch won't be initialized. A valid
    //! patch should be created via an instance. The patch has some kind of smart
    //! pointer behavior so when an patch object is no more useful the object is deleted.
    class patch
    {
    public:
        
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
        std::vector<object*> objects() const xpd_noexcept;
    private:
        patch(patch const& other) xpd_delete_f;
        patch& operator=(patch const& other) xpd_delete_f;
        
        patch(void* ptr, size_t uid) xpd_noexcept;
        ~patch() xpd_noexcept;
        
        void*               m_ptr;
        const size_t        m_unique_id;
        friend class instance;
    };
}

#endif // XPD_PATCH_HPP
