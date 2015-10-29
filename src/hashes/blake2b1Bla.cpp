#include <string.h>

#include "blake2b1Bla.hpp" 
#include "../registry.hpp"

#if defined(__SSE2__)
//SSE optimized
#include "blake2-sse/blake2-config.h"
#include "blake2-sse/blake2.h"
#include "blake2-sse/blake2b-round.h"
#include "blake2-sse/blake2-impl.h"
#include <emmintrin.h>
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
//not SSE optimized
#include "blake2-ref/blake2.h"
#include "blake2-ref/blake2-impl.h"

#endif

#if defined(_MSC_VER)
#define ALIGN(x) __declspec(align(x))
#else
#define ALIGN(x) __attribute__ ((__aligned__(x)))
#endif


using namespace Catena_Variants;

//Register the registrable with the Registry so it can be used by CatenaFactory
Registry<Blake2b1Bla> regB2b1Bla;

Blake2b1Bla::Blake2b1Bla()
:Registerable("Blake2b-1-Bla", "B2b1Bla", "Single round of Blake2b (SSE or reference) with Blamka")
{
  _state = (blake2b_state*)aligned_alloc(64,sizeof(blake2b_state));
}

Blake2b1Bla::Blake2b1Bla(Blake2b1Bla const& other)
:Registerable("Blake2b-1-Bla", "B2b1Bla", "Single round of Blake2b (SSE or reference) with Blamka")
{
  _state = (blake2b_state*)aligned_alloc(64,sizeof(blake2b_state));
  memcpy(_state, other._state, sizeof(blake2b_state));
}

Blake2b1Bla& 
Blake2b1Bla::operator=(Blake2b1Bla const& other)
{
  if(this != &other)
  {
    free(_state);
    _state = (blake2b_state*)aligned_alloc(64,sizeof(blake2b_state));
    
    memcpy(_state, other._state, sizeof(blake2b_state));   
  }
  return *this;
}

Blake2b1Bla::~Blake2b1Bla()
{
  free(_state);
}

void 
Blake2b1Bla::ResetState()
{
  blake2b_init(_state,H_LEN);
}


#ifdef __SSE2__
/* Copies of necessary parts of blake2b-sse, that aren't directly accessible
 */

ALIGN( 64 ) static const uint64_t blake2b_IV[8] =
{
  0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};


static inline int blake2b_increment_counter( blake2b_state *S, const uint64_t inc )
{
#if __x86_64__
  // ADD/ADC chain
  __uint128_t t = ( ( __uint128_t )S->t[1] << 64 ) | S->t[0];
  t += inc;
  S->t[0] = ( uint64_t )( t >>  0 );
  S->t[1] = ( uint64_t )( t >> 64 );
#else
  S->t[0] += inc;
  S->t[1] += ( S->t[0] < inc );
#endif
  return 0;
}


static inline int blake2b_set_lastblock( blake2b_state *S )
{
  // if( S->last_node ) blake2b_set_lastnode( S );

  S->f[0] = ~0ULL;
  return 0;
}

/* Blake2b compression function modified to do only one single round
 */
