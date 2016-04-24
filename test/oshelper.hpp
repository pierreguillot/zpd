/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../xpd/xpd.hpp"
#include <string>
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#ifndef XPD_OSHELPER_HPP
#define XPD_OSHELPER_HPP

namespace oshelper
{
    class directory
    {
    public:
#ifdef _WIN32
        static const char separator = '\\';
#else
        static const char separator = '/';
#endif
        inline directory() : m_path() {}
        inline directory(std::string const& fullpath) : m_path(fullpath) {}
        inline directory(const char* fullpath) : m_path(fullpath) {}
        inline directory(directory const& other) : m_path(other.m_path) {}
        inline directory& operator=(std::string const& fullpath) {m_path = fullpath; return *this;}
        inline directory& operator=(const char* fullpath) {m_path = fullpath; return *this;}
        inline directory& operator=(directory const& other) {m_path = other.m_path;return *this;}
        inline operator bool() {return !m_path.empty();}
        
        inline std::string path() const
        {
            std::string::size_type const pos = m_path.find_last_of(separator);
            if(pos != std::string::npos) {
                return m_path.substr(0, pos+1);
            }
            return std::string();
        }
        
        inline std::string fullpath() const {return m_path;}
        
        inline std::string name() const
        {
            std::string::size_type const pos = m_path.find_last_of(separator);
            if(pos != std::string::npos) {
                return m_path.substr(pos+1);
            }
            return m_path;
        }
        
        inline directory parent() const
        {
            std::string p = path();
            if(!p.empty() && p[p.size()-1] == separator)
            {
                p.erase(p.end()-1);
            }
            if(!p.empty())
            {
                return directory(p);
            }
            return directory();
        }
        
        static inline directory current()
        {
            char cwd[1024];
#ifdef _WIN32
            if(_getcwd(cwd, sizeof(cwd)) != NULL)
            {
                return directory dir(cwd);
            }
#else
            if(getcwd(cwd, sizeof(cwd)) != NULL)
            {
                return directory(cwd);
            }
            return directory();
#endif
        }
    private:
        std::string m_path;
    };
}

#endif // XPD_OSHELPER_HPP
