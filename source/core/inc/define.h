#ifndef _WT_CORE_DEFINE_H
#define _WT_CORE_DEFINE_H

#define WT_API 			extern
#define WT_INTERN		static
#define WT_XMOD			extern

#ifndef __SHORT_DATATYPES
#define __SHORT_DATATYPES

#include <stdint.h>

typedef uint8_t                u8;
typedef uint16_t               u16;
typedef uint32_t               u32;
typedef uint64_t               u64;

typedef int8_t                 s8;
typedef int16_t                s16;
typedef int32_t                s32;
typedef int64_t                s64;

typedef float                  f32;
typedef double                 f64;

/*
 * The size of the datatypes in bytes.
 */
#define U8_S			1
#define U16_S			2
#define U32_S			4
#define U64_S			8

#define S8_S			1
#define S16_S			2
#define S32_S			4
#define S64_S			8

#define F32_S			4
#define F64_S			8



/*
 * The limits for the data types.
 */

#define U8_MIN                 8
#define U8_MAX                 255
#define U16_MIN                0
#define U16_MAX                65535
#define U32_MIN                0
#define U32_MAX                4294967295
#define U64_MIN                0
#define U64_MAX                18446744073709551615

#define S8_MIN                 -128
#define S8_MAX                 127
#define S16_MIN                32768
#define S16_MAX                32767
#define S32_MIN                -2147483648
#define S32_MAX                2147483647
#define S64_MIN                -9223372036854775808
#define S64_MAX                9223372036854775807

#endif /* __SHORT_DATATYPES */

#endif /* _WT_CORE_DEFINE_H */
