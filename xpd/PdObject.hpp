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
    //! @brief The Pure Data Object.
    //! @details The instance is a wrapper for the Pure Data's native comment.
    //! With the default constructor, theObjectGui won't be initialized. A valid
    //! Object should be created via a patch. The Gui should be used as tempory object,
    //! because it locks the patch.
    class Object
    {
    public:
        
        //! @brief The constructor for an empty Object.
        //! @details Creates an Object that can be used as an empty reference inside
        //! another class.
        Object() noexcept;
        
        //! @brief The copy constructor.
        //! @details Creates a copy of an Object and increments his counter.
        Object(Object const& other) noexcept;
        
        //! @brief The move constructor.
        //! @details Creates a copy of an Object without incrementing his counter. The
        //! Object patch will be useless.
        Object(Object&& other) noexcept;
        
        //! @brief The copy operator.
        //! @details Copies the Object and increments his counter.
        Object& operator=(Object const& other) noexcept;
        
        //! @brief The move operator.
        //! @details Copies the Object without incrementing his counter. The other
        //! Object will be destroyed if needed.
        Object& operator=(Object&& other) noexcept;
        
        //! @brief The destructor.
        //! @details The Object will be destroyed if no other copy exists.
        virtual ~Object() noexcept;
        
        //! @brief Gets if the Object is valid.
        bool isValid() const noexcept;
        
        //! @brief The text of the Object.
        std::string getText() const;
        
        //! @brief The bounds of the Object.
        Rectangle<int> getBounds() const noexcept;
        
    protected:
        
        void* getpatchPtr() const noexcept;
        
        void* getPtr() const noexcept;
        
        //! @brief The constructor for a new Object.
        //! @details Creates a new valid Object. You should never have to use it. Use the
        //! patch to retrieve an Object.
        Object(patch const& patch, void* ptr) noexcept;
        
    private:
        
        void*       m_ptr;
        patch       m_patch;
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
    class Gui : public Object, private smuggler
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
        
        //! @brief The constructor for an empty Object.
        //! @details Creates an Object that can be used as an empty reference inside
        //! another class.
        Gui() noexcept;
        
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
        
        Point<int> getLabelPosition() const noexcept;
        
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
