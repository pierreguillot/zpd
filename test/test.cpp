/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../xpd/xpd.hpp"

using namespace xpd;

static int test_counter = 0;
static char const* section_name = NULL;
#define TEST_START(message) {test_counter = 0; section_name = NULL; printf("Test Start : %s\n", message);}
#define TEST_END() {printf("Number of Tests %i\n", test_counter);}

#define TEST_SECTION(message) {section_name = message;}
#define TEST_TRUE(message, a) if((++test_counter) && !(a)) {if(section_name){printf("Error %s in [%s]\n", message, section_name);}else{printf("Error %s\n", message);} return 1;}
#define TEST_FALSE(message, a) if((++test_counter)&& (a)) {if(section_name){printf("Error %s in [%s]\n", message, section_name);}else{printf("Error %s\n", message);} return 1;}

class instance_test : public instance
{
    
    
};


int main(int argc, char** argv)
{
    TEST_START("xpd")
    
    
    
    TEST_END()
    return 0;
}