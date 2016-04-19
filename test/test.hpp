/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../xpd/xpd.hpp"
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>

using namespace xpd;

#ifndef XPD_TEST_HPP
#define XPD_TEST_HPP

#define XPD_TEST_NLOOP 100

class instance_test : private instance
{
public:
    instance_test(const size_t index) : m_index(index)
    {
        m_samples = new float*[2];
        m_samples[0] = new float[128];
        m_samples[1] = new float[128];
    }
    
    ~instance_test()
    {
        delete [] m_samples[0];
        delete [] m_samples[1];
        delete [] m_samples;
    }
    
    void receive(console::post post) xpd_final
    {
        if(post.type == console::fatal) {
            m_count_fatal++;
        }
        else if(post.type == console::error) {
            m_count_error++;
        }
        else if(post.type == console::normal) {
            m_count_normal++;
        }
        else {
            m_count_log++;
        }
    }
    
    void receive(tie tie, symbol s, std::vector<atom> atoms) xpd_final
    {
        if(s == symbol::bang_s) {
            m_count_bang++;
        }
        else if(s == symbol::float_s) {
            m_count_float++;
        }
        else if(s == symbol::symbol_s) {
            m_count_symbol++;
        }
        else if(s == symbol::list_s) {
            m_count_list++;
        }
        else {
            m_count_anything++;
        }
    }
    
    void receive(midi::event event)
    {
        ;
    }
    
    static void test_post(instance_test* inst)
    {
        inst->m_count_log = 0;
        inst->m_count_normal = 0;
        inst->m_count_error = 0;
        inst->m_count_fatal = 0;
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            inst->send(console::post(console::log, "test"));
            inst->send(console::post(console::normal, "test"));
            inst->send(console::post(console::error, "test"));
            inst->send(console::post(console::fatal, "test"));
        }
        assert("test_post log" && inst->m_count_log == XPD_TEST_NLOOP);
        assert("test_post normal" && inst->m_count_normal == XPD_TEST_NLOOP);
        assert("test_post error" && inst->m_count_error == XPD_TEST_NLOOP);
        assert("test_post fatal" && inst->m_count_fatal == XPD_TEST_NLOOP);
    }
    
    static void test_message(instance_test* inst)
    {
        char uid[512];
        patch* p = inst->load("test.pd", "");
        assert("test_message patch" && p);
        sprintf(uid, "%i", int(p->unique_id()));
        tie from(std::string(uid) + "-fromxpd");
        tie to(std::string(uid) + "-toxpd");
        inst->bind(to);
        
        inst->m_count_bang = 0;
        inst->m_count_float = 0;
        inst->m_count_symbol = 0;
        inst->m_count_list = 0;
        inst->m_count_anything = 0;
        symbol zozo("zozo");
        std::vector<atom> vec;
        vec.reserve(3);
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            vec.clear();
            inst->send(from, symbol::bang_s, vec);
            vec.push_back(1.f);
            inst->send(from, symbol::float_s, vec);
            vec[0] = zozo;
            inst->send(from, symbol::symbol_s, vec);
            vec[0] = 1.f;
            vec.push_back(1.f);
            vec.push_back(zozo);
            inst->send(from, symbol::list_s, vec);
            vec[0] = zozo;
            inst->send(from, symbol("zaza"), vec);
        }
        inst->unbind(to);
        assert("test_message bang" && inst->m_count_bang == XPD_TEST_NLOOP);
        assert("test_message float" && inst->m_count_float == XPD_TEST_NLOOP);
        assert("test_message symbol" && inst->m_count_symbol == XPD_TEST_NLOOP);
        assert("test_message list" && inst->m_count_list == XPD_TEST_NLOOP);
        assert("test_message anything" && inst->m_count_anything == XPD_TEST_NLOOP);
        inst->close(*p);
    }
    
private:
    const size_t m_index;
    size_t  m_ninouts;
    float** m_samples;
    
    size_t m_count_fatal;
    size_t m_count_error;
    size_t m_count_normal;
    size_t m_count_log;
    
    size_t m_count_bang;
    size_t m_count_float;
    size_t m_count_symbol;
    size_t m_count_list;
    size_t m_count_anything;
};


#endif // XPD_TEST_HPP
