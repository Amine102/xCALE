#include "DBN_generate_csv_data.h"

using namespace PILGRIM;

void how_to_generate_csv_data()
{


      pmDynamicBayesianNetwork *DBN;
      pmDynamicBayesianNetwork *D = new pmDynamicBayesianNetwork();

    //loading DBN from an xml file
      DBN = D->load_from_xml("DBN_Ayush");

    //using generate_csv_data("name_of_file",value_last_slice,number_of_samples) to generate a csv file with random data 
      DBN->generate_csv_data("Random_csv_Ayush",5,3);

}
