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

#define XPD_TEST_NLOOP  64
#define XPD_TEST_NTHD   4

class tester : private xpd::instance, public xpd::console::history
{
public:
    void receive(xpd::console::post const& post) xpd_final
    {
        xpd::console::history::add(post);
    }
    
    static void generate(tester* inst)
    {
        inst->clear();
        /*
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            inst->send(xpd::console::post(xpd::console::log,    "log"));
            inst->send(xpd::console::post(xpd::console::normal, "normal"));
            inst->send(xpd::console::post(xpd::console::error,  "error"));
            inst->send(xpd::console::post(xpd::console::fatal,  "fatal"));
        }
         */
    }
};

TEST_CASE("post", "[instance post]")
{
    SECTION("Generate posts")
    {
        tester      inst[XPD_TEST_NTHD];
        thd_thread  thd[XPD_TEST_NTHD];
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            thd_thread_detach(thd+i, (thd_thread_method)(&tester::generate), inst+i);
        }
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            thd_thread_join(thd+i);
        }
    }
    /*
    SECTION("Constructor default")
    {
        tester      inst[XPD_TEST_NTHD];
        thd_thread  thd[XPD_TEST_NTHD];
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            thd_thread_detach(&thd[i], (thd_thread_method)(&tester::generate), &inst[i]);
        }
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            thd_thread_join(&thd[i]);
        }
        
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            CHECK(inst[i].get_number_of_posts(xpd::console::log) == XPD_TEST_NLOOP);
            CHECK(inst[i].get_number_of_posts(xpd::console::normal) == XPD_TEST_NLOOP);
            CHECK(inst[i].get_number_of_posts(xpd::console::error) == XPD_TEST_NLOOP);
            CHECK(inst[i].get_number_of_posts(xpd::console::fatal) == XPD_TEST_NLOOP);
            
            CHECK(inst[i].get_number_of_posts_to_level(xpd::console::log) == XPD_TEST_NLOOP * 4);
            CHECK(inst[i].get_number_of_posts_to_level(xpd::console::normal) == XPD_TEST_NLOOP * 3);
            CHECK(inst[i].get_number_of_posts_to_level(xpd::console::error) == XPD_TEST_NLOOP * 2);
            CHECK(inst[i].get_number_of_posts_to_level(xpd::console::fatal) == XPD_TEST_NLOOP);
            
            for(size_t i = 0; i < inst[i].get_number_of_posts(xpd::console::all); i += 4)
            {
                xpd::console::post p1 = inst->get_post(i, xpd::console::all);
                xpd::console::post p2 = inst->get_post(i+1, xpd::console::all);
                xpd::console::post p3 = inst->get_post(i+2, xpd::console::all);
                xpd::console::post p4 = inst->get_post(i+3, xpd::console::all);
                CHECK(p1.type == xpd::console::log);
                CHECK(p1.text == std::string("log\n"));
                CHECK(p2.type == xpd::console::normal);
                CHECK(p2.text == std::string("normal\n"));
                CHECK(p3.type == xpd::console::error);
                CHECK(p3.text == std::string("error\n"));
                CHECK(p4.type == xpd::console::fatal);
                CHECK(p4.text == std::string("fatal\n"));
            }
            
            for(size_t i = 0; i < inst[i].get_number_of_posts(xpd::console::log); i++)
            {
                xpd::console::post p1 = inst->get_post(i, xpd::console::log);
                CHECK(p1.type == xpd::console::log);
                CHECK(p1.text == std::string("log\n"));
            }
            
            for(size_t i = 0; i < inst[i].get_number_of_posts(xpd::console::normal); i++)
            {
                xpd::console::post p1 = inst->get_post(i, xpd::console::log);
                CHECK(p1.type == xpd::console::normal);
                CHECK(p1.text == std::string("normal\n"));
            }
            
            for(size_t i = 0; i < inst[i].get_number_of_posts(xpd::console::error); i++)
            {
                xpd::console::post p1 = inst->get_post(i, xpd::console::log);
                CHECK(p1.type == xpd::console::normal);
                CHECK(p1.text == std::string("normal\n"));
            }
            
            for(size_t i = 0; i < inst[i].get_number_of_posts(xpd::console::fatal); i++)
            {
                xpd::console::post p1 = inst->get_post(i, xpd::console::fatal);
                CHECK(p1.type == xpd::console::fatal);
                CHECK(p1.text == std::string("fatal\n"));
            }
            /*
            for(size_t i = 0; i < inst->get_number_of_posts_to_level(console::all); i += 4)
            {
                console::post p1 = inst->get_post_to_level(i, console::all);
                console::post p2 = inst->get_post_to_level(i+1, console::all);
                console::post p3 = inst->get_post_to_level(i+2, console::all);
                console::post p4 = inst->get_post_to_level(i+3, console::all);
                assert("test_post get_p log" && p1.type == console::log && p1.text == "log\n");
                assert("test_post get_p normal" && p2.type == console::normal && p2.text == "normal\n");
                assert("test_post get_p error" && p3.type == console::error && p3.text == "error\n");
                assert("test_post get_p fatal" && p4.type == console::fatal && p4.text == "fatal\n");
            }
            
            for(size_t i = 0; i < inst->get_number_of_posts_to_level(console::log); i += 4)
            {
                console::post p1 = inst->get_post_to_level(i, console::log);
                console::post p2 = inst->get_post_to_level(i+1, console::log);
                console::post p3 = inst->get_post_to_level(i+2, console::log);
                console::post p4 = inst->get_post_to_level(i+3, console::log);
                assert("test_post get_p log" && p1.type == console::log && p1.text == "log\n");
                assert("test_post get_p normal" && p2.type == console::normal && p2.text == "normal\n");
                assert("test_post get_p error" && p3.type == console::error && p3.text == "error\n");
                assert("test_post get_p fatal" && p4.type == console::fatal && p4.text == "fatal\n");
            }
            
            for(size_t i = 0; i < inst->get_number_of_posts_to_level(console::normal); i += 3)
            {
                console::post p2 = inst->get_post_to_level(i, console::normal);
                console::post p3 = inst->get_post_to_level(i+1, console::normal);
                console::post p4 = inst->get_post_to_level(i+2, console::normal);
                assert("test_post get_p normal" && p2.type == console::normal && p2.text == "normal\n");
                assert("test_post get_p error" && p3.type == console::error && p3.text == "error\n");
                assert("test_post get_p fatal" && p4.type == console::fatal && p4.text == "fatal\n");
            }
            
            for(size_t i = 0; i < inst->get_number_of_posts_to_level(console::error); i += 2)
            {
                console::post p3 = inst->get_post_to_level(i, console::error);
                console::post p4 = inst->get_post_to_level(i+1, console::error);
                assert("test_post get_p error" && p3.type == console::error && p3.text == "error\n");
                assert("test_post get_p fatal" && p4.type == console::fatal && p4.text == "fatal\n");
            }
            
            for(size_t i = 0; i < inst->get_number_of_posts_to_level(console::fatal); i++)
            {
                console::post p1 = inst->get_post_to_level(i, console::fatal);
                assert("test_post get_p fatal" && p1.type == console::fatal && p1.text == "fatal\n");
            }
     
        }
    }
     */
}

#undef XPD_TEST_NLOOP




