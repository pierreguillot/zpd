/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

extern "C"
{
#include "../zpd/z_pd.h"
}

static int test_counter = 0;
static char const* section_name = NULL;
#define TEST_START(message) {test_counter = 0; section_name = NULL; printf("Test Start : %s\n", message);}
#define TEST_END() {printf("Number of Tests %i\n", test_counter);}

#define TEST_SECTION(message) {section_name = message;}
#define TEST_TRUE(message, a) if((++test_counter) && !(a)) {if(section_name){printf("Error %s in [%s]\n", message, section_name);}else{printf("Error %s\n", message);} return 1;}
#define TEST_FALSE(message, a) if((++test_counter)&& (a)) {if(section_name){printf("Error %s in [%s]\n", message, section_name);}else{printf("Error %s\n", message);} return 1;}

#include <stdio.h>

#ifdef ZPD_TEST_VERBOSE
#define zprintf printf
#else
static void zprintf(char const* message, ...)
{
    
}
#endif

typedef struct _test_instance
{
    z_instance      z_instance;
    unsigned char   z_index;
    size_t          z_ninputs;
    z_sample**      z_inputs;
    size_t          z_noutputs;
    z_sample**      z_outputs;
    unsigned char   z_post_inc;
    unsigned char   z_log_inc;
    unsigned char   z_error_inc;
    unsigned char   z_fatal_inc;
    
    unsigned char   z_bang_inc;
    unsigned char   z_float_inc;
    unsigned char   z_symbol_inc;
    unsigned char   z_list_inc;
    unsigned char   z_anything_inc;
}z_test_instance;

static void test_hook_post(z_test_instance* instance, const char *s)
{
    instance->z_post_inc++;
    zprintf("Instance %u (post) : %s", instance->z_index, s);
}

static void test_hook_log(z_test_instance* instance, const char *s)
{
    instance->z_log_inc++;
    zprintf("Instance %u (log) : %s", instance->z_index, s);
}

static void test_hook_error(z_test_instance* instance, const char *s)
{
    instance->z_error_inc++;
    zprintf("Instance %u (error) : %s", instance->z_index, s);
}

static void test_hook_fatal(z_test_instance* instance, const char *s)
{
    instance->z_fatal_inc++;
    zprintf("Instance %u (fatal) : %s", instance->z_index, s);
}


static void test_hook_bang(z_test_instance* instance, z_tie* tie)
{
    instance->z_bang_inc++;
    zprintf("Instance %u (bang) : from %s\n", instance->z_index, z_pd_tie_get_name(tie));
}

static void test_hook_float(z_test_instance* instance, z_tie* tie, z_float f)
{
    instance->z_float_inc++;
    zprintf("Instance %u (float) : from %s - %f\n", instance->z_index, z_pd_tie_get_name(tie), f);
}

static void test_hook_symbol(z_test_instance* instance, z_tie* tie, z_symbol* s)
{
    instance->z_symbol_inc++;
    zprintf("Instance %u (symbol) : from %s - %s\n", instance->z_index, z_pd_tie_get_name(tie), z_pd_symbol_get_name(s));
}

static void test_hook_list(z_test_instance* instance, z_tie* tie, z_list *list)
{
    size_t i = 0;
    size_t const size = z_pd_list_get_size(list);
    instance->z_list_inc++;
    zprintf("Instance %u (list) : from %s -", instance->z_index, z_pd_tie_get_name(tie));
    for(i = 0; i < size; ++i)
    {
        if(z_pd_list_get_type(list, i) == Z_FLOAT)
        {
            zprintf(" %f", z_pd_list_get_float(list, i));
        }
        else if(z_pd_list_get_type(list, i) == Z_SYMBOL)
        {
            zprintf(" %s", z_pd_symbol_get_name(z_pd_list_get_symbol(list, i)));
        }
    }
    zprintf("\n");
}

static void test_hook_anything(z_test_instance* instance, z_tie* tie, z_symbol *s, z_list *list)
{
    size_t i = 0;
    size_t const size = z_pd_list_get_size(list);
    instance->z_anything_inc++;
    zprintf("Instance %u (anything) : from %s - %s", instance->z_index, z_pd_tie_get_name(tie), z_pd_symbol_get_name(s));
    for(i = 0; i < size; ++i)
    {
        if(z_pd_list_get_type(list, i) == Z_FLOAT)
        {
            zprintf(" %f", z_pd_list_get_float(list, i));
        }
        else if(z_pd_list_get_type(list, i) == Z_SYMBOL)
        {
            zprintf(" %s", z_pd_symbol_get_name(z_pd_list_get_symbol(list, i)));
        }
    }
    zprintf("\n");
}


