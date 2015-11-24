#include "argonCF.hpp"
#include "../registry.hpp"

#if defined(__SSE2__)
//SSE optimized
#include <x86intrin.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include "Blake2/blake2-round.h"

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
//no SSE
#endif

using namespace Catena_Variants;
//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<argonCF> regacf;

argonCF::argonCF()
:Registerable("ArgonCF", "AC", "Argon 2 compression function")
{}

void
argonCF::ResetState() {}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"



#ifdef __SSE2__

#define LOAD(p)  _mm_load_si128( (__m128i *)(p) )
#define STORE(p,r) _mm_store_si128((__m128i *)(p), r)

#define LOADU(p)  _mm_loadu_si128( (__m128i *)(p) )
#define STOREU(p,r) _mm_storeu_si128((__m128i *)(p), r)

void
argonCF::Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
            uint8_t* hash)
{
    const __m128i r16 = _mm_setr_epi8(2, 3, 4, 5, 6, 7, 0, 1, 10, 11, 12, 13, 14, 15, 8, 9);
    const __m128i r24 = _mm_setr_epi8(3, 4, 5, 6, 7, 0, 1, 2, 11, 12, 13, 14, 15, 8, 9, 10);
    __m128i t0, t1;

    uint32_t QWORDS_IN_BLOCK = 64;
    __m128i block_XY[QWORDS_IN_BLOCK];
    __m128i state[64];
    __m128i tmp;

    for (uint8_t i = 0; i < QWORDS_IN_BLOCK; i++) {
        block_XY[i] = _mm_load_si128((__m128i *) &i2[i*16]);
    }
    for (uint8_t i = 0; i < QWORDS_IN_BLOCK; i++) {
        tmp = _mm_load_si128((__m128i *) &i1[i*16]);
        block_XY[i] = state[i] = _mm_xor_si128(tmp, block_XY[i]);
    }
    
    BLAKE2_ROUND(state[0], state[1], state[2], state[3],
            state[4], state[5], state[6], state[7]);

    BLAKE2_ROUND(state[8], state[9], state[10], state[11],
            state[12], state[13], state[14], state[15]);

    BLAKE2_ROUND(state[16], state[17], state[18], state[19],
            state[20], state[21], state[22], state[23]);

    BLAKE2_ROUND(state[24], state[25], state[26], state[27],
            state[28], state[29], state[30], state[31]);

    BLAKE2_ROUND(state[32], state[33], state[34], state[35],
            state[36], state[37], state[38], state[39]);

    BLAKE2_ROUND(state[40], state[41], state[42], state[43],
            state[44], state[45], state[46], state[47]);

    BLAKE2_ROUND(state[48], state[49], state[50], state[51],
            state[52], state[53], state[54], state[55]);

    BLAKE2_ROUND(state[56], state[57], state[58], state[59],
            state[60], state[61], state[62], state[63]);


    BLAKE2_ROUND(state[0], state[8], state[16], state[24],
            state[32], state[40], state[48], state[56]);

    BLAKE2_ROUND(state[1], state[9], state[17], state[25],
            state[33], state[41], state[49], state[57]);

    BLAKE2_ROUND(state[2], state[10], state[18], state[26],
            state[34], state[42], state[50], state[58])

            BLAKE2_ROUND(state[3], state[11], state[19], state[27],
            state[35], state[43], state[51], state[59]);

    BLAKE2_ROUND(state[4], state[12], state[20], state[28],
            state[36], state[44], state[52], state[60]);

    BLAKE2_ROUND(state[5], state[13], state[21], state[29],
            state[37], state[45], state[53], state[61]);

    BLAKE2_ROUND(state[6], state[14], state[22], state[30],
            state[38], state[46], state[54], state[62]);

    BLAKE2_ROUND(state[7], state[15], state[23], state[31],
            state[39], state[47], state[55], state[63]);

    for (uint8_t i = 0; i < QWORDS_IN_BLOCK; i++) {
        // Feedback
        state[i] = _mm_xor_si128(state[i], block_XY[i]);
    }
     for (uint8_t i = 0; i < QWORDS_IN_BLOCK; i++) {
        _mm_store_si128((__m128i *) &hash[i*16], state[i]); 
    }
}
#else

