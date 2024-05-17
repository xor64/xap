// Copyright 2024 Gvidas Juknevicius <mcorange@mcorangehq.xyz>
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// Portions of this code (marked with "From nob"),
// were adapted from <https://github.com/tsoding/musializer/blob/master/src/nob.h>,
// originally authored by Alexey Kutepov <reximkut@gmail.com>
// All rights reserved by the original creator.


/// # Configurable macros
/// | macro name          | Description                                           |
/// |---------------------|-------------------------------------------------------|
/// | XAP_DISPLAY_VERSION | If defined, enables the version flag                  |
/// | XAP_EXIT_ON_ERROR   | If defined, the program exits immediately on an error |
/// | XAP_USE_COLOR       | If defined, enables the use of color and styles       |
///
/// Redefinable macros:
/// | Macro name   | Description |
/// |--------------|-------------|
/// | XAP_ASSERT   | Assert      |
/// | XAP_REALLOC  | Mem         |
/// | XAP_ALLOC    | Mem         |
/// | XAP_FREE     | Mem         |
///
/// # Public functions
/// 
/// Note: s_size_t is a macro of ptrdiff_t
///
/// | Description                 | Function Definition                                          |
/// |-----------------------------|--------------------------------------------------------------|
/// | Main entry func for XAP     | xap_result_t xap_parse(xap_t* xap, int argc, char** argv);   |
/// | Free all allocated memory   | void xap_free(xap_t xap);                                    |
/// | Get char* value by name     | char* xap_get_arg_value_str(xap_t* xap, char* arg_name);     |
/// | Get size_t* value by name   | size_t* xap_get_arg_value_uint(xap_t* xap, char* arg_name);  |
/// | Get s_size_t* value by name | s_size_t* xap_get_arg_value_int(xap_t* xap, char* arg_name); | 
/// | Get float* value by name    | float* xap_get_arg_value_float(xap_t* xap, char* arg_name);  |
/// | Get bool* value by name     | bool* xap_get_arg_value_bool(xap_t* xap, char* arg_name);    |
/// | Make all text uppercase     | void xap_to_upper(char* str);                                |
/// | Make all text lowercase     | void xap_to_lower(char* str);                                |


#ifndef _H_XAP
#define _H_XAP

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <stdarg.h>

#define s_size_t ptrdiff_t



#ifndef xap_log_error
    #define xap_log_error(fmt, ...) _xap_log_error(fmt, __VA_ARGS__)
#endif

#ifndef XAP_ASSERT
    #define XAP_ASSERT assert
#endif
#ifndef XAP_REALLOC
    #define XAP_REALLOC realloc
#endif
#ifndef XAP_ALLOC
    #define XAP_ALLOC malloc
#endif
#ifndef XAP_FREE
    #define XAP_FREE free
#endif

#ifdef XAP_USE_COLOR
    #define XAP_C_UL  "\e[4m" 
    #define XAP_C_RED "\e[0;31m"
    #define XAP_C_RS  "\e[0m"
    #define XAP_C_BL "\e[1m"
#else
    #define XAP_C_UL  "" 
    #define XAP_C_RED ""
    #define XAP_C_RS  ""
    #define XAP_C_BL ""
#endif


// From nob, starting from here

#define XAP_DA_INIT_CAP 256

#define XAP_ARRAY_LEN(array) (sizeof(array)/sizeof(array[0]))
#define XAP_ARRAY_GET(array, index) \
    (XAP_ASSERT(index >= 0), XAP_ASSERT(index < XAP_ARRAY_LEN(array)), array[index])


