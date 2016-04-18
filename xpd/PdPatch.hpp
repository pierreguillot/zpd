/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef Z_PD_PATCH_HPP
#define Z_PD_PATCH_HPP

#include "xpd_instance.hpp"

namespace xpd
{
    class Gui;
    class Object;
    // ==================================================================================== //
    //                                          PATCHER                                     //
    // ==================================================================================== //
    
    //! @brief The Pure Data patch.
    //! @details The instance is a wrapper for the Pure Data's native patch.
    //! With the default constructor, the Patch won't be initialized. A valid
    //! Patch should be created via an instance. The Patch has some kind of smart
    //! pointer behavior so when an Patch object is no more useful the object is deleted.
    class Patch
    {
    public:
        
        //! @brief The constructor for a new Patch.
        //! @details Creates a new valid Patch.
        Patch(instance& instance, std::string const& name, std::string const& path) noexcept;
        
        //! @brief The destructor.
        //! @details The Patch will be destroyed if no other copy exists.
        ~Patch() noexcept;
        
        //! @brief Gets the file's name.
        std::string getName() const;
        
        //! @brief Gets the file's path.
        std::string getPath() const;
        
        //! @brief Gets the dollar zero of the Patch.
        int getDollarZero();
        
        //! @brief Gets the Patch position.
        int get_x() const noexcept;
        
        //! @brief Gets the Patch position.
        int get_y() const noexcept;
        
        //! @brief Gets the Patch position.
        int get_width() const noexcept;
        
        //! @brief Gets the Patch position.
        int get_height() const noexcept;
        
        //! @brief Gets the Patch position.
        Point<int> getPosition() const noexcept;
        
        //! @brief Gets the Patch size.
        Point<int> getSize() const noexcept;
        
        //! @brief Gets the Gui objects from the patch.
        std::vector<Gui> getGuis() const noexcept;
        
        //! @brief Gets the Comment objects from the patch.
        std::vector<Object> getComments() const noexcept;
    private:
        Patch(Patch const& other) = delete;
        Patch& operator=(Patch const& other) = delete;
        
        void*                   m_ptr;
        friend class Object;
    };
}

#endif // Z_PD_PATCH_HPP
