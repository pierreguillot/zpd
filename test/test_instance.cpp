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

class instance_tester : private xpd::instance
{
public:
    instance_tester() : m_counter(0ul)
    {
        ;
    }
    
    ~instance_tester()
    {
        if(m_patch)
        {
            xpd::instance::close(m_patch);
        }
    }
    
    bool load(std::string const& name)
    {
        if(m_patch)
        {
            xpd::instance::close(m_patch);
        }
        if(m_tto)
        {
            xpd::instance::unbind(m_tto);
        }
        m_patch = xpd::instance::load(name, "");
        if(bool(m_patch))
        {
            char uid[512];
            sprintf(uid, "%i", int(m_patch.unique_id()));
            m_tfrom = std::string(uid) + std::string("-fromxpd");
            m_tto   = std::string(uid) + std::string("-toxpd");
            xpd::instance::bind(m_tto);
            return true;
        }
        return false;
    }
    
    bool prepare(const int nins, const int nouts, const int samplerate, const int nsamples)
    {
        xpd::instance::prepare(nins, nouts, samplerate, nsamples);
        m_blksize = nsamples;
        return xpd::instance::samplerate() == samplerate;
    }
    
    void send(std::vector<xpd::atom> const& vec)
    {
        xpd::instance::send(m_tfrom, xpd::symbol("list"), vec);
    }
    
    void receive(xpd::tie name, xpd::symbol selector, std::vector<xpd::atom> const& atoms) xpd_final
    {
        m_counter++;
    }
    
    void receive(xpd::console::post const& post) xpd_final
    {
        m_counter++;
    }
    
    size_t counter() const xpd_noexcept
    {
        return m_counter;
    }
    
    static void test(instance_tester* inst)
    {
        inst->m_counter = 0ul;
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            inst->perform(inst->m_blksize, 0, NULL, 0, NULL);
        }
    }
    
private:
    xpd::patch m_patch;
    xpd::tie   m_tfrom;
    xpd::tie   m_tto;
    size_t     m_counter;
    size_t     m_blksize;
};

TEST_CASE("instance", "[instance]")
{
    instance_tester inst[XPD_TEST_NTHD];
    thd_thread      thd[XPD_TEST_NTHD];
    
    SECTION("post")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            REQUIRE(inst[i].load("test_post.pd"));
            REQUIRE(inst[i].prepare(0, 0, 44100, 256));
            thd_thread_detach(thd+i, (thd_thread_method)(&instance_tester::test), inst+i);
        }
        
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            thd_thread_join(thd+i);
            CHECK(inst[i].counter() == XPD_TEST_NLOOP);
        }
    }
    
    SECTION("message")
    {
        std::vector<xpd::atom> vec;
        vec.push_back(1.2f);
        vec.push_back(xpd::symbol("zaza"));
        vec.push_back(xpd::symbol("zozo"));
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            REQUIRE(inst[i].load("test_message.pd"));
            REQUIRE(inst[i].prepare(0, 0, 44100, 256));
            inst[i].send(vec);
            thd_thread_detach(thd+i, (thd_thread_method)(&instance_tester::test), inst+i);
        }
        
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            thd_thread_join(thd+i);
            //CHECK(inst[i].counter() == XPD_TEST_NLOOP);
        }
    }
    
    
}

#undef XPD_TEST_NLOOP




