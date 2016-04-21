/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_SYMBOL_HPP
#define XPD_SYMBOL_HPP

#include "xpd_def.hpp"
#include <string>

namespace xpd
{
    // ==================================================================================== //
    //                                      SYMBOL                                          //
    // ==================================================================================== //
    
    //! @brief A symbol is used as for fast comparaison of string.
    //! @details A symbol is bound to a sting. The symbol on is global to all the xpd
    //! environment. At the creation, the method looks for the sting in a global table and
    //! return a reference to the symbol if name is there, otherwise the method inserts the sting
    //! in the table and associates with this new symbol.
    class symbol
    {
    public:
        static const symbol _s;         //!< @brief The predefined empty symbol.
        static const symbol bang_s;     //!< @brief The predefined bang symbol.
        static const symbol float_s;    //!< @brief The predefined float symbol.
        static const symbol symbol_s;   //!< @brief The predefined symbol symbol.
        static const symbol list_s;     //!< @brief The predefined list symbol.
        
        //! @brief The std::string constructor.
        //! @details The method should never throw exceptions except if the insertion failed.
        //! @param name The name of the symbol.
        symbol(std::string const& name);
        
        //! @brief The c-string constructor.
        //! @details The method should never throw exceptions except if the insertion failed.
        //! @param name The name of the symbol.
        symbol(char const* name);
        
        //! @brief The default constructor.
        //! @details Creates an invalid symbol.
       inline xpd_constexpr symbol() xpd_noexcept : ptr(xpd_nullptr) {}
        
        //! @brief The copy constructor.
        //! @details Creates an invalid symbol.
        //! @param other The other symbol.
       inline xpd_constexpr symbol(symbol const& other) xpd_noexcept : ptr(other.ptr) {}

        //! @brief The symbol assignment.
        //! @details Returns a copy of another symbol.
        //! @param other The other symbol.
        //! @return The reference of the symbol.
        inline symbol& operator=(symbol const& other) xpd_noexcept {ptr = other.ptr; return *this;}
        
        //! @brief The std::string assignment.
        //! @details Change the reference of the symbol. The method should never throw
        //! exceptions except if the insertion failed.
        //! @param name The new name of the symbol.
        //! @return The reference of the symbol.
        symbol& operator=(std::string const& name);
        
        //! @brief The c-string assignment.
        //! @details Change the reference of the symbol. The method should never throw
        //! exceptions except if the insertion failed.
        //! @param name The new name of the symbol.
        //! @return The reference of the symbol.
        symbol& operator=(char const* name);
        
        //! @brief Compare the symbol with another.
        //! @param other The other symbol.
        //! @return true if the other symbol doesn't shared the same reference, otherwise
        //! false.
       inline xpd_constexpr bool operator!=(symbol const& other)const xpd_noexcept {return other.ptr != ptr;}
        
        //! @brief Compare the symbol with another.
        //! @param other The other symbol.
        //! @return true if the other symbol shared the same reference, otherwise false.
       inline xpd_constexpr bool operator==(symbol const& other) const xpd_noexcept{return other.ptr == ptr;}
        
        //! @brief Checks the validity of the symbol.
        //! @return true if the symbol if valid, otherwise false.
       inline xpd_constexpr operator bool() const xpd_noexcept {return bool(ptr);}
        
        //! @brief Gets the name of the symbol.
        //! @return The name of the symbol.
        std::string name() const;
        
    private:
        void* ptr;
        friend class smuggler;
        inline xpd_constexpr void const* get() const xpd_noexcept{return ptr;}
        inline xpd_constexpr symbol(void *_ptr) : ptr(_ptr) {}
    };
}



#endif // XPD_SYMBOL_HPP
