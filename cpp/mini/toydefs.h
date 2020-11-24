#pragma once
#include <cstdint>

#define TBITS 16

#if TBITS<=16
typedef uint16_t small_type;
typedef uint32_t double_type;
typedef int32_t double_stype;
#else
typedef uint32_t small_type;
typedef uint64_t double_type;
typedef int64_t double_stype;
#endif

#if TBITS==16
const small_type TModulus = 65167;
const small_type TOrder = 64879;
#define TGX 62171
#define TGY 14828
const small_type NegateMask = 0xffff;
#elif TBITS==24
const small_type TModulus = 16776547;
const small_type TOrder = 16769119;
#define TGX 11188894
#define TGY 13938823
const small_type NegateMask = 0xffffff;
#elif TBITS==31
const small_type TModulus = 2147476087;
const small_type TOrder = 2147411683;
#define TGX 1921962486
#define TGY 4944776
const small_type NegateMask = 0x7fffffff;
#endif
const double_type TCarry = (double_type)NegateMask + 1;