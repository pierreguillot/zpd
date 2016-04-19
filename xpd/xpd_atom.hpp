/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_ATOM_HPP
#define XPD_ATOM_HPP

#include "xpd_symbol.hpp"

namespace xpd
{
    typedef float float_t;
    
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
            null_t,     //!< @brief The atom is null or undefined.
            float_t,    //!< @brief The atom owns a float value.
            symbol_t    //!< @brief The atom owns a symbol.
        };
        
        //! @brief The default constructor.
        //! @details Creates an null atom.
        inline xpd_constexpr atom() xpd_noexcept : m_type(null_t), m_word(0.f) {}
        
        //! @brief The float constructor.
        //! @details Creates an float atom.
       inline xpd_constexpr atom(float value) xpd_noexcept : m_type(float_t), m_word(value) {}
        
        //! @brief The symbol constructor.
        //! @details Creates an symbol atom.
       inline xpd_constexpr atom(symbol& symbol) xpd_noexcept : m_type(symbol_t), m_word(symbol) {}
        
        //! @brief The copy constructor.
        //! @details Creates an copy of another atom.
       inline xpd_constexpr atom(atom& other) xpd_noexcept : m_type(other.m_type), m_word(m_type == float_t ? other.m_word.w_float : other.m_word.w_symbol) {}
        
        //! @brief The float assignment.
        //! @details Sets the atom to a new float value.
        //! @param value The float value.
        //! @return The reference of the atom.
        inline atom& operator=(float value) xpd_noexcept {m_type = float_t; m_word.w_float = value; return *this;}
        
        //! @brief The symbol assignment.
        //! @details Sets the atom to a new symbol.
        //! @param symbol The symbol value.
        //! @return The reference of the atom.
        inline atom& operator=(symbol& symbol) xpd_noexcept {m_type = symbol_t; m_word.w_symbol = symbol; return *this;}
        
        //! @brief The copy assignment.
        //! @details Returns a copy of another atom.
        //! @param other The other atom.
        //! @return The reference of the atom.
        inline atom& operator=(atom& other) {m_type = other.m_type; m_type == float_t ? m_word.w_float = other.m_word.w_float : m_word.w_symbol = other.m_word.w_symbol; return *this;}
        
        
        //! @brief Gets the float value of the atom.
        //! @details Returns the float value of the atom if the type if float_t otherwise the
        //! behavior is undefined.
        //! @return The float value of the atom.
       inline xpd_constexpr operator float() const xpd_noexcept {return m_word.w_float;}
        
        //! @brief Gets the symbol of the atom.
        //! @details Returns the symbol of the atom if the type if symbol_t otherwise the
        //! behavior is undefined.
        //! @return The symbol of the atom.
       inline xpd_constexpr operator symbol() const xpd_noexcept {return m_word.w_symbol;}
        
        //! @brief Gets the type of the atom.
        //! @details Returns the current type of the atom.
        //! @return The type of the atom.
        inline xpd_constexpr atom::type_t type() const xpd_noexcept {return m_type;}
    private:

#ifdef _XPD_CPP11_NOSUPPORT_
        struct a_word
        {
            float w_float;
            symbol  w_symbol;
           inline xpd_constexpr a_word() xpd_noexcept : w_float(0.f) {};
           inline xpd_constexpr a_word(const float value) xpd_noexcept : w_float(value) {}
           inline xpd_constexpr a_word(symbol& symbol) xpd_noexcept : w_symbol(symbol) {}
            
        };
#else
        union a_word
        {
            float_t w_float;
            symbol  w_symbol;
           inline xpd_constexpr a_word() xpd_noexcept : w_float(0.f) {};
           inline xpd_constexpr a_word(const float_t value) xpd_noexcept : w_float(value) {}
           inline xpd_constexpr a_word(symbol& symbol) xpd_noexcept : w_symbol(symbol) {}
            
        };
#endif
        type_t m_type;
        a_word m_word;
    };
}

#endif // XPD_ATOM_HPP