static inline void blake2round(blake2b_state* S, 
  const uint8_t block[BLAKE2B_BLOCKBYTES], unsigned ridx){
    __m128i row1l, row1h;
  __m128i row2l, row2h;
  __m128i row3l, row3h;
  __m128i row4l, row4h;
  __m128i b0, b1;
  __m128i t0, t1;
#if defined(HAVE_SSSE3) && !defined(HAVE_XOP)
  const __m128i r16 = _mm_setr_epi8( 2, 3, 4, 5, 6, 7, 0, 1, 10, 11, 12, 13, 14, 15, 8, 9 );
  const __m128i r24 = _mm_setr_epi8( 3, 4, 5, 6, 7, 0, 1, 2, 11, 12, 13, 14, 15, 8, 9, 10 );
#endif
#if defined(HAVE_SSE41)
  const __m128i m0 = LOADU( block + 00 );
  const __m128i m1 = LOADU( block + 16 );
  const __m128i m2 = LOADU( block + 32 );
  const __m128i m3 = LOADU( block + 48 );
  const __m128i m4 = LOADU( block + 64 );
  const __m128i m5 = LOADU( block + 80 );
  const __m128i m6 = LOADU( block + 96 );
  const __m128i m7 = LOADU( block + 112 );
#else
  const uint64_t  m0 = ( ( uint64_t * )block )[ 0];
  const uint64_t  m1 = ( ( uint64_t * )block )[ 1];
  const uint64_t  m2 = ( ( uint64_t * )block )[ 2];
  const uint64_t  m3 = ( ( uint64_t * )block )[ 3];
  const uint64_t  m4 = ( ( uint64_t * )block )[ 4];
  const uint64_t  m5 = ( ( uint64_t * )block )[ 5];
  const uint64_t  m6 = ( ( uint64_t * )block )[ 6];
  const uint64_t  m7 = ( ( uint64_t * )block )[ 7];
  const uint64_t  m8 = ( ( uint64_t * )block )[ 8];
  const uint64_t  m9 = ( ( uint64_t * )block )[ 9];
  const uint64_t m10 = ( ( uint64_t * )block )[10];
  const uint64_t m11 = ( ( uint64_t * )block )[11];
  const uint64_t m12 = ( ( uint64_t * )block )[12];
  const uint64_t m13 = ( ( uint64_t * )block )[13];
  const uint64_t m14 = ( ( uint64_t * )block )[14];
  const uint64_t m15 = ( ( uint64_t * )block )[15];
#endif
  row1l = LOAD( &S->h[0] );
  row1h = LOAD( &S->h[2] );
  row2l = LOAD( &S->h[4] );
  row2h = LOAD( &S->h[6] );
  row3l = LOAD( &blake2b_IV[0] );
  row3h = LOAD( &blake2b_IV[2] );
  row4l = _mm_xor_si128( LOAD( &blake2b_IV[4] ), LOAD( &S->t[0] ) );
  row4h = _mm_xor_si128( LOAD( &blake2b_IV[6] ), LOAD( &S->f[0] ) );

  switch(ridx){
    case 0:ROUND_BLAMKA_SIGMA( 0 );break;
    case 1:ROUND_BLAMKA_SIGMA( 1 );break;
    case 2:ROUND_BLAMKA_SIGMA( 2 );break;
    case 3:ROUND_BLAMKA_SIGMA( 3 );break;
    case 4:ROUND_BLAMKA_SIGMA( 4 );break;
    case 5:ROUND_BLAMKA_SIGMA( 5 );break;
    case 6:ROUND_BLAMKA_SIGMA( 6 );break;
    case 7:ROUND_BLAMKA_SIGMA( 7 );break;
    case 8:ROUND_BLAMKA_SIGMA( 8 );break;
    case 9:ROUND_BLAMKA_SIGMA( 9 );break;
    case 10:ROUND_BLAMKA_SIGMA( 10 );break;
    case 11:ROUND_BLAMKA_SIGMA( 11 );break;
  }
  
  
  
  row1l = _mm_xor_si128( row3l, row1l );
  row1h = _mm_xor_si128( row3h, row1h );
  STORE( &S->h[0], _mm_xor_si128( LOAD( &S->h[0] ), row1l ) );
  STORE( &S->h[2], _mm_xor_si128( LOAD( &S->h[2] ), row1h ) );
  row2l = _mm_xor_si128( row4l, row2l );
  row2h = _mm_xor_si128( row4h, row2h );
  STORE( &S->h[4], _mm_xor_si128( LOAD( &S->h[4] ), row2l ) );
  STORE( &S->h[6], _mm_xor_si128( LOAD( &S->h[6] ), row2h ) );
}

void 
Blake2b1Bla::Hash(int vindex, const uint8_t* i1, const uint8_t* i2, 
	uint8_t hash[H_LEN])
{
	memcpy(_state->buf, i1, H_LEN);
	memcpy(_state->buf + H_LEN, i2, H_LEN);
	_state->buflen = 128;
	blake2b_increment_counter(_state, _state->buflen);
	blake2b_set_lastblock(_state);
	//No Padding necessary because the last 1024bits of _state.buf are 0 anyways
	const int rindex = vindex % 12;
	blake2round(_state, _state->buf, rindex);

	memcpy(hash, &_state->h[0], H_LEN);
}


#else	//No SSE


