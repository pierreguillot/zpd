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
        inline std::string name() const {return m_name;}
        
        //! @brief Gets the file's path.
        inline std::string path() const  {return m_path;}
        
        //! @brief Gets the id of the patch.
        inline constexpr size_t unique_id() const noexcept {return m_unique_id;}
        
        //! @brief Gets the patch position.
        int x() const noexcept;
        
        //! @brief Gets the patch position.
        int y() const noexcept;
        
        //! @brief Gets the patch position.
        int width() const noexcept;
        
        //! @brief Gets the patch position.
        int height() const noexcept;
        
        //! @brief Gets the objects from the patch.
        std::vector<object*> objects() const noexcept;
    private:
        patch(patch const& other) = delete;
        patch& operator=(patch const& other) = delete;
        
        patch(void* ptr, std::string const& name, std::string const& path, size_t uid) noexcept;
        ~patch() noexcept;
        
        void*               m_ptr;
        const std::string   m_name;
        const std::string   m_path;
        const size_t        m_unique_id;
        friend class instance;
    };
}

#endif // XPD_PATCH_HPP
