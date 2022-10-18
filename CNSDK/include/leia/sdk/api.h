#pragma once

#include "leia/common/defines.h"

#ifdef LEIASDK_EXPORTS
# define LEIASDK_API LEIA_EXPORT
#else
# ifdef LEIASDK_STATIC
#  define LEIASDK_API
# else
#  define LEIASDK_API LEIA_IMPORT
# endif
#endif
