/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../xpd/xpd.hpp"
#include <iostream>
#include <string>
#include <cassert>

#ifndef XPD_TEST_TYPES_HPP
#define XPD_TEST_TYPES_HPP

namespace test
{
    class tester
    {
        tester(const char* name) : m_name(name), m_count(0), m_errors(0) {}
        virtual ~tester() {}
        
        virtual void perform() = 0;
        
        void check_true(const char* mess, bool status)
        {
            m_count++;
            if(!status)
            {
                m_errors = m_errors << m_count;
            }
        }
        
    private:
        std::string m_name;
        size_t      m_count;
        size_t      m_errors;
    };
    
    class tie
    {
    public:
        static void perform()
        {
            xpd::tie t0;
            xpd::tie t1 = "zaza";
            xpd::tie t2 = std::string("zaza");
            xpd::tie t3 = t2;
            assert("test::tie 1" && !t0 && t1 && t1 == t2 && t2 == t3);
            assert("test::tie 2" && !t0 && t1.name() == "zaza" && t2.name() == "zaza" && t3.name() == "zaza");
            t0 = "zozo";
            t1 = std::string("blab");
            t2 = t0;
            t3 = t3;
            assert("test::tie 3 " && t0 && t0 == t2 && t1 != t2 && t1 != t0 && t3 == t3);
            assert("test::tie 4" && t0.name() == "zozo" && t1.name() == "blab" && t2.name() == "zozo" && t3.name() == "zaza");
        }
    };
    
    class symbol
    {
    public:
        static void perform()
        {
            xpd::symbol t0;
            xpd::symbol t1 = "zaza";
            xpd::symbol t2 = std::string("zaza");
            xpd::symbol t3 = t2;
            assert("test::symbol 1" && !t0 && t1 && t1 == t2 && t2 == t3);
            assert("test::symbol 2" && !t0 && t1.name() == "zaza" && t2.name() == "zaza" && t3.name() == "zaza");
            t0 = "zozo";
            t1 = std::string("blab");
            t2 = t0;
            t3 = t3;
            assert("test::symbol 3" && t0 && t0 == t2 && t1 != t2 && t1 != t0 && t3 == t3);
            assert("test::symbol 4" && t0.name() == "zozo" && t1.name() == "blab" && t2.name() == "zozo" && t3.name() == "zaza");
        }
    };
    
    class atom
    {
    public:
        static void perform()
        {
            xpd::atom a0;
            assert("test::atom 1" && a0.type() == xpd::atom::null_t);
            xpd::atom a1(1.f);
            assert("test::atom 2" && a1.type() == xpd::atom::float_t && float(a1) == 1.f);
            xpd::atom a2(xpd::symbol("zozo"));
            assert("test::atom 3" && a2.type() == xpd::atom::symbol_t && xpd::symbol(a2) == xpd::symbol("zozo"));
        }
    };
    
    class types
    {
    public:
        static void perform()
        {
            std::cout << "perform test for types... ";
            tie::perform();
            symbol::perform();
            atom::perform();
            std::cout << "ok\n";
        }
    };
}


#endif // XPD_TEST_TYPES_HPP
