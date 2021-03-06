/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <af/array.h>
#include <af/algorithm.h>
#include <af/defines.h>
#include <err_common.hpp>
#include <handle.hpp>
#include <backend.hpp>
#include <ArrayInfo.hpp>
#include <sort.hpp>
#include <sort_index.hpp>
#include <sort_by_key.hpp>
#include <copy.hpp>

#include<cstdio>

using af::dim4;
using namespace detail;

template<typename T>
static inline af_array sort(const af_array in, const unsigned dim, const bool isAscending)
{
    const Array<T> &inArray = getArray<T>(in);
    if(isAscending) {
        return getHandle(sort<T, 1>(inArray, dim));
    } else {
        return getHandle(sort<T, 0>(inArray, dim));
    }
}

af_err af_sort(af_array *out, const af_array in, const unsigned dim, const bool isAscending)
{
    try {
        ArrayInfo info = getInfo(in);
        af_dtype type = info.getType();

        DIM_ASSERT(1, info.elements() > 0);
        // Only Dim 0 supported
        ARG_ASSERT(2, dim == 0);

        af_array val;

        switch(type) {
            case f32: val = sort<float  >(in, dim, isAscending);  break;
            case f64: val = sort<double >(in, dim, isAscending);  break;
            case s32: val = sort<int    >(in, dim, isAscending);  break;
            case u32: val = sort<uint   >(in, dim, isAscending);  break;
            case s16: val = sort<short  >(in, dim, isAscending);  break;
            case u16: val = sort<ushort >(in, dim, isAscending);  break;
            case u8:  val = sort<uchar  >(in, dim, isAscending);  break;
            case b8:  val = sort<char   >(in, dim, isAscending);  break;
            default:  TYPE_ERROR(1, type);
        }
        std::swap(*out, val);
    }
    CATCHALL;

    return AF_SUCCESS;
}

template<typename T>
static inline void sort_index(af_array *val, af_array *idx, const af_array in,
                              const unsigned dim, const bool isAscending)
{
    const Array<T> &inArray = getArray<T>(in);

    // Initialize Dummy Arrays
    Array<T> valArray = createEmptyArray<T>(af::dim4());
    Array<uint> idxArray = createEmptyArray<uint>(af::dim4());

    if(isAscending) {
        sort_index<T, 1>(valArray, idxArray, inArray, dim);
    } else {
        sort_index<T, 0>(valArray, idxArray, inArray, dim);
    }
    *val = getHandle(valArray);
    *idx = getHandle(idxArray);
}

af_err af_sort_index(af_array *out, af_array *indices, const af_array in, const unsigned dim, const bool isAscending)
{
    try {
        ArrayInfo info = getInfo(in);
        af_dtype type = info.getType();

        DIM_ASSERT(2, info.elements() > 0);
        // Only Dim 0 supported
        ARG_ASSERT(3, dim == 0);

        af_array val;
        af_array idx;

        switch(type) {
            case f32: sort_index<float  >(&val, &idx, in, dim, isAscending);  break;
            case f64: sort_index<double >(&val, &idx, in, dim, isAscending);  break;
            case s32: sort_index<int    >(&val, &idx, in, dim, isAscending);  break;
            case u32: sort_index<uint   >(&val, &idx, in, dim, isAscending);  break;
            case s16: sort_index<short  >(&val, &idx, in, dim, isAscending);  break;
            case u16: sort_index<ushort >(&val, &idx, in, dim, isAscending);  break;
            case u8:  sort_index<uchar  >(&val, &idx, in, dim, isAscending);  break;
            case b8:  sort_index<char   >(&val, &idx, in, dim, isAscending);  break;
            default:  TYPE_ERROR(1, type);
        }
        std::swap(*out , val);
        std::swap(*indices, idx);
    }
    CATCHALL;

    return AF_SUCCESS;
}

