/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_ATOM_HPP
#define XPD_ATOM_HPP

#include "xpd_symbol.hpp"

namespace xpd
{
    // ==================================================================================== //
    //                                      ATOM                                            //
    // ==================================================================================== //
    //! @brief An atom is simple variant object that can be a float or a symbol.
    //! @details An atom is used to communicate with the objects and patches within the xpd
    //! environment. The interface mostly use a std::vector of atom.
    class atom
    {
    public:
        //! @brief The available type of an atom.
        //! @details For the moment it can be null_t, float_t or symbol_t
        enum type_t
        {
            null_t  = 0,   //!< @brief The atom is null or undefined.
            float_t = 1,   //!< @brief The atom owns a float value.
            symbol_t= 2    //!< @brief The atom owns a symbol.
        };
        
        //! @brief The default constructor.
        //! @details Creates an null atom.
        inline xpd_constexpr atom() xpd_noexcept : m_type(null_t), m_float(0.f), m_symbol() {}
        
        //! @brief The float constructor.
        //! @details Creates an float atom.
        //! @param value The float value.
        inline xpd_constexpr atom(float value) xpd_noexcept : m_type(float_t), m_float(value), m_symbol(){}
        
        //! @brief The symbol constructor.
        //! @details Creates an symbol atom.
        //! @param sym The symbol.
        inline xpd_constexpr atom(symbol const& sym) xpd_noexcept : m_type(symbol_t), m_float(0.f), m_symbol(sym) {}
        
        //! @brief The copy constructor.
        //! @details Creates an copy of another atom.
        //! @param other The other atom.
        inline xpd_constexpr atom(atom const& other) xpd_noexcept : m_type(other.m_type), m_float(other.m_float), m_symbol(other.m_symbol) {}
        
        //! @brief The float assignment.
        //! @details Sets the atom to a new float value.
        //! @param value The float value.
        //! @return The reference of the atom.
        inline atom& operator=(float value) xpd_noexcept {m_type = float_t; m_float = value; m_symbol = symbol(); return *this;}
        
        //! @brief The symbol assignment.
        //! @details Sets the atom to a new symbol.
        //! @param sym The symbol.
        //! @return The reference of the atom.
        inline atom& operator=(symbol const& sym) xpd_noexcept {m_type = symbol_t; m_float = 0.f; m_symbol = sym; return *this;}
        
        //! @brief The copy assignment.
        //! @details Returns a copy of another atom.
        //! @param other The other atom.
        //! @return The reference of the atom.
        inline atom& operator=(atom const& other) {m_type = other.m_type; m_float = other.m_float; m_symbol = other.m_symbol; return *this;}
        
        
        //! @brief Gets the float value of the atom.
        //! @details Returns the float value of the atom if the type if float_t otherwise the
        //! behavior is undefined.
        //! @return The float value of the atom.
       inline xpd_constexpr operator float() const xpd_noexcept {return m_float;}
        
        //! @brief Gets the symbol of the atom.
        //! @details Returns the symbol of the atom if the type if symbol_t otherwise the
        //! behavior is undefined.
        //! @return The symbol of the atom.
       inline xpd_constexpr operator symbol() const xpd_noexcept {return m_symbol;}
        
        //! @brief Gets the type of the atom.
        //! @details Returns the current type of the atom.
        //! @return The type of the atom.
        inline xpd_constexpr atom::type_t type() const xpd_noexcept {return m_type;}
    private:
        type_t m_type;
        float  m_float;
        symbol m_symbol;
    };
}


#endif // XPD_ATOM_HPP
