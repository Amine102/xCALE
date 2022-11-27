/*=============================================================================
 * Product        :
 * File           : pmCSVdynDataSet.h
 * Author         :
 * Creation       :

  *=============================================================================
 *        (c) Copyright 2019, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef _LoadDynDataSet_H
#define _LoadDynDataSet_H

#include <pl.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

#include <plStructureLearner.h>

#include "pilgrim/general/pmCommonTypeDefinitions.h"
#include "pilgrim/general/pmCSVDataSet.h"



namespace PILGRIM {
class pmCSVdynDataSet {
 public:

 /*
 =====CONSTRUCTORS=====
 */
   /** 
    * \brief : guessing all needed param in pmCSVDataSet constructors
    * \details
    * \
    */
   pmCSVdynDataSet( char *_file_name);


   /**
    * \brief
    * \details
    * \param nb_header_lines: Number of header lines to ignore during importation
    * \param nb_columns: Number of columns / Variables
    * \param variables Conjunction describing the variables present in the file for initial
    * \param variables Conjunction describing the variables present in the file for transition
    * \param delimiter: CSV separator auto set as ";" from generate_csv
    */
   pmCSVdynDataSet(char *_file_name,
                  size_t &nb_header_lines,
                  size_t &nb_columns,
                  const plVariablesConjunction &variables_t0,
                  const plVariablesConjunction &variables_T,
                  char delimiter = ';'
                  );


 /*
 =====DESTRUCTOR=====
 */
   ~pmCSVdynDataSet(void);


 /*
 =====GETTERS=====
 */

   //Get data descriptor
   plCSVFileDataDescriptor* /*<pmMyCellDataType>**/ get_data_descriptor_t0();
   plCSVFileDataDescriptor* /*<pmMyCellDataType>**/ get_data_descriptor_T();

   //Get t0 data set
   pmCSVDataSet* get_data_set_t0();

   //Get transition data set
   pmCSVDataSet* get_data_set_T();

   //Get data records
   bool get_data_record_t0(std::vector<std::string>& records);
   bool get_data_record_T(std::vector<std::string>& records);

   //Get nb of records
   size_t get_n_records_t0();
   size_t get_n_records_T();

   //Get nb of columns
   unsigned int get_num_fields_t0();
   unsigned int get_num_fields_T();

   //Get variable conjunction
   virtual plVariablesConjunction observed_variables_t0();
   virtual plVariablesConjunction observed_variables_T();


/*
=====PRINTER=====
*/
    void summary();

/*
=====METHOD=====
*/
   void split();

/*
=====PROTECTED ATTRIBUTES====
*/
 protected:
   char *file_name;

   // DataSet attribute
   pmCSVDataSet* data_set_t0;
   pmCSVDataSet* data_set_T;
};
};  // namespace PILGRIM
#endif

/**
 * Creation: 2019/06/04
 * Last update: 2019/06/07
 * _t0 means initial and _T means transition
 * Adding split function to create both data_set_t0 and data_set_T
 * The split function might be simplifiable
 * Basic getters, same as pmCSVDataSet
 * Modif : Côme Vitis
 */