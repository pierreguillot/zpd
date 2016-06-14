/*
 // Copyright (c) 2015-2016-2016 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include <iostream>
#include <cstdio>
#include "test.hpp"
extern "C"
{
#include "../thread/src/thd.h"
}

#define XPD_TEST_NLOOP      16
#define XPD_TEST_NTHD       4
#define XPD_TEST_BLKSIZE    256

class instance_tester : private xpd::instance
{
public:
    
    instance_tester()
    {
        char uid[512];
        
        // Post part
        m_counter_post = 0;
        m_patch_post = xpd::instance::load("test_post.pd", "");
        sprintf(uid, "%i-xpd: bang", int(m_patch_post.unique_id()));
        m_post_expected = std::string(uid);
        
        // Message part
        m_counter_message = 0;
        m_patch_message = xpd::instance::load("test_message.pd", "");
        sprintf(uid, "%i", int(m_patch_message.unique_id()));
        m_tie_from = std::string(uid) + std::string("-fromxpd");
        m_tie_to   = std::string(uid) + std::string("-toxpd");
        xpd::instance::bind(m_tie_to);
        m_sym_list = xpd::symbol("list");
        m_vector_expected.push_back(1.2f);
        m_vector_expected.push_back(xpd::symbol("zaza"));
        m_vector_expected.push_back(xpd::symbol("zozo"));
        xpd::instance::send(m_tie_from, m_sym_list, m_vector_expected);
        
        xpd::instance::prepare(2, 2, 44100, XPD_TEST_BLKSIZE);
    }

    ~instance_tester()
    {
        xpd::instance::close(m_patch_post);
        xpd::instance::close(m_patch_message);
        xpd::instance::unbind(m_tie_to);
    }
    
    // ==================================================================================== //
    //                                      POST                                            //
    // ==================================================================================== //
 
    void receive(xpd::console::post const& post) xpd_final
    {
        if(post.type == xpd::console::normal && post.text == m_post_expected)
        {
            ++m_counter_post;
        }
    }
    
    inline bool state_post() const xpd_noexcept
    {
        return m_counter_post == XPD_TEST_NLOOP;
    }
    
    // ==================================================================================== //
    //                                      POST                                            //
    // ==================================================================================== //
    
    void receive(xpd::tie name, xpd::symbol selector, std::vector<xpd::atom> const& atoms) xpd_final
    {
        if(name == m_tie_to && selector == m_sym_list && atoms.size() == 3
           && float(atoms[0]) == float(m_vector_expected[0])
           && xpd::symbol(atoms[1]) == xpd::symbol(m_vector_expected[1])
           && xpd::symbol(atoms[2]) == xpd::symbol(m_vector_expected[2]))
        {
            ++m_counter_message;
        }
    }
    
    inline bool state_message() const xpd_noexcept
    {
        return m_counter_message == XPD_TEST_NLOOP;
    }
    
    static void test(instance_tester* inst)
    {
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            inst->perform(XPD_TEST_BLKSIZE, 0, NULL, 0, NULL);
        }
    }
    
private:
    std::string m_post_expected;
    size_t      m_counter_post;
    xpd::patch  m_patch_post;
    
    xpd::tie    m_tie_from;
    xpd::tie    m_tie_to;
    xpd::symbol m_sym_list;
    size_t      m_counter_message;
    xpd::patch  m_patch_message;
    std::vector<xpd::atom> m_vector_expected;
    
    size_t      m_counter_midi;
    xpd::patch  m_patch_midi;
};

TEST_CASE("instance", "[instance post]")
{
    instance_tester inst[XPD_TEST_NTHD];
    thd_thread  thd[XPD_TEST_NTHD];
    
    for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
    {
        thd_thread_detach(thd+i, (thd_thread_method)(&instance_tester::test), inst+i);
    }
    
    SECTION("states")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            thd_thread_join(thd+i);
            CHECK(inst[i].state_post());
            CHECK(inst[i].state_message());
        }
    }
}

#undef XPD_TEST_NLOOP




