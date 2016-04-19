/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef Z_PD_OBJECT_HPP
#define Z_PD_OBJECT_HPP

#include "Pdpatch.hpp"

namespace xpd
{
    //! @brief The object.
    //! @details In fact this just a description of an object.
    class object
    {
    public:
        
        //! @brief The destructor.
        inline virtual ~object() {};
        
        //! @brief Gets the text of the object.
        inline std::string get_text() const {return m_text;}
        
        //! @brief Gets the x position of the object.
        inline constexpr int get_x() const noexcept {return m_x;}
        
        //! @brief Gets the y position of the object.
        inline constexpr int get_y() const noexcept {return m_y;}
        
        //! @brief Gets the width of the object.
        inline constexpr int get_width() const noexcept {return m_width;}
        
        //! @brief Gets the height of the object.
        inline constexpr int get_height() const noexcept {return m_height;}
        
    protected:
        object(patch const* patch, void* ptr) noexcept;
        
        std::string m_text;
        int         m_x;
        int         m_y;
        int         m_width;
        int         m_height;
        friend class patch;
    };
    
    // ==================================================================================== //
    //                                          GUI                                         //
    // ==================================================================================== //
    
    //! @brief The Pure Data GUI.
    //! @details The instance is a wrapper for the Pure Data's native GUI.
    //! With the default constructor, the Gui won't be initialized. A valid
    //! Gui should be created via a patch. The Gui should be used as tempory object,
    //! because it locks the patch.
    class Gui : public object
    {
    public:
        
        enum class Type : size_t
        {
            Invalid          = 0,
            HorizontalSlider = 1,
            VerticalSlider   = 2,
            Toggle           = 3,
            Number           = 4,
            HorizontalRadio  = 5,
            VerticalRadio    = 6
        };
        
        //! @brief The copy constructor.
        //! @details Creates a copy of an Object and increments his counter.
        Gui(Gui const& other) noexcept;
        
        //! @brief The move constructor.
        //! @details Creates a copy of an Object without incrementing his counter. The
        //! Object patch will be useless.
        Gui(Gui&& other) noexcept;
        
        //! @brief The copy operator.
        //! @details Copies the Object and increments his counter.
        Gui& operator=(Gui const& other) noexcept;
        
        //! @brief The move operator.
        //! @details Copies the Object without incrementing his counter. The other
        //! Object will be destroyed if needed.
        Gui& operator=(Gui&& other) noexcept;
        
        //! @brief The destructor.
        //! @details The Object will be destroyed if no other copy exists.
        virtual ~Gui() noexcept;
        
        //! @brief The class name of the Object.
        Type getType() const noexcept;
        
        //! @brief The Name of the Object.
        std::string getName() const;
        
        std::string getLabel() const;
        
        tie getReceivetie() const;
        
        bool isParameter() const noexcept;
        
        float getMinimum() const noexcept;
        
        float getMaximum() const noexcept;
        
        float getValue() const noexcept;
        
        size_t getNumberOfSteps() const noexcept;
        
        //Point<int> getLabelPosition() const noexcept;
        
    private:

        //! @brief The constructor for a new Object.
        //! @details Creates a new valid Object. You should never have to use it. Use the
        //! patch to retrieve an Object.
        Gui(patch const& patch, Type type, void* ptr) noexcept;
        
        Type        m_type;
        friend class patch;
    };
}

#endif
