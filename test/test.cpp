/*
 // Copyright (c) 2015-2016-2016 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../xpd/xpd.hpp"
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"




int main( int argc, char* const argv[] )
{
    if(argc && argv)
    {
        xpd::environment::initialize();
        std::cout << "version "
        << xpd::environment::version_major()
        << "." << xpd::environment::version_minor()
        << "." << xpd::environment::version_bug() << "\n";
        xpd::environment::searpath_clear();
        
        std::cout << argv[0] << "\n";
        int result =  Catch::Session().run(argc, argv);
        xpd::environment::clear();
        return result;
    }
    std::cout << "Environment not initialized...";
    return -1;
    
}



