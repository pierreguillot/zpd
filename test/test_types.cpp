/*
 // Copyright (c) 2015-2016-2016 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/


#include "../xpd/xpd.hpp"
#include "catch.hpp"

// ================================================================================ //
//                                 TYPES TESTS                                  //
// ================================================================================ //


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


TEST_CASE("atom", "[atom]")
{
    SECTION("Constructor default")
    {
        xpd::atom t;
        CHECK(t.type() == xpd::atom::null_t);
        CHECK(float(t) == 0.f);
        CHECK(xpd::symbol(t) == xpd::symbol());
    }
    
    SECTION("Constructor float")
    {
        xpd::atom t(1.2f);
        CHECK(t.type() == xpd::atom::float_t);
        CHECK(float(t) == 1.2f);
        CHECK(xpd::symbol(t) == xpd::symbol());
    }
    
    SECTION("Constructor xpd::symbol")
    {
        xpd::atom t(xpd::symbol("zaza"));
        CHECK(t.type() == xpd::atom::symbol_t);
        CHECK(float(t) == 0.f);
        CHECK(xpd::symbol(t) == xpd::symbol("zaza"));
        CHECK(xpd::symbol(t).name() == std::string("zaza"));
    }
    
    SECTION("Constructor copy")
    {
        xpd::symbol b1;
        xpd::atom t1(b1);
        CHECK(t1.type() == xpd::atom::null_t);
        CHECK(float(t1) == 0.f);
        CHECK(xpd::symbol(t1) == xpd::symbol());
        
        xpd::atom b2(1.2f);
        xpd::atom t2(b2);
        CHECK(t2.type() == xpd::atom::float_t);
        CHECK(float(t2) == 1.2f);
        CHECK(xpd::symbol(t2) == xpd::symbol());
        
        xpd::atom b3(xpd::symbol("zaza"));
        xpd::atom t3(b3);
        CHECK(t3.type() == xpd::atom::symbol_t);
        CHECK(float(t3) == 0.f);
        CHECK(xpd::symbol(t3) == xpd::symbol("zaza"));
        CHECK(xpd::symbol(t3).name() == std::string("zaza"));
    }
    
    SECTION("Assignement float")
    {
        xpd::atom t;
        CHECK(t.type() == xpd::atom::null_t);
        t = 1.2;
        CHECK(t.type() == xpd::atom::float_t);
        CHECK(float(t) == 1.2f);
        CHECK(xpd::symbol(t) == xpd::symbol());
    }
    
    SECTION("Assignement symbol")
    {
        xpd::atom t;
        CHECK(t.type() == xpd::atom::null_t);
        t = xpd::symbol("zaza");
        CHECK(t.type() == xpd::atom::symbol_t);
        CHECK(float(t) == 0.f);
        CHECK(xpd::symbol(t) == xpd::symbol("zaza"));
        CHECK(xpd::symbol(t).name() == std::string("zaza"));
    }
    
    SECTION("Assignement copy")
    {
        xpd::atom t;
        CHECK(t.type() == xpd::atom::null_t);
        t = xpd::atom(1.2);
        CHECK(t.type() == xpd::atom::float_t);
        CHECK(float(t) == 1.2f);
        CHECK(xpd::symbol(t) == xpd::symbol());
        t = xpd::atom(xpd::symbol("zaza"));
        CHECK(t.type() == xpd::atom::symbol_t);
        CHECK(float(t) == 0.f);
        CHECK(xpd::symbol(t) == xpd::symbol("zaza"));
        CHECK(xpd::symbol(t).name() == std::string("zaza"));
        t = xpd::atom();
        CHECK(t.type() == xpd::atom::null_t);
    }
}




