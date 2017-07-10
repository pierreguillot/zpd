/*
 // Copyright (c) 2015-2016-2016 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "test.hpp"
#include "directory.hpp"

class pacth_tester : public xpd::instance
{
};

static void test_gui_int(xpd::patch p, xpd::object o)
{
    xpd::gui g4;
    xpd::gui g2 = o;
    CHECK(((g2.name() == "bng" && g2.type() == xpd::gui::bang) || g2.type() != xpd::gui::bang));
    CHECK(((g2.name() == "bng" && g2.type() == xpd::gui::bang) || g2.type() != xpd::gui::bang));
    CHECK(((g2.name() == "hsl" && g2.type() == xpd::gui::slider_horizontal) || g2.type() != xpd::gui::slider_horizontal));
    CHECK(((g2.name() == "vsl" && g2.type() == xpd::gui::slider_vertical) || g2.type() != xpd::gui::slider_vertical));
    CHECK(((g2.name() == "tgl" && g2.type() == xpd::gui::toggle) || g2.type() != xpd::gui::toggle));
    CHECK(((g2.name() == "nbx" && g2.type() == xpd::gui::number) || g2.type() != xpd::gui::number));
    CHECK(((g2.name() == "hradio" && g2.type() == xpd::gui::radio_horizontal) || g2.type() != xpd::gui::radio_horizontal));
    CHECK(((g2.name() == "vradio" && g2.type() == xpd::gui::radio_vertical) || g2.type() != xpd::gui::radio_vertical));
    CHECK(((g2.name() == "vu" && g2.type() == xpd::gui::vu_meter) || g2.type() != xpd::gui::vu_meter));
    CHECK(((g2.name() == "cnv" && g2.type() == xpd::gui::panel) || g2.type() != xpd::gui::panel));
    
    
    CHECK((g2.receive_tie().name() == g2.name() + "r"));
    CHECK(((g2.name() == "vu" && g2.send_tie().name() == "") || g2.send_tie().name() == g2.name() + "s"));
    CHECK((g2.label() == g2.name() + "l"));
    xpd::gui g3 = g2;
    g4 = o;
    
    if(g4.type() == xpd::gui::bang)
    {
        CHECK(g4.minimum() == 0);
        CHECK(g4.maximum() == 0);
        CHECK(g4.value() == 0);
        CHECK(g4.nsteps() == 0);
    }
    else if(g4.type() == xpd::gui::slider_horizontal)
    {
        CHECK(g4.minimum() == 0);
        CHECK(g4.maximum() == 127);
        CHECK(g4.value() == 1);
        CHECK(g4.nsteps() == 0);
    }
    else if(g4.type() == xpd::gui::slider_vertical)
    {
        CHECK(g4.minimum() == 0);
        CHECK(g4.maximum() == 127);
        CHECK(g4.value() == 1);
        CHECK(g4.nsteps() == 0);
    }
    else if(g4.type() == xpd::gui::toggle)
    {
        CHECK(g4.minimum() == 0);
        CHECK(g4.maximum() == 1);
        CHECK(g4.value() == 1);
        CHECK(g4.nsteps() == 2);
    }
    else if(g4.type() == xpd::gui::number)
    {
        CHECK(g4.minimum() == 0);
        CHECK(g4.maximum() == 127);
        CHECK(g4.value() == 1);
        CHECK(g4.nsteps() == 0);
    }
    else if(g4.type() == xpd::gui::radio_horizontal)
    {
        CHECK(g4.minimum() == 0);
        CHECK(g4.maximum() == 7);
        CHECK(g4.value() == 1);
        CHECK(g4.nsteps() == 8);
    }
    else if(g4.type() == xpd::gui::radio_vertical)
    {
        CHECK(g4.minimum() == 0);
        CHECK(g4.maximum() == 7);
        CHECK(g4.value() == 1);
        CHECK(g4.nsteps() == 8);
    }
    else if(g4.type() == xpd::gui::vu_meter)
    {
        CHECK(g4.minimum() == 0);
        CHECK(g4.maximum() == 0);
        CHECK(g4.value() == 0);
        CHECK(g4.nsteps() == 0);
    }
    else if(g4.type() == xpd::gui::panel)
    {
        CHECK(g4.minimum() == 0);
        CHECK(g4.maximum() == 0);
        CHECK(g4.value() == 0);
        CHECK(g4.nsteps() == 0);
    }
    
    
    if(g3.type() == xpd::gui::bang || g3.type() == xpd::gui::toggle)
    {
        CHECK((g3.label_x() == g3.x() + 17));
        CHECK((g3.label_y() == g3.y() + 7));
    }
    else if(g3.type() == xpd::gui::panel)
    {
        CHECK((g3.label_x() == g3.x() + 20));
        CHECK((g3.label_y() == g3.y() + 12));
    }
    else
    {
        CHECK(g3.label_x() == g3.x());
        CHECK((g3.label_y() == g3.y() - 8));
    }
}

TEST_CASE("patch", "[patch]")
{
    pacth_tester inst;
    
    oshelper::directory dir = oshelper::directory::current();
    while(dir && dir.name() != "zpd")
    {
        dir = dir.parent();
    }
    if(dir && dir.name() == "zpd")
    {
        dir = dir.fullpath() + oshelper::directory::separator + "test" + oshelper::directory::separator + "patches";
        inst.searchpath_add(dir.fullpath());
    }
    
    SECTION("Informations")
    {
        xpd::patch p1 = inst.load("test_patch.pd", "");
        REQUIRE(bool(p1));
        xpd::patch p2 = inst.load("test_patch.pd", "/home/maison");
        CHECK(bool(!p2));
        p2 = inst.load("test_patch.pd", "");
        REQUIRE(bool(p2));
        CHECK(p1.name() == std::string("test_patch.pd"));
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
    
    SECTION("Objects & GUIs")
    {
        xpd::patch p1 = inst.load("test_patch.pd", "");
        REQUIRE(bool(p1));
        std::vector<xpd::object> objects(p1.objects());
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
                xpd::gui g4;
                xpd::gui g2 = objects[i];
                CHECK(((g2.name() == "bng" && g2.type() == xpd::gui::bang) || g2.type() != xpd::gui::bang));
                test_gui_int(p1, g2);
            }
            catch(...)
            {
                is_gui = false;
                CHECK((objects[i].x() >= p1.width()
                      || objects[i].y() >= p1.height()
                      || objects[i].x() + objects[i].width() <= 0
                      || objects[i].y() + objects[i].height() <= 0));
            }
            if(is_gui)
            {
                test_gui_int(p1, objects[i]);
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
}




