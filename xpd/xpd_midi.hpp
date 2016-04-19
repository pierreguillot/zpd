/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_MIDI_HPP
#define XPD_MIDI_HPP

#include "xpd_def.hpp"

namespace xpd
{

    //! @brief A class that manages midi messages.
    //! @detail The class acts like a namespace for event.
    class midi
    {
    public:
        
        //! @brief The class stores the informations of a midi event.
        //! @detail The class acts like a namespace for event.
        class event
        {
        public:
            //! @brief The available type of midi events.
            //! @details The type is used .
#if (__cplusplus <= 199711L)
            enum type_t
            {
                note_t            = 0,   //!< @brief The note (in or out) event.
                control_change_t  = 1,   //!< @brief The control change event.
                program_change_t  = 2,   //!< @brief The program change event.
                pitch_bend_t      = 3,   //!< @brief The pitch bend event.
                after_touch_t     = 4,   //!< @brief The after touch event.
                poly_after_touch_t= 5,   //!< @brief The poly after touch event.
                byte_t            = 6    //!< @brief The byte event.
            };
#else
            enum class type_t : int
            {
                note_t            = 0,   //!< @brief The note (in or out) event.
                control_change_t  = 1,   //!< @brief The control change event.
                program_change_t  = 2,   //!< @brief The program change event.
                pitch_bend_t      = 3,   //!< @brief The pitch bend event.
                after_touch_t     = 4,   //!< @brief The after touch event.
                poly_after_touch_t= 5,   //!< @brief The poly after touch event.
                byte_t            = 6    //!< @brief The byte event.
            };
#endif
            
            //! @brief The event raw constructor.
            inline xpd_constexpr event(type_t t, int v1, int v2, int v3) xpd_noexcept :
            m_type(t), m_val1(v1), m_val2(v2), m_val3(v3) {}
            
            //! @brief The note event constructor.
            static inline xpd_constexpr event note(int channel, int pitch, int velocity) xpd_noexcept
            {return event(note_t, channel, pitch, velocity);}
            
            //! @brief The control change event constructor.
            static inline xpd_constexpr event control_change(int channel, int controler, int value) xpd_noexcept
            {return event(control_change_t, channel, controler, value);}
            
            //! @brief The program change event constructor.
            static inline xpd_constexpr event program_change(int channel, int program) xpd_noexcept
            {return event(control_change_t, channel, program, 0);}
            
            //! @brief The pitch bend event constructor.
            static inline xpd_constexpr event pitch_bend(int channel, int bend) xpd_noexcept
            {return event(pitch_bend_t, channel, bend, 0);}
            
            //! @brief The pitch bend event constructor.
            static inline xpd_constexpr event after_touch(int channel, int value) xpd_noexcept
            {return event(after_touch_t, channel, 0, value);}
            
            //! @brief The pitch bend event constructor.
            static inline xpd_constexpr event poly_after_touch(int channel, int pitch, int value) xpd_noexcept
            {return event(after_touch_t, channel, pitch, value);}
            
            //! @brief The pitch bend event constructor.
            static inline xpd_constexpr event byte(int value) xpd_noexcept
            {return event(after_touch_t, 0, 0, value);}
            
            //! @brief Gets the type of the event.
           inline xpd_constexpr type_t type() const xpd_noexcept {return m_type;}
            
            //! @brief Gets the channel value.
           inline xpd_constexpr int channel() const xpd_noexcept {return m_val1;}
            
            //! @brief Gets the pitch value.
           inline xpd_constexpr int pitch() const xpd_noexcept {return m_val2;}
            
            //! @brief Gets the velocity value.
           inline xpd_constexpr int velocity() const xpd_noexcept {return m_val3;}
            
            //! @brief Gets the controler.
           inline xpd_constexpr int controler() const xpd_noexcept {return m_val2;}
            
            //! @brief Gets the controler, after touch or poly after touch value.
           inline xpd_constexpr int value() const xpd_noexcept {return m_val3;}
            
            //! @brief Gets the program value.
           inline xpd_constexpr int program() const xpd_noexcept {return m_val2;}
            
            //! @brief Gets the bend value.
           inline xpd_constexpr int bend() const xpd_noexcept {return m_val2;}
            
            //! @brief Gets the after touch value.
           inline xpd_constexpr int after_touch() const xpd_noexcept {return m_val3;}
            
        private:
            type_t m_type;
            int         m_val1;
            int         m_val2;
            int         m_val3;
        };
        
    };
}

#endif // XPD_MIDI_HPP
