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
#include "xpd_vector.hpp"


namespace xpd
{
    class Smuggler;
    
    template<typename T> class Point
    {
    public:
        T x;
        T y;
    };
    
    template<typename T> class Rectangle
    {
    public:
        T x;
        T y;
        T w;
        T h;
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
        inline static constexpr void const* getvector(vector const& list) noexcept {return list.ptr;}
        inline static vector createvector(void *ptr) noexcept {return vector(ptr);}
    };
}

#endif // Z_PD_TYPES_HPP
