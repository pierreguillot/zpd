/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef Z_PD_TYPES_HPP
#define Z_PD_TYPES_HPP

#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <memory>
#include <cassert>
#include <cstring>
#include <cmath>
#include <exception>

#if (__cplusplus <= 199711L)
#define noexcept
#define nullptr NULL
#define constexpr
#define override
#else
#include <atomic>
#include <mutex>
#endif

#include "xpd_tie.hpp"
#include "xpd_symbol.hpp"


namespace xpd
{
    class Smuggler;
    class List;
    
    
    // ==================================================================================== //
    //                                      GPOINTER                                        //
    // ==================================================================================== //
    //! @brief A gpointer.
    class Gpointer
    {
    public:
        inline constexpr Gpointer() : ptr(nullptr) {}
        inline constexpr Gpointer(Gpointer const& other) : ptr(other.ptr) {}
        inline Gpointer& operator=(Gpointer const& other) {ptr = other.ptr; return *this;}
        inline bool operator!=(Gpointer const& other)const noexcept {return other.ptr != ptr;}
        inline bool operator==(Gpointer const& other) const noexcept{return other.ptr == ptr;}
        
    private:
        void* ptr;
        friend class Smuggler;
        friend class List;
        inline constexpr void const* get() const noexcept{return ptr;}
        inline constexpr Gpointer(void *_ptr) : ptr(_ptr) {}
    };
    
    // ==================================================================================== //
    //                                      LIST                                          //
    // ==================================================================================== //
    //! @brief A list.
    class List
    {
    public:
        enum class Type
        {
            Nothing,
            Float,
            symbol,
            Gpointer
        };
        List();
        List(size_t size);
        List(List const& other);
        List(List&& other);
        ~List();
        List& operator=(List const& other);
        List& operator=(List&& other);
        void resize(size_t size);
        size_t getSize() const noexcept;
        Type getType(size_t index) const;
        float getFloat(size_t index) const;
        symbol getsymbol(size_t index) const;
        Gpointer getGpointer(size_t index) const;
        void setFloat(size_t index, float value);
        void setsymbol(size_t index, symbol& symbol);
        void setGpointer(size_t index, Gpointer& pointer);
    private:
        void* ptr;
        friend class Smuggler;
        inline void const* get() const noexcept{return ptr;}
        inline List(void *_ptr) : ptr(_ptr) {}
    };
    
    
    //! @brief The smuggler is optimized for internal use.
    //! @details The class doesn't break the efficiency of creation of some type, but you
    //! should use it if and only if you know what you do.
    class Smuggler
    {
    public:
        ~Smuggler() noexcept {}
    protected:
        inline static constexpr void const* gettie(tie const& tie) noexcept {return tie.ptr;}
        inline static constexpr tie createtie(void *ptr) noexcept {return tie(ptr);}
        inline static constexpr void const* getsymbol(symbol const& symbol) noexcept {return symbol.ptr;}
        inline static constexpr symbol createsymbol(void *ptr) noexcept {return symbol(ptr);}
        inline static constexpr void const* getGpointer(Gpointer const& gpointer) noexcept {return gpointer.ptr;}
        inline static constexpr Gpointer createGpointer(void *ptr) noexcept {return Gpointer(ptr);}
        
        inline static constexpr void const* getList(List const& list) noexcept {return list.ptr;}
        inline static List createList(void *ptr) noexcept {return List(ptr);}
    };
}

#endif // Z_PD_TYPES_HPP
