/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.


#include "cpd_post.h"
#include "../pd/src/m_pd.h"
#include "../pd/src/s_stuff.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern cpd_instance* c_current_instance;

struct cpd_post_manager
{
    cpd_hook_post   c_hook;
};


// ==================================================================================== //
//                                      INTERNAL                                        //
// ==================================================================================== //

extern void cpd_post_manager_init(struct cpd_post_manager* manager)
{
    manager = (struct cpd_post_manager *)malloc(sizeof(struct cpd_post_manager));
    if(manager)
    {
        manager->c_hook = NULL;
    }
}

extern void cpd_post_manager_clear(struct cpd_post_manager* manager)
{
    free(manager);
}


// ==================================================================================== //
//                                      INTERFACE                                       //
// ==================================================================================== //

void cpd_instance_post_sethook(cpd_instance* instance, cpd_hook_post posthook)
{
    instance->c_post->c_hook = posthook;
}

void cpd_instance_post_send(cpd_instance* instance, cpd_post post)
{
    if(instance->c_post && instance->c_post->c_hook)
    {
        instance->c_post->c_hook(instance, post);
    }
}

// ==================================================================================== //
//                                      PURE DATA                                       //
// ==================================================================================== //

extern void cpd_print(const char* s)
{
    int level = 2;
    cpd_instance* instance = c_current_instance;
#ifdef DEBUG
    printf("%s", s);
#endif
    if(!instance)
    {
        return;
    }
    if(strncmp(s, "error:", 6) == 0)
    {
        level = 0;
        s+=5;
    }
    else if(strncmp(s, "verbose(", 8) == 0 && isdigit(s[8]))
    {
        level = atoi(s+8);
        s+=12;
    }
    if(instance->c_post && instance->c_post->c_hook)
    {
        instance->c_post->c_hook(instance, (cpd_post){(cpd_postlevel)level, s});
    }
}





