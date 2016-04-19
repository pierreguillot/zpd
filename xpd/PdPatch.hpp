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
    class gui;
    class object;
    // ==================================================================================== //
    //                                          PATCHER                                     //
    // ==================================================================================== //
    
    //! @brief The Pure Data patch.
    //! @details The instance is a wrapper for the Pure Data's native patch.
    //! With the default constructor, the patch won't be initialized. A valid
    //! patch should be created via an instance. The patch has some kind of smart
    //! pointer behavior so when an patch object is no more useful the object is deleted.
    class patch
    {
    public:
        
        //! @brief The constructor for a new patch.
        //! @details Creates a new valid patch.
        patch(instance& instance, std::string const& name, std::string const& path) noexcept;
        
        //! @brief The destructor.
        //! @details The patch will be destroyed if no other copy exists.
        ~patch() noexcept;
        
        //! @brief Gets the file's name.
        std::string name() const;
        
        //! @brief Gets the file's path.
        std::string path() const;
        
        //! @brief Gets the id of the patch.
        int unique_id() const noexcept;
        
        //! @brief Gets the patch position.
        int get_x() const noexcept;
        
        //! @brief Gets the patch position.
        int get_y() const noexcept;
        
        //! @brief Gets the patch position.
        int get_width() const noexcept;
        
        //! @brief Gets the patch position.
        int get_height() const noexcept;
        
        //! @brief Gets the Gui objects from the patch.
        std::vector<gui> get_guis() const noexcept;
        
        //! @brief Gets the Comment objects from the patch.
        std::vector<object> get_comments() const noexcept;
    private:
        patch(patch const& other) = delete;
        patch& operator=(patch const& other) = delete;
        
        void*                   m_ptr;
        friend class Object;
    };
}

#endif // XPD_PATCH_HPP