static z_test_instance* test_new_instance(unsigned char index, size_t ninputs, size_t noutputs, size_t vectorsize)
{
    size_t i;
    z_sample* temp;
    z_test_instance* inst = (z_test_instance *)z_pd_instance_new(sizeof(z_test_instance));
    if(inst)
    {
        inst->z_index   = index;
        inst->z_inputs  = (z_sample **)malloc(ninputs * sizeof(z_sample *));
        inst->z_post_inc = 0;
        inst->z_log_inc = 0;
        inst->z_error_inc = 0;
        inst->z_fatal_inc = 0;
        inst->z_bang_inc = 0;
        inst->z_float_inc = 0;
        inst->z_symbol_inc = 0;
        inst->z_list_inc = 0;
        inst->z_anything_inc = 0;
        if(inst->z_inputs)
        {
            inst->z_ninputs = ninputs;
            temp = (z_sample *)malloc(ninputs * vectorsize * sizeof(z_sample *));
            if(temp)
            {
                for(i = 0; i < inst->z_ninputs; i++)
                {
                    inst->z_inputs[i] = temp+i*vectorsize;
                }
            }
            else
            {
                free(inst->z_inputs);
                z_pd_instance_free((z_instance*)inst);
                return NULL;
            }
        }
        else
        {
            z_pd_instance_free((z_instance*)inst);
            return NULL;
        }
        inst->z_outputs  = (z_sample **)malloc(noutputs * sizeof(z_sample *));
        if(inst->z_inputs)
        {
            inst->z_noutputs = noutputs;
            temp = (z_sample *)malloc(noutputs * vectorsize * sizeof(z_sample *));
            if(temp)
            {
                for(i = 0; i < inst->z_noutputs; i++)
                {
                    inst->z_outputs[i] = temp+i*vectorsize;
                }
            }
            else
            {
                free(inst->z_inputs[0]);
                free(inst->z_inputs);
                free(inst->z_outputs);
                z_pd_instance_free((z_instance*)inst);
                return NULL;
            }
        }
        else
        {
            free(inst->z_inputs[0]);
            free(inst->z_inputs);
            z_pd_instance_free((z_instance*)inst);
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
    zprintf("Instance %u with %zu inputs, %zu outputs and %zu vectorsize\n", index, ninputs, noutputs, vectorsize);
    return inst;
}

static void test_free_instance(z_test_instance* inst)
{
    free(inst->z_inputs[0]);
    free(inst->z_inputs);
    free(inst->z_outputs[0]);
    free(inst->z_outputs);
    z_pd_instance_free((z_instance*)inst);
}

static void test_start_part(const char* message)
{
    zprintf("--------------------\n");
    zprintf("%s\n", message);
    zprintf("--------------------\n");
}

static void test_end_part()
{
    zprintf("--------------------\n\n\n");
}

static void test_allocation_failed(const char* message)
{
    zprintf("Allocation failed %s \n", message);
}

static char* test_get_patch_folder(char* location)
{
    size_t size, i;
    if(location)
    {
        size = strlen(location);
        for(i = 0; i < size; ++i)
        {
            if(strncmp(location+i, "/build/", 7) == 0)
            {
                memset(location+i+7, '\0', size-i);
                sprintf(location+i+7, "../test/");
                return location;
            }
            if(strncmp(location+i, "/zpd/", 5) == 0)
            {
                memset(location+i+5, '\0', size-i);
                sprintf(location+i+5, "/test/");
                return location;
            }
            if(strncmp(location+i, "/test/", 6) == 0)
            {
                memset(location+i+6, '\0', size-i);
                return location;
            }
        }
    }
    return NULL;
}


int main(int argc, char** argv)
{
    size_t i = 0;
    char *location = NULL, bindingname[1000];
    z_test_instance *inst1 = NULL, *inst2= NULL;
    z_tie *tie1 = NULL, *tie2 = NULL;
    z_patch *patch1 = NULL, *patch2 = NULL;
    z_list* list = NULL;
    
    z_hook_console hook_console;
    hook_console.m_post     = (z_hook_print)test_hook_post;
    hook_console.m_log      = (z_hook_print)test_hook_log;
    hook_console.m_error    = (z_hook_print)test_hook_error;
    hook_console.m_fatal    = (z_hook_print)test_hook_fatal;
    z_hook_message hook_message;
    hook_message.m_bang     = (z_hook_bang)test_hook_bang;
    hook_message.m_float    = (z_hook_float)test_hook_float;
    hook_message.m_symbol   = (z_hook_symbol)test_hook_symbol;
    hook_message.m_gpointer = (z_hook_gpointer)NULL;
    hook_message.m_list     = (z_hook_list)test_hook_list;
    hook_message.m_anything = (z_hook_anything)test_hook_anything;
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////
    
    TEST_START("zpd")
    TEST_SECTION("Initialization")
    {
        z_pd_init();
        z_pd_init();
        z_pd_searchpath_clear();
        zprintf("Pure Data %u.%u.%u :\n", z_pd_version_getmajor(), z_pd_version_getminor(), z_pd_version_getbug());
        TEST_TRUE("test location", (location = test_get_patch_folder(argv[0])))
        z_pd_searchpath_add(location);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////
    
    TEST_SECTION("Instance")
    {
        TEST_FALSE("z_pd_instance_new", z_pd_instance_new(-1))
        TEST_TRUE("z_pd_instance_new", (inst1 = test_new_instance(1, 2, 2, 64)))
        TEST_TRUE("z_pd_instance_new", (inst2 = test_new_instance(1, 2, 2, 64)))
        
        z_pd_instance_set_hook_console((z_instance *)inst1, &hook_console);
        z_pd_instance_set_hook_console((z_instance *)inst2, &hook_console);
        z_pd_instance_set((z_instance *)inst1);
        z_pd_console_post("Print a normal post");
        z_pd_console_log("Print a log post");
        z_pd_console_error("Print an error");
        z_pd_console_fatal("Print a fatal error");
        z_pd_instance_set((z_instance *)inst2);
        z_pd_console_post("Print a normal post");
        z_pd_console_log("Print a log post");
        z_pd_console_error("Print an error");
        z_pd_console_fatal("Print a fatal error");
        
        TEST_TRUE("z_pd_console_post", inst1->z_post_inc == 1)
        TEST_TRUE("z_pd_console_post", inst1->z_log_inc == 1)
        TEST_TRUE("z_pd_console_post", inst1->z_error_inc == 1)
        TEST_TRUE("z_pd_console_post", inst1->z_fatal_inc == 1)
        
        TEST_TRUE("z_pd_console_post", inst2->z_post_inc == 1)
        TEST_TRUE("z_pd_console_post", inst2->z_log_inc == 1)
        TEST_TRUE("z_pd_console_post", inst2->z_error_inc == 1)
        TEST_TRUE("z_pd_console_post", inst2->z_fatal_inc == 1)
        
        TEST_TRUE("z_pd_tie_create", (tie1 = z_pd_tie_create("tie1")))
        TEST_TRUE("z_pd_tie_create", (tie2 = z_pd_tie_create("tie2")))
        TEST_TRUE("z_pd_list_create", (list = z_pd_list_create(3)))
        
        z_pd_list_set_float(list, 0, 0.f);
        z_pd_list_set_float(list, 1, 1.f);
        z_pd_list_set_symbol(list, 2, z_pd_symbol_create("symbol"));
        TEST_TRUE("z_pd_list_get_size", (z_pd_list_get_size(list) == 3))
        TEST_TRUE("z_pd_list_get_type", (z_pd_list_get_type(list, 0) == Z_FLOAT))
        TEST_TRUE("z_pd_list_get_type", (z_pd_list_get_type(list, 1) == Z_FLOAT))
        TEST_TRUE("z_pd_list_get_type", (z_pd_list_get_type(list, 2) == Z_SYMBOL))
        TEST_TRUE("z_pd_list_get_float", (z_pd_list_get_float(list, 0) == 0.f))
        TEST_TRUE("z_pd_list_get_float", (z_pd_list_get_float(list, 1) == 1.f))
        TEST_TRUE("z_pd_list_get_symbol", (z_pd_list_get_symbol(list, 2) == z_pd_symbol_create("symbol")))
        
        z_pd_instance_bind((z_instance *)inst1, tie1, &hook_message);
        z_pd_instance_bind((z_instance *)inst1, tie1, &hook_message);
        z_pd_instance_bind((z_instance *)inst2, tie2, &hook_message);
        
        z_pd_messagesend_bang(tie1);
        z_pd_messagesend_float(tie1, 32.54f);
        z_pd_messagesend_gpointer(tie1, NULL);
        z_pd_messagesend_symbol(tie1, z_pd_symbol_create("blabha"));
        z_pd_messagesend_list(tie1, list);
        z_pd_messagesend_anything(tie1, z_pd_symbol_create("blabha"), list);
        
        z_pd_messagesend_bang(tie2);
        z_pd_messagesend_float(tie2, 32.54f);
        z_pd_messagesend_gpointer(tie2, NULL);
        z_pd_messagesend_symbol(tie2, z_pd_symbol_create("blabha"));
        z_pd_messagesend_list(tie2, list);
        z_pd_messagesend_anything(tie2, z_pd_symbol_create("blabha"), list);
        
        TEST_TRUE("z_pd_messagesend_bang", inst1->z_bang_inc == 1)
        TEST_TRUE("z_pd_messagesend_float", inst1->z_float_inc == 1)
        TEST_TRUE("z_pd_messagesend_symbol", inst1->z_symbol_inc == 1)
        TEST_TRUE("z_pd_messagesend_list", inst1->z_list_inc == 1)
        TEST_TRUE("z_pd_messagesend_anything", inst1->z_anything_inc == 1)
        
        TEST_TRUE("z_pd_messagesend_bang", inst2->z_bang_inc == 1)
        TEST_TRUE("z_pd_messagesend_float", inst2->z_float_inc == 1)
        TEST_TRUE("z_pd_messagesend_symbol", inst2->z_symbol_inc == 1)
        TEST_TRUE("z_pd_messagesend_list", inst2->z_list_inc == 1)
        TEST_TRUE("z_pd_messagesend_anything", inst2->z_anything_inc == 1)
        
        z_pd_instance_unbind((z_instance *)inst1, tie1);
        z_pd_instance_unbind((z_instance *)inst2, tie2);
        
        z_pd_messagesend_bang(tie1);
        z_pd_messagesend_float(tie1, 32.54f);
        z_pd_messagesend_gpointer(tie1, NULL);
        z_pd_messagesend_symbol(tie1, z_pd_symbol_create("blabha"));
        z_pd_messagesend_list(tie1, list);
        z_pd_messagesend_anything(tie1, z_pd_symbol_create("blabha"), list);
        
        z_pd_messagesend_bang(tie2);
        z_pd_messagesend_float(tie2, 32.54f);
        z_pd_messagesend_gpointer(tie2, NULL);
        z_pd_messagesend_symbol(tie2, z_pd_symbol_create("blabha"));
        z_pd_messagesend_list(tie2, list);
        z_pd_messagesend_anything(tie2, z_pd_symbol_create("blabha"), list);
        
        TEST_TRUE("z_pd_messagesend_bang", inst1->z_bang_inc == 1)
        TEST_TRUE("z_pd_messagesend_float", inst1->z_float_inc == 1)
        TEST_TRUE("z_pd_messagesend_symbol", inst1->z_symbol_inc == 1)
        TEST_TRUE("z_pd_messagesend_list", inst1->z_list_inc == 1)
        TEST_TRUE("z_pd_messagesend_anything", inst1->z_anything_inc == 1)
        
        TEST_TRUE("z_pd_messagesend_bang", inst2->z_bang_inc == 1)
        TEST_TRUE("z_pd_messagesend_float", inst2->z_float_inc == 1)
        TEST_TRUE("z_pd_messagesend_symbol", inst2->z_symbol_inc == 1)
        TEST_TRUE("z_pd_messagesend_list", inst2->z_list_inc == 1)
        TEST_TRUE("z_pd_messagesend_anything", inst2->z_anything_inc == 1)
        
        z_pd_list_free(list);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////
    
    TEST_SECTION("Patches & Objects")
    {
        int size = 0, x = 0, y = 0, width = 0, height = 0; char* text = NULL;
        z_pd_instance_set((z_instance *)inst1);
        TEST_FALSE("z_pd_patch_new", (patch1 = z_pd_patch_new("test.pd", "zaza")))
        TEST_TRUE("z_pd_patch_new", (patch1 = z_pd_patch_new("test.pd", NULL)))
        sprintf(bindingname, "%i-patch", z_pd_patch_get_dollarzero(patch1));
        TEST_TRUE("z_pd_patch_get_height", (tie1 = z_pd_tie_create(bindingname)))
        
        TEST_TRUE("z_pd_patch_get_name", z_pd_patch_get_name(patch1))
        TEST_TRUE("z_pd_patch_get_path", z_pd_patch_get_path(patch1))
        TEST_TRUE("z_pd_patch_get_x", z_pd_patch_get_x(patch1) == 100)
        TEST_TRUE("z_pd_patch_get_x", z_pd_patch_get_y(patch1) == 100)
        TEST_TRUE("z_pd_patch_get_width", z_pd_patch_get_width(patch1) == 85)
        TEST_TRUE("z_pd_patch_get_height", z_pd_patch_get_height(patch1) == 60)
        
        z_pd_instance_set((z_instance *)inst2);
        TEST_TRUE("z_pd_patch_new", (patch2 = z_pd_patch_new("test.pd", NULL)));
        sprintf(bindingname, "%i-patch", z_pd_patch_get_dollarzero(patch2));
        TEST_TRUE("z_pd_patch_get_height", (tie2 = z_pd_tie_create(bindingname)))
        
        z_pd_instance_bind((z_instance *)inst1, tie1, &hook_message);
        z_pd_instance_bind((z_instance *)inst2, tie2, &hook_message);
        
        
        z_object* obj = NULL;
        TEST_TRUE("z_pd_patch_get_first_object", (obj = z_pd_patch_get_first_object(patch2)));
        while((obj = z_pd_patch_get_next_object(patch2, obj)))
        {
            TEST_TRUE("z_pd_patch_get_next_object", obj);
            TEST_TRUE("z_pd_object_get_name", z_pd_object_get_name(obj));
            z_pd_object_get_text(obj, &size, &text);
            TEST_TRUE("z_pd_object_get_text", (size && text));
            size = 0;
            free(text);
            z_pd_object_get_bounds(obj, patch2, &x, &y, &width, &height);
            x = 0; y = 0; width = 0; height = 0;
        }
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////
    
    if(1)
    {
        
        
        sprintf(bindingname, "%i-value", z_pd_patch_get_dollarzero(patch1));
        tie1 = z_pd_tie_create(bindingname);
        if(!tie1)
        {
            test_allocation_failed("Tie 1");
            return 0;
        }
        z_pd_messagesend_float(tie1, 10.f);
        sprintf(bindingname, "%i-value", z_pd_patch_get_dollarzero(patch2));
        tie2 = z_pd_tie_create(bindingname);
        if(!tie1)
        {
            test_allocation_failed("Tie 2");
            return 0;
        }
        z_pd_messagesend_float(tie2, 20.f);
       
        test_end_part();
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////
    
    if(1)
    {
        test_start_part("Patch DSP");
        sprintf(bindingname, "%i-bang", z_pd_patch_get_dollarzero(patch1));
        tie1 = z_pd_tie_create(bindingname);
        if(!tie1)
        {
            test_allocation_failed("Tie 1");
            return 0;
        }
        sprintf(bindingname, "%i-bang", z_pd_patch_get_dollarzero(patch2));
        tie2 = z_pd_tie_create(bindingname);
        if(!tie2)
        {
            test_allocation_failed("Tie 2");
            return 0;
        }
        
        z_pd_instance_set((z_instance *)inst1);
        z_pd_instance_dsp_prepare((z_instance *)inst1, 2, 2, 44100, 64);
        if(z_pd_instance_get_samplerate((z_instance *)inst1) != 44100)
        {
            z_pd_console_error("Wrong sample rate %i", z_pd_instance_get_samplerate((z_instance *)inst1));
            return 1;
        }
        for(i = 0; i < 4; ++i)
        {
            z_pd_messagesend_bang(tie1);
            z_pd_instance_dsp_perform((z_instance *)inst1, 64,
                                      inst1->z_ninputs, (z_sample const**)inst1->z_inputs,
                                      inst1->z_noutputs, (z_sample**)inst1->z_outputs);
            
        }
        
        z_pd_instance_set((z_instance *)inst2);
        z_pd_instance_dsp_prepare((z_instance *)inst2, 2, 2, 44100, 64);
        if(z_pd_instance_get_samplerate((z_instance *)inst2) != 44100)
        {
            z_pd_console_error("Wrong sample rate %i", z_pd_instance_get_samplerate((z_instance *)inst1));
            return 1;
        }
        for(i = 0; i < 4; ++i)
        {
            z_pd_messagesend_bang(tie2);
            z_pd_instance_dsp_perform((z_instance *)inst2, 64,
                                      inst2->z_ninputs, (z_sample const**)inst2->z_inputs,
                                      inst2->z_noutputs, (z_sample**)inst2->z_outputs);
        }
        
        
        z_pd_instance_dsp_release((z_instance *)inst1);
        z_pd_instance_dsp_release((z_instance *)inst2);
        
        test_end_part();
    }
    
    if(1)
    {
        test_start_part("Free system");
        z_pd_instance_set((z_instance *)inst1);
        z_pd_patch_free(patch1);
        z_pd_instance_set((z_instance *)inst2);
        z_pd_patch_free(patch2);
        
        test_free_instance(inst1);
        test_free_instance(inst2);
        
        z_pd_clear();
        test_end_part();
    }
    TEST_END()
    return 0;
}