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
#include <cstdlib>
#include <cassert>

using namespace xpd;

#ifndef XPD_TEST_HPP
#define XPD_TEST_HPP

#define XPD_TEST_NLOOP 1

class instance_test : private instance
{
public:
    instance_test(const size_t index) : m_index(index)
    {
        ;
    }
    
    ~instance_test()
    {
        ;
    }
    
    void receive(console::post const& post) xpd_final
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
    
    void receive(tie tie, symbol s, std::vector<atom> const& atoms) xpd_final
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
        
        std::vector<atom> const& temp = m_atoms;
        assert("receive atoms s" && s == m_name);
        assert("receive atoms size" && atoms.size() == temp.size());
        
        int to_check_strange;
        /*
        for(size_t i = 0; i < atoms.size(); ++i)
        {
            assert("receive atoms type" && atoms[i].type() == temp[i].type());
            if(atoms[i].type() == atom::float_t)
            {
                assert("receive atoms type" && float(atoms[i]) == float(temp[i]));
            }
            else if(atoms[i].type() == atom::symbol_t)
            {
                assert("receive atoms type" && symbol(atoms[i]) == symbol(temp[i]));
            }
        }
         */
    }
    
    void receive(midi::event const& event)
    {
        if(event.type() == midi::event::note_t) {
            m_count_note++;
        }
        else if(event.type() == midi::event::control_change_t) {
            m_count_ctl++;
        }
        else if(event.type() == midi::event::program_change_t) {
            m_count_pgm++;
        }
        else if(event.type() == midi::event::pitch_bend_t) {
            m_count_bend++;
        }
        else if(event.type() == midi::event::after_touch_t) {
            m_count_after++;
        }
        else if(event.type() == midi::event::poly_after_touch_t) {
            m_count_poly++;
        }
        else {
            m_count_byte++;
        }
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
        tie to1(std::string(uid) + "-toxpd1");
        tie to2(std::string(uid) + "-toxpd2");
        inst->bind(to1);
        inst->bind(to1);
        inst->bind(to2);
        
        inst->m_count_bang = 0;
        inst->m_count_float = 0;
        inst->m_count_symbol = 0;
        inst->m_count_list = 0;
        inst->m_count_anything = 0;
        inst->m_atoms.reserve(5);
        symbol zozo("zozo");
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            inst->m_name = symbol::bang_s;
            inst->m_atoms.clear();
            inst->send(from, inst->m_name, inst->m_atoms);
            
            inst->m_name = symbol::float_s;
            inst->m_atoms.push_back(1.f);
            assert("test_message float" && inst->m_atoms[0].type() == atom::float_t && float(inst->m_atoms[0]) == 1.f);
            inst->send(from, inst->m_name, inst->m_atoms);
            
           
            inst->m_name = symbol::symbol_s;
            inst->m_atoms[0] = zozo;
            assert("test_message symbol" && inst->m_atoms[0].type() == atom::symbol_t && symbol(inst->m_atoms[0]) == zozo);
            inst->send(from, inst->m_name, inst->m_atoms);
            
            inst->m_name = symbol::list_s;
            inst->m_atoms[0] = 1.f;
            inst->m_atoms.push_back(1.f);
            inst->m_atoms.push_back(zozo);
            inst->send(from, inst->m_name, inst->m_atoms);
            
            inst->m_name = symbol(std::string("zaza"));
            inst->m_atoms[0] = "zozo";
            inst->m_atoms[2] = std::string("zuzu");
            inst->m_atoms.push_back(symbol("zizi"));
            inst->send(from, inst->m_name, inst->m_atoms);
        }
        inst->unbind(to1);
        inst->unbind(to2);
        assert("test_message bang" && inst->m_count_bang == XPD_TEST_NLOOP * 2);
        assert("test_message float" && inst->m_count_float == XPD_TEST_NLOOP * 2);
        assert("test_message symbol" && inst->m_count_symbol == XPD_TEST_NLOOP * 2);
        assert("test_message list" && inst->m_count_list == XPD_TEST_NLOOP * 2);
        assert("test_message anything" && inst->m_count_anything == XPD_TEST_NLOOP * 2);
        inst->close(*p);
    }
    
    static void test_midi(instance_test* inst)
    {
        patch* p = inst->load("test.pd", "");
        inst->m_count_note = 0;
        inst->m_count_ctl = 0;
        inst->m_count_pgm = 0;
        inst->m_count_bend = 0;
        inst->m_count_after = 0;
        inst->m_count_poly = 0;
        inst->m_count_byte = 0;
 
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            inst->send(midi::event::note(1, 1, 127));
            inst->send(midi::event::control_change(1, 1, 127));
            inst->send(midi::event::program_change(1, 1));
            inst->send(midi::event::pitch_bend(1, 1));
            inst->send(midi::event::after_touch(1, 1));
            inst->send(midi::event::poly_after_touch(1, 1, 1));
            inst->send(midi::event::byte(1));
        }

        assert("test_midi note" && inst->m_count_note == XPD_TEST_NLOOP);
        assert("test_midi ctl" && inst->m_count_ctl == XPD_TEST_NLOOP);
        assert("test_midi pgm" && inst->m_count_pgm == XPD_TEST_NLOOP);
        assert("test_midi bend" && inst->m_count_bend == XPD_TEST_NLOOP);
        assert("test_midi after" && inst->m_count_after == XPD_TEST_NLOOP);
        assert("test_midi poly" && inst->m_count_poly == XPD_TEST_NLOOP);
        assert("test_midi byte" && inst->m_count_byte == XPD_TEST_NLOOP);
        inst->close(*p);
    }
    
    
    static void test_dsp_int(instance_test* inst, int sr, int blks, int nins, int nouts)
    {
        inst->prepare(nins, nouts, sr, blks);
        assert("test_dsp sample rate" && inst->samplerate() == sr);
        
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            for(size_t k = 0; k < nins; k++)
            {
                for(size_t j = 0; j < blks; j++)
                {
                    inst->m_input[k][j] = ((rand() % 4000) - 2000.f) / 2000.f;
                }
            }
            inst->perform(blks, nins, const_cast<float const **>(inst->m_input), nouts, inst->m_output);
            for(size_t k = 0; k < nouts; k++)
            {
                for(size_t j = 0; j < blks; j++)
                {
                    assert("test_dsp samples" && inst->m_output[k][j] == inst->m_input[k][j]);
                }
            }
        }
    }
    
    static void test_dsp(instance_test* inst)
    {
        patch* p = inst->load("test.pd", "");
        inst->m_input[0] = new float[256];
        inst->m_input[1] = new float[256];
        inst->m_output[0] = new float[256];
        inst->m_output[1] = new float[256];
        assert("test_dsp inputs outputs" && inst->m_input[0] && inst->m_input[1] && inst->m_output[0] && inst->m_output[1]);
        
        test_dsp_int(inst, 44100, 64, 2, 2);
        test_dsp_int(inst, 44100, 256, 2, 1);
        test_dsp_int(inst, 96000, 128, 1, 1);
        
        delete [] inst->m_input[0];
        delete [] inst->m_input[1];
        delete [] inst->m_output[0];
        delete [] inst->m_output[1];
        inst->close(*p);
    }
    
private:
    const size_t m_index;
    size_t  m_ninouts;
    float* m_input[2];
    float* m_output[2];
    symbol            m_name;
    std::vector<atom> m_atoms;
    
    size_t m_count_fatal;
    size_t m_count_error;
    size_t m_count_normal;
    size_t m_count_log;
    
    size_t m_count_bang;
    size_t m_count_float;
    size_t m_count_symbol;
    size_t m_count_list;
    size_t m_count_anything;
    
    size_t m_count_note;
    size_t m_count_ctl;
    size_t m_count_pgm;
    size_t m_count_bend;
    size_t m_count_after;
    size_t m_count_poly;
    size_t m_count_byte;
};


#endif // XPD_TEST_HPP
