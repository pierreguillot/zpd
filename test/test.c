/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#include "../zpd/z_pd.h"

#include <stdio.h>
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif


typedef struct _test_instance
{
    z_instance t_instance;
    unsigned char index;
    z_sample* ins[2];
    z_sample* outs[2];
}z_test_instance;

static void test_print_hook(z_test_instance* instance, const char *s)
{
    printf("Print instance %u : %s", instance->index, s);
}

static void test_hook_bang(z_test_instance* instance, z_tie* tie)
{
    printf("Message instance %u bang : from %s\n", instance->index, z_pd_tie_get_name(tie));
}

static void test_hook_float(z_test_instance* instance, z_tie* tie, z_float f)
{
    printf("Message instance %u float : from %s - %f\n", instance->index, z_pd_tie_get_name(tie), f);
}

static void test_hook_symbol(z_test_instance* instance, z_tie* tie, z_symbol* s)
{
    printf("Message instance %u symbol : from %s - %s\n", instance->index, z_pd_tie_get_name(tie), z_pd_symbol_get_name(s));
}

static void test_hook_list(z_test_instance* instance, z_tie* tie, z_list *list)
{
    size_t i = 0;
    size_t const size = z_pd_list_get_size(list);
    printf("Message instance %u list : from %s -", instance->index, z_pd_tie_get_name(tie));
    for(i = 0; i < size; ++i)
    {
        if(z_pd_list_get_type(list, i) == Z_FLOAT)
        {
            printf(" %f", z_pd_list_get_float(list, i));
        }
        else if(z_pd_list_get_type(list, i) == Z_SYMBOL)
        {
            printf(" %s", z_pd_symbol_get_name(z_pd_list_get_symbol(list, i)));
        }
    }
    printf("\n");
}

static void test_hook_anything(z_test_instance* instance, z_tie* tie, z_symbol *s, z_list *list)
{
    size_t i = 0;
    size_t const size = z_pd_list_get_size(list);
    printf("Message instance %u anything : from %s - %s", instance->index, z_pd_tie_get_name(tie), z_pd_symbol_get_name(s));
    for(i = 0; i < size; ++i)
    {
        if(z_pd_list_get_type(list, i) == Z_FLOAT)
        {
            printf(" %f", z_pd_list_get_float(list, i));
        }
        else if(z_pd_list_get_type(list, i) == Z_SYMBOL)
        {
            printf(" %s", z_pd_symbol_get_name(z_pd_list_get_symbol(list, i)));
        }
    }
    printf("\n");
}



