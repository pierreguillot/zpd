/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "test.hpp"
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>

#include "oshelper.hpp"
#include "test_types.hpp"

extern "C"
{
#include "../thread/src/thd.h"
}

using namespace xpd;


typedef void (*test_method)(void *);

struct dual_instance
{
    instance_test inst1;
    instance_test inst2;
};

void test_post(dual_instance* ins)
{
    thd_thread t1, t2;
    std::cout << "perform tests for console posts...";
    {
        thd_thread_detach(&t1, (thd_thread_method)(&instance_test::test_post), &ins->inst1);
        thd_thread_detach(&t2, (thd_thread_method)(&instance_test::test_post), &ins->inst2);
        thd_thread_join(&t1);
        thd_thread_join(&t2);
    }
    std::cout << "ok\n";
}

void test_binding(dual_instance* ins)
{
    thd_thread t1, t2;
    std::cout << "perform tests for binding and atoms...";
    {
        thd_thread_detach(&t1, (thd_thread_method)(&instance_test::test_message), &ins->inst1);
        thd_thread_detach(&t2, (thd_thread_method)(&instance_test::test_message), &ins->inst2);
        thd_thread_join(&t1);
        thd_thread_join(&t2);
    }
    std::cout << "ok\n";
}

void test_midi(dual_instance* ins)
{
    thd_thread t1, t2;
    std::cout << "perform tests for midi events...";
    {
        thd_thread_detach(&t1, (thd_thread_method)(&instance_test::test_midi), &ins->inst1);
        thd_thread_detach(&t2, (thd_thread_method)(&instance_test::test_midi), &ins->inst2);
        thd_thread_join(&t1);
        thd_thread_join(&t2);
    }
    std::cout << "ok\n";
}

void test_dsp(dual_instance* ins)
{
    thd_thread t1, t2;
    std::cout << "perform tests for dsp...";
    {
        thd_thread_detach(&t1, (thd_thread_method)(&instance_test::test_dsp), &ins->inst1);
        thd_thread_detach(&t2, (thd_thread_method)(&instance_test::test_dsp), &ins->inst2);
        thd_thread_join(&t1);
        thd_thread_join(&t2);
    }
    std::cout << "ok\n";
}

void test_patch(dual_instance* ins)
{
    thd_thread t1, t2;
    std::cout << "perform tests for patch, object & gui...";
    {
        thd_thread_detach(&t1, (thd_thread_method)(&instance_test::test_patch), &ins->inst1);
        thd_thread_detach(&t2, (thd_thread_method)(&instance_test::test_patch), &ins->inst2);
        thd_thread_join(&t1);
        thd_thread_join(&t2);
    }
    std::cout << "ok\n";
}

int main(int argc, char** argv)
{
    environment::initialize();
    dual_instance is;

    thd_thread ta, tb, tc, td, te;
    std::cout << "tests xpd version " << environment::version_major()
    << "." << environment::version_minor()
    << "." << environment::version_bug() << "\n";
    environment::searpath_clear();
    oshelper::directory dir = oshelper::directory::current();
    while(dir && dir.name() != "zpd")
    {
        dir = dir.parent();
    }
    if(dir)
    {
        dir = dir.fullpath() + oshelper::directory::separator + "test";
        environment::searchpath_add(dir.fullpath());
    }
    
    test::types::perform();

    thd_thread_detach(&ta, (thd_thread_method)(test_post), &is);
    thd_thread_join(&ta);
    
    thd_thread_detach(&tb, (thd_thread_method)(test_binding), &is);
    thd_thread_join(&tb);

    thd_thread_detach(&tc, (thd_thread_method)(test_midi), &is);
    thd_thread_join(&tc);
    
    thd_thread_detach(&td, (thd_thread_method)(test_dsp), &is);
    thd_thread_join(&td);
    
    thd_thread_detach(&te, (thd_thread_method)(test_patch), &is);
    thd_thread_join(&te);

    environment::clear();
    return 0;
}