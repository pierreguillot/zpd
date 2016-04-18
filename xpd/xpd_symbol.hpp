/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_SYMBOL_HPP
#define XPD_SYMBOL_HPP

#include <exception>
#include <string>

namespace xpd
{
    class Smuggler;
    class List;
    // ==================================================================================== //
    //                                      SYMBOL                                          //
    // ==================================================================================== //
    //! @brief A symbol is used as for fast comparaison of string with Pure Data.
    //! @details A symbol is bound to a sting. The tie on is global to all the xpd
    //! environment. At the creation, the method looks for the sting in a global table and
    //! return a reference to the tie if name is there, otherwise the method inserts the sting
    //! in the table and associates with this new tie.
    class symbol
    {
    public:
        //! @brief The std::string constructor.
        //! @details The method should never throw exceptions except if the insertion failed.
        //! @param name The name of the symbol.
        symbol(std::string const& name);
        
        symbol(char const* name);
        inline constexpr symbol() noexcept : ptr(nullptr) {}
        inline constexpr symbol(symbol const& other) noexcept : ptr(other.ptr) {}
        inline symbol& operator=(symbol const& other) noexcept {ptr = other.ptr; return *this;}
        symbol& operator=(std::string const& name);
        symbol& operator=(char const* name);
        inline constexpr bool operator!=(symbol const& other)const noexcept {return other.ptr != ptr;}
        inline constexpr bool operator==(symbol const& other) const noexcept{return other.ptr == ptr;}
        inline constexpr operator bool() const noexcept {return bool(ptr);}
        std::string name() const;
        
    private:
        void* ptr;
        friend class Smuggler;
        friend class List;
        inline constexpr void const* get() const noexcept{return ptr;}
        inline constexpr symbol(void *_ptr) : ptr(_ptr) {}
    };
}

#endif // XPD_SYMBOL_HPP
