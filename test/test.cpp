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
#include "Thread.hpp"

using namespace xpd;

static char* test_get_patch_folder(char* location)
{
    size_t size, i;
    if(location)
    {
        size = strlen(location);
        for(i = 0; i < size; ++i)
        {
            if(strncmp(location+i, "/build/", 7) == 0)
            {
                memset(location+i+7, '\0', size-i);
                sprintf(location+i+7, "../test/");
                return location;
            }
            if(strncmp(location+i, "/zpd/", 5) == 0)
            {
                memset(location+i+5, '\0', size-i);
                sprintf(location+i+5, "/test/");
                return location;
            }
            if(strncmp(location+i, "/test/", 6) == 0)
            {
                memset(location+i+6, '\0', size-i);
                return location;
            }
        }
    }
    return NULL;
}

typedef void (*test_method)(void *);

struct dual_instance
{
    instance_test inst1;
    instance_test inst2;
};

void test_post(dual_instance* ins)
{
    std::cout << "perform tests for console posts...";
    {
        synch::Thread t1((test_method)(&instance_test::test_post), &ins->inst1);
        synch::Thread t2((test_method)(&instance_test::test_post), &ins->inst2);
        t1.join();
        t2.join();
    }
    std::cout << "ok\n";
}

void test_binding(dual_instance* ins)
{
    std::cout << "perform tests for binding and atoms...";
    {
        synch::Thread t1((test_method)(&instance_test::test_message), &ins->inst1);
        synch::Thread t2((test_method)(&instance_test::test_message), &ins->inst2);
        t1.join();
        t2.join();
    }
    std::cout << "ok\n";
}

void test_midi(dual_instance* ins)
{
    std::cout << "perform tests for midi events...";
    {
        synch::Thread t1((test_method)(&instance_test::test_midi), &ins->inst1);
        synch::Thread t2((test_method)(&instance_test::test_midi), &ins->inst2);
        t1.join();
        t2.join();
    }
    std::cout << "ok\n";
}

void test_dsp(dual_instance* ins)
{
    std::cout << "perform tests for dsp...";
    {
        synch::Thread t1((test_method)(&instance_test::test_dsp), &ins->inst1);
        synch::Thread t2((test_method)(&instance_test::test_dsp), &ins->inst2);
        t1.join();
        t2.join();
    }
    std::cout << "ok\n";
}

void test_patch(dual_instance* ins)
{
    std::cout << "perform tests for patch, object & gui...";
    {
        synch::Thread t1((test_method)(&instance_test::test_patch), &ins->inst1);
        synch::Thread t2((test_method)(&instance_test::test_patch), &ins->inst2);
        t1.join();
        t2.join();
    }
    std::cout << "ok\n";
}

int main(int argc, char** argv)
{
    dual_instance is;
    
    std::cout << "tests xpd version " << environment::version_major()
    << "." << environment::version_minor()
    << "." << environment::version_bug() << "\n";
    
    environment::searpath_clear();
    environment::searchpath_add(test_get_patch_folder(argv[0]));
    
    std::cout << "perform tie, symbol and atom...";
    {
        type_test::test_tie();
        type_test::test_symbol();
        type_test::test_atom();
    }
    std::cout << "ok\n";
    
    synch::Thread ta((test_method)(test_post), &is);
    ta.join();
    synch::Thread tb((test_method)(test_binding), &is);
    tb.join();
    synch::Thread tc((test_method)(test_midi), &is);
    tc.join();
    synch::Thread td((test_method)(test_dsp), &is);
    td.join();
    synch::Thread te((test_method)(test_patch), &is);
    te.join();
    
    return 0;
}