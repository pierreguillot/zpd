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
    instance_test *inst1, *inst2;
    patch *p1, *p2;
    try
    {
        inst1 = new instance_test(1);
    }
    catch(...)
    {
        std::cout << "error can't allocate instance";
        return -1;
    }
    try
    {
        inst2 = new instance_test(2);
    }
    catch(...)
    {
        std::cout << "error can't allocate instance";
        return -1;
    }
    
    std::cout << "version " << environment::version_major()
    << "." << environment::version_minor()
    << "." << environment::version_bug() << "\n";
    
    environment::searpath_clear();
    environment::searchpath_add(test_get_patch_folder(argv[0]));
    
    std::cout << "perform test for posts...\n";
    {
        synch::Thread t1((test_method)(&instance_test::test_post), inst1);
        synch::Thread t2((test_method)(&instance_test::test_post), inst2);
        t1.join();
        t2.join();
    }
    
    std::cout << "perform test for binding and messages...\n";
    {
        synch::Thread t1((test_method)(&instance_test::test_message), inst1);
        synch::Thread t2((test_method)(&instance_test::test_message), inst2);
        t1.join();
        t2.join();
    }
    
    /*
    p1 = inst1->load("test.pd", "");
    p2 = inst2->load("test.pd", "");
    if(p1 && p2)
    {
        inst1->prepare(2, 2, 44100, 64);
        inst2->prepare(1, 1, 44100, 128);
        
        for(size_t i = 0; i < 8; ++i)
        {
            char truc1[512];
            sprintf(truc1, "%i", int(p1->unique_id()));
            std::cout << p1->unique_id() << "\n";
            inst1->send(tie(std::string(truc1) + "-bang"), symbol("bang"), std::vector<atom>());
            inst1->perform(64, 2, const_cast<float const **>(inst1->m_vecteurs), 2, inst1->m_vecteurs);
            char truc2[512];
            sprintf(truc2, "%i", int(p2->unique_id()));
            std::cout << truc2 << "\n";
            inst2->send(tie(std::string(truc2) + "-bang"), symbol("bang"), std::vector<atom>());
            inst2->perform(128, 1, const_cast<float const **>(inst1->m_vecteurs), 1, inst1->m_vecteurs);
        }
     
        
        
        inst1->release();
        inst2->release();
        
        inst1->close(*p1);
        inst2->close(*p2);
    }
     */
    delete inst1;
    delete inst2;
    return 0;
}