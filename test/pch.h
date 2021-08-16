#ifndef PCH_069e38df4317ffc666c5_H
#define PCH_069e38df4317ffc666c5_H


#include <assert.h>

#define META_LIB_ASSERT(expr, errorDescription) assert(expr)
//#define META_LIB_ASSERT(expr, errorDescription) do if(!(expr)) throw std::runtime_error(#errorDescription); while(0)
//do if(!(expr)) throw std::runtime_error(#errorDescription); while(0);


#if defined(__BORLANDC__)
#include <condefs.h>
#endif // defined(__BORLANDC__)

#if _MSC_VER == 1400
#   pragma warning(disable: 4100 4311 4312)
#   pragma warning(disable: 4505) /// 'function' : unreferenced local function has been removed
#   pragma warning(disable: 4996)
#endif // _MSC_VER == 1400


#endif  // PCH_069e38df4317ffc666c5_H
