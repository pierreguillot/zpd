/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_TIE_HPP
#define XPD_TIE_HPP

#include "xpd_def.hpp"
#include <string>

namespace xpd
{
    // ==================================================================================== //
    //                                      TIE                                             //
    // ==================================================================================== //
    //! @brief A tie is used as a connection to send and receive messages.
    //! @details A tie is bound to a sting so it can be used to communicate with other objects
    //! that use this string. The tie on is global to all the xpd environment. At the
    //! creation, the method looks for the sting in a global table and return a reference to
    //! the tie if name is there, otherwise the method inserts the sting in the table and
    //! associates with this new tie. If you want to communicate with a specific patch, you
    //! should use the id of a patch.
    class tie
    {
    public:
        //! @brief The std::string constructor.
        //! @details The method should never throw exceptions except if the insertion failed.
        //! @param name The name of the tie.
        tie(std::string const& name);
        
        //! @brief The c-string constructor.
        //! @details The method should never throw exceptions except if the insertion failed.
        //! @param name The name of the tie.
        tie(char const* name);
        
        //! @brief The default constructor.
        //! @details Creates an invalid tie.
        inline constexpr tie() noexcept : ptr(nullptr) {}
        
        //! @brief The copy constructor.
        //! @details Creates an invalid tie.
        //! @param The name of the tie.
        inline constexpr tie(tie const& other) noexcept : ptr(other.ptr) {}
        
        //! @brief The tie assignment.
        //! @details Returns a copy of another tie.
        //! @param other The other tie.
        //! @return The reference of the tie.
        inline tie& operator=(tie const& other) noexcept {ptr = other.ptr; return *this;}
        
        //! @brief The std::string assignment.
        //! @details Change the reference of the tie. The method should never throw exceptions
        //! except if the insertion failed.
        //! @param The new name of the tie.
        //! @return The reference of the tie.
        tie& operator=(std::string const& name);
        
        //! @brief The c-string assignment.
        //! @details Change the reference of the tie. The method should never throw exceptions
        //! except if the insertion failed.
        //! @param The new name of the tie.
        //! @return The reference of the tie.
        tie& operator=(char const* name);
        
        //! @bried Compare the tie with another.
        //! @param other The other tie.
        //! @return true if the other tie doesn't shared the same reference, otherwise false.
        inline constexpr bool operator!=(tie const& other)const noexcept {return other.ptr != ptr;}
        
        //! @bried Compare the tie with another.
        //! @param other The other tie.
        //! @return true if the other tie shared the same reference, otherwise false.
        inline constexpr bool operator==(tie const& other) const noexcept {return other.ptr == ptr;}
        
        //! @bried Checks the validity of the tie.
        //! @return true if the tie if valid, otherwise false.
        inline constexpr operator bool() const noexcept {return bool(ptr);}
        
        //! @bried Gets the name of the tie.
        //! @return The name of the tie.
        std::string name() const;
        
    private:
        void* ptr;
        friend class smuggler;
        inline constexpr void const* get() const noexcept{return ptr;}
        inline constexpr tie(void *_ptr) : ptr(_ptr) {}
    };
}

#endif // XPD_TIE_HPP
