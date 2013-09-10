#ifndef __helpers__
#define __helpers__

#include "pluginterfaces/base/ftypes.h"

#if BYTEORDER == kBigEndian
#define SWAP64_LE(x)
#define SWAP64_BE(x)	SWAP_64(x)
#define SWAP32_LE(x)
#define SWAP32_BE(x)	SWAP_32(x)
#else
#define SWAP64_LE(x)	SWAP_64(x)
#define SWAP64_BE(x)
#define SWAP32_LE(x)	SWAP_32(x)
#define SWAP32_BE(x)
#endif

#endif
