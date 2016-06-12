/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "xpd_tie.hpp"

extern "C"
{
#include "../cpd/cpd.h"
}


namespace xpd
{
    
    // ==================================================================================== //
    //                                      TIE                                             //
    // ==================================================================================== //
    
    tie::tie(std::string const& name) : ptr(cpd_tie_create(name.c_str()))
    {
#define LCOV_EXCL_START
        if(!ptr)
        {
            throw "can't allocate tie.";
        }
#define LCOV_EXCL_STOP
    }
    
    tie::tie(char const* name) : ptr(cpd_tie_create(name))
    {
        if(!ptr)
#define LCOV_EXCL_START
        {
            throw "can't allocate tie.";
        }
#define LCOV_EXCL_STOP
    }
    
    tie& tie::operator=(std::string const& name)
    {
        ptr = cpd_tie_create(name.c_str());
#define LCOV_EXCL_START
        if(!ptr)
        {
            throw "can't allocate tie.";
        }
#define LCOV_EXCL_STOP
        return *this;
    }
    
    tie& tie::operator=(char const* name)
    {
        ptr = cpd_tie_create(name);
#define LCOV_EXCL_START
        if(!ptr)
        {
            throw "can't allocate tie.";
        }
#define LCOV_EXCL_STOP
        return *this;
    }
    
    std::string tie::name() const
    {
        return cpd_tie_get_name(reinterpret_cast<cpd_tie const *>(ptr));
    }
}