int main(int argc, char** argv)
{
    z_pd_init();
    printf("Test z_pd %u.%u.%u :\n", z_pd_version_getmajor(), z_pd_version_getminor(), z_pd_version_getbug());
    
    z_test_instance* instance1 = (z_test_instance *)z_pd_instance_new(sizeof(z_test_instance),
                                                                      (z_hook_print)test_print_hook);
    z_test_instance* instance2 = (z_test_instance *)z_pd_instance_new(sizeof(z_test_instance),
                                                                      (z_hook_print)test_print_hook);
    printf("--------------------\n\n\n");
    
    if(instance1 && instance2)
    {
        instance1->index = 1;
        instance2->index = 2;
        
        
        {
            printf("--------------------\n");
            printf("Test Print Hook :\n");
            printf("--------------------\n");
            z_pd_instance_set((z_instance *)instance1);
            z_pd_console_post("print instance 1");
            
            z_pd_instance_set((z_instance *)instance2);
            z_pd_console_post("print instance 2");
            printf("--------------------\n\n\n");
        }
        
        
        {
            /*
            printf("--------------------\n");
            printf("Test Messages Hook :\n");
            printf("--------------------\n");
            z_tie* tie1 = z_pd_tie_create("tie1");
            z_tie* tie2 = z_pd_tie_create("tie2");
            z_list* list1 = z_pd_list_create(3);
            z_pd_list_set_float(list1, 0, 1.f);
            z_pd_list_set_symbol(list1, 1, z_pd_symbol_create("listsym"));
            z_pd_list_set_float(list1, 2, 3.f);
            
            z_pd_instance_bind((z_instance *)instance1, tie1,
                               (z_hook_bang)test_hook_bang,
                               (z_hook_float)test_hook_float,
                               (z_hook_symbol)test_hook_symbol,
                               (z_hook_gpointer)NULL,
                               (z_hook_list)test_hook_list,
                               (z_hook_anything)test_hook_anything);
            
            z_pd_instance_bind((z_instance *)instance2, tie2,
                               (z_hook_bang)test_hook_bang,
                               (z_hook_float)test_hook_float,
                               (z_hook_symbol)test_hook_symbol,
                               (z_hook_gpointer)NULL,
                               (z_hook_list)test_hook_list,
                               (z_hook_anything)test_hook_anything);
            
            z_pd_messagesend_bang(tie1);
            z_pd_messagesend_float(tie1, 32.54f);
            z_pd_messagesend_gpointer(tie1, NULL);
            z_pd_messagesend_symbol(tie1, z_pd_symbol_create("blabha"));
            z_pd_messagesend_list(tie1, list1);
            z_pd_messagesend_anything(tie1, z_pd_symbol_create("blabha"), list1);
            
            z_pd_messagesend_bang(tie2);
            z_pd_messagesend_float(tie2, 32.54f);
            z_pd_messagesend_gpointer(tie2, NULL);
            z_pd_messagesend_symbol(tie2, z_pd_symbol_create("blabha"));
            z_pd_messagesend_list(tie2, list1);
            z_pd_messagesend_anything(tie2, z_pd_symbol_create("blabha"), list1);
            z_pd_list_free(list1);
            printf("--------------------\n\n\n");
            */
        }
        
        {
            printf("--------------------\n");
            printf("Test Patch :\n");
            printf("--------------------\n");
            char cwd[FILENAME_MAX];
            if(getcwd(cwd, sizeof(cwd)) != NULL)
            {
                printf("--------------------\n");
                printf("Patch Creation :\n");
                printf("--------------------\n");
                z_pd_instance_set((z_instance *)instance1);
                z_patch* patch1 = z_pd_patch_new("../test.pd", cwd);
                printf("Instance 1 : new patch %s from %s\n", z_pd_patch_get_name(patch1), z_pd_patch_get_path(patch1));
                
                
                z_pd_instance_set((z_instance *)instance2);
                z_patch* patch2 = z_pd_patch_new("../test.pd", cwd);
                printf("Instance 2 : new patch %s from %s\n", z_pd_patch_get_name(patch2), z_pd_patch_get_path(patch2));
                printf("--------------------\n\n");
                
                
                printf("--------------------\n");
                printf("Patch Binding :\n");
                printf("--------------------\n");
                sprintf(cwd, "%i-patch", z_pd_patch_get_dollarzero(patch1));
                printf("Instance 1 : %s\n", cwd);
                /*
                z_pd_instance_bind((z_instance *)instance1, z_pd_tie_create(cwd),
                                   (z_hook_bang)test_hook_bang,
                                   (z_hook_float)test_hook_float,
                                   (z_hook_symbol)test_hook_symbol,
                                   (z_hook_gpointer)NULL,
                                   (z_hook_list)test_hook_list,
                                   (z_hook_anything)test_hook_anything);
                
                
                sprintf(cwd, "%i-patch", z_pd_patch_get_dollarzero(patch2));
                printf("Instance 2 : %s\n", cwd);
                z_pd_instance_bind((z_instance *)instance2, z_pd_tie_create(cwd),
                                   (z_hook_bang)test_hook_bang,
                                   (z_hook_float)test_hook_float,
                                   (z_hook_symbol)test_hook_symbol,
                                   (z_hook_gpointer)NULL,
                                   (z_hook_list)test_hook_list,
                                   (z_hook_anything)test_hook_anything);
                printf("--------------------\n\n");
                */
                
                printf("--------------------\n");
                printf("Patch Message :\n");
                printf("--------------------\n");
                sprintf(cwd, "%i-value", z_pd_patch_get_dollarzero(patch1));
                z_pd_messagesend_float(z_pd_tie_create(cwd), 10.f);
                sprintf(cwd, "%i-value", z_pd_patch_get_dollarzero(patch2));
                z_pd_messagesend_float(z_pd_tie_create(cwd), 20.f);
                printf("--------------------\n\n");
                
                
                printf("--------------------\n");
                printf("Patch DSP :\n");
                printf("--------------------\n");
                instance1->ins[0] = (z_sample *)malloc(64 * sizeof(z_sample));
                instance1->ins[1] = (z_sample *)malloc(64 * sizeof(z_sample));
                instance1->outs[0] = (z_sample *)malloc(64 * sizeof(z_sample));
                instance1->outs[1] = (z_sample *)malloc(64 * sizeof(z_sample));
                z_pd_instance_set((z_instance *)instance1);
                z_pd_instance_dsp_prepare((z_instance *)instance1, 2, 2, 44100, 64);
                
                for(size_t i = 0; i < 4; ++i)
                {
                    sprintf(cwd, "%i-bang", z_pd_patch_get_dollarzero(patch1));
                    z_pd_messagesend_bang(z_pd_tie_create(cwd));
                    z_pd_instance_dsp_perform((z_instance *)instance1, 64,
                                              2, (z_sample const**)instance1->ins,
                                              2, (z_sample**)instance1->outs);
                    
                }
                
                instance2->ins[0] = (z_sample *)malloc(64 * sizeof(z_sample));
                instance2->ins[1] = (z_sample *)malloc(64 * sizeof(z_sample));
                instance2->outs[0] = (z_sample *)malloc(64 * sizeof(z_sample));
                instance2->outs[1] = (z_sample *)malloc(64 * sizeof(z_sample));
                z_pd_instance_set((z_instance *)instance2);
                z_pd_instance_dsp_prepare((z_instance *)instance2, 2, 2, 44100, 64);
               
                for(size_t i = 0; i < 4; ++i)
                {
                    sprintf(cwd, "%i-bang", z_pd_patch_get_dollarzero(patch2));
                    z_pd_messagesend_bang(z_pd_tie_create(cwd));
                    z_pd_instance_dsp_perform((z_instance *)instance2, 64,
                                              2, (z_sample const**)instance2->ins,
                                              2, (z_sample**)instance2->outs);
                }
                
                
                free(instance1->ins[0]);
                free(instance1->ins[1]);
                free(instance1->outs[0]);
                free(instance1->outs[1]);
                z_pd_instance_dsp_release((z_instance *)instance1);
                
                free(instance2->ins[0]);
                free(instance2->ins[1]);
                free(instance2->outs[0]);
                free(instance2->outs[1]);
                z_pd_instance_dsp_release((z_instance *)instance2);

                z_pd_instance_set((z_instance *)instance1);
                z_pd_patch_free(patch1);
                z_pd_instance_set((z_instance *)instance2);
                z_pd_patch_free(patch2);
            }            
        }
        
        z_pd_instance_free((z_instance *)instance1);
        z_pd_instance_free((z_instance *)instance2);
    }
    
    z_pd_clear();
    return 0;
}