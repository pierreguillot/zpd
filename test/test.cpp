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

#ifdef _WIN32 
#include <direct.h>
#else
#include <unistd.h>
#endif

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


static bool set_environment()
{
    std::string path;
    char cwd[1024];
    environment::searpath_clear();
#ifdef _WIN32
    if(_getcwd(cwd, sizeof(cwd)) == NULL)
#else
        if(getcwd(cwd, sizeof(cwd)) == NULL)
#endif
        {
            return "";
        }
    path = cwd;
    size_t pos = path.find("zpd");
    if(pos != std::string::npos)
    {
        path.erase(path.begin()+pos+4, path.end());
        
#ifdef _WIN32
        if(path[path.size()-1] == '\\')
            path.append("test\\");
        else
            path.append("\\test\\");
#else
        if(path[path.size()-1] == '/')
            path.append("test/");
        else
            path.append("/test/");
#endif
        environment::searchpath_add(path);
        std::cout << " with " << path << "\n";
        return true;
    }
    std::cout << "can't define the environment\n";
    return false;
}


int main(int argc, char** argv)
{
    dual_instance is;

    thd_thread ta, tb, tc, td, te;
    std::cout << "tests xpd version " << environment::version_major()
    << "." << environment::version_minor()
    << "." << environment::version_bug();
    
    if(!set_environment())
    {
        return 0;
    }

    std::cout << "perform tie, symbol and atom...";
    type_test::test_tie();
    type_test::test_symbol();
    type_test::test_atom();
    std::cout << "ok\n";

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

    return 0;
}