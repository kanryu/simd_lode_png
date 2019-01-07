#pragma once

/* From libpng 1.6.0 libpng requires an ANSI X3.159-1989 ("ISOC90") compliant C
* compiler for correct compilation.  The following header files are required by
* the standard.  If your compiler doesn't provide these header files, or they
* do not match the standard, you will need to provide/improve them.
*/
#include <limits.h>
#include <stddef.h>


/* Some typedefs to get us started.  These should be safe on most of the common
* platforms.
*
* png_uint_32 and png_int_32 may, currently, be larger than required to hold a
* 32-bit value however this is not normally advisable.
*
* png_uint_16 and png_int_16 should always be two bytes in size - this is
* verified at library build time.
*
* png_byte must always be one byte in size.
*
* The checks below use constants from limits.h, as defined by the ISOC90
* standard.
*/
#if CHAR_BIT == 8 && UCHAR_MAX == 255
typedef unsigned char png_byte;
#else
#  error "libpng requires 8-bit bytes"
#endif

#if INT_MIN == -32768 && INT_MAX == 32767
typedef int png_int_16;
#elif SHRT_MIN == -32768 && SHRT_MAX == 32767
typedef short png_int_16;
#else
#  error "libpng requires a signed 16-bit type"
#endif

#if UINT_MAX == 65535
typedef unsigned int png_uint_16;
#elif USHRT_MAX == 65535
typedef unsigned short png_uint_16;
#else
#  error "libpng requires an unsigned 16-bit type"
#endif

#if INT_MIN < -2147483646 && INT_MAX > 2147483646
typedef int png_int_32;
#elif LONG_MIN < -2147483646 && LONG_MAX > 2147483646
typedef long int png_int_32;
#else
#  error "libpng requires a signed 32-bit (or more) type"
#endif

#if UINT_MAX > 4294967294U
typedef unsigned int png_uint_32;
#elif ULONG_MAX > 4294967294U
typedef unsigned long int png_uint_32;
#else
#  error "libpng requires an unsigned 32-bit (or more) type"
#endif

/* Prior to 1.6.0 it was possible to disable the use of size_t, 1.6.0, however,
* requires an ISOC90 compiler and relies on consistent behavior of sizeof.
*/
typedef size_t png_size_t;
typedef ptrdiff_t png_ptrdiff_t;

/* Typedef for floating-point numbers that are converted to fixed-point with a
* multiple of 100,000, e.g., gamma
*/
typedef png_int_32 png_fixed_point;


/* Add typedefs for pointers */
typedef void                  * png_voidp;
typedef const void            * png_const_voidp;
typedef png_byte              * png_bytep;
typedef const png_byte        * png_const_bytep;
typedef png_uint_32           * png_uint_32p;
typedef const png_uint_32     * png_const_uint_32p;
typedef png_int_32            * png_int_32p;
typedef const png_int_32      * png_const_int_32p;
typedef png_uint_16           * png_uint_16p;
typedef const png_uint_16     * png_const_uint_16p;
typedef png_int_16            * png_int_16p;
typedef const png_int_16      * png_const_int_16p;
typedef char                  * png_charp;
typedef const char            * png_const_charp;
typedef png_fixed_point       * png_fixed_point_p;
typedef const png_fixed_point * png_const_fixed_point_p;
typedef png_size_t            * png_size_tp;
typedef const png_size_t      * png_const_size_tp;

/* This is used for the transformation routines, as some of them
* change these values for the row.  It also should enable using
* the routines for other purposes.
*/
typedef struct png_row_info_struct
{
	png_uint_32 width;    /* width of row */
	png_size_t rowbytes;  /* number of bytes in row */
	png_byte color_type;  /* color type of row */
	png_byte bit_depth;   /* bit depth of row */
	png_byte channels;    /* number of channels (1, 2, 3, or 4) */
	png_byte pixel_depth; /* bits per pixel (depth * channels) */
} png_row_info;

typedef png_row_info * png_row_infop;
typedef png_row_info * * png_row_infopp;

#ifndef PNG_UNUSED
/* Unused formal parameter warnings are silenced using the following macro
* which is expected to have no bad effects on performance (optimizing
* compilers will probably remove it entirely).  Note that if you replace
* it with something other than whitespace, you must include the terminating
* semicolon.
*/
#  define PNG_UNUSED(param) (void)param;
#endif

#ifndef png_debug
#  define png_debug(l, m) ((void)0)
#endif

#define PNG_FILTER_VALUE_NONE  0
#define PNG_FILTER_VALUE_SUB   1
#define PNG_FILTER_VALUE_UP    2
#define PNG_FILTER_VALUE_AVG   3
#define PNG_FILTER_VALUE_PAETH 4
#define PNG_FILTER_VALUE_LAST  5

#ifdef __cplusplus
extern "C" {
#endif

void png_read_filter_row_sub3_sse2(png_row_infop row_info, png_bytep row,
	png_const_bytep prev);
void png_read_filter_row_sub4_sse2(png_row_infop row_info, png_bytep row,
	png_const_bytep prev);
void png_read_filter_row_avg3_sse2(png_row_infop row_info, png_bytep row,
	png_const_bytep prev);
void png_read_filter_row_avg4_sse2(png_row_infop row_info, png_bytep row,
	png_const_bytep prev);
void png_read_filter_row_paeth3_sse2(png_row_infop row_info, png_bytep row,
	png_const_bytep prev);
void png_read_filter_row_paeth4_sse2(png_row_infop row_info, png_bytep row,
	png_const_bytep prev);
#ifdef __cplusplus
}
#endif
