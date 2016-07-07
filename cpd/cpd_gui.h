/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_gui_include_h
#define cpd_gui_include_h

#include "cpd_object.h"

//! @defgroup gui gui
//! @brief The gui of cpd.

//! @addtogroup gui
//! @{

//! @brief The available types of gui object.
typedef enum
{
    CPD_GUI_BANG          = 0, //!< @brief The gui is a bang.
    CPD_GUI_SLIDERH       = 1, //!< @brief The gui is an horizontal slider.
    CPD_GUI_SLIDERV       = 2, //!< @brief The gui is a vertical slider.
    CPD_GUI_TOGGLE        = 3, //!< @brief The gui is a toggle.
    CPD_GUI_NUMBER        = 4, //!< @brief The gui is a number box.
    CPD_GUI_RADIOH        = 5, //!< @brief The gui is an horizontal radio.
    CPD_GUI_RADIOV        = 6, //!< @brief The gui is an horizontal radio.
    CPD_GUI_VUMETER       = 7, //!< @brief The gui is a VU-meter.
    CPD_GUI_PANEL         = 8  //!< @brief The gui is a panel.
} cpd_guitype;


//! @brief Gets the label of a gui.
//! @param gui The gui.
//! @return The label of the gui.
CPD_EXTERN cpd_symbol* cpd_gui_get_label(cpd_gui const* gui);

//! @brief Gets the receive tie of a gui.
//! @param gui The gui.
//! @return The receive tie of the gui.
CPD_EXTERN cpd_tie* cpd_gui_get_receive_tie(cpd_gui const* gui);

//! @brief Gets the send tie of a gui.
//! @param gui The gui.
//! @return The send tie of the gui.
CPD_EXTERN cpd_tie* cpd_gui_get_send_tie(cpd_gui const* gui);

//! @brief Gets the type of a gui.
//! @param gui The gui.
//! @return The type of the gui.
CPD_EXTERN cpd_guitype cpd_gui_get_type(cpd_gui const* gui);

//! @brief Gets the maximum value of a gui.
//! @param gui The gui.
//! @return The maximum value if it has one, otherwise 0.
CPD_EXTERN float cpd_gui_get_maximum_value(cpd_gui const* gui);

//! @brief Gets the maximum value of a gui.
//! @param gui The gui.
//! @return The minimum value if it has one, otherwise 0.
CPD_EXTERN float cpd_gui_get_minimum_value(cpd_gui const* gui);

//! @brief Gets the number of steps of a gui.
//! @param gui The gui.
//! @return The number of steps of the values if it has one, otherwise 0.
CPD_EXTERN int cpd_gui_get_number_of_steps(cpd_gui const* gui);

//! @brief Gets the number of steps of a gui.
//! @param gui The gui.
//! @return The current value of the gui if it has one, otherwise 0.
CPD_EXTERN float cpd_gui_get_value(cpd_gui const* gui);

//! @brief Gets the x position of the label of a gui.
//! @param gui The gui.
//! @param patch The patch.
//! @return The x position of the label of the gui.
CPD_EXTERN int cpd_gui_get_label_x(cpd_gui const* gui, cpd_patch const* patch);

//! @brief Gets the y position of the label of a gui.
//! @param gui The gui.
//! @param patch The patch.
//! @return The y position of the label of the gui.
CPD_EXTERN int cpd_gui_get_label_y(cpd_gui const* gui, cpd_patch const* patch);

//! @}


#endif // cpd_gui_include_h
