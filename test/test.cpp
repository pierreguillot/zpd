/*
 // Copyright (c) 2015-2016-2016 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../xpd/xpd.hpp"
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "directory.hpp"




int main( int argc, char* const argv[] )
{
    xpd::environment::initialize();
    std::cout << "version "
    << xpd::environment::version_major()
    << "." << xpd::environment::version_minor()
    << "." << xpd::environment::version_bug() << "\n";
    xpd::environment::searpath_clear();
    
    oshelper::directory dir = oshelper::directory::current();
    while(dir && dir.name() != "zpd")
    {
        dir = dir.parent();
    }
    if(dir && dir.name() == "zpd")
    {
        dir = dir.fullpath() + oshelper::directory::separator + "test" + oshelper::directory::separator + "patches";
        xpd::environment::searchpath_add(dir.fullpath());
    }
    else
    {
        std::cout << "search path not initialized.\n";
    }
    
    
    int result =  Catch::Session().run(argc, argv);
    xpd::environment::clear();
    return result;
}



