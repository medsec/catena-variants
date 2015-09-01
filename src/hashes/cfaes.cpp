#include "cfaes.hpp"
#include "../registry.hpp"
//#include <thread>
#if defined(__AES__)
//SSE optimized
#include <emmintrin.h>
#include <wmmintrin.h>
#else
#include "aes.hpp"
#endif


using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<Cfaes> regcfaes;

Cfaes::Cfaes()
:Registerable("CFAES", "Cfaes", 
	"CFAES Hash function")
{}

void
Cfaes::ResetState()
{}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#ifdef __AES__

#define ROUND_KEYS     11
#define ROUND_KEYS_FOUR 5

typedef __m128i  AES_KEY[ROUND_KEYS];
typedef __m128i  AXU_KEY[ROUND_KEYS_FOUR];

// ---------------------------------------------------------------------

static __m128i aes_keygen_assist(__m128i temp1, __m128i temp2)
{
    __m128i temp3;
    temp2 = _mm_shuffle_epi32(temp2, 0xff);
    temp3 = _mm_slli_si128(temp1, 0x4);
    temp1 = _mm_xor_si128(temp1, temp3);
    temp3 = _mm_slli_si128(temp3, 0x4);
    temp1 = _mm_xor_si128(temp1, temp3);
    temp3 = _mm_slli_si128(temp3, 0x4);
    temp1 = _mm_xor_si128(temp1, temp3);
    return _mm_xor_si128(temp1, temp2);
}

// ---------------------------------------------------------------------

#define aes_create_round_key(temp1, temp2, rcon, k) \
    temp2 = _mm_aeskeygenassist_si128(temp1, rcon); \
    temp1 = aes_keygen_assist(temp1, temp2); \
    _mm_store_si128(k, temp1)

// ---------------------------------------------------------------------

static void aesfour_expand_key(__m128i userkey, AXU_KEY enc_key)
{
    __m128i temp1, temp2;
    __m128i *key = (__m128i*)enc_key;

    _mm_storeu_si128(&temp1, userkey);
    _mm_storeu_si128(&key[0], temp1);

    aes_create_round_key(temp1, temp2, 0x01, &key[1]);
    aes_create_round_key(temp1, temp2, 0x02, &key[2]);
    aes_create_round_key(temp1, temp2, 0x04, &key[3]);
    aes_create_round_key(temp1, temp2, 0x08, &key[4]);
}


static __m128i aesfour_encrypt(__m128i in, __m128i* k)
{
    __m128i x = _mm_xor_si128(in, k[0]);
    x = _mm_aesenc_si128(x, k[1]);
    x = _mm_aesenc_si128(x, k[2]);
    x = _mm_aesenc_si128(x, k[3]);
    return _mm_aesenc_si128(x, k[4]);
}

void 
Cfaes::Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
			uint8_t* hash)
{
    uint64_t zeros[2]={0, 0};
    __m128i S[128]; 
    __m128i bigS = _mm_load_si128((__m128i *) zeros);
    __m128i bigStmp = bigS;

    //load s1 and s2
    for (uint8_t i = 0; i < 64; i++) {
        S[i] = _mm_load_si128((__m128i *) &i1[i*16]);
    }
    for (uint8_t i = 0; i < 64; i++) {
        S[i+64] = _mm_load_si128((__m128i *) &i2[i*16]);
    }

    //XOR all bigS
    for (uint8_t i = 0; i < 128; i++) {
        bigS = _mm_xor_si128(bigS, S[i]);
    }

    //prepare aes
    AES_KEY aeskey;
    __m128i userKey128 = _mm_load_si128((__m128i *) zeros);
    aesfour_expand_key(userKey128, aeskey);

    //first aes round, plus bigS sum
    for (uint8_t i = 0; i < 128; i++) {
        S[i] = aesfour_encrypt(_mm_xor_si128(bigS, S[i]), aeskey);
        bigStmp = _mm_xor_si128(bigStmp, S[i]);   
    }
    bigS=bigStmp;

    //second aes round
    for (uint8_t i = 0; i < 64; i++) {
        S[i] = aesfour_encrypt(_mm_xor_si128(bigS, S[i]), aeskey);
    }

    for (uint8_t i = 0; i < 64; i++) {
        _mm_store_si128((__m128i *) &hash[i*16], S[i]); 
    }


}

#else 


void 
Cfaes::Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
			uint8_t* hash)
{
    uint64_t S[256];
    uint64_t bigS[2]={0, 0};
    uint64_t bigStmp[2]={0, 0};
    memcpy(&S[0], i1, H_LEN_FAST);
    memcpy(&S[128], i2, H_LEN_FAST);

    AES_KEY aeskey;
    uint64_t userKey[2]={0, 0};
    aes_expand_enc_key((uint8_t*)userKey, 128, &aeskey);

    for(uint16_t i = 0; i!=128;++i){
        bigS[0]^=S[2*i];
        bigS[1]^=S[2*i+1];
    }

    for(uint16_t i = 0; i!=128;++i){
        S[2*i]^=bigS[0];
        S[2*i+1]^=bigS[1];
        aesfour_encrypt((uint8_t*)&S[2*i], (uint8_t*)&S[2*i], &aeskey);
        bigStmp[0]^=S[2*i];
        bigStmp[1]^=S[2*i+1];
    }

    for(uint16_t i = 0; i!=64;++i){
        S[2*i]^=bigStmp[0];
        S[2*i+1]^=bigStmp[1];
        aesfour_encrypt((uint8_t*)&S[2*i], (uint8_t*)&S[2*i], &aeskey);
    }

    for(uint16_t i = 0; i!=64;++i){
        S[2*i]^=bigStmp[0];
        S[2*i+1]^=bigStmp[1];
        aesfour_encrypt((uint8_t*)&S[2*i], (uint8_t*)&S[2*i], &aeskey);
    }
    memcpy(hash, S, H_LEN_FAST);
}

#endif


#pragma GCC diagnostic pop

uint16_t
Cfaes::getHlenFast()const{
  return H_LEN_FAST;
}