/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_CONSOLE_HPP
#define XPD_CONSOLE_HPP

#include "PdTypes.hpp"
#include <vector>

namespace xpd
{
    //! @brief A class that manages the console
    class console
    {
    public:
        enum class level
        {
            all = -1,
            fatal = 0,
            error = 1,
            post = 2,
            log = 3
        };
        
        //! @brief A small class that describes a message in the console
        class message
        {
        public:
            level       lvl;
            std::string txt;
        };
        
        //! @brief A small class that manages the history of message in the console
        class history
        {
        public:
            //! @brief the constructor.
            history();
            
            //! @brief Gets the number of fatal messages.
            size_t get_number_of_messages(level lvl = level::all) const noexcept;
            
            //! @brief Gets the number of messages until a level.
            size_t get_number_of_messages_until(level lvl) const noexcept;
            
            //! @brief Gets a message at an index.
            message get_message(size_t index, level lvl = level::all) const;
            
            //! @brief Gets a message at an index until a level.
            message get_message_until(size_t index, level lvl) const;
            
            //! @brief Clears the history.
            void clear() noexcept;
            
            //! @brief Adds a message in the history.
            void add(message mess) noexcept;
            
        private:
            size_t               m_fatal_count;
            size_t               m_error_count;
            size_t               m_post_count;
            size_t               m_log_count;
            std::vector<message> m_messages;
        };
    };
}

#endif //XPD_CONSOLE_HPP
