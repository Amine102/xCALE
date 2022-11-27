/*=============================================================================
 * Product        : 
 * File           : Log.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef _log_H
#define _log_H

#include <iostream>
#include <fstream>
#include <strstream>


namespace PILGRIM {
class Log {
 public:
    Log(char);
    ~Log();
    /**
     * \brief  
     * \details 
     * \param variables : 
     */
    void Write(char* logline);
    /**
     * \brief  
     * \details 
     * \param variables : 
    */
    static  void setOutFilePrefix(char *outFileName,
                                  size_t &winSize,
                                  size_t k = 0,
                                  bool _incrementalLearning = false);
    /**
     * \brief  
     * \details 
     * \param variables : 
     */
    static char outLogFileName[256];
    /**
     * \brief  
     * \details 
     * \param variables : 
     */
    static char outCPCFileName[256];
    /**
     * \brief  
     * \details 
     * \param variables : 
     */
    static char outTestSummaryFileName[256];
    /**
     * \brief  
     * \details 
     * \param variables : 
     */
    static char outTimeComplxFileName[256];
    char *fileName;
    char logType;
    FILE *fp;
};
}; // namespace PILGRIM
#endif 
