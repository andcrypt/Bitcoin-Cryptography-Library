#pragma once
#include <cstdint>

#define TBITS 16

#if TBITS<=8
typedef uint8_t small_type;
typedef uint16_t double_type;
typedef int16_t double_stype;
#elif TBITS<=16
typedef uint16_t small_type;
typedef uint32_t double_type;
typedef int32_t double_stype;
#else
typedef uint32_t small_type;
typedef uint64_t double_type;
typedef int64_t double_stype;
#endif

#if TBITS==7
const small_type TModulus = 103;
const small_type TOrder = 97;
#define TGX 95
#define TGY 76
#define TB 5
#elif TBITS==8
const small_type TModulus = 211;
const small_type TOrder = 199;
#define TGX 190
#define TGY 5
#define TB 2
#elif TBITS==16
const small_type TModulus = 65167;
const small_type TOrder = 64879;
#define TGX 62171
#define TGY 14828
#define TB 7
#elif TBITS==24
const small_type TModulus = 16776547;
const small_type TOrder = 16769119;
#define TGX 11188894
#define TGY 13938823
#define TB 7
#elif TBITS==31
const small_type TModulus = 2147476087;
const small_type TOrder = 2147411683;
#define TGX 1921962486
#define TGY 4944776
#define TB 7
#endif
const double_type TCarry = double_type(1) << double_type(TBITS);
const small_type NegateMask = TCarry-1;