/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_GUI_HPP
#define XPD_GUI_HPP

#include "xpd_object.hpp"

namespace xpd
{
    //! @brief The gui.
    //! @details The gui is a specification of an object.
    class gui : public object
    {
    public:
        
        //! @brief The available type of gui.
        enum type_t
        {
            bang                = 0, //!< @brief The gui is a bang.
            slider_horizontal   = 1, //!< @brief The gui is an horizontal slider.
            slider_vertical     = 2, //!< @brief The gui is a vertical slider.
            toggle              = 3, //!< @brief The gui is a toggle.
            number              = 4, //!< @brief The gui is a number box.
            radio_horizontal    = 5, //!< @brief The gui is an horizontal radio.
            radio_vertical      = 6, //!< @brief The gui is a vertical radio.
            vu_meter            = 7, //!< @brief The gui is a VU-meter.
            panel               = 8  //!< @brief The gui is a panel.
        };
        
        inline xpd_constexpr gui() xpd_noexcept : object() {}
        
        inline xpd_constexpr gui(gui const& other) xpd_noexcept : object(other) {}
        
        gui(object const& other);
        
        inline gui& operator=(gui const& other) xpd_noexcept {object::operator=(other); return *this;}
        
        gui& operator=(object const& other);
        
        //! @brief The destructor.
        inline virtual ~gui() xpd_noexcept {};
        
        type_t type() const xpd_noexcept;
        
        symbol receive_tie() const xpd_noexcept;
        
        symbol send_tie() const xpd_noexcept;
        
        symbol label() const xpd_noexcept;
        
        float minimum() const xpd_noexcept;
        
        float maximum() const xpd_noexcept;
        
        float value() const xpd_noexcept;
        
        size_t nsteps() const xpd_noexcept;
    };
}

#endif // XPD_GUI_HPP