template<typename Tk, typename Tv>
static inline void sort_by_key(af_array *okey, af_array *oval, const af_array ikey, const af_array ival,
                               const unsigned dim, const bool isAscending)
{
    const Array<Tk> &ikeyArray = getArray<Tk>(ikey);
    const Array<Tv> &ivalArray = getArray<Tv>(ival);

    // Initialize Dummy Arrays
    Array<Tk> okeyArray = createEmptyArray<Tk>(af::dim4());
    Array<Tv> ovalArray = createEmptyArray<Tv>(af::dim4());

    if(isAscending) {
        sort_by_key<Tk, Tv, 1>(okeyArray, ovalArray, ikeyArray, ivalArray, dim);
    } else {
        sort_by_key<Tk, Tv, 0>(okeyArray, ovalArray, ikeyArray, ivalArray, dim);
    }
    *okey = getHandle(okeyArray);
    *oval = getHandle(ovalArray);
}

template<typename Tk>
void sort_by_key_tmplt(af_array *okey, af_array *oval, const af_array ikey, const af_array ival,
                       const unsigned dim, const bool isAscending)
{
    ArrayInfo info = getInfo(ival);
    af_dtype vtype = info.getType();

    switch(vtype) {
    case f32: sort_by_key<Tk, float  >(okey, oval, ikey, ival, dim, isAscending);  break;
    case f64: sort_by_key<Tk, double >(okey, oval, ikey, ival, dim, isAscending);  break;
    case s32: sort_by_key<Tk, int    >(okey, oval, ikey, ival, dim, isAscending);  break;
    case u32: sort_by_key<Tk, uint   >(okey, oval, ikey, ival, dim, isAscending);  break;
    case s16: sort_by_key<Tk, short  >(okey, oval, ikey, ival, dim, isAscending);  break;
    case u16: sort_by_key<Tk, ushort >(okey, oval, ikey, ival, dim, isAscending);  break;
    case u8:  sort_by_key<Tk, uchar  >(okey, oval, ikey, ival, dim, isAscending);  break;
    case b8:  sort_by_key<Tk, char   >(okey, oval, ikey, ival, dim, isAscending);  break;
    default:  TYPE_ERROR(1, vtype);
    }

    return;
}

af_err af_sort_by_key(af_array *out_keys, af_array *out_values,
                      const af_array keys, const af_array values,
                      const unsigned dim, const bool isAscending)
{
    try {
        ArrayInfo info = getInfo(keys);
        af_dtype type = info.getType();

        ArrayInfo vinfo = getInfo(values);

        DIM_ASSERT(3, info.elements() > 0);
        DIM_ASSERT(4, info.dims() == vinfo.dims());
        // Only Dim 0 supported
        ARG_ASSERT(5, dim == 0);

        af_array oKey;
        af_array oVal;

        switch(type) {
            case f32: sort_by_key_tmplt<float  >(&oKey, &oVal, keys, values, dim, isAscending);  break;
            case f64: sort_by_key_tmplt<double >(&oKey, &oVal, keys, values, dim, isAscending);  break;
            case s32: sort_by_key_tmplt<int    >(&oKey, &oVal, keys, values, dim, isAscending);  break;
            case u32: sort_by_key_tmplt<uint   >(&oKey, &oVal, keys, values, dim, isAscending);  break;
            case s16: sort_by_key_tmplt<short  >(&oKey, &oVal, keys, values, dim, isAscending);  break;
            case u16: sort_by_key_tmplt<ushort >(&oKey, &oVal, keys, values, dim, isAscending);  break;
            case u8:  sort_by_key_tmplt<uchar  >(&oKey, &oVal, keys, values, dim, isAscending);  break;
            case b8:  sort_by_key_tmplt<char   >(&oKey, &oVal, keys, values, dim, isAscending);  break;
            default:  TYPE_ERROR(1, type);
        }
        std::swap(*out_keys , oKey);
        std::swap(*out_values , oVal);
    }
    CATCHALL;

    return AF_SUCCESS;
}
