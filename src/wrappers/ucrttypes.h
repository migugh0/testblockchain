/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * No warranty is given of any kind, including but not limited to the warranties
 * of merchantability, fitness for a particular purpose and noninfringement.
 */
#ifndef _UCRTTYPES_H_
#define _UCRTTYPES_H_

/* Resolves UCRT compatibility issues */

#ifdef _WIN32
#ifndef POSIX

#include <sys/types.h>
#include <crtdefs.h>

#include <stdint.h>

#ifndef _BSDTYPES_DEFINED
#ifndef __u_char_defined
typedef	unsigned char	u_char;
#define __u_char_defined
#endif
#ifndef __u_short_defined
typedef	unsigned short	u_short;
#define __u_short_defined
#endif
#ifndef __u_int_defined
typedef	unsigned int	u_int;
#define __u_int_defined
#endif
#ifndef __u_long_defined
typedef	unsigned long	u_long;
#define __u_long_defined
#endif
#define _BSDTYPES_DEFINED
#endif /* !_BSDTYPES_DEFINED */

#ifndef ushort_defined
typedef	unsigned short	ushort;
#define ushort_defined
#endif
#ifndef uint_defined
typedef	unsigned int	uint;
#define uint_defined
#endif
#ifndef ulong_defined
typedef	unsigned long	ulong;
#define ulong_defined
#endif

#endif	/* !POSIX */
#endif	/* _WIN32 */

#endif	/* _UCRTTYPES_H_ */