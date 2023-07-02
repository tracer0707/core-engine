#pragma once

#define GL4 0
#define DX11 1

#define CURRENT_RENDERER GL4

#define DOUBLE_PRECISION 1

#if CURRENT_RENDERER == 0

#define uint unsigned int

#if DOUBLE_PRECISION == 1
#define real double
#elif
#define real float
#endif

#endif