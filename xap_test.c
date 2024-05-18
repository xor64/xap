#include <stdio.h>
#include <stdlib.h>

// Macro isnt directly used by XAP
#define VERSION "0.0.1"

#define XAP_DISPLAY_VERSION
#define XAP_USE_COLOR
#define XAP_EXIT_ON_ERROR
#define XAP_IMPL
#include "xap.h"

static size_t DEFAULT_UINT = 420;
static s_size_t DEFAULT_INT = -420;
static float DEFAULT_FLOAT = 69.420;
static bool DEFAULT_BOOL = false;

static xap_arg_t args[] = {
    {
        .s_long = "str",
        .s_short = 's',
        .description = "An example how the XAP_ARG_STR type works",
        .type = XAP_ARG_STR,
        .default_value = "Hello :3"
    },
    {
        .s_long = "uint",
        .s_short = 'u',
        .description = "An example how the XAP_ARG_UINT type works",
        .type = XAP_ARG_UINT,
        .default_value = &DEFAULT_UINT
    },
    {
        .s_long = "int",
        .s_short = 'i',
        .description = "An example how the XAP_ARG_INT type works",
        .type = XAP_ARG_INT,
        .default_value = &DEFAULT_INT
    },
    {
        .s_long = "float",
        .s_short = 'f',
        .description = "An example how the XAP_ARG_FLOAT type works",
        .type = XAP_ARG_FLOAT,
        .default_value = &DEFAULT_FLOAT
    },
    {
        .s_long = "bool",
        .s_short = 'b',
        .description = "An example how the XAP_ARG_BOOL type works",
        .type = XAP_ARG_BOOL,
        .default_value = &DEFAULT_BOOL
    },
    {
        .s_long = "toggle",
        .s_short = 't',
        .description = "An example how the XAP_ARG_STR type works",
        .type = XAP_ARG_TOGGLE,
        .default_value = NULL // XAP_ARG_STR doesnt accept default values
    }
};


int main(int argc, char** argv) {
    xap_t xap = {
        .description = "Testing program for XAP :3",
        .version = VERSION,
        .footer = "This pogram is licenced under blah blah blah",
        .post_args_name = "files",
        .post_args_description = "Files to blah blah :3"
    };
    
    
    
    xap_arg_t a_req = {
        .s_long = "required",
        .s_short = 'r',
        .description = "A required value",
        .type = XAP_ARG_STR,
        .default_value = NULL,
        .required = true
    };

    
    xap_arg_add_arr(&xap, args); // Add from a static arr
    xap_arg_add(&xap, a_req);    // Add a single arg

    // Join 2 xap instances, for some reason idk
    // xap_arg_add_many(&xap, xap2->args.items, xap2->args.count);

    xap_parse(&xap, argc, argv);

    
    char* v_str = xap_get_arg_value_str(&xap, "str");
    size_t* v_uint = xap_get_arg_value_uint(&xap, "uint");
    // s_size_t is a macro for ptrdiff_t aka signed size_t
    s_size_t* v_int = xap_get_arg_value_int(&xap, "int");
    float* v_float = xap_get_arg_value_float(&xap, "float");
    bool* v_bool = xap_get_arg_value_bool(&xap, "bool");
    
    // Print all args, for debugging
    // xap_print_args(&xap);
    
}
