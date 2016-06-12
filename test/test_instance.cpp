/*
 // Copyright (c) 2015-2016-2016 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include <iostream>
#include "test.hpp"

class instance_tester : private xpd::instance
{
    
};

TEST_CASE("instance", "[instance]")
{
    SECTION("Constructor")
    {
        instance_tester inst;
    }
    
    SECTION("Constructor Multi")
    {
        instance_tester inst[10];
    }
}

#undef XPD_TEST_NLOOP