#define XAP_DA_PUSH(da, item)                                                            \
    do {                                                                                 \
        if ((da)->count >= (da)->capacity) {                                             \
            (da)->capacity = (da)->capacity == 0 ? XAP_DA_INIT_CAP : (da)->capacity*2;   \
            (da)->items = XAP_REALLOC((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            XAP_ASSERT((da)->items != NULL && "Buy more RAM lol");                       \
        }                                                                                \
                                                                                         \
        (da)->items[(da)->count++] = (item);                                             \
    } while (0)

#define XAP_DA_FREE(da) XAP_FREE((da).items)
/proc/sys/kernel/core_pattern
#define XAP_DA_APPEND(da, new_items, new_items_count)                                           \
    do {                                                                                        \
        if ((da)->count + (new_items_count) > (da)->capacity) {                                 \
            if ((da)->capacity == 0) {                                                          \
                (da)->capacity = XAP_DA_INIT_CAP;                                               \
            }                                                                                   \
            while ((da)->count + (new_items_count) > (da)->capacity) {                          \
                (da)->capacity *= 2;                                                            \
            }                                                                                   \
            (da)->items = XAP_REALLOC((da)->items, (da)->capacity*sizeof(*(da)->items));        \
            XAP_ASSERT((da)->items != NULL && "Buy more RAM lol");                              \
        }                                                                                       \
        memcpy((da)->items + (da)->count, (new_items), (new_items_count)*sizeof(*(da)->items)); \
        (da)->count += (new_items_count);                                                       \
    } while (0)

// Nob code ends here

#define xap_arg_add(xap, arg) XAP_DA_PUSH(&(xap)->args, arg)

typedef enum xap_arg_type_e {
    XAP_ARG_STR,
    XAP_ARG_INT,
    XAP_ARG_UINT,
    XAP_ARG_FLOAT,
    XAP_ARG_BOOL,
    XAP_ARG_TOGGLE,
} xap_arg_type_e;

typedef struct xap_arg_t {
    char* s_long;
    char  s_short;
    char* description;
    void* value;
    void* default_value;
    bool required;
    xap_arg_type_e type;
} xap_arg_t;

typedef struct xap_args_t {
    xap_arg_t* items;
    size_t count;
    size_t capacity;
} xap_args_t;

typedef struct xap_t {
    xap_args_t args;
    char* version;
    char* program;
    char* description;
    char* footer;
    char** post_args;
    size_t post_arg_len;
    char* post_args_name; // Display name for the left over args without the -
    char* post_args_description; // Description for the left over args
} xap_t;


typedef enum xap_result_t {
    XAP_ERR = 0,
    XAP_OK = 1,
    XAP_EXIT = 1,
} xap_result_t;


xap_arg_t* xap_get_arg(xap_t* xap, char* arg_name);
xap_result_t _xap_parse(xap_t* xap, int argc, char** argv); // used internally
xap_result_t xap_parse(xap_t* xap, int argc, char** argv);
char* xap_get_arg_value_str(xap_t* xap, char* arg_name);
size_t* xap_get_arg_value_uint(xap_t* xap, char* arg_name);
s_size_t* xap_get_arg_value_int(xap_t* xap, char* arg_name);
float* xap_get_arg_value_float(xap_t* xap, char* arg_name);
bool* xap_get_arg_value_bool(xap_t* xap, char* arg_name);
xap_result_t xap_parse_arg(xap_arg_t* arg, char* arg_text);
void xap_free(xap_t xap);
void xap_to_upper(char* str);
void xap_to_lower(char* str);
void xap_show_help(xap_t* xap);
void _xap_log_error(char* format, ...);


#ifdef XAP_IMPL

// From nob
char* xap_shift_args(int* argc, char*** argv) {
    XAP_ASSERT(*argc > 0);
    char* result = **argv;
    (*argv) += 1;
    (*argc) -= 1;
    return result;
}

xap_result_t xap_pre_parse(xap_t* xap) {
    xap_arg_t help = {
        .s_short = 'h',
        .s_long = "help",
        .description = "Shows this help text",
        .type = XAP_ARG_TOGGLE,
    };

    xap_arg_add(xap, help);

#ifdef XAP_DISPLAY_VERSION
    xap_arg_t version = {
        .s_short = 'v',
        .s_long = "version",
        .description = "Shows the program version",
        .type = XAP_ARG_TOGGLE,
    };

    xap_arg_add(xap, version);
#endif
}

xap_result_t xap_post_check(xap_t* xap) {
    for (size_t i = 0; i < xap->args.count; i++) {
        xap_arg_t* arg_def = &xap->args.items[i];
        if (arg_def->required) {
            if (arg_def->value == NULL) {
                xap_show_help(xap);
                xap_log_error("Missing required value --%s\n", arg_def->s_long);
                return XAP_EXIT;
            }
        }
    }
    return XAP_OK;
}

xap_result_t xap_parse(xap_t* xap, int argc, char** argv) {
    xap_result_t res = _xap_parse(xap, argc, argv);

#ifdef XAP_EXIT_ON_ERROR
    if (res == XAP_EXIT) {
        exit(1);
    } 
#endif 
    return res;
}

xap_result_t _xap_parse(xap_t* xap, int argc, char** argv) {
    xap->program = xap_shift_args(&argc, &argv);
    xap_pre_parse(xap);
    while (argc > 0) {
        char* arg_str = xap_shift_args(&argc, &argv);
        size_t arg_len = strlen(arg_str);

        if (arg_len > 0 && (*arg_str) == '-') {
            if (arg_len == 2 && (*(arg_str+1)) == '-') {
                // -- means stop parsing or something
                xap->post_args = argv;
                xap->post_arg_len = argc;
                break;
            } else if (arg_len > 1 && (*(arg_str+1)) == '-') {
                // Long
                arg_str = arg_str + 2;
                for (size_t i = 0; i < xap->args.count; i++) {
                    xap_arg_t* arg_def = &xap->args.items[i];
                    if (strcmp(arg_def->s_long, arg_str) == 0) {
                        xap_result_t res = xap_parse_arg(arg_def, arg_str);
                        if (res != XAP_OK) return res; 
                        break;
                    }
                }

            } else {
                // Short 
                arg_str = arg_str + 1;
                for (size_t i = 0; i < xap->args.count; i++) {
                    xap_arg_t* arg_def = &xap->args.items[i];
                    if (arg_len > 2) { // with -
                        // TODO: Print err for invalid arg
                        XAP_ASSERT(0 && "Invalid short flag, it can only contain 1 char");
                        return XAP_ERR;
                    }

                    if (arg_def->s_short == arg_str[0]) {
                        xap_result_t res = xap_parse_arg(arg_def, arg_str);
                        if (res != XAP_OK) return res;
                        break;
                    }
                }
            }
        } else {
            argv += 1;
            argc -= 1;
            xap->post_args = argv;
            xap->post_arg_len = argc;
            break;
        }

    }

    bool* show_help = xap_get_arg_value_bool(xap, "help");
    if (show_help && *show_help) {
        xap_show_help(xap);
        return XAP_EXIT;
    }

#ifdef XAP_DISPLAY_VERSION
    bool* show_version = xap_get_arg_value_bool(xap, "version");
    if (show_version && *show_version) {
        printf("%s version %s", xap->program, xap->version ? xap->version : "None");
        return XAP_EXIT;
    }
#endif
    
    return xap_post_check(xap);
}

void xap_free(xap*) {
    for (size_t i = 0; i < xap->args.count; i++) {
        xap_arg_t* arg_def = &xap->args.items[i];
        XAP_FREE(arg_def->value)
    }
    XAP_FREE(xap->args.items);
}

void xap_show_help(xap_t* xap) {
    char buf[256];

    if (xap->description) {
        printf("%s\n", xap->description);
    }

    printf(XAP_C_UL "" XAP_C_BL "Usage" XAP_C_RS "" XAP_C_BL ": %s " XAP_C_RS "[options] ", xap->program);
    size_t indent_size = 0;


    for (size_t i = 0; i < xap->args.count; i++) {
        xap_arg_t* arg_def = &xap->args.items[i];
        
        size_t len = strlen(arg_def->s_long);
        if (arg_def->required) {
            memcpy(buf, arg_def->s_long, 256);
            xap_to_upper(buf);

            printf(XAP_C_BL "--%s" XAP_C_RS " <%s> ", arg_def->s_long, buf);
            len = (len * 2) + 3;
            if (len > indent_size) indent_size = len;
        } else {
            if (len > indent_size) indent_size = len;
        }
    }



    if (xap->post_args_name != NULL) {
        printf("[args]");
    }
    printf("...\n\n");

    if (xap->post_args_name && xap->post_args_description) {
        printf(XAP_C_UL "" XAP_C_BL "Arguments:\n" XAP_C_RS);
        printf("    [%s]... %s\n\n",xap->post_args_name, xap->post_args_description);
    }
    

    if (xap->args.count > 0) {
        printf(XAP_C_UL "" XAP_C_BL "Options:\n" XAP_C_RS);

        for (size_t i = 0; i < xap->args.count; i++) {
            xap_arg_t* arg_def = &xap->args.items[i];
            printf("    " XAP_C_BL "-%c" XAP_C_RS ", " XAP_C_BL "--%s" XAP_C_RS, arg_def->s_short, arg_def->s_long);
            
            size_t len = strlen(arg_def->s_long);

            if (arg_def->required) {
                memcpy(buf, arg_def->s_long, 256);
                xap_to_upper(buf);
                len = (len * 2) + 3;
                printf(" <%s>", buf);
            }

            for (size_t y = 0; y <= indent_size - len; y++) {
                printf(" ");
            }
            printf("%s ", arg_def->description);
            
            if (arg_def->default_value) {
                switch (arg_def->type) {
                    case XAP_ARG_STR: {
                        printf("[default: \"%s\"]\n", (char*)arg_def->default_value);
                    } break;
                    case XAP_ARG_UINT: {
                        printf("[default: %zu]\n", *(size_t*)arg_def->default_value);
                    } break;
                    case XAP_ARG_INT: {
                        printf("[default: %ti]\n", *(s_size_t*)arg_def->default_value);
                    } break;
                    case XAP_ARG_FLOAT: {
                        printf("[default: %.6f]\n", *(float*)arg_def->default_value);
                    } break;
                    case XAP_ARG_BOOL: {
                        printf("[default: %s]\n", 
                                (*(bool*)arg_def->default_value) ? "true" : "false");
                    } break;
                    default:
                        break;
                }
            } else {
                printf("\n");
            }
        }
    }

    if (xap->footer) {
        printf("\n%s\n", xap->footer);
    }
}

void _xap_log_error(char* format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, XAP_C_BL"[" XAP_C_RED "ERROR" XAP_C_RS "" XAP_C_BL "] " XAP_C_RS);
    vfprintf(stderr, format, args);
    va_end(args);
}

void xap_print_args(xap_t* xap) {
    for (size_t i = 0; i < xap->args.count; i++) {
        xap_arg_t* arg_def = &xap->args.items[i];
        printf("%s -> ", arg_def->s_long);
        void* def = arg_def->default_value;
        void* val = arg_def->value;
        switch (arg_def->type) {
            case XAP_ARG_STR: {
                printf("%s (\"%s\") (XAP_ARG_STR)\n", 
                        val ? (char*)val : "NULL",
                        def ? (char*)def : "NULL");
            } break;
            case XAP_ARG_UINT: {
                printf("%zu (%zu) (XAP_ARG_UINT)\n", 
                        val ? *(size_t*)val : 0,
                        def ? *(size_t*)def : 0);
            } break;
            case XAP_ARG_INT: {
                printf("%ti (%ti) (XAP_ARG_INT)\n", 
                        val ? *(s_size_t*)val : 0, 
                        def ? *(s_size_t*)def : 0);
            } break;
            case XAP_ARG_FLOAT: {
                printf("%.6f (%.6f) (XAP_ARG_FLOAT)\n", 
                        val ? *(float*)val : 0.0f,
                        def ? *(float*)def : 0.0f);
            } break;
            case XAP_ARG_BOOL: {
                bool v = val ? *(bool*)val : false;
                bool d = def ? *(bool*)def : false;
                printf("%s (%s) (XAP_ARG_BOOL)\n", 
                        v ? "true" : "false",
                        d ? "true" : "false");
            } break;
            case XAP_ARG_TOGGLE: {
                bool v = val ? *(bool*)val : false;
                printf("%s (NULL) (XAP_ARG_TOGGLE)\n", 
                        v ? "true" : "false");
            } break;
            default:
                break;
        }
    }

}

xap_arg_t* xap_get_arg(xap_t* xap, char* arg_name) {
    for (size_t i = 0; i < xap->args.count; i++) {
        xap_arg_t* arg_def = &xap->args.items[i];
        if (strcmp(arg_def->s_long, arg_name) == 0) {
            return arg_def;
        }
    }
    return NULL;
}

char* xap_get_arg_value_str(xap_t* xap, char* arg_name) {
    xap_arg_t* arg_def = xap_get_arg(xap, arg_name);
    if (!arg_def) return NULL;
    return (char*)arg_def->value;
}

size_t* xap_get_arg_value_uint(xap_t* xap, char* arg_name) {
    xap_arg_t* arg_def = xap_get_arg(xap, arg_name);
    if (!arg_def) return NULL;
    return (size_t*)arg_def->value;
}

s_size_t* xap_get_arg_value_int(xap_t* xap, char* arg_name) {
    xap_arg_t* arg_def = xap_get_arg(xap, arg_name);
    if (!arg_def) return NULL;
    return (s_size_t*)arg_def->value;
}

float* xap_get_arg_value_float(xap_t* xap, char* arg_name) {
    xap_arg_t* arg_def = xap_get_arg(xap, arg_name);
    if (!arg_def) return NULL;
    return (float*)arg_def->value;
}

bool* xap_get_arg_value_bool(xap_t* xap, char* arg_name) {
    xap_arg_t* arg_def = xap_get_arg(xap, arg_name);
    if (!arg_def) return NULL;
    return (bool*)arg_def->value;
}






xap_result_t xap_parse_arg(xap_arg_t* arg, char* arg_text) {
    switch (arg->type) {
        case XAP_ARG_STR: {
            size_t len = strlen(arg_text) + 1; // for null byte
            char* arg_val = XAP_ALLOC(sizeof(char)*len);
            memmove(arg_val, arg_text, len);
            arg->value = arg_val;
        } break;
        case XAP_ARG_UINT: {
            size_t val = strtoul(arg_text, NULL, 0); // supports dec, hex, bin, oct, i think
            size_t* arg_val = XAP_ALLOC(sizeof(size_t));
            *arg_val = val;
            arg->value = arg_val;
        } break;
        case XAP_ARG_INT: {
            s_size_t val = strtol(arg_text, NULL, 0); // supports dec, hex, bin, oct, i think
            s_size_t* arg_val = XAP_ALLOC(sizeof(s_size_t));
            *arg_val = val;
            arg->value = arg_val;
        } break;
        case XAP_ARG_FLOAT: {
            float val = strtof(arg_text, NULL);
            float* arg_val = XAP_ALLOC(sizeof(float));
            *arg_val = val;
            arg->value = arg_val;
        } break;
        case XAP_ARG_BOOL: {
            bool* arg_val = XAP_ALLOC(sizeof(bool));
            
            xap_to_lower(arg_text);

            if (strcmp(arg_text, "true") == 0) {
                *arg_val = false;
            } if (strcmp(arg_text, "true") == 0) {
                *arg_val = true;
            } else {
                // TODO: Print error
                XAP_ASSERT(0 && "Invalid bool text");
                return XAP_ERR;
            }
            arg->value = arg_val;
        } break; 
        case XAP_ARG_TOGGLE: {
            bool* arg_val = XAP_ALLOC(sizeof(bool));
            *arg_val = true;
            arg->value = arg_val;
        } break;
        default:
            XAP_ASSERT(0 && "Invalid arg type");
            return XAP_ERR;
    }
    return XAP_OK;
}

void xap_to_lower(char* str) {
    for(int i = 0; str[i]; i++){
      str[i] = tolower(str[i]);
    }
}

void xap_to_upper(char* str) {
    for(int i = 0; str[i]; i++){
      str[i] = toupper(str[i]);
    }
}

#endif
#endif
