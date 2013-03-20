#ifndef __TYPES_H__
#define __TYPES_H__

//#include "Fixed.h"
//#include "aquafixed.h"

typedef bool			Boolean;
typedef unsigned short	UInt16;
typedef unsigned int	UInt32;
typedef short			Int16;
typedef	long			Int32;
typedef	char			Char;
typedef	char			Int8;
typedef unsigned char	UInt8;
typedef float			CoordType;
typedef unsigned int	Pixel;

#define PIRADIANES 0.0174532925199f

#include "Vectors.h"
//#include "FastPow.h"

#ifndef max
#define max(a, b) ((a > b) ? a : b)
#endif

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

#endif
