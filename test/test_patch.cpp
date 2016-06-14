/*
 // Copyright (c) 2015-2016-2016 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "test.hpp"

class pacth_tester : public xpd::instance
{
public:
};

static bool test_object_int(xpd::patch p, xpd::object o)
{
    return (o.x() >= p.width() || o.y() >= p.height() || o.x() + o.width() <= 0 || o.y() + o.height() <= 0);
}

/*
static void test_gui_int(xpd::patch p, xpd::object o)
{
    xpd::gui g4;
    xpd::gui g2 = o;
    assert("test_patch gui type" &&
           ((g2.name() == "bng" && g2.type() == gui::bang) || g2.type() != gui::bang));
    assert("test_patch gui type" &&
           ((g2.name() == "hsl" && g2.type() == gui::slider_horizontal) || g2.type() != gui::slider_horizontal));
    assert("test_patch gui type" &&
           ((g2.name() == "vsl" && g2.type() == gui::slider_vertical) || g2.type() != gui::slider_vertical));
    assert("test_patch gui type" &&
           ((g2.name() == "tgl" && g2.type() == gui::toggle) || g2.type() != gui::toggle));
    assert("test_patch gui type" &&
           ((g2.name() == "nbx" && g2.type() == gui::number) || g2.type() != gui::number));
    assert("test_patch gui type" &&
           ((g2.name() == "hradio" && g2.type() == gui::radio_horizontal) || g2.type() != gui::radio_horizontal));
    assert("test_patch gui type" &&
           ((g2.name() == "vradio" && g2.type() == gui::radio_vertical) || g2.type() != gui::radio_vertical));
    assert("test_patch gui type" &&
           ((g2.name() == "vu" && g2.type() == gui::vu_meter) || g2.type() != gui::vu_meter));
    assert("test_patch gui type" &&
           ((g2.name() == "cnv" && g2.type() == gui::panel) || g2.type() != gui::panel));
    
    assert("test_patch gui receive" && g2.receive_tie().name() == g2.name() + "r");
    assert("test_patch gui send" && ((g2.name() == "vu" && g2.send_tie().name() == "") || g2.send_tie().name() == g2.name() + "s"));
    assert("test_patch gui label" && g2.label() == g2.name() + "l");
    gui g3 = g2;
    g4 = o;
    
    if(g4.type() == gui::bang)
    {
        assert("test_patch gui min" && g4.minimum() == 0);
        assert("test_patch gui max" && g4.maximum() == 0);
        assert("test_patch gui val" && g4.value() == 0);
        assert("test_patch gui nst" && g4.nsteps() == 0);
    }
    else if(g4.type() == gui::slider_horizontal)
    {
        assert("test_patch gui min" && g4.minimum() == 0);
        assert("test_patch gui max" && g4.maximum() == 127);
        assert("test_patch gui val" && g4.value() == 1);
        assert("test_patch gui nst" && g4.nsteps() == 0);
    }
    else if(g4.type() == gui::slider_vertical)
    {
        assert("test_patch gui min" && g4.minimum() == 0);
        assert("test_patch gui max" && g4.maximum() == 127);
        assert("test_patch gui val" && g4.value() == 1);
        assert("test_patch gui nst" && g4.nsteps() == 0);
    }
    else if(g4.type() == gui::toggle)
    {
        assert("test_patch gui min" && g4.minimum() == 0);
        assert("test_patch gui max" && g4.maximum() == 1);
        assert("test_patch gui val" && g4.value() == 1);
        assert("test_patch gui nst" && g4.nsteps() == 2);
    }
    else if(g4.type() == gui::number)
    {
        assert("test_patch gui min" && g4.minimum() == 0);
        assert("test_patch gui max" && g4.maximum() == 127);
        assert("test_patch gui val" && g4.value() == 1);
        assert("test_patch gui nst" && g4.nsteps() == 0);
    }
    else if(g4.type() == gui::radio_horizontal)
    {
        assert("test_patch gui min" && g4.minimum() == 0);
        assert("test_patch gui max" && g4.maximum() == 7);
        assert("test_patch gui val" && g4.value() == 1);
        assert("test_patch gui nst" && g4.nsteps() == 8);
    }
    else if(g4.type() == gui::radio_vertical)
    {
        assert("test_patch gui min" && g4.minimum() == 0);
        assert("test_patch gui max" && g4.maximum() == 7);
        assert("test_patch gui val" && g4.value() == 1);
        assert("test_patch gui nst" && g4.nsteps() == 8);
    }
    else if(g4.type() == gui::vu_meter)
    {
        assert("test_patch gui min" && g4.minimum() == 0);
        assert("test_patch gui max" && g4.maximum() == 0);
        assert("test_patch gui val" && g4.value() == 0);
        assert("test_patch gui nst" && g4.nsteps() == 0);
    }
    else if(g4.type() == gui::panel)
    {
        assert("test_patch gui min" && g4.minimum() == 0);
        assert("test_patch gui max" && g4.maximum() == 0);
        assert("test_patch gui val" && g4.value() == 0);
        assert("test_patch gui nst" && g4.nsteps() == 0);
    }
    
    
    if(g3.type() == gui::bang || g3.type() == gui::toggle)
    {
        assert("test_patch gui label x" && g3.label_x() == g3.x() + 17);
        assert("test_patch gui label y" && g3.label_y() == g3.y() + 7);
    }
    else if(g3.type() == gui::panel)
    {
        assert("test_patch gui label x" && g3.label_x() == g3.x() + 20);
        assert("test_patch gui label y" && g3.label_y() == g3.y() + 12);
    }
    else
    {
        assert("test_patch gui label x" && g3.label_x() == g3.x());
        assert("test_patch gui label y" && g3.label_y() == g3.y() - 8);
    }
}
 */

TEST_CASE("patch", "[patch]")
{
    pacth_tester inst;
    
    SECTION("Informations")
    {
        xpd::patch p1 = inst.load("test.pd", "");
        REQUIRE(bool(p1));
        xpd::patch p2 = inst.load("test_patch.pd", "/home/maison");
        CHECK(bool(!p2));
        p2 = inst.load("test.pd", "");
        REQUIRE(bool(p2));
        CHECK(p1.name() == std::string("test.pd"));
        CHECK(!p1.path().empty());
        CHECK(p1.name() == p2.name());
        CHECK(p1.path() == p2.path());
        CHECK(p1.x() == 100);
        CHECK(p1.y() == 100);
        CHECK(p1.width() == 400);
        CHECK(p1.height() == 150);
        inst.close(p2);
        inst.close(p1);
    }
    /*
    SECTION("Objects & GUIs")
    {
        xpd::patch p1 = inst.load("test.pd", "");
        std::vector<xpd::object> objects(p1.objects());
        REQUIRE(bool(p1));
        CHECK(!objects.empty());
        for(size_t i = 0; i < objects.size(); ++i)
        {
            xpd::gui g1;
            CHECK(bool(objects[i]));
            CHECK(!objects[i].name().empty());
            CHECK(!objects[i].text().empty());
            bool is_gui = true;
            try
            {
                xpd::gui g = objects[i];
            }
            catch(...)
            {
                is_gui = false;
                CHECK(test_object_int(p1, objects[i]));
            }
            if(is_gui)
            {
                //test_gui_int(p1, objects[i]);
            }
            else
            {
                try
                {
                    g1 = objects[i];
                }
                catch(...)
                {
                    
                }
            }
            
        }
        inst.close(p1);
    }
     */
    
}




