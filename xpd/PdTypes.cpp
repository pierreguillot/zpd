/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "PdTypes.hpp"


extern "C"
{
#include "../zpd/z_pd.h"
}


namespace xpd
{
    
    
    
    
    
    
    
    
    
    
    List::List() : ptr(z_pd_list_create(0))
    {
        ;
    }
    
    List::List(size_t size) : ptr(z_pd_list_create(size))
    {
        ;
    }
    
    List::List(List const& other) : ptr(z_pd_list_create_copy(reinterpret_cast<z_list *>(other.ptr)))
    {
        
    }
    
    List::List(List&& other) : ptr(z_pd_list_create(0))
    {
        std::swap(ptr, other.ptr);
    }
    
    List::~List()
    {
        z_pd_list_free(reinterpret_cast<z_list *>(ptr));
    }
    
    List& List::operator=(List const& other)
    {
        z_pd_list_copy(reinterpret_cast<z_list *>(ptr), reinterpret_cast<z_list const *>(other.ptr));
        return *this;
    }
    
    List& List::operator=(List&& other)
    {
        std::swap(ptr, other.ptr);
        return *this;
    }
    
    List::Type List::getType(size_t index) const
    {
        z_listtype const t = z_pd_list_get_type(reinterpret_cast<z_list *>(ptr), index);
        if(t == Z_NULL)
            return Type::Nothing;
        if(t == Z_FLOAT)
            return Type::Float;
        if(t == Z_SYMBOL)
            return Type::symbol;
        return Type::Gpointer;
    }
    
    void List::resize(size_t size)
    {
        z_pd_list_resize(reinterpret_cast<z_list *>(ptr), size);
    }
    
    float List::getFloat(size_t index) const
    {
        return z_pd_list_get_float(reinterpret_cast<z_list *>(ptr), index);
    }
    
    symbol List::getsymbol(size_t index) const
    {
        return symbol(z_pd_list_get_symbol(reinterpret_cast<z_list *>(ptr), index));
    }
    
    Gpointer List::getGpointer(size_t index) const
    {
        return Gpointer(z_pd_list_get_gpointer(reinterpret_cast<z_list *>(ptr), index));
    }
    
    size_t List::getSize() const noexcept
    {
        return z_pd_list_get_size(reinterpret_cast<z_list *>(ptr));
    }
    
    void List::setFloat(size_t index, float value)
    {
        z_pd_list_set_float(reinterpret_cast<z_list *>(ptr), index, value);
    }
    
    void List::setsymbol(size_t index, symbol& symbol)
    {
        z_pd_list_set_symbol(reinterpret_cast<z_list *>(ptr), index,
                             reinterpret_cast<z_symbol *>(symbol.ptr));
    }
    
    void List::setGpointer(size_t index, Gpointer& pointer)
    {
        z_pd_list_set_gpointer(reinterpret_cast<z_list *>(ptr), index,
                             reinterpret_cast<z_gpointer *>(pointer.ptr));
    }
}

