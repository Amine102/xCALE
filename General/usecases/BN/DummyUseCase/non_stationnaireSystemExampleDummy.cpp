#include "non_stationnaireSystemExampleDummy.h"
#include "TestingLearnIncremental.h"
#include "HelperFunctions.h"
#include "createDummy1.h"
#include "createDummy2.h"
#include "createDummy3.h"
#include "learnDummy.h"
#include <algorithm>

using namespace std;
using namespace PILGRIM;

void non_stationnaireSystemExampleDummy()
{
    createDummy1();
    createDummy2();
    createDummy3();

    vector<string> datafiles_path;
    string datafile1 = "../../benchmarks/data/Dummy1_csv.data";
    string datafile2 = "../../benchmarks/data/Dummy2_csv.data";
    string datafile3 = "../../benchmarks/data/Dummy3_csv.data";

    datafiles_path.push_back(datafile1);
    datafiles_path.push_back(datafile2);
    datafiles_path.push_back(datafile3);

    int numberOfDataFiles = datafiles_path.size();
    int numberOfPartsOfSplits = 10;
    int TotalNumberOfGeneratedFiles = numberOfDataFiles * numberOfPartsOfSplits;

    for (int i = 0; i < numberOfDataFiles; i++)
    {
        splitData(datafiles_path[i], numberOfPartsOfSplits);
    }

    bool changingForgettingFactor = AskUserIfChangingForgetFactor();

    double ff;
    if (changingForgettingFactor == false)
    {
        ff = getForgettingFactorFromUser();
    }
    else
    {
        cout << "The ForgettingFactor will be initialized to 0.5" << endl;
        ff = 0.5;
    }

    pmBayesianNetwork bnDummy;
    pmCSVDataSet *data;
    plVariablesConjunction vars;
    size_t window_size = 1;
    size_t numberOfColumns = 5;

    for (int i = 0, fileIndex = 0, counter = 0; i < numberOfPartsOfSplits && fileIndex <= datafiles_path.size(); i++, counter++)
    {
        string datafile = datafiles_path[fileIndex] + to_string(i) + ".data";
        cout << datafile << endl;
        char *data_file = strcpy(new char[datafile.length() + 1], datafile.c_str());

        if (changingForgettingFactor == true)
        {
            ff = (double)counter / (double)(counter + 1);
        }
        //First Time Learning is non-incremental Learning
        if (i == 0 && fileIndex == 0)
        {
            data = new pmCSVDataSet(data_file);
            cout<<"aaaa"<<endl;
            vars = data->observed_variables();
            cout<<"aaaa"<<endl;
            bnDummy = pmBayesianNetwork(vars);
            cout<<"aaaa"<<endl;
            pmBayesianNetwork bnDummyTemp("../../benchmarks/networks/jdBnDummy1.xml", "jdDummy");
            cout<<"aaaa"<<endl;
            bnDummy.copy_structure(bnDummyTemp);
        }
        else
        {
            data = new pmCSVDataSet(data_file, window_size, numberOfColumns, ';', vars);            

            if (i == (numberOfPartsOfSplits - 1) && fileIndex != (datafiles_path.size() - 1))
            {
                i = -1;
                fileIndex++;
                bnDummy.summary();
                cout << "===============================================================================" << endl;
            }
            // exportVectorOfProba(bnDummy.getComputableObjectList(), "P(E)", counter);
        }
        bnDummy.setForgettingFactor(ff);
        bnDummy.learnParameters(data);
        bnDummy.summary();
        bnDummy.save_as_xml("../../benchmarks/networks/lpBnDummy.xml", "lpBnDummy");
        exportVectorOfProba(bnDummy.getComputableObjectList(), "P(E|B C)", counter);
    }
    pmBayesianNetwork bnDummy_final("../../benchmarks/networks/lpBnDummy.xml", "lpBnDummy");
    bnDummy_final.summary();
}