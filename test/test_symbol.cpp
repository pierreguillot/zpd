/*
 // Copyright (c) 2015-2016-2016 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "test.hpp"

TEST_CASE("symbol", "[symbol]")
{
    SECTION("Constructor default")
    {
        xpd::symbol t;
        CHECK(!bool(t));
    }
    
    SECTION("Constructor char*")
    {
        xpd::symbol t("zaza");
        CHECK(bool(t));
        CHECK(t.name() == std::string("zaza"));
    }
    
    SECTION("Constructor std::string")
    {
        xpd::symbol t(std::string("zaza"));
        CHECK(bool(t));
        CHECK(t.name() == std::string("zaza"));
    }
    
    SECTION("Constructor copy")
    {
        xpd::symbol b(std::string("zaza"));
        xpd::symbol t(b);
        CHECK(bool(b));
        CHECK(bool(t));
        CHECK(b.name() == std::string("zaza"));
        CHECK(t.name() == std::string("zaza"));
    }
    
    SECTION("Assignement char*")
    {
        xpd::symbol t("zozo");
        CHECK(bool(t));
        CHECK(t.name() == std::string("zozo"));
        t = "zaza";
        CHECK(bool(t));
        CHECK(t.name() == std::string("zaza"));
    }
    
    SECTION("Assignement std::string")
    {
        xpd::symbol t(std::string("zozo"));
        CHECK(bool(t));
        CHECK(t.name() == std::string("zozo"));
        t = std::string("zaza");
        CHECK(bool(t));
        CHECK(t.name() == std::string("zaza"));
    }
    
    SECTION("Assignement copy")
    {
        xpd::symbol t;
        CHECK(!bool(t));
        t = xpd::symbol("zaza");
        CHECK(bool(t));
        CHECK(t.name() == std::string("zaza"));
        t = xpd::symbol();
        CHECK(!bool(t));
    }
    
    SECTION("Comparaisons")
    {
        CHECK(xpd::symbol() == xpd::symbol());
        CHECK(xpd::symbol() != xpd::symbol("zaza"));
        CHECK(xpd::symbol("zaza") == xpd::symbol("zaza"));
        CHECK(xpd::symbol("zozo") != xpd::symbol("zaza"));
    }
}




