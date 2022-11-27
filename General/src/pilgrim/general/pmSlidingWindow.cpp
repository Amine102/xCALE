/*=============================================================================
* Product        : ProBT 
* File           : slidingWindow.cpp
* Author         : Amanullah YASIN
* Creation       : July 02 2012
*
*=============================================================================
*        (c) Copyright 2007, ProBayes SAS - all rights reserved

*=============================================================================
*
*------------------------- Description ---------------------------------------
*    
*   
*-----------------------------------------------------------------------------
*/

#include <iostream>
#include "pilgrim/general/pmSlidingWindow.h"
// #include <conio.h>
#include <fstream>

using namespace boost;

pmBayesianNetwork *pmSlidingWindow::previousBN = NULL;
pmBayesianNetwork *pmSlidingWindow::BN = NULL;
pmCSVDataSet *pmSlidingWindow::ds = NULL;
plVariablesConjunction pmSlidingWindow::variables;
unsigned int pmSlidingWindow::N = 0;
unsigned int pmSlidingWindow::N_old = 0;
unsigned int pmSlidingWindow::freqNotFoundInOldCache = 0;
unsigned int pmSlidingWindow::freqFoundInOldCache = 0;
std::map<string, std::vector<plFloat>> pmSlidingWindow::freqCache;
std::map<string, std::vector<plFloat>> pmSlidingWindow::freqCacheOld;
float pmSlidingWindow::forgettingCoefficient = 1;

pmSlidingWindow::pmSlidingWindow() {}
pmSlidingWindow::~pmSlidingWindow() {}

void pmSlidingWindow::initSlidingWindow(pmCSVDataSet *data, float fc)
{
    pmSlidingWindow::ds = data;
    // pmSlidingWindow::previousBN = previousBN;
    pmSlidingWindow::variables = data->observed_variables();
    pmSlidingWindow::setForgettingCoefficient(fc);
}

