/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_MIDI_HPP
#define XPD_MIDI_HPP


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
            enum class type : int
            {
                note             = 0,   //!< @brief The note (in or out) event.
                control_change   = 1,   //!< @brief The control change event.
                program_change   = 2,   //!< @brief The program change event.
                pitch_bend       = 3,   //!< @brief The pitch bend event.
                after_touch      = 4,   //!< @brief The after touch event.
                poly_after_touch = 5,   //!< @brief The poly after touch event.
                byte = 6                //!< @brief The byte event.
            };
            
            //! @brief The event raw constructor.
            inline constexpr event(type t, int v1, int v2, int v3) noexcept :
            m_type(t), m_val1(v1), m_val2(v2), m_val3(v3) {}
            
            //! @brief The note event constructor.
            static inline constexpr event note(int channel, int pitch, int velocity) noexcept
            {return event(type::note, channel, pitch, velocity);}
            
            //! @brief The control change event constructor.
            static inline constexpr event control_change(int channel, int controler, int value) noexcept
            {return event(type::control_change, channel, controler, value);}
            
            //! @brief The program change event constructor.
            static inline constexpr event program_change(int channel, int program) noexcept
            {return event(type::control_change, channel, program, 0);}
            
            //! @brief The pitch bend event constructor.
            static inline constexpr event pitch_bend(int channel, int bend) noexcept
            {return event(type::pitch_bend, channel, bend, 0);}
            
            //! @brief The pitch bend event constructor.
            static inline constexpr event after_touch(int channel, int value) noexcept
            {return event(type::after_touch, channel, 0, value);}
            
            //! @brief The pitch bend event constructor.
            static inline constexpr event poly_after_touch(int channel, int pitch, int value) noexcept
            {return event(type::after_touch, channel, pitch, value);}
            
            //! @brief The pitch bend event constructor.
            static inline constexpr event byte(int value) noexcept
            {return event(type::after_touch, 0, 0, value);}
            
            //! @brief Gets the type of the event.
            inline constexpr type get_type() const noexcept {return m_type;}
            
            //! @brief Gets the channel value.
            inline constexpr int get_channel() const noexcept {return m_val1;}
            
            //! @brief Gets the pitch value.
            inline constexpr int get_pitch() const noexcept {return m_val2;}
            
            //! @brief Gets the velocity value.
            inline constexpr int get_velocity() const noexcept {return m_val3;}
            
            //! @brief Gets the controler.
            inline constexpr int get_controler() const noexcept {return m_val2;}
            
            //! @brief Gets the controler, after touch or poly after touch value.
            inline constexpr int get_value() const noexcept {return m_val3;}
            
            //! @brief Gets the program value.
            inline constexpr int get_program() const noexcept {return m_val2;}
            
            //! @brief Gets the bend value.
            inline constexpr int get_bend() const noexcept {return m_val2;}
            
            //! @brief Gets the after touch value.
            inline constexpr int get_after_touch() const noexcept {return m_val3;}
            
        private:
            type    m_type;
            int     m_val1;
            int     m_val2;
            int     m_val3;
        };
        
    };
}

#endif // XPD_MIDI_HPP
