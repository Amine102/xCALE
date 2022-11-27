#include "createDummy.h"

using namespace std;
using namespace PILGRIM;

void createDummy()
{
    cout<<"creating variables"<<endl;
    plVariable A("A", PL_BINARY_TYPE);
    plVariable B("B", PL_BINARY_TYPE);
    plVariable C("C", PL_BINARY_TYPE);
    plVariable D("D", PL_BINARY_TYPE);
    plVariable E("E", PL_BINARY_TYPE);

cout<<"creating table of A"<<endl;
    plProbValue tableA[] = {0.3, 0.7};
    plProbTable P_A(A, tableA);

cout<<"creating table of D"<<endl;
    plProbValue tableD[] = {0.2, 0.8};
    plProbTable P_D(D, tableD);

cout<<"creating table of B"<<endl;
    plProbValue tableB_knowing_A[] = {
        0.9, 0.1,
        0.01, 0.99};
    plDistributionTable P_B_knowing_A(B, A, tableB_knowing_A);

cout<<"creating table of C"<<endl;
    plProbValue tableC_knowing_A_D[] = {
        0.2, 0.8,
        0.5, 0.5,
        0.7, 0.3,
        0.1, 0.9};
    plDistributionTable P_C_knowing_A_D(C, A ^ D, tableC_knowing_A_D);

cout<<"creating table of E"<<endl;
    plProbValue tableE_knowing_B_C[] = {
        0.1, 0.9,
        0.2, 0.8,
        0.01, 0.99,
        0.7, 0.3};
    plDistributionTable P_E_knowing_B_C(E, B ^ C, tableE_knowing_B_C);

cout<<"creating Joint Distribution"<<endl;
    plJointDistribution jdDummy(A ^ D ^ B ^ C ^ E , 
                                P_A * P_D * P_B_knowing_A * P_C_knowing_A_D * P_E_knowing_B_C);

cout<<"creating bayesian network"<<endl;
    pmBayesianNetwork bnDummy(jdDummy);
    cout << endl;
    cout << "Network summary : " << endl;
    bnDummy.summary();

    // Generate sample data as csv file
    bnDummy.generate_csvData("../../benchmarks/data/Dummy_csv.data", 2000);
    cout << "Dummy_csv.data generated in benchmarks/data/ " << endl;

    // Save the BNJuggler as xml
    bnDummy.save_as_xml("../../benchmarks/networks/jdBnDummy.xml", "jdDummy");
    cout << "jdBnDummy.xml generated in benchmarks/networks/ " << endl;
}