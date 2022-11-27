#include "queryEyeColorGeneBN.h"
#include <exception>

using namespace std;
using namespace PILGRIM;

void queryEyeColorGeneBN()
{
    try
    {
        unsigned int i = 0;
        // Load  and print the BNJuggler from xml file
        pmBayesianNetwork bnEyeColorGene("../../benchmarks/networks/eyeGeneBN.xml", "eyeGeneBN");
        bnEyeColorGene.summary();

        // Query the new loaded BNJuggler to compare results
        plVariablesConjunction Variables = bnEyeColorGene.getVariables();
        plVariable SonEyeColorVar = Variables.get_variable_with_name("SonEyeColorVar");
        plVariable AlleleSon1 = Variables.get_variable_with_name("AlleleSon1");
        plVariable AlleleSon2 = Variables.get_variable_with_name("AlleleSon2");
        plVariable FatherEyeColor = Variables.get_variable_with_name("FatherEyeColor");
        plVariable AlleleFather1 = Variables.get_variable_with_name("AlleleFather1");
        plVariable AlleleFather2 = Variables.get_variable_with_name("AlleleFather2");
        plVariable MotherEyeColor = Variables.get_variable_with_name("MotherEyeColor");
        plVariable AlleleMother1 = Variables.get_variable_with_name("AlleleMother1");
        plVariable AlleleMother2 = Variables.get_variable_with_name("AlleleMother2");

        plDistribution pld;

        //Evidence and Query 1
        plValues evidence1(AlleleSon1^AlleleSon2);
        evidence1[AlleleSon1]=0;
        evidence1[AlleleSon2]=0;
        cout << "1st query" << endl;
        plDistribution pld_new = bnEyeColorGene.query(SonEyeColorVar, evidence1);
        pld_new.tabulate();
    }
    catch (const std::exception &e)
    {
        std::cerr << " Error while loading from file : " << e.what();
    }
}