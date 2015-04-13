#pragma once

#include <stdint.h>
#include <byteswap.h>

/* The default unit for all length values are bytes */

/*Sizes*/
#define H_LEN      64
#define KEY_LEN    16


/* Modes  */
#define PASSWORD_HASHING_MODE 0
#define KEY_DERIVATION_MODE   1
#define REGULAR 0
#define CLIENT 1


#ifdef ARC_BIG_ENDIAN
  #define TO_LITTLE_ENDIAN_64(n) bswap_64(n)
  #define TO_LITTLE_ENDIAN_32(n) bswap_32(n)
#else
  #define TO_LITTLE_ENDIAN_64(n) (n)
  #define TO_LITTLE_ENDIAN_32(n) (n)
#endif

/* H_LEN 0s */
const uint8_t ZERO8[H_LEN] = {0}; 