#ifndef _ED_Util_CTAssert_h_
#define _ED_Util_CTAssert_h_

namespace Util {

/**
 * Compile-time assert.
 */
template<bool> class CTAssert;

template<> class CTAssert<true> {};

}

#endif /* _ED_Util_CTAssert_h_ */
