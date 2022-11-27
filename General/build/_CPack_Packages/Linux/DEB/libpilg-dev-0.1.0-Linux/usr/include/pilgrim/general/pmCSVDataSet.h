/*==============================================================================
 * Product        :
 * File           : plCSVDataSet.h
 * Author         :

 * Creation       :



  *=============================================================================

 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *==============================================================================
 */

#ifndef _LoadDataSet_H

#define _LoadDataSet_H



#include <pl.h>

#include <iostream>

#include <vector>

#include <string> 



#include <plStructureLearner.h>



#include "pilgrim/general/pmCommonTypeDefinitions.h"






namespace PILGRIM {

class pmCSVDataSet {

 public:

 /*
 =====CONSTRUCTORS=====
 */

   /**
    * \brief
    * \details
    * \param winsize Number of header lines to ignore during importation
    * \param nmbFields Number of columns
    * \param separator CSV separator considered
    * \param start_num_gaussians minimum number of gaussians
    * \param force_num_gaussians is the start_num_gaussians the exact final number of gaussians
    */
   pmCSVDataSet(  const std::string& _file_name,
                  size_t &winSize,
                  size_t &nmbFields,
                  char separator,
                  int start_num_gaussians = 2,
                  bool force_num_gaussians = false);


   /**
    * \brief
    * \details
    * \param winsize Number of header lines to ignore during importation
    * \param nmbFields Number of columns
    * \param separator CSV separator considered
    * \param variables Conjunction describing the variables present in the file
    */
   pmCSVDataSet( const std::string& _file_name,
                 size_t &winSize,

                 size_t &nmbFields,

                 char separator,

                 const plVariablesConjunction &variables);


   /**
    * \Short constructor guessing winSize and nmbFields
    */
   pmCSVDataSet(const std::string& _file_name);


   /**
    * \Constructor adding a table for continuous variables thats we dont want to discretize and maximum cardinality for discret variables 
    */
   pmCSVDataSet(const std::string& _file_name,
                
                std::vector <std::string> vars_undiscretized ,
                
                int vars_cardinality);
        
     /**
    * \ Constructor used when we want to use classic discretization methods of ProbT 
    */
   pmCSVDataSet(const std::string& _file_name,
   
                int n_discretization_bins);

/******/
pmCSVDataSet(const std::string& _file_name, const plVariablesConjunction &variables);

 /*
 =====DESTRUCTOR=====
 */

   ~pmCSVDataSet(void);

 /*
 =====GETTERS=====
 */
   // Get data descriptor
   plCSVFileDataDescriptor* get_data_descriptor();

   // Get number of variables / columns
   unsigned int get_num_fields();

   // Get number of record
   size_t get_n_records();

   // Get the file name
   std::string& get_filename();

   // Get the variable conjunction observed
   virtual plVariablesConjunction observed_variables();

   // Get data record from the vector of records
   bool get_data_record(std::vector<std::string>& records);


/*
=====INIT=====
*/

   // Old init constructor method, does'nt work
   void create_and_set_variables(size_t &nmbFields);

   // New init constructor method if no VariablesConjunction given
   void create_and_guess_variables(std::pair<bool,int> force_num_gaussians = std::make_pair(false, 2));

   // Used when we want to specify that one variable is discretized and indicate the number of bins
   void create_and_guess_variables(int n_discretization_bins);
   
   // Used when specifying the variables set that we dont want to discretize and the maximum cardinality that a discret variable can have 
   void create_and_guess_variables(std::vector<std::string> vars_undiscretized,int vars_cardinality);


   // Guess delimiter
   char guess_delimiter();

   // Guess number of variables
   size_t guess_nb_variables(char delim);

/*
=====PRINTERS=====
*/
   // Print simple stat summary
   void summary();

   // Print variables names in datadescriptor
   void print_variables();

/*
=====PROTECTED ATTRIBUTES AND INIT METHOD====
*/
 protected:
   // Init protected
   void loadDataSet(size_t &winSize, size_t &nmbFields, char separator);

   std::string file_name;
   plCSVFileDataDescriptor* /*<pmMyCellDataType>*/ data_descriptor;
};

};  // namespace PILGRIM

#endif

/**
 * Last update: 2020/07/22
 * Adding the continuous variables discretization 
 * Modifying create_and_guess_variables()
 * Adding create_and_guess_variables(int n_discretization_bins)
 * Adding create_and_guess_variables(std::vector<std::string> vars_undiscretized,int vars_cardinality)
 * Cleaning the code and adding some comments
 *
 * Modif : Houda Abouchamala
 */
