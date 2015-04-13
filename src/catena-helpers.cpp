#ifdef __SSE2__
#include <emmintrin.h>
#endif 

#include "catena-helpers.hpp"

namespace Catena_Variants{

void XOR(const uint8_t *input1, const uint8_t *input2, uint8_t *output)
{
#ifdef __SSE2__
  int blocks = H_LEN/sizeof(__m128i);
  for(int i = 0; i < blocks; i++){
    __m128i left = _mm_load_si128((__m128i*) (input1 + (i*sizeof(__m128i))));
    __m128i right = _mm_load_si128((__m128i*) (input2 + (i*sizeof(__m128i))));
    __m128i result = _mm_xor_si128 (left,right);
    _mm_store_si128((__m128i*) (output + (i*sizeof(__m128i))), result);
  }
#else
  uint32_t i;
  for(i = 0; i < H_LEN; i++){
    output[i] = input1[i] ^ input2[i];
  }
#endif
}


/* Our tests showed the memset to be always executed independent of 
* optimization. To ensure it stays that way, enable the no optimization flags.
*/
void 
#if defined(__clang__)
#if __has_attribute(optnone)
 __attribute__((optnone))
#endif //end has attribute

#elif defined(__GNUC__)

#define GCC_VERSION (__GNUC__ * 10000 \
                    + __GNUC_MINOR__ * 100 \
                    + __GNUC_PATCHLEVEL__)
//only supported by GCC >= 4.4
#if GCC_VERSION >= 40400
  __attribute__((optimize("O0")))
#endif //end GCC >= 4.4

#endif //end compiler_id
erasepwd(uint8_t* pwd, const uint32_t pwdlen){
  memset((char *)pwd, 0, pwdlen);
  free(pwd);
}

};