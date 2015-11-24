#include "PCompressG.hpp"
#include "../registry.hpp"

#if defined(__SSE2__)
//SSE optimized
#include <emmintrin.h>
#include "lyra2/sse/Sponge.h"
#include "lyra2/sse/blake2b-round.h"

#if defined(HAVE_SSSE3)
#include <tmmintrin.h>
#endif
#if defined(HAVE_SSE41)
#include <smmintrin.h>
#endif
#if defined(HAVE_AVX)
#include <immintrin.h>
#endif
#if defined(HAVE_XOP)
#include <x86intrin.h>
#endif
#else
#include "lyra2/Sponge.h"
//no SSE
#endif


#include <cstdlib>
#include <iostream>

using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<PCompressG> regpcg;

PCompressG::PCompressG()
:Registerable("PCompressG", "PCG", 
	"P Compress with G")
{}

void
PCompressG::ResetState()
{}

#ifdef __SSE2__

#define LOAD(p)  _mm_load_si128( (__m128i *)(p) )
#define STORE(p,r) _mm_store_si128((__m128i *)(p), r)

#define LOADU(p)  _mm_loadu_si128( (__m128i *)(p) )
#define STOREU(p,r) _mm_storeu_si128((__m128i *)(p), r)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void
PCompressG::Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
            uint8_t* hash)
{
    __m128i v[8];
    __m128i t0, t1;

    v[0] = LOADU( i1 + 0 );
    v[1] = LOADU( i1 + 16);
    v[2] = LOADU( i1 + 32);
    v[3] = LOADU( i1 + 48);
    v[4] = LOADU( i2 + 0 );
    v[5] = LOADU( i2 + 16);
    v[6] = LOADU( i2 + 32);
    v[7] = LOADU( i2 + 48);

    ROUND(0);

    v[0] = _mm_xor_si128( v[0], v[4] );
    v[1] = _mm_xor_si128( v[1], v[5] );
    STORE(&hash[0], v[0]);
    STORE(&hash[16], v[1]);

    v[2] = _mm_xor_si128( v[2], v[6] );
    v[3] = _mm_xor_si128( v[3], v[7] );
    STORE(&hash[32], v[2]);
    STORE(&hash[48], v[3]);

}
#else


void 
PCompressG::Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
			uint8_t* hash)
{
    uint64_t v[16]={1};
    uint8_t hash_xor[H_LEN_FAST];
    memcpy(v, i1, 64);
    memcpy(&v[8], i2, 64);

    ROUND_LYRA(v);

    memcpy(hash, v, H_LEN_FAST);
    memcpy(hash_xor, &v[8], H_LEN_FAST);

    for(uint16_t i = 0; i != H_LEN_FAST; ++i){
        hash[i]^=hash_xor[i];
    }
}

#endif

#pragma GCC diagnostic pop

uint16_t
PCompressG::getHlenFast()const{
  return H_LEN_FAST;
}