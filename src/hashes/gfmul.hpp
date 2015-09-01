#pragma once

#ifdef __PCLMUL__
#include <emmintrin.h>
#include <wmmintrin.h>
#include <smmintrin.h>
#endif //__PCLMUL__

#include "../hashfast.hpp"

namespace Catena_Variants{

class GFMul :  public HashFast<GFMul>
{
	public:
		GFMul();
		~GFMul() = default;
		GFMul(GFMul const&) = default;
		GFMul& operator=(GFMul const&) = default;

		/*Takes two H_LEN Inputs and multiplies them in 128 bit blocks in a GF2^128
	 	*/
		virtual void Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
			uint8_t* hash);

		/* Does nothing
		*/
		virtual void ResetState(); 

		virtual uint16_t getHlenFast()const;
	
	private:
	#ifdef __PCLMUL__
	// Bitwise shift for __m128i types
	__m128i sll128(__m128i a, uint8_t count);
	/* Galois Field multiplication in GF2^128 with modulus x^128+x^7+x^2+x+1
	* see "Intel® Carry-Less Multiplication Instruction and its Usage for Computing
	* the GCM Mode" by Shay Gueron and Michael E. Kounavis
	*/
	__m128i gfmul128(__m128i A, __m128i B);

	#else
	/* Carry less multiplication of two 64bit words
	* see Faster Multiplication in GF(2)[x] by Richard P. Brent et al.
	*/
	void clmul64(uint64_t a, uint64_t b, uint64_t* r);
	/* Galois Field multiplication in GF2^128 with modulus x^128+x^7+x^2+x+1
	 */
	void gfmul128(const uint64_t* a, const uint64_t* b, uint64_t* r);
	
	#endif

	uint16_t const H_LEN_FAST = 64;

};
}