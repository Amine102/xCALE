/// \file plMaxDigits.h
/// Definition of pl_maxdigits10() function.

/*=============================================================================
 * Product        : ProBT
 * File           : plMaxDigits.h
 * Author         : David Raulo
 * Creation       : May 06 2015
 *
 *=============================================================================
 *        (c) Copyright 2015, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plMaxDigits_h
#define plMaxDigits_h

#include <plConfig.h>
#include <limits>

/** how many digits to print for float values.
    See http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2005/n1822.pdf
 */
template<typename T>
inline int pl_maxdigits10(const T& /* v */)
{
  return std::numeric_limits<T>::digits * 30103L / 100000L + 2;
}

#ifdef PL_USE_PLPROBA
/** how many digits to print for plProbability. */
class plProbability;
template<>
inline int pl_maxdigits10<plProbability>(const plProbability&)
{
  return 33L * 30103L / 100000L + 2;
}
#endif

#endif //plMaxDigits_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
