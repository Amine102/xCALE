/// \file plFactorial.h
/*=======================================================================
 * Product        : ProBT 
 * File           : plFactorial.h
 * Author         : Juan-Manuel Ahuactzin-Larios
 * Creation       : Thu Sep  7 14:31:55 2006
 *
 *=======================================================================
 *                      (c) Copyright 2005, Probayes SAS,
 *                              all rights reserved
 *=======================================================================
 *
 *------------------------- Description ---------------------------------
 *   
 *   
 *-----------------------------------------------------------------------
 */


#ifndef plFactorial_h
#define plFactorial_h

#include "plConfig.h"

#include <plGamma.h>

//============================================================================
/// Return the log of n!
inline plFloat plLogFactorial(unsigned int n)
{
  return plLgamma(n+1);
}

//============================================================================
/// Return n!
inline plFloat plFactorial(unsigned int n)
{
  return plGammaFunc(n+1);
}

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
