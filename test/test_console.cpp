/*
 // Copyright (c) 2015-2016-2016 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include <iostream>
#include "test.hpp"
extern "C"
{
#include "../thread/src/thd.h"
}

#define XPD_TEST_NLOOP  16
#define XPD_TEST_NTHD   4

class console_tester : public xpd::instance, public xpd::console::history
{
public:
    void receive(xpd::console::post const& post) xpd_final
    {
        xpd::console::history::add(post);
    }
    
    static void generate(console_tester* inst)
    {
        inst->clear();
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            inst->send(xpd::console::post(xpd::console::log,    "log"));
            inst->send(xpd::console::post(xpd::console::normal, "normal"));
            inst->send(xpd::console::post(xpd::console::error,  "error"));
            inst->send(xpd::console::post(xpd::console::fatal,  "fatal"));
        }
    }
};

TEST_CASE("console", "[console]")
{
    console_tester  inst[XPD_TEST_NTHD];
    thd_thread      thd[XPD_TEST_NTHD];
    for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
    {
        thd_thread_detach(thd+i, (thd_thread_method)(&console_tester::generate), inst+i);
    }
    for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
    {
        thd_thread_join(thd+i);
    }
    
    
    
    SECTION("get number of posts")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            CHECK(inst[i].get_number_of_posts(xpd::console::log) == XPD_TEST_NLOOP);
            CHECK(inst[i].get_number_of_posts(xpd::console::normal) == XPD_TEST_NLOOP);
            CHECK(inst[i].get_number_of_posts(xpd::console::error) == XPD_TEST_NLOOP);
            CHECK(inst[i].get_number_of_posts(xpd::console::fatal) == XPD_TEST_NLOOP);
        }
    }
    
    
    SECTION("get post all")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            for(size_t j = 0; j < inst[i].get_number_of_posts(xpd::console::all); j += 4)
            {
                xpd::console::post p1 = inst->get_post(j, xpd::console::all);
                xpd::console::post p2 = inst->get_post(j+1, xpd::console::all);
                xpd::console::post p3 = inst->get_post(j+2, xpd::console::all);
                xpd::console::post p4 = inst->get_post(j+3, xpd::console::all);
                CHECK(p1.type == xpd::console::log);
                CHECK(p1.text == std::string("log"));
                CHECK(p2.type == xpd::console::normal);
                CHECK(p2.text == std::string("normal"));
                CHECK(p3.type == xpd::console::error);
                CHECK(p3.text == std::string("error"));
                CHECK(p4.type == xpd::console::fatal);
                CHECK(p4.text == std::string("fatal"));
            }
        }
    }
    
    SECTION("get post log")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            for(size_t j = 0; j < inst[i].get_number_of_posts(xpd::console::log); j++)
            {
                xpd::console::post p1 = inst->get_post(j, xpd::console::log);
                CHECK(p1.type == xpd::console::log);
                CHECK(p1.text == std::string("log"));
            }
        }
    }
    
    SECTION("get post normal")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            for(size_t j = 0; j < inst[i].get_number_of_posts(xpd::console::normal); j++)
            {
                xpd::console::post p1 = inst->get_post(j, xpd::console::normal);
                CHECK(p1.type == xpd::console::normal);
                CHECK(p1.text == std::string("normal"));
            }
        }
    }
    
    SECTION("get post error")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            for(size_t j = 0; j < inst[i].get_number_of_posts(xpd::console::error); j++)
            {
                xpd::console::post p1 = inst->get_post(j, xpd::console::error);
                CHECK(p1.type == xpd::console::error);
                CHECK(p1.text == std::string("error"));
            }
        }
    }
    
    SECTION("get post fatal")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            for(size_t j = 0; j < inst[i].get_number_of_posts(xpd::console::fatal); j++)
            {
                xpd::console::post p1 = inst->get_post(j, xpd::console::fatal);
                CHECK(p1.type == xpd::console::fatal);
                CHECK(p1.text == std::string("fatal"));
            }
        }
    }
    
    SECTION("get number of posts to level")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            CHECK(inst[i].get_number_of_posts_to_level(xpd::console::log) == XPD_TEST_NLOOP * 4);
            CHECK(inst[i].get_number_of_posts_to_level(xpd::console::normal) == XPD_TEST_NLOOP * 3);
            CHECK(inst[i].get_number_of_posts_to_level(xpd::console::error) == XPD_TEST_NLOOP * 2);
            CHECK(inst[i].get_number_of_posts_to_level(xpd::console::fatal) == XPD_TEST_NLOOP);
        }
    }
    
    SECTION("get post to level all")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            for(size_t j = 0; j < inst[i].get_number_of_posts_to_level(xpd::console::all); j += 4)
            {
                xpd::console::post p1 = inst->get_post_to_level(j, xpd::console::all);
                xpd::console::post p2 = inst->get_post_to_level(j+1, xpd::console::all);
                xpd::console::post p3 = inst->get_post_to_level(j+2, xpd::console::all);
                xpd::console::post p4 = inst->get_post_to_level(j+3, xpd::console::all);
                CHECK(p1.type == xpd::console::log);
                CHECK(p1.text == std::string("log"));
                CHECK(p2.type == xpd::console::normal);
                CHECK(p2.text == std::string("normal"));
                CHECK(p3.type == xpd::console::error);
                CHECK(p3.text == std::string("error"));
                CHECK(p4.type == xpd::console::fatal);
                CHECK(p4.text == std::string("fatal"));
            }
        }
    }
    
    SECTION("get post to level log")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            for(size_t j = 0; j < inst[i].get_number_of_posts_to_level(xpd::console::log); j += 4)
            {
                xpd::console::post p1 = inst->get_post_to_level(j, xpd::console::log);
                xpd::console::post p2 = inst->get_post_to_level(j+1, xpd::console::log);
                xpd::console::post p3 = inst->get_post_to_level(j+2, xpd::console::log);
                xpd::console::post p4 = inst->get_post_to_level(j+3, xpd::console::log);
                CHECK(p1.type == xpd::console::log);
                CHECK(p1.text == std::string("log"));
                CHECK(p2.type == xpd::console::normal);
                CHECK(p2.text == std::string("normal"));
                CHECK(p3.type == xpd::console::error);
                CHECK(p3.text == std::string("error"));
                CHECK(p4.type == xpd::console::fatal);
                CHECK(p4.text == std::string("fatal"));
            }
        }
    }
    
    SECTION("get post to level normal")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            for(size_t j = 0; j < inst[i].get_number_of_posts_to_level(xpd::console::normal); j += 3)
            {
                xpd::console::post p2 = inst->get_post_to_level(j, xpd::console::normal);
                xpd::console::post p3 = inst->get_post_to_level(j+1, xpd::console::normal);
                xpd::console::post p4 = inst->get_post_to_level(j+2, xpd::console::normal);
                CHECK(p2.type == xpd::console::normal);
                CHECK(p2.text == std::string("normal"));
                CHECK(p3.type == xpd::console::error);
                CHECK(p3.text == std::string("error"));
                CHECK(p4.type == xpd::console::fatal);
                CHECK(p4.text == std::string("fatal"));
            }
        }
    }
    
    SECTION("get post to level error")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            for(size_t j = 0; j < inst[i].get_number_of_posts_to_level(xpd::console::error); j += 2)
            {
                xpd::console::post p3 = inst->get_post_to_level(j, xpd::console::error);
                xpd::console::post p4 = inst->get_post_to_level(j+1, xpd::console::error);
                CHECK(p3.type == xpd::console::error);
                CHECK(p3.text == std::string("error"));
                CHECK(p4.type == xpd::console::fatal);
                CHECK(p4.text == std::string("fatal"));
            }
        }
    }
    
    SECTION("get post to level fatal")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            for(size_t j = 0; j < inst[i].get_number_of_posts_to_level(xpd::console::fatal); j++)
            {
                xpd::console::post p4 = inst->get_post_to_level(j, xpd::console::fatal);
                CHECK(p4.type == xpd::console::fatal);
                CHECK(p4.text == std::string("fatal"));
            }
        }
    }
}

#undef XPD_TEST_NLOOP
#undef XPD_TEST_NTHD




