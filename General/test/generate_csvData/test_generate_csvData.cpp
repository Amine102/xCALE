#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCommonTypeDefinitions.h>
#include <pl.h>

#include "testing.h"

using namespace std;

struct BNAsiaenvironment {
public:
    BNAsiaenvironment(){
        bnAsia = pmBayesianNetwork("../benchmarks/networks/Asia.xdsl");
    }
public:
    pmBayesianNetwork bnAsia;
};

/**
 * @brief Checking the parameters and there value
 * 
 * @return testing::Uint32 
 */
testing::Uint32 fn1() {
    testing::Uint32 the_errors = 0;

    BNAsiaenvironment BNAsiaenvironment;
    BNAsiaenvironment.bnAsia.generate_csvData("../benchmarks/data/dataAsia_test_csv.data", 100);

    ifstream data;
    data.open("../benchmarks/data/dataAsia_test_csv.data");
    string dataLine;
    string header = "\"Bronchitis\";\"Dyspnea\";\"LungCancer\";\"Smoking\";\"TbOrCa\";\"Tuberculosis\";\"VisitToAsia\";\"XRay\"";
    if (data.is_open()) {
        getline(data, dataLine);
        // check the header
        PILG_TEST_FAIL_COUNT(dataLine == header, the_errors);
        while (data)
        {   
            int i = 0;
            getline(data, dataLine);
            size_t pos = 0;
            string value; 
            // check value for each colone
            while ( (pos=dataLine.find(';')) != string::npos && i < 8) {  
                bool test;
                value = dataLine.substr(0, pos);
                if (i ==  0 || i == 1 || i == 2 || i == 5){
                    test = (value == "Absent" || value == "Present");
                }
                else{
                    switch (i)
                    {
                    case 3:
                        test = (value == "Smoker" || value == "NonSmoker");
                        break;
                    case 4:
                        test = (value == "CancerORTuberculosis" || value == "Nothing");
                        break;
                    case 6:
                        test = (value == "Visit" || value == "NoVisit");
                        break;
                    case 7:
                        test = (value == "Normal" || value == "Abnormal");
                    
                    default:
                        break;
                    }
                }
                dataLine.erase(0, pos + 1);
                PILG_TEST_FAIL_COUNT(test, the_errors);
                ++i;
            }
        }
        
    }


    std::cout << PILG_TEST_PRESENT_RESULTS(the_errors) << '\n';
    return the_errors;
}

int main() {
    testing::Uint32 the_errors = 0;

    the_errors += fn1();

    std::cout << "Error count = " << the_errors << std::endl;

    return the_errors;
}