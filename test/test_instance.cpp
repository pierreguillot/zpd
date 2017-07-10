/*
 // Copyright (c) 2015-2016-2016 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "test.hpp"
extern "C"
{
#include "../thread/src/thd.h"
}
#include "directory.hpp"

#define XPD_TEST_NLOOP      16
#define XPD_TEST_NTHD       4
#define XPD_TEST_BLKSIZE    256
#define XPD_TEST_NINS       2
#define XPD_TEST_NOUTS      2
#define XPD_TEST_SR         44100

class instance_tester : private xpd::instance
{
public:
    
    instance_tester()
    {
        char uid[512];
        
        oshelper::directory dir = oshelper::directory::current();
        while(dir && dir.name() != "zpd")
        {
            dir = dir.parent();
        }
        if(dir && dir.name() == "zpd")
        {
            dir = dir.fullpath() + oshelper::directory::separator + "test" + oshelper::directory::separator + "patches";
            searchpath_add(dir.fullpath());
        }
        else
        {
            std::cout << "search path not initialized.\n";
        }
        
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
        
        // Midi part
        m_counter_midi_note = 0;
        m_counter_midi_ctrl = 0;
        m_counter_midi_pgrm = 0;
        m_counter_midi_bend = 0;
        m_counter_midi_tuch = 0;
        m_counter_midi_poly = 0;
        m_patch_midi = xpd::instance::load("test_midi.pd", "");
        xpd::instance::send(xpd::midi::event::note(1, 127, 127));
        xpd::instance::send(xpd::midi::event::control_change(1, 2, 127));
        xpd::instance::send(xpd::midi::event::program_change(1, 3));
        xpd::instance::send(xpd::midi::event::pitch_bend(1, 4));
        xpd::instance::send(xpd::midi::event::after_touch(1, 5));
        xpd::instance::send(xpd::midi::event::poly_after_touch(1, 127, 6));
        
        // Dsp part
        m_ins   = new xpd::sample*[XPD_TEST_NINS];
        m_outs  = new xpd::sample*[XPD_TEST_NOUTS];
        for(int i = 0; i < XPD_TEST_NINS; i++)
        {
            m_ins[i] = new xpd::sample[XPD_TEST_BLKSIZE];
        }
        for(int i = 0; i < XPD_TEST_NOUTS; i++)
        {
            m_outs[i] = new xpd::sample[XPD_TEST_BLKSIZE];
        }
        xpd::instance::prepare(XPD_TEST_NINS, XPD_TEST_NOUTS, XPD_TEST_SR, XPD_TEST_BLKSIZE);
        m_patch_dsp = xpd::instance::load("test_dsp.pd", "");
        std::srand(std::time(NULL));
        for(int i = 0; i < XPD_TEST_BLKSIZE; i++)
        {
            m_ins[0][i] = xpd::sample(std::rand() % 20000 - 10000) / 10000.f;
        }
    }

    ~instance_tester()
    {
        xpd::instance::close(m_patch_post);
        xpd::instance::close(m_patch_message);
        xpd::instance::close(m_patch_midi);
        xpd::instance::close(m_patch_dsp);
        xpd::instance::unbind(m_tie_to);
        
        for(int i = 0; i < XPD_TEST_NINS; i++)
        {
            delete [] m_ins[i];
        }
        for(int i = 0; i < XPD_TEST_NOUTS; i++)
        {
            delete [] m_outs[i];
        }
        delete [] m_ins;
        delete [] m_outs;
    }
    
    bool is_loaded() const
    {
        return static_cast<bool>(m_patch_post) && static_cast<bool>(m_patch_message) &&
        static_cast<bool>(m_patch_midi) && static_cast<bool>(m_patch_dsp);
    }
    
    // ==================================================================================== //
    //                                      POST                                            //
    // ==================================================================================== //
 
    void receive(xpd::console::post const& post) xpd_final
    {
        std::cout << "receive: |"<<post.text << "| |" << m_post_expected << "|\n";
        if(post.type == xpd::console::normal && post.text == m_post_expected)
        {
            ++m_counter_post;
        }
    }
    
    inline size_t get_npost() const xpd_noexcept
    {
        return m_counter_post;
    }
    
    // ==================================================================================== //
    //                                      MESSAGE                                         //
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
    
    inline size_t get_nmessage() const xpd_noexcept
    {
        return m_counter_message;
    }
    
    // ==================================================================================== //
    //                                      MIDI                                            //
    // ==================================================================================== //
    
    void receive(xpd::midi::event const& event) xpd_final
    {
        if(event.type() == xpd::midi::event::note_t)
        {
            if(event.channel() == 1 && event.pitch() == 127 && event.velocity() == 127)
                ++m_counter_midi_note;
            else
                std::cout << "wrong note_t\n";
        }
        else if(event.type() == xpd::midi::event::control_change_t)
        {
            if(event.channel() == 1 && event.controller() == 2 && event.value() == 127)
                ++m_counter_midi_ctrl;
            else
                std::cout << "wrong control_change_t\n";
        }
        else if(event.type() == xpd::midi::event::program_change_t)
        {
            if(event.channel() == 1 && event.program() == 3)
                ++m_counter_midi_pgrm;
            else
                std::cout << "wrong program_change_t\n";
        }
        else if(event.type() == xpd::midi::event::pitch_bend_t)
        {
            if(event.channel() == 1 && event.pitch() == 4)
                ++m_counter_midi_bend;
            else
                std::cout << "wrong pitch_bend_t\n";
        }
        else if(event.type() == xpd::midi::event::after_touch_t)
        {
            if(event.channel() == 1 && event.value() == 5)
                ++m_counter_midi_tuch;
            else
                std::cout << "wrong after_touch_t\n";
        }
        else if(event.type() == xpd::midi::event::poly_after_touch_t)
        {
            if(event.channel() == 1 && event.pitch() == 127 && event.value() == 6)
                ++m_counter_midi_poly;
            else
                std::cout << "wrong poly_after_touch_t\n";
        }
        else
        {
            std::cout << "wrong type\n";
        }
    }
    
    inline size_t get_nmidi_note() const xpd_noexcept
    {
        return m_counter_midi_note;
    }
    
    inline size_t get_nmidi_ctrl() const xpd_noexcept
    {
        return m_counter_midi_ctrl;
    }
    
    inline size_t get_nmidi_pgrm() const xpd_noexcept
    {
        return m_counter_midi_pgrm;
    }
    
    inline size_t get_nmidi_bend() const xpd_noexcept
    {
        return m_counter_midi_bend;
    }
    
    inline size_t get_nmidi_tuch() const xpd_noexcept
    {
        return m_counter_midi_tuch;
    }
    
    inline size_t get_nmidi_poly() const xpd_noexcept
    {
        return m_counter_midi_poly;
    }
    
    // ==================================================================================== //
    //                                      DSP                                            //
    // ==================================================================================== //
    inline int get_samplerate() const xpd_noexcept
    {
        return xpd::instance::samplerate();
    }
    
    inline bool check_out1() const xpd_noexcept
    {
        for(int i = 0; i < XPD_TEST_BLKSIZE; ++i)
        {
            if(m_outs[0][i] != i)
                return false;
        }
        return true;
    }
    
    inline bool check_out2() const xpd_noexcept
    {
        for(int i = 0; i < XPD_TEST_BLKSIZE; ++i)
        {
            if(m_outs[1][i] != m_ins[0][i])
            {
                std::cout << "[" << i <<"]" << m_outs[1][i] << " & " << m_ins[0][i] << "\n";
                return false;
            }
        }
        return true;
    }
    
    // ==================================================================================== //
    //                                      PERFORM                                            //
    // ==================================================================================== //
    
    static void test(instance_tester* inst)
    {
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            inst->perform(XPD_TEST_BLKSIZE, XPD_TEST_NINS, (const xpd::sample**)inst->m_ins, XPD_TEST_NOUTS, inst->m_outs);
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

    size_t      m_counter_midi_note;
    size_t      m_counter_midi_ctrl;
    size_t      m_counter_midi_pgrm;
    size_t      m_counter_midi_bend;
    size_t      m_counter_midi_tuch;
    size_t      m_counter_midi_poly;
    xpd::patch  m_patch_midi;
    
    xpd::sample**   m_ins;
    xpd::sample**   m_outs;
    xpd::patch      m_patch_dsp;
};

TEST_CASE("instance", "[instance post]")
{
    instance_tester inst[XPD_TEST_NTHD];
    thd_thread  thd[XPD_TEST_NTHD];
    
    for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
    {
        REQUIRE(inst[i].is_loaded());
        thd_thread_detach(thd+i, (thd_thread_method)(&instance_tester::test), inst+i);
    }
    
    SECTION("states")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            thd_thread_join(thd+i);
            CHECK(inst[i].get_samplerate() == XPD_TEST_SR);
            CHECK(inst[i].get_npost() == XPD_TEST_NLOOP);
            /*
            CHECK(inst[i].get_nmessage() == XPD_TEST_NLOOP);
            CHECK(inst[i].get_nmidi_note() == XPD_TEST_NLOOP);
            CHECK(inst[i].get_nmidi_ctrl() == XPD_TEST_NLOOP);
            CHECK(inst[i].get_nmidi_pgrm() == XPD_TEST_NLOOP);
            CHECK(inst[i].get_nmidi_bend() == XPD_TEST_NLOOP);
            CHECK(inst[i].get_nmidi_tuch() == XPD_TEST_NLOOP);
            CHECK(inst[i].get_nmidi_poly() == XPD_TEST_NLOOP);
            CHECK(inst[i].check_out1());
            CHECK(inst[i].check_out2());
             */
        }
    }
}

#undef XPD_TEST_NLOOP




