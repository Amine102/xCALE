/*=============================================================================
 * Product        : 
 * File           : pmCommonTypeDefinitions.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef pm_COMMON_TYPES_H
#define pm_COMMON_TYPES_H

#include <cmath>
#include <vector>
#include <map>
#include <pl.h>

typedef long double pmCommonDataType;
typedef long double pmScoreValueType;

const pmCommonDataType minCommonData = -HUGE_VAL;
const pmCommonDataType maxCommonData =  HUGE_VAL;
const pmCommonDataType minScoreValue = -HUGE_VAL;
const pmCommonDataType maxScoreValue =  HUGE_VAL;

typedef int pmMyCellDataType;
typedef std::vector< pmMyCellDataType > pmMyRowDataType;

typedef int cellDataType;
typedef std::vector< cellDataType > rowDataType;
typedef std::vector< cellDataType > colDataType;

typedef plCSVFileDataDescriptor/*< pmMyCellDataType >*/ pmCSVDataDescriptor;
//typedef plDataDescriptor<long double> pmRowDataSet;

//typedef plDataDescriptor<rowDataType> pmDataSet;

typedef std::vector<std::vector<unsigned int>> CPCSteps;
typedef std::map<unsigned int, std::vector<unsigned int>> CPC_t;

typedef std::vector<std::vector<unsigned int>> Matrix_t;

#endif  // pm_COMMON_TYPES_H
