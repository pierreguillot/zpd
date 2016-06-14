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
        
        xpd::instance::prepare(2, 2, 44100, XPD_TEST_BLKSIZE);
    }

    ~instance_tester()
    {
        if(m_patch_post)
        {
            xpd::instance::close(m_patch_post);
        }
    }
    
    // ==================================================================================== //
    //                                      POST                                            //
    // ==================================================================================== //
 
    void receive(xpd::console::post const& post) xpd_final
    {
        if(post.type == xpd::console::normal && post.text == m_post_message)
        {
            ++m_counter_post;
        }
        std::cout << post.type << " |" << post.text <<"|\n";
    }
    
    inline bool state_post() const xpd_noexcept
    {
        return m_counter_post == XPD_TEST_NLOOP;
    }
    
    // ==================================================================================== //
    //                                      POST                                            //
    // ==================================================================================== //
    
    void send(std::vector<xpd::atom> const& vec)
    {
        xpd::instance::send(m_tfrom, xpd::symbol("list"), vec);
    }
    
    static void test(instance_tester* inst)
    {
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            inst->perform(XPD_TEST_BLKSIZE, 0, NULL, 0, NULL);
        }
    }
    
private:
    xpd::tie   m_tfrom;
    xpd::tie   m_tto;
    
    std::string m_post_message;
    size_t      m_counter_post;
    xpd::patch  m_patch_post;
};

TEST_CASE("instance", "[instance post]")
{
    instance_tester inst[XPD_TEST_NTHD];
    thd_thread  thd[XPD_TEST_NTHD];
    
    for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
    {
        thd_thread_detach(thd+i, (thd_thread_method)(&instance_tester::test), inst+i);
    }
    
    SECTION("post")
    {
        
        
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            thd_thread_join(thd+i);
            CHECK(inst[i].state_post());
        }
    }
}

#undef XPD_TEST_NLOOP




