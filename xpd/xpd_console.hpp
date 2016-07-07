/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_CONSOLE_INCLUDE_HPP
#define XPD_CONSOLE_INCLUDE_HPP

#include "xpd_def.hpp"
#include <string>
#include <vector>

namespace xpd
{
    //! @brief A class that manages console posts and an historic.
    //! @details The class acts like a namespace, its owns three subclasses the level, the
    //! post and an history.
    class console
    {
    public:
        
        //! @brief The available level of posts.
        //! @details The level can be used to filters the posts.
        enum level
        {
            all     = 	32767,   //!< @brief All the posts.
            fatal   = 0,            //!< @brief The fatal error posts.
            error   = 1,            //!< @brief The error posts.
            normal  = 2,            //!< @brief The normal posts.
            log     = 3             //!< @brief The log posts.
        };
            
        
        //! @brief A class that describes a post.
        //! @details The post are used by the instance to send and receive post to and
        //! from the console.
        class post
        {
        public:
            level       type;   //!< @brief The level of the post.
            std::string text;   //!< @brief The text of the post.
            
            inline post(level t, std::string txt) xpd_noexcept : type(t), text(txt) {}
        };
        
        //! @brief A class that manages an history of posts.
        //! @details The history record posts and facilitates the retrieving of posts
        //! from a specified level.
        class history
        {
        public:
            //! @brief the constructor.
            //! @details Preallocte space for posts
            //! @todo later figure out if we should let the user specify the space.
            history();
            
            //! @brief Gets the number of posts of a specified level.
            //! @details The count of posts by level is optimized to avoid unecessary extra
            //! computation.
            //! @param lvl The level of the posts (default level::all).
            //! @return The number of posts from this level.
            size_t get_number_of_posts(level lvl = all) const xpd_noexcept;
            
            //! @brief Gets the number of all posts to a specified level.
            //! @details The method retrieves the number of posts that are more or equally
            //! important as the specified level.
            //! @param lvl The limit level of the posts (default level::all).
            //! @return The number of posts to this level.
            size_t get_number_of_posts_to_level(level lvl) const xpd_noexcept;
            
            //! @brief Gets a post at an index.
            //! @details If a level if specified only this level of post will be taken into
            //! account.
            //! @param index The index of the post
            //! @param lvl The level of the posts (default level::all).
            //! @return The post.
            post get_post(size_t index, level lvl = all) const;
            
            //! @brief Gets a post at an index to a specified level.
            //! @details The method takes into account all the posts that are more or
            //! equally important as the specified level.
            //! @param index The index of the post
            //! @param lvl The limit level of the posts (default level::all).
            //! @return The post.
            post get_post_to_level(size_t index, level lvl) const;
            
            //! @brief Clears the history.
            void clear() xpd_noexcept;
            
            //! @brief Adds a post in the history.
            void add(post const& mess) xpd_noexcept;
            
        private:
            size_t  m_counters[4];
            std::vector<post> m_posts;
        };
    };
}

#endif //XPD_CONSOLE_INCLUDE_HPP
