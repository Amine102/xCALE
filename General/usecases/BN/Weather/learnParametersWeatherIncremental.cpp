#include "learnParametersWeatherIncremental.h"

using namespace std;
using namespace PILGRIM;
/**
 * \brief Use it after 'learnParameterWeather.cpp'
 * Load the BN created in 'createWeather.cpp', in xml.
 * Load the 2nd part of the splitted data set created in 'createWeather.cpp' in csv.data
 *  and learn BN's parameters incrementally using the new method (incrementation on data)
 * Save it as xml file
 */
void learnParametersIncrementalWeather(){

    //code from learnDummyIncremental.cpp
    
    cout << "Starting Splitting the data and the incremental Learning process using the divided data" << endl;
    
    cout << "Loading file " << endl;
    pmBayesianNetwork bnWeatherTemp("../../benchmarks/networks/lpBnWeather.xml", "lpBnWeather");
    const plVariablesConjunction &vars = bnWeatherTemp.getVariables();
    // --------------------------------------------------------------------------------------------------------------
    //Load data
    string path_to_data= "../../benchmarks/data/Weather_csv_file2.data";
    char *data_file = new char[path_to_data.length() +1];
    strcpy(data_file, path_to_data.c_str());

    size_t window_size = 1;
    size_t numberOfColumns = 3;
    pmCSVDataSet *data = new pmCSVDataSet(data_file, window_size, numberOfColumns, ';', vars);
    //Print & export
    data->summary();
    //----------------------------------------------------------------------------------------------------------------
    cout << "Network summary : " << endl;
    bnWeatherTemp.summary();
    cout << "Set Forgetting Factor" << endl;
    bnWeatherTemp.setForgettingFactor(0.7);
    cout << "Will enter learnParameters" << endl;
    bnWeatherTemp.learnParameters(data);
    cout << "Summary is " << endl;
    bnWeatherTemp.summary();
    bnWeatherTemp.save_as_xml("../../benchmarks/networks/lpiBnWeather.xml", "lpiBnWeather");
    
}