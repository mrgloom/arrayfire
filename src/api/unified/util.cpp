/*******************************************************
 * Copyright (c) 2015, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <af/array.h>
#include <af/util.h>
#include "symbol_manager.hpp"

af_err af_print_array(af_array arr)
{
    return CALL(arr);
}

af_err af_print_array_gen(const char *exp, const af_array arr, const int precision)
{
    return CALL(exp, arr, precision);
}

af_err af_save_array(int *index, const char* key, const af_array arr, const char *filename, const bool append)
{
    return CALL(index, key, arr, filename, append);
}

af_err af_read_array_index(af_array *out, const char *filename, const unsigned index)
{
    return CALL(out, filename, index);
}

af_err af_read_array_key(af_array *out, const char *filename, const char* key)
{
    return CALL(out, filename, key);
}

af_err af_read_array_key_check(int *index, const char *filename, const char* key)
{
    return CALL(index, filename, key);
}

af_err af_array_to_string(char **output, const char *exp, const af_array arr,
        const int precision, const bool transpose)
{
    return CALL(output, exp, arr, precision, transpose);
}

af_err af_example_function(af_array* out, const af_array in, const af_someenum_t param)
{
    return CALL(out, in, param);
}

af_err af_get_version(int *major, int *minor, int *patch)
{
    return CALL(major, minor, patch);
}