void pmSlidingWindow::setForgettingCoefficient(float forgCoefficient)
{
    pmSlidingWindow::forgettingCoefficient = forgCoefficient;
    //slFrequencyCounter<rowDataType>::setForgettingCoefficient(forgettingCoefficient);
}
void pmSlidingWindow::save_CurrentBN(pmBayesianNetwork *currBN)
{
    pmSlidingWindow::BN = currBN;
    // pmCSVDataSet data_descriptor = ds->get_data_descriptor();

    // * First generate PlBayesianNetwork object *
    pmBayesianNetwork *temppmBN = new pmBayesianNetwork(currBN->getJointDistribution());

    //* assign learn objects to the network
    setLearnObjects(temppmBN);

    //* learn parameters *
    temppmBN->learnParameters(ds);

    pmSlidingWindow::previousBN = temppmBN;
    unsigned int tempN = pmSlidingWindow::N;
    pmSlidingWindow::N = ds->get_n_records() + static_cast<int>(N) * forgettingCoefficient; // assign dataset size
    cout << "\n Number of records seen so far: " << pmSlidingWindow::N << " previous N:" << tempN << " Cache size: " << freqCache.size();
}
bool pmSlidingWindow::getOldFrequencyCount(const plVariablesConjunction &varAB, vector<plFloat> &freq)
{
    if (!getOldFreqFromCache(varAB, freq))
    {
        freqNotFoundInOldCache++;
        if (previousBN != NULL) // check if previous network exist in the cache
        {
            cout << "PREVIOUS BN IS USED" << endl;
            clock_t tStart = clock();
            unsigned int nSamples = 500;
            //cout<<"\n ==> Old Frequency count from MAP for "<<varAB<<endl;
            plDistribution P_AB;
            plDistribution Pab;
            try
            {
                previousBN->getJointDistribution().ask(P_AB, varAB);
                P_AB.n_compile(Pab, nSamples);
                cout << Pab << endl;
                std::vector<plProbValue> varPr;
                Pab.tabulate(varPr);
                //cout<<Pab;
                for (size_t i = 0; i < varPr.size(); i++)
                {
                    //freq.push_back(ceil(this->N * varPr.at(i)));
                    // cout<<"probability found by the presvious BN is "<<varPr.at(i)<<endl;
                    freq.push_back(N_old * varPr.at(i));
                    //cout<<"N:"<<this->N<<", prob:"<<varPr.at(i)<<", Fr:"<<freq.back()<<endl;
                }
            }
            catch (std::exception &e)
            {
                std::cerr << e.what() << endl;
                cout << "Error: Compile or tabulate plDistribution   ";
                // system("pause");
                cout << "stop!! " << endl;
            }
            cout << "\n+ time taken (Ask): " << static_cast<double>(clock() - tStart) / CLOCKS_PER_SEC;
            return true;
        }
        else
        {
            // cout<<"\n Warning: Could not find old frequencies for "<<varAB;
            return false;
        }
    }
    else
    {
        freqFoundInOldCache++;
        return true;
    }
}
bool pmSlidingWindow::getOldFreqFromCache(const plVariablesConjunction &varABC, vector<plFloat> &freq)
{
    string key;
    generateKey(varABC, key);
    //cout<<"\n >> "<<key;

    std::map<string, std::vector<plFloat>>::iterator freqItr;
    freqItr = freqCacheOld.find(key);

    if (freqItr == freqCacheOld.end())
    {
        return false;
    }

    else
    {
        freq = freqItr->second;
        return true;
    }
}
bool pmSlidingWindow::getFreqFromCache(const plVariablesConjunction &varABC, vector<plFloat> &freq)
{
    string key;
    generateKey(varABC, key);
    return getFreqFromCache(key, freq);
}
bool pmSlidingWindow::getFreqFromCache(string &key, vector<plFloat> &freq)
{
    std::map<string, std::vector<plFloat>>::iterator freqItr;
    freqItr = freqCache.find(key);

    if (freqItr == freqCache.end())
    {
        return false;
    }
    else
    {
        freq = freqItr->second;
        return true;
    }
}
void pmSlidingWindow::saveFreqInCache(plVariablesConjunction &varABC, vector<plFloat> &freq)
{
    string key;
    generateKey(varABC, key);
    freqCache.insert(make_pair(key, freq));
}
void pmSlidingWindow::saveFreqInCache(string &key, vector<plFloat> &freq)
{
    freqCache.insert(make_pair(key, freq));
}
void pmSlidingWindow::computeAndSaveFreqInCache(std::vector<unsigned int> &varSet, string key)
{
    pmFrequencyCounter *fq = new pmFrequencyCounter(pmSlidingWindow::ds, pmSlidingWindow::variables, true);

    std::vector<plFloat> freq;
    if (!getFreqFromCache(key, freq) && !varSet.empty())
    {
        fq->frequencyCount(varSet, freq);
        saveFreqInCache(key, freq);
        freq.clear();
    }
}
void pmSlidingWindow::generateKey(const plVariablesConjunction &varABC, string &key)
{
    // Generate a string by variables names as {v0}{v1}

    plVariablesConjunction::const_iterator plItr;
    stringstream varNames;
    //varNames <<"v";

    for (plItr = varABC.begin(); plItr != varABC.end(); ++plItr)
    {
        //varNames <<"_"; // to avoid v1 and 2 as v12 (twelve)
        varNames << *plItr;
    }

    key = varNames.str();
}
void pmSlidingWindow::generateKey(const std::vector<unsigned int> &varSet, string &key)
{
    // Generate a string by variables names as {v0}{v1}

    stringstream varNames;
    //varNames <<"v";

    for (unsigned int i = 0; i < varSet.size(); i++)
    {
        varNames << "{" << variables[i].name() << "}";
    }
    key = varNames.str();
}
std::vector<unsigned int> pmSlidingWindow::parseKey(const std::string &s)
{
    string ss(s);
    vector<unsigned int> varSet;
    while (!ss.empty())
    {
        std::string::size_type OpeningCurlyBracket_position = ss.find('{');
        std::string::size_type ClosingCurlyBracket_position = ss.find('}');

        int lengthOfVariable = ClosingCurlyBracket_position - OpeningCurlyBracket_position;
        string variable_str = ss.substr(OpeningCurlyBracket_position + 1, lengthOfVariable - 1);

        int variable_pos = pmSlidingWindow::variables.get_variable_position(variable_str);
        varSet.push_back(variable_pos);

        ss.erase(OpeningCurlyBracket_position, lengthOfVariable + 1);
    }
    return varSet;
}
void pmSlidingWindow::setLearnObjects(pmBayesianNetwork *temppmBN)
{
    pmNodeIterator vi, vi_end;
    pmNode node;
    ////
    vector<plFloat> init_values;
    plVariablesConjunction nodeVar;
    plVariablesConjunction parentsVars;
    vector<plLearnObject *> learnObjects;

    for (boost::tie(vi, vi_end) = BN->vertices(); vi != vi_end; ++vi)
    {
        cout << ".";
        node = *vi;
        init_values.clear();
        nodeVar = BN->getVariable(node);
        //nodeCard = BN->getCardinality(BN->getVariableIndex(node)); //

        if (boost::in_degree(node, BN->get_graph()) == 0)
        {
            // if node has no parents
            plLearnHistogram *plHisto;
            if (getOldFrequencyCount(nodeVar, init_values)) // check if old frequencies exist
                plHisto = new plLearnHistogram(nodeVar, init_values);
            else
                plHisto = new plLearnHistogram(nodeVar);

            learnObjects.push_back(plHisto);
            // temppmBN->set_distribution(*plHisto); // to set parameter distribution in plBayesianNetwork object
        }
        else
        {
            // if node has parents
            parentsVars = BN->getParents(node);
            plLearnDistributionTable *learn_node_k_prnt = new plLearnDistributionTable(nodeVar, parentsVars);
            if (getOldFrequencyCount(parentsVars ^ nodeVar, init_values))
            {
                std::vector<plFloat> init_freq_rowWise;
                int count = 0;
                plValues val(parentsVars);
                val.reset();
                do
                {
                    //exteract values for each combination of plValues
                    init_freq_rowWise.clear();
                    unsigned int nodeCard = nodeVar.cardinality();

                    for (unsigned int j = 0; j < nodeCard; j++)
                    {
                        init_freq_rowWise.push_back(init_values[j + (nodeCard * count)]);
                    }
                    //cout<<endl;

                    (*learn_node_k_prnt).push(plLearnHistogram(nodeVar, init_freq_rowWise), val); //
                    count++;
                } while (val.next());

                learnObjects.push_back(learn_node_k_prnt);

                // temppmBN->set_distribution(*learn_node_k_prnt);
            }
            else
            {
                // if initial frequencies are empty
                const plLearnHistogram *histo = new plLearnHistogram(nodeVar);
                plCndLearnObject<plLearnHistogram> *lrnObj = new plCndLearnObject<plLearnHistogram>(BN->getVariable(node),
                                                                                                    BN->getParents(node), *histo);
                learnObjects.push_back(lrnObj);

                // temppmBN->set_distribution(*lrnObj);
            }
        }
    } //endfor
    temppmBN->setLearnObject(learnObjects);
}
void pmSlidingWindow::updateCache()
{
    std::vector<unsigned int> keyVars;
    std::map<string, std::vector<plFloat>>::iterator cacheItr;
    for (cacheItr = freqCacheOld.begin(); cacheItr != freqCacheOld.end(); ++cacheItr)
    {
        keyVars = parseKey(cacheItr->first); //parseKey prend {Bronchitis}{LungCancer} as an argument and returns a varlist containning the index of the variables
        computeAndSaveFreqInCache(keyVars, cacheItr->first);
    }
    // cout << "\n ***** Old cache size: " << freqCacheOld.size() << " **** new cache size: " << freqCacheOld.size() << endl;

    MoveFreqToFreqOld();

    //Printing Results after updating Cache
}
void pmSlidingWindow::MoveFreqToFreqOld()
{
    freqCacheOld = freqCache;
    freqCache.clear();
}
void pmSlidingWindow::UpdateN(pmCSVDataSet *ds)
{
    pmSlidingWindow::N_old = N;
    pmSlidingWindow::N = ds->get_n_records() + pmSlidingWindow::N_old * forgettingCoefficient; // assign dataset size
    pmSlidingWindow::ds = ds;
}
void pmSlidingWindow::UpdateN(plDataDescriptor* pDataDesc)
{
    pmSlidingWindow::N_old = N;
    pmSlidingWindow::N = pDataDesc->get_n_records() + pmSlidingWindow::N_old * forgettingCoefficient; // assign dataset size
}
void pmSlidingWindow::UpdateCacheAndPrevBN(pmBayesianNetwork bn)
{
    pmSlidingWindow::updateCache();
    pmSlidingWindow::updatePreviousBN(bn);
}
void pmSlidingWindow::UpdateALL(pmCSVDataSet *ds, pmBayesianNetwork prevBN)
{
    cout << "Update Previous BN" << endl;
    pmSlidingWindow::updatePreviousBN(prevBN);

    pmSlidingWindow::UpdateCache_N(ds);
}
void pmSlidingWindow::UpdateCache_N(pmCSVDataSet *ds)
{
    cout << "\nCache will be Updated" << endl;
    pmSlidingWindow::updateCache();

    pmSlidingWindow::Summary();

    cout << "Update N using the data" << endl;
    pmSlidingWindow::UpdateN(ds);
    cout << "pmSlidingWindow::N after updating it is ";
    cout << pmSlidingWindow::N << endl;
}
void pmSlidingWindow::updatePreviousBN(pmBayesianNetwork bn)
{
    pmSlidingWindow::previousBN = new pmBayesianNetwork(bn.getVariables());
    pmSlidingWindow::previousBN->copy_structure(bn);
}
void pmSlidingWindow::freqCacheSummary()
{
    for (auto const &elem : pmSlidingWindow::freqCache)
    {
        std::cout << elem.first << "\n";
        for (size_t i = 0; i < elem.second.size(); i++)
        {
            cout << elem.second.at(i) << ' ';
        }
        cout << "\n";
    }
    cout << "\n";
}

void pmSlidingWindow::freqOldCacheSummary()
{
    for (auto const &elem : pmSlidingWindow::freqCacheOld)
    {
        std::cout << elem.first << "\n";
        for (size_t i = 0; i < elem.second.size(); i++)
        {
            cout << elem.second.at(i) << ' ';
        }
        cout << "\n";
    }
    cout << "\n";
}

void pmSlidingWindow::Summary()
{
    cout << "FreqCache is " << endl;
    freqCacheSummary();
    cout << "\n\nFreqOldCache is :" << endl;
    freqOldCacheSummary();
    // cout << "\n\npmSlidingWindow::N is " << pmSlidingWindow::N << endl;
}

// void pmSlidingWindow::initializeSlidingWindow(plVariablesConjunction vars){
//     pmSlidingWindow::variables=vars;
// }