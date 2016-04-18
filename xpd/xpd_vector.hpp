/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_VECTOR_HPP
#define XPD_VECTOR_HPP

#include "xpd_symbol.hpp"

namespace xpd
{
    class Smuggler;
    typedef float float_t;
    
    class atom
    {
    public:
        enum class type_t
        {
            null_t,
            float_t,
            symbol_t
        };
        
        atom& operator=(float_t value);
        atom& operator=(symbol& symbol);
        operator float_t() const noexcept;
        operator symbol() const noexcept;
        type_t type() const noexcept;
    private:
        friend class vector;
        inline constexpr atom(void* _ptr) noexcept : ptr(_ptr) {}
        void*  ptr;
    };
    
    // ==================================================================================== //
    //                                      VECTOR                                          //
    // ==================================================================================== //
    //! @brief A vector is used to store a set of float and symbols.
    //! @details A vector is simplified container that acts like the std::vector but doesn't
    //! break the internal mechanism of messages so it is much more efficient for
    //! communicating with objects and patches.
    class vector
    {
    public:
        
        vector();
        vector(size_t size);
        vector(vector const& other);
        vector(vector&& other);
        ~vector();
        vector& operator=(vector const& other);
        vector& operator=(vector&& other);
        void resize(size_t size);
        
        size_t size() const noexcept;
        atom::type_t getType(size_t index) const;
        float_t getFloat(size_t index) const;
        symbol getsymbol(size_t index) const;
        void setFloat(size_t index, float_t value);
        void setsymbol(size_t index, symbol& symbol);
        
        atom const operator[](size_t index) const;
        atom operator[](size_t index);
        void test();
    private:
        void* ptr;
        friend class Smuggler;
        inline void const* get() const noexcept{return ptr;}
        inline vector(void *_ptr) : ptr(_ptr) {}
    };
    /*
    // ==================================================================================== //
    //                                      SYMBOL                                          //
    // ==================================================================================== //
    //! @brief A symbol is used as for fast comparaison of string with Pure Data.
    //! @details A symbol is bound to a sting. The symbol on is global to all the xpd
    //! environment. At the creation, the method looks for the sting in a global table and
    //! return a reference to the symbol if name is there, otherwise the method inserts the sting
    //! in the table and associates with this new symbol.
    class symbol
    {
    public:
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
        inline constexpr symbol() noexcept : ptr(nullptr) {}
        
        //! @brief The copy constructor.
        //! @details Creates an invalid symbol.
        //! @param The name of the symbol.
        inline constexpr symbol(symbol const& other) noexcept : ptr(other.ptr) {}
        
        //! @brief The symbol assignment.
        //! @details Returns a copy of another symbol.
        //! @param other The other symbol.
        //! @return The reference of the symbol.
        inline symbol& operator=(symbol const& other) noexcept {ptr = other.ptr; return *this;}
        
        //! @brief The std::string assignment.
        //! @details Change the reference of the symbol. The method should never throw
        //! exceptions except if the insertion failed.
        //! @param The new name of the symbol.
        //! @return The reference of the symbol.
        symbol& operator=(std::string const& name);
        
        //! @brief The c-string assignment.
        //! @details Change the reference of the symbol. The method should never throw
        //! exceptions except if the insertion failed.
        //! @param The new name of the symbol.
        //! @return The reference of the symbol.
        symbol& operator=(char const* name);
        
        //! @bried Compare the symbol with another.
        //! @param other The other symbol.
        //! @return true if the other symbol doesn't shared the same reference, otherwise
        //! false.
        inline constexpr bool operator!=(symbol const& other)const noexcept {return other.ptr != ptr;}
        
        //! @bried Compare the symbol with another.
        //! @param other The other symbol.
        //! @return true if the other symbol shared the same reference, otherwise false.
        inline constexpr bool operator==(symbol const& other) const noexcept{return other.ptr == ptr;}
        
        //! @bried Checks the validity of the symbol.
        //! @return true if the symbol if valid, otherwise false.
        inline constexpr operator bool() const noexcept {return bool(ptr);}
        
        //! @bried Gets the name of the symbol.
        //! @return The name of the symbol.
        std::string name() const;
        
    private:
        class manager;
        void* ptr;
        friend class Smuggler;
        friend class vector;
        inline constexpr void const* get() const noexcept{return ptr;}
        inline constexpr symbol(void *_ptr) : ptr(_ptr) {}
    };
     */
}

#endif // XPD_VECTOR_HPP
