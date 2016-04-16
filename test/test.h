/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

static int test_counter = 0;
static char const* section_name = NULL;
#define TEST_START(message) {test_counter = 0; section_name = NULL; printf("Test Start : %s\n", message);}
#define TEST_END() {printf("Number of Tests %i\n", test_counter);}

#define TEST_SECTION_START(message) {section_name = message;}
#define TEST_TRUE(message, a) if((++test_counter) && !(a)) {if(section_name){printf("Error %s in [%s]\n", message, section_name);}else{printf("Error %s\n", message);} return 1;}
#define TEST_FALSE(message, a) if((++test_counter)&& (a)) {if(section_name){printf("Error %s in [%s]\n", message, section_name);}else{printf("Error %s\n", message);} return 1;}
