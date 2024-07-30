#pragma once

#define GL4 0
#define DX11 1

#define CURRENT_RENDERER GL4

#define DOUBLE_PRECISION 0

#if CURRENT_RENDERER == GL4

#define UInt32 unsigned int

#if DOUBLE_PRECISION == 1
#define Real double
#else
#define Real float
#endif

#endif