static const uint64_t blake2b_IV[8] =
{
  0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

static const uint8_t blake2b_sigma[12][16] =
{
  {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 } ,
  { 14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3 } ,
  { 11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4 } ,
  {  7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8 } ,
  {  9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13 } ,
  {  2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9 } ,
  { 12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11 } ,
  { 13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10 } ,
  {  6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5 } ,
  { 10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13 , 0 } ,
  {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 } ,
  { 14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3 }
};

static inline int blake2b_increment_counter( blake2b_state *S, const uint64_t inc )
{
  S->t[0] += inc;
  S->t[1] += ( S->t[0] < inc );
  return 0;
}


static inline int blake2b_set_lastblock( blake2b_state *S )
{
  // if( S->last_node ) blake2b_set_lastnode( S );

  S->f[0] = ~0ULL;
  return 0;
}

/*Main change compared with Blake2b*/
static inline uint64_t fBlaMka(uint64_t x, uint64_t y){
    uint32_t lessX = (uint32_t) x;
    uint32_t lessY = (uint32_t) y;
    
    uint64_t lessZ = (uint64_t) lessX;
    lessZ = lessZ * lessY;
    lessZ = lessZ << 1;
    
    uint64_t z = lessZ + x + y;
    
    return z;
}

/* Blake2b compression function modified to do only one single round
 */
static inline void blake2round(blake2b_state* S, 
    const uint8_t block[BLAKE2B_BLOCKBYTES], unsigned ridx)
{
  uint64_t m[16];
  uint64_t v[16];
  int i;

  for( i = 0; i < 16; ++i )
    m[i] = load64( block + i * sizeof( m[i] ) );

  for( i = 0; i < 8; ++i )
    v[i] = S->h[i];

  v[ 8] = blake2b_IV[0];
  v[ 9] = blake2b_IV[1];
  v[10] = blake2b_IV[2];
  v[11] = blake2b_IV[3];
  v[12] = S->t[0] ^ blake2b_IV[4];
  v[13] = S->t[1] ^ blake2b_IV[5];
  v[14] = S->f[0] ^ blake2b_IV[6];
  v[15] = S->f[1] ^ blake2b_IV[7];
#define G(r,i,a,b,c,d) \
  do { \
    a = fBlaMka(a, b) + m[blake2b_sigma[r][2*i+0]]; \
    d = rotr64(d ^ a, 32); \
    c = fBlaMka(c, d); \
    b = rotr64(b ^ c, 24); \
    a = fBlaMka(a, b) + m[blake2b_sigma[r][2*i+1]]; \
    d = rotr64(d ^ a, 16); \
    c = fBlaMka(c, d); \
    b = rotr64(b ^ c, 63); \
  } while(0)
#define ROUND(r)  \
  do { \
    G(r,0,v[ 0],v[ 4],v[ 8],v[12]); \
    G(r,1,v[ 1],v[ 5],v[ 9],v[13]); \
    G(r,2,v[ 2],v[ 6],v[10],v[14]); \
    G(r,3,v[ 3],v[ 7],v[11],v[15]); \
    G(r,4,v[ 0],v[ 5],v[10],v[15]); \
    G(r,5,v[ 1],v[ 6],v[11],v[12]); \
    G(r,6,v[ 2],v[ 7],v[ 8],v[13]); \
    G(r,7,v[ 3],v[ 4],v[ 9],v[14]); \
  } while(0)

  switch(ridx){
    case 0:ROUND( 0 );break;
    case 1:ROUND( 1 );break;
    case 2:ROUND( 2 );break;
    case 3:ROUND( 3 );break;
    case 4:ROUND( 4 );break;
    case 5:ROUND( 5 );break;
    case 6:ROUND( 6 );break;
    case 7:ROUND( 7 );break;
    case 8:ROUND( 8 );break;
    case 9:ROUND( 9 );break;
    case 10:ROUND( 10 );break;
    case 11:ROUND( 11 );break;
  }
  
 for( i = 0; i < 8; ++i )
    S->h[i] = S->h[i] ^ v[i] ^ v[i + 8];

#undef G
#undef ROUND
}

void 
Blake2b1Bla::Hash(int vindex, const uint8_t* i1, 
       const uint8_t* i2, uint8_t* hash){
  uint8_t buffer[BLAKE2B_OUTBYTES];

  memcpy(_state->buf, i1, H_LEN);
  memcpy(_state->buf + H_LEN, i2, H_LEN);
  _state->buflen = 128;
  blake2b_increment_counter(_state, _state->buflen);
  blake2b_set_lastblock(_state);
  //No Padding necessary because the last 1024bits of _state.buf are 0 anyways
  const int rindex = vindex % 12;
  blake2round(_state, _state->buf, rindex);
  
  for( int i = 0; i < 8; ++i ) /* Output full hash to temp buffer */
    store64( buffer + sizeof(_state->h[i] ) * i, _state->h[i] );

  memcpy(hash, buffer, H_LEN );
}

#endif

uint16_t
Blake2b1Bla::getHlenFast()const{
  return H_LEN_FAST;
}