/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "xpd_vector.hpp"

extern "C"
{
#include "../cpd/cpd.h"
}


namespace xpd
{
    // ==================================================================================== //
    //                                      ATOM                                            //
    // ==================================================================================== //
    
    atom& atom::operator=(float_t value)
    {
        cpd_atom_set_float(reinterpret_cast<z_atom *>(ptr), value);
        return *this;
    }
    
    atom& atom::operator=(symbol& symbol)
    {
        cpd_atom_set_symbol(reinterpret_cast<z_atom *>(ptr), reinterpret_cast<z_symbol *>(symbol.ptr));
        return *this;
    }
    
    atom::operator float_t() const noexcept
    {
        return cpd_atom_get_float(reinterpret_cast<z_atom *>(ptr));
    }
    
    atom::operator symbol() const noexcept
    {
        return cpd_atom_get_symbol(reinterpret_cast<z_atom *>(ptr));
    }
    
    atom::type_t atom::type() const noexcept
    {
        z_atomtype const typ = cpd_atom_get_type(reinterpret_cast<z_atom *>(ptr));
        if(typ == Z_FLOAT)
            return atom::type_t::float_t;
        if(typ == Z_SYMBOL)
            return atom::type_t::symbol_t;
        return atom::type_t::null_t;
    }
}

