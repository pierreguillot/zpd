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

int main(int argc, char** argv)
{
    instance_test *inst1 = new instance_test();
    instance_test *inst2 = new instance_test();
    
    assert("test_dsp instance_test" && inst1 && inst2);
    std::cout << "tests xpd version " << environment::version_major()
    << "." << environment::version_minor()
    << "." << environment::version_bug() << "\n";
    
    environment::searpath_clear();
    environment::searchpath_add(test_get_patch_folder(argv[0]));
    
    std::cout << "perform tests for console posts...";
    {
        synch::Thread t1((test_method)(&instance_test::test_post), inst1);
        synch::Thread t2((test_method)(&instance_test::test_post), inst2);
        t1.join();
        t2.join();
    }
    std::cout << "ok\n";
    
    std::cout << "perform tests for binding and atoms...";
    {
        synch::Thread t1((test_method)(&instance_test::test_message), inst1);
        synch::Thread t2((test_method)(&instance_test::test_message), inst2);
        t1.join();
        t2.join();
    }
    std::cout << "ok\n";
    
    std::cout << "perform tests for midi events...";
    {
        synch::Thread t1((test_method)(&instance_test::test_midi), inst1);
        synch::Thread t2((test_method)(&instance_test::test_midi), inst2);
        t1.join();
        t2.join();
    }
    std::cout << "ok\n";
    
    std::cout << "perform tests for dsp...";
    {
        synch::Thread t1((test_method)(&instance_test::test_dsp), inst1);
        synch::Thread t2((test_method)(&instance_test::test_dsp), inst2);
        t1.join();
        t2.join();
    }
    std::cout << "ok\n";
    
    std::cout << "perform tests for patchs...";
    {
        synch::Thread t1((test_method)(&instance_test::test_patch), inst1);
        synch::Thread t2((test_method)(&instance_test::test_patch), inst2);
        t1.join();
        t2.join();
    }
    std::cout << "ok\n";
    
    delete inst1;
    delete inst2;
    return 0;
}