/* Memory block size in bytes */
const uint32_t BLOCK_SIZE = 1024;
const uint32_t WORDS_IN_BLOCK = BLOCK_SIZE / sizeof (uint64_t);

struct block {
    uint64_t v[WORDS_IN_BLOCK];

    block(uint8_t in = 0) { //default ctor
        memset(v, in, BLOCK_SIZE);
    }

    uint64_t& operator[](const uint8_t i) { //Subscript operator
        return v[i];
    }

    block& operator=(const block& r) { //Assignment operator
        memcpy(v, r.v, BLOCK_SIZE);
        return *this;
    }

    block& operator^=(const block& r) { //Xor-assignment
        for (uint8_t j = 0; j < WORDS_IN_BLOCK; ++j) {
            v[j] ^= r.v[j];
        }
        return *this;
    }

    block(const block& r) {
        memcpy(v, r.v, BLOCK_SIZE);
    }
};

static inline uint64_t rotr64( const uint64_t w, const unsigned c )
{
  return ( w >> c ) | ( w << ( 64 - c ) );
}

#define G(a,b,c,d) \
    a = a + b ; \
    d = rotr64(d ^ a, 32); \
    c = c + d; \
    b = rotr64(b ^ c, 24); \
    a = a + b ; \
    d = rotr64(d ^ a, 16); \
    c = c + d; \
    b = rotr64(b ^ c, 63); 

#define BLAKE2_ROUND_NOMSG(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15)  \
    G(v0, v4, v8, v12); \
    G(v1, v5, v9, v13); \
    G(v2, v6, v10, v14); \
    G(v3, v7, v11, v15); \
    G(v0, v5, v10, v15); \
    G(v1, v6, v11, v12); \
    G(v2, v7, v8, v13); \
    G(v3, v4, v9, v14); 

void
argonCF::Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
            uint8_t* hash)
{
    block blockR;
    uint64_t v2[WORDS_IN_BLOCK];
    memcpy(blockR.v, i1, 1024);
    memcpy(v2, i2, 1024);

    for (uint8_t j = 0; j < WORDS_IN_BLOCK; ++j) {
            blockR.v[j] ^= v2[j];
        }
    /////////
    block block_tmp = blockR;
    
    //Apply Blake2 on columns of 64-bit words: (0,1,...,15) , then (16,17,..31)... finally (112,113,...127)
    for (unsigned i = 0; i < 8; ++i) {
        BLAKE2_ROUND_NOMSG(blockR[16 * i], blockR[16 * i + 1], blockR[16 * i + 2], blockR[16 * i + 3],
                blockR[16 * i + 4], blockR[16 * i + 5], blockR[16 * i + 6], blockR[16 * i + 7],
                blockR[16 * i + 8], blockR[16 * i + 9], blockR[16 * i + 10], blockR[16 * i + 11],
                blockR[16 * i + 12], blockR[16 * i + 13], blockR[16 * i + 14], blockR[16 * i + 15]);
    }
    // Apply Blake2 on rows of 64-bit words: (0,1,16,17,...112,113), then (2,3,18,19,...,114,115).. finally (14,15,30,31,...,126,127)
    for (unsigned i = 0; i < 8; i++) {
        BLAKE2_ROUND_NOMSG(blockR[2 * i], blockR[2 * i + 1], blockR[2 * i + 16], blockR[2 * i + 17],
                blockR[2 * i + 32], blockR[2 * i + 33], blockR[2 * i + 48], blockR[2 * i + 49],
                blockR[2 * i + 64], blockR[2 * i + 65], blockR[2 * i + 80], blockR[2 * i + 81],
                blockR[2 * i + 96], blockR[2 * i + 97], blockR[2 * i + 112], blockR[2 * i + 113]);
    }

    blockR ^= block_tmp;

    memcpy(hash, blockR.v, 1024);
  
}
#pragma GCC diagnostic pop

#endif

uint16_t
argonCF::getHlenFast()const{
  return H_LEN_FAST;
}
