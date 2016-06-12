/*
 // Copyright (c) 2015-2016-2016 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "test.hpp"

TEST_CASE("tie", "[tie]")
{
    SECTION("Constructor default")
    {
        xpd::tie t;
        CHECK(!bool(t));
    }
    
    SECTION("Constructor char*")
    {
        xpd::tie t("zaza");
        CHECK(bool(t));
        CHECK(t.name() == std::string("zaza"));
    }
    
    SECTION("Constructor std::string")
    {
        xpd::tie t(std::string("zaza"));
        CHECK(bool(t));
        CHECK(t.name() == std::string("zaza"));
    }
    
    SECTION("Constructor copy")
    {
        xpd::tie b(std::string("zaza"));
        xpd::tie t(b);
        CHECK(bool(b));
        CHECK(bool(t));
        CHECK(b.name() == std::string("zaza"));
        CHECK(t.name() == std::string("zaza"));
    }
    
    SECTION("Assignement char*")
    {
        xpd::tie t("zozo");
        CHECK(bool(t));
        CHECK(t.name() == std::string("zozo"));
        t = "zaza";
        CHECK(bool(t));
        CHECK(t.name() == std::string("zaza"));
    }
    
    SECTION("Assignement std::string")
    {
        xpd::tie t(std::string("zozo"));
        CHECK(bool(t));
        CHECK(t.name() == std::string("zozo"));
        t = std::string("zaza");
        CHECK(bool(t));
        CHECK(t.name() == std::string("zaza"));
    }
    
    SECTION("Assignement copy")
    {
        xpd::tie t;
        CHECK(!bool(t));
        t = xpd::tie("zaza");
        CHECK(bool(t));
        CHECK(t.name() == std::string("zaza"));
        t = xpd::tie();
        CHECK(!bool(t));
    }
    
    SECTION("Comparaisons")
    {
        CHECK(xpd::tie() == xpd::tie());
        CHECK(xpd::tie() != xpd::tie("zaza"));
        CHECK(xpd::tie("zaza") == xpd::tie("zaza"));
        CHECK(xpd::tie("zozo") != xpd::tie("zaza"));
    }
}




