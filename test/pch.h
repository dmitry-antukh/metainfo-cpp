#ifndef DP_PCH_H_27122007
#define DP_PCH_H_27122007


#include <assert.h>

#define META_LIB_ASSERT(expr, error) assert(expr)


#if defined(__BORLANDC__)
#include <condefs.h>
#endif // defined(__BORLANDC__)

#if _MSC_VER == 1400
#   pragma warning(disable: 4100 4311 4312)
#   pragma warning(disable: 4505) /// 'function' : unreferenced local function has been removed
#   pragma warning(disable: 4996)
#endif // _MSC_VER == 1400


#endif  // DP_PCH_H_27122007

