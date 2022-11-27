
#include "pilgrim/general/pmFrequencyCounter.h"
#include "pilgrim/general/pmSlidingWindow.h"
#include "plDataDescriptor.h"
#include "pilgrim/general/pmUtility.h"
#include "plConfig.h"
#include <iostream>

using namespace boost;
//=============================================================================
pmFrequencyCounter /*<rowDataType>*/ ::pmFrequencyCounter(plDataDescriptor * /*<rowDataType>**/ pDataDesc, const plVariablesConjunction &allVariables)
{
  this->setDataDescriptor(pDataDesc);
  this->allVariables = allVariables;
  this->freeMissingProbTable = false;
  this->setMissingProbabilityTable(NULL, false);
  this->freeJointDistribution = false;
  this->setJointDistribution(NULL, false);
  this->setEstimationMethod(Uniform);
  unsigned long int exCnt = 0;
  this->pDataDesc->rewind();
  exCnt = this->pDataDesc->get_n_records();
  this->exempleCount = exCnt;
}

//=============================================================================
pmFrequencyCounter /*<rowDataType>*/ ::pmFrequencyCounter(pmCSVDataSet * /*<rowDataType>**/ data, const plVariablesConjunction &allVariables, bool useSlWind, float SW_forgettingCoeff, bool AllowUpdates)
{
  this->setDataDescriptor(data->get_data_descriptor());
  this->allVariables = allVariables;
  this->freeMissingProbTable = false;
  this->setMissingProbabilityTable(NULL, false);
  this->freeJointDistribution = false;
  this->setJointDistribution(NULL, false);
  this->setEstimationMethod(Uniform);
  this->usingSlidingWindow = useSlWind;
  this->data = data;
  if (this->usingSlidingWindow)
  {
    if (AllowUpdates == true)
    {
      pmSlidingWindow::UpdateN(data);
      this->exempleCount = pmSlidingWindow::N;
      pmSlidingWindow::initSlidingWindow(data, SW_forgettingCoeff);
    }
  }
  else
  {
    unsigned long int exCnt = 0;
    this->pDataDesc->rewind();
    exCnt = this->pDataDesc->get_n_records();
    this->exempleCount = exCnt;
  }
}

//=============================================================================
pmFrequencyCounter::pmFrequencyCounter(pmFrequencyCounter &other) : weights(other.weights)
{
  this->setDataDescriptor(other.pDataDesc);
  this->allVariables = other.allVariables;
  this->freeMissingProbTable = false;
  this->setMissingProbabilityTable(NULL, false);
  this->freeJointDistribution = false;
  this->setJointDistribution(NULL, false);
  this->estimationMethod = other.estimationMethod;
  this->exempleCount = 0;
}

//=============================================================================
/*template <typename rowDataType>
pmFrequencyCounter<rowDataType>::~pmFrequencyCounter() {
  this->setMissingProbabilityTable(NULL, false);
  this->setJointDistribution(NULL, false);
}
*/
//=============================================================================

void pmFrequencyCounter::setDataDescriptor(plDataDescriptor * /*<rowDataType>**/ pDataDesc)
{
  this->pDataDesc = pDataDesc;
}

//=============================================================================

plDataDescriptor * /*<rowDataType>**/ pmFrequencyCounter::getDataDescriptor()
{
  return this->pDataDesc;
}
//=============================================================================

void pmFrequencyCounter::setVariables(const plVariablesConjunction &allVariables)
{
  this->allVariables = allVariables;
}

//=============================================================================

const plVariablesConjunction &pmFrequencyCounter::getVariables()
{
  return this->allVariables;
}

//=============================================================================

void pmFrequencyCounter::setEstimationMethod(
    enumEstimationMethod estimationMethod)
{
  this->estimationMethod = estimationMethod;
}

//=============================================================================

pmFrequencyCounter::enumEstimationMethod pmFrequencyCounter::getEstimationMethod()
{
  return this->estimationMethod;
}

//=============================================================================

void pmFrequencyCounter::setMissingProbabilityTable(
    const vector<vector<plProbValue>> *missing_probability_table,
    bool autoDelete)
{
  if (this->freeMissingProbTable && this->missing_probability_table)
    delete this->missing_probability_table;
  this->missing_probability_table = missing_probability_table;
  this->freeMissingProbTable = autoDelete;
}

//=============================================================================

const vector<vector<plProbValue>> *pmFrequencyCounter::getMissingProbabilityTable()
{
  return this->missing_probability_table;
}

//=============================================================================

void pmFrequencyCounter::setJointDistribution(plJointDistribution *pJD,
                                              bool autoDelete)
{
  if (this->freeJointDistribution && this->pJointDistribution)
    delete this->pJointDistribution;
  this->pJointDistribution = pJD;
  this->freeJointDistribution = autoDelete;
}

//=============================================================================

plJointDistribution *pmFrequencyCounter::getJointDistribution()
{
  return this->pJointDistribution;
}

//=============================================================================

void pmFrequencyCounter::computeFrequencies()
{
  unsigned long int exCnt = 0;
  this->pDataDesc->rewind();
  exCnt = this->pDataDesc->get_n_records();
  if (usingSlidingWindow)
    this->exempleCount = exCnt + pmSlidingWindow::N;
  else
    this->exempleCount = exCnt;
}
//=============================================================================

void pmFrequencyCounter::frequencyCount(const vector<bool> &searchedVars, vector<plFloat> &freq)
{
  plVariablesConjunction searchedVariables;
  pmUtility::selectVariables(this->allVariables,
                             searchedVars,
                             searchedVariables);
  bool flag = pmSlidingWindow::getFreqFromCache(searchedVariables, freq);
  if (flag)
  {
    return;
  }
  else
  {
    if (usingSlidingWindow)
    {
      plLearnHistogram *plearnHisto;
      std::vector<plFloat> freqOld;
      if (pmSlidingWindow::getOldFrequencyCount(searchedVariables, freqOld))
      {
        // for adding forgetting factor 0.5, divide freqold vector by 2.
        // to avoid 0/2 division error, first we add 2 in the vector, then divide by 2 and later minus 1.
        // So, by doing this 0 will remains 0 after division.
        //
        std::transform(freqOld.begin(), freqOld.end(), freqOld.begin(),
                       std::bind1st(std::multiplies<plFloat>(), pmSlidingWindow::forgettingCoefficient)); // forgettingCoefficient));

        plearnHisto = new plLearnHistogram(searchedVariables, freqOld);
      }
      else
      {
        plearnHisto = new plLearnHistogram(searchedVariables);
      }

      vector<plLearnObject *> learnObjs(1);
      learnObjs[0] = plearnHisto;
      plLearnDistribVector learnDistribVect(learnObjs);
      this->learnDistributions(learnDistribVect, searchedVars);
      //std::cout << learnHisto.get_distribution() << std::endl;
      delete (plearnHisto);
      plearnHisto = reinterpret_cast<plLearnHistogram *>(learnDistribVect.get_learn_objects()[0]->clone());
      plearnHisto->get_frequency(freq);
      delete (plearnHisto);
      //======================================================================
      // 			cout<<"\n debug: freqOld: "<<freqOld.size()<<" freq: "<<freq.size();
      // 			std::vector<plFloat>::iterator Itr;
      //  			unsigned int i;
      // // 			if (!freqOld.empty())
      //  				for (Itr = freqOld.begin(), i = 0; Itr != freqOld.end(); Itr++, i++)
      //  					{
      //  					//freq[i] += (*Itr); // to add in existing freq
      //  					cout<<"\n debug: old freq: "<<freqOld[i]<<" + current: "<<freq[i];
      //  					}
      //======================================================================
        pmSlidingWindow::saveFreqInCache(searchedVariables, freq);
    }
    else
    {
      plLearnHistogram *plearnHisto = new plLearnHistogram(searchedVariables);
      vector<plLearnObject *> learnObjs(1);
      learnObjs[0] = plearnHisto;
      plLearnDistribVector learnDistribVect(learnObjs);
      this->learnDistributions(learnDistribVect, searchedVars);
      //std::cout << learnHisto.get_distribution() << std::endl;
      delete (plearnHisto);
      plearnHisto = reinterpret_cast<plLearnHistogram *>(learnDistribVect.get_learn_objects()[0]->clone());
      plearnHisto->get_frequency(freq);
      delete (plearnHisto);
    }
  } //end if flag
}

//=============================================================================
// added by F. Schnitzler, 2nd December 2010

void pmFrequencyCounter::frequencyCount(const std::vector<unsigned int> &SearchedVariablesIndices,
                                        vector<plFloat> &freq)
{
  plVariablesConjunction searchedVariables;
  pmUtility::selectVariables(this->allVariables,
                             SearchedVariablesIndices,
                             searchedVariables);

  bool flag = pmSlidingWindow::getFreqFromCache(searchedVariables, freq);

  if (flag)
  {
    return;
  }
  else
  {
    if (usingSlidingWindow)
    {
      plLearnHistogram *plearnHisto;
      std::vector<plFloat> freqOld;
      if (pmSlidingWindow::getOldFrequencyCount(searchedVariables, freqOld))
      {
        // 							std::vector<plFloat>::iterator Itr;
        //  							cout<<"\n\n";
        //  							for (Itr = freqOld.begin(); Itr != freqOld.end(); Itr++)
        //  				 					{
        //  									cout<<": "<<*Itr;
        //  				 					}

        // for adding forgetting factor 0.5, divide freqold vector by 2.
        // to avoid 0/2 problem, first we add 2 in the vector, then divide by 2 and later minus 1.
        // So, by doing this 0 will remains 0 after division.
        std::transform(freqOld.begin(), freqOld.end(), freqOld.begin(),
                       std::bind1st(std::multiplies<plFloat>(), pmSlidingWindow::forgettingCoefficient)); //forgettingCoefficient));

        // 				cout<<"\n";
        // 				for (Itr = freqOld.begin(); Itr != freqOld.end(); Itr++)
        // 					{
        // 					cout<<": "<<*Itr;
        // 					}

        plearnHisto = new plLearnHistogram(searchedVariables, freqOld);
      }
      else
      {
        plearnHisto = new plLearnHistogram(searchedVariables);
      }

      vector<plLearnObject *> learnObjs(1);
      learnObjs[0] = plearnHisto;
      plLearnDistribVector learnDistribVect(learnObjs);
      this->learnDistributions(learnDistribVect, SearchedVariablesIndices);
      //std::cout << learnHisto.get_distribution() << std::endl;
      //learnDistribVect.get_learn_objects()[0].get_frequency(freq);
      delete (plearnHisto);
      plearnHisto = reinterpret_cast<plLearnHistogram *>(learnDistribVect.get_learn_objects()[0]->clone());
      plearnHisto->get_frequency(freq);
      delete (plearnHisto);
      // 			std::vector<plFloat>::iterator Itr;
      // 			unsigned int i;
      //
      // 			if (!freqOld.empty())
      // 				for (Itr = freqOld.begin(), i = 0; Itr != freqOld.end(); Itr++, i++)
      // 					{
      // 					freq[i] += (*Itr); // to add in existing freq
      // 					//cout<<"\n ask() old freq: "<<freqOld[i]<<" + current: "<<freq[i]<<" = "<<freqOld[i]+freq[i];
      // 					}
        pmSlidingWindow::saveFreqInCache(searchedVariables, freq);
    }
    else
    {
      plLearnHistogram *plearnHisto = new plLearnHistogram(searchedVariables);
      vector<plLearnObject *> learnObjs(1);
      learnObjs[0] = plearnHisto;
      plLearnDistribVector learnDistribVect(learnObjs);
      this->learnDistributions(learnDistribVect, SearchedVariablesIndices);
      delete (plearnHisto);
      plearnHisto = reinterpret_cast<plLearnHistogram *>(learnDistribVect
                                                             .get_learn_objects()[0]
                                                             ->clone());
      plearnHisto->get_frequency(freq);
      delete (plearnHisto);
    }
  }
}

//=============================================================================

long unsigned int pmFrequencyCounter::getExempleCount()
{
  return this->exempleCount;
}

//=============================================================================

long unsigned int pmFrequencyCounter::
    learnDistributions(plLearnDistribVector &learnDistribVect)
{
  vector<bool> searchedVars(this->allVariables.dim());
  constructSearchedVariableList(learnDistribVect.get_learn_objects(),
                                searchedVars);
  return this->learnDistributions(learnDistribVect, searchedVars);
}

//=============================================================================
long unsigned int pmFrequencyCounter::
    learnDistributions(plLearnDistribVector &learnDistribVect,
                       const std::vector<unsigned int> &SearchedVariablesIndices)
{
  const plValues *pCurrentLine;
  const vector<bool> *pPresentValues;
  unsigned int i;
  long unsigned int idxLine;
  plValues completeLine(this->allVariables);
  plVariablesConjunction *pMissingVars = NULL;
  plVariablesConjunction *pKnownVars = NULL;
  vector<bool> trueVector(learnDistribVect.get_learn_objects().size(), true);
  const vector<plProbValue> *pCurrentMissingProba;
  plVariablesConjunction_const_iterator vari, vari_end;

  bool message_state = plError::message_is_ignored(50);
  plError::ignore_this_message(50, true);

  this->pDataDesc->rewind();

  idxLine = -1;

  std::vector<unsigned int>::const_iterator vi, vi_end;

  while ((this->pDataDesc->get_data_record(pCurrentLine, pPresentValues)))
  {
    idxLine++;
    if (this->isMissingData(*pPresentValues, SearchedVariablesIndices))
    {
      // 13 second on condition for 5000 MI on 200 samples
      pMissingVars = new plVariablesConjunction();
      pKnownVars = new plVariablesConjunction();
      for (i = 0,
          vari = this->allVariables.begin(),
          vari_end = this->allVariables.end();
           vari != vari_end;
           ++vari, ++i)
      {
        if ((*pPresentValues)[i])
        {
          completeLine[i] = (*pCurrentLine)[i];
          (*pKnownVars) ^= *vari;
        }
        else
        {
          (*pMissingVars) ^= *vari;
        }
      }
      pCurrentMissingProba = this->computeCurrentMissingProbabilities(
          idxLine,
          completeLine,
          *pMissingVars,
          *pKnownVars);
      if (pCurrentMissingProba)
      {
        completeLine.reset(*pMissingVars);
        i = 0;
        do
        {
          learnDistribVect.add_point_if_not_frozen(completeLine,
                                                   trueVector,
                                                   (*pCurrentMissingProba)[i++]);
        } while (completeLine.next(*pMissingVars));
        delete pMissingVars;
        delete pKnownVars;
      }
    }
    else
    {
      for (vi = SearchedVariablesIndices.begin(),
          vi_end = SearchedVariablesIndices.end();
           vi < vi_end;
           ++vi)
        completeLine[*vi] = (*pCurrentLine)[*vi];
      // bottleneck > 2k seconds for 5000 MI on 200 samples
      learnDistribVect.add_point_if_not_frozen(completeLine, trueVector);
      // 4 seconds for 5000 MI on 200 samples
    }
  }
  plError::ignore_this_message(50, message_state);

  long unsigned int res = idxLine + 1;
  return res;
}

//=============================================================================

long unsigned int pmFrequencyCounter::
    learnDistributions(plLearnDistribVector &learnDistribVect,
                       const vector<bool> &searchedVars)
{
  const plValues *pCurrentLine;
  const vector<bool> *pPresentValues;
  unsigned int i, n = this->pDataDesc->get_num_fields();
  long unsigned int idxLine;
  plValues completeLine(this->allVariables);
  plVariablesConjunction *pMissingVars = NULL;
  plVariablesConjunction *pKnownVars = NULL;
  vector<bool> trueVector(learnDistribVect.get_learn_objects().size(), true);
  const vector<plProbValue> *pCurrentMissingProba;
  plVariablesConjunction_const_iterator vi, vi_end;

  bool message_state = plError::message_is_ignored(50);
  plError::ignore_this_message(50, true);

  this->pDataDesc->rewind();
  idxLine = -1;

  while ((this->pDataDesc->get_data_record(pCurrentLine, pPresentValues)))
  {
    idxLine++;
    if (this->isMissingData(*pPresentValues, searchedVars))
    {
      // 13 second on condition for 5000 MI on 200 samples
      pMissingVars = new plVariablesConjunction();
      pKnownVars = new plVariablesConjunction();
      for (i = 0,
          vi = this->allVariables.begin(),
          vi_end = this->allVariables.end();
           vi != vi_end;
           ++vi, ++i)
      {
        if ((*pPresentValues)[i])
        {
          completeLine[i] = (*pCurrentLine)[i];
          (*pKnownVars) ^= *vi;
        }
        else
        {
          (*pMissingVars) ^= *vi;
        }
      }
      pCurrentMissingProba = this->computeCurrentMissingProbabilities(
          idxLine,
          completeLine,
          *pMissingVars,
          *pKnownVars);
      if (pCurrentMissingProba)
      {
        completeLine.reset(*pMissingVars);
        i = 0;
        do
        {
          learnDistribVect.add_point_if_not_frozen(completeLine,
                                                   trueVector,
                                                   (*pCurrentMissingProba)[i++]);
        } while (completeLine.next(*pMissingVars));
        delete pMissingVars;
        delete pKnownVars;
      }
    }
    else
    {
      for (i = 0; i < n; i++)
        completeLine[i] = (*pCurrentLine)[i];
      // bottleneck > 2k seconds for 5000 MI on 200 samples
      learnDistribVect.add_point_if_not_frozen(completeLine, trueVector);
      // 4 seconds for 5000 MI on 200 samples
    }
  }
  plError::ignore_this_message(50, message_state);

  return idxLine + 1;
}

//=============================================================================

plJointDistribution *pmFrequencyCounter::
    learnAndBuildDistribution(plLearnDistribVector &learnDistribVect,
                              const vector<bool> &searchedVars)
{
  const plValues *pCurrentLine; //remplacement de vector<rowDataType> par rowDataType
  const vector<bool> *pPresentValues;
  unsigned int i, n = this->pDataDesc->get_num_fields();
  long unsigned int idxLine;
  plValues completeLine(this->allVariables);
  plVariablesConjunction *pMissingVars = NULL;
  plVariablesConjunction *pKnownVars = NULL;
  vector<bool> trueVector(learnDistribVect.get_learn_objects().size(), true);
  const vector<plProbValue> *pCurrentMissingProba;
  plVariablesConjunction_const_iterator vi, vi_end;

  bool message_state = plError::message_is_ignored(50);
  plError::ignore_this_message(50, true);

  this->pDataDesc->rewind();
  idxLine = -1;

  while ((this->pDataDesc->get_data_record(pCurrentLine, pPresentValues)))
  {
    idxLine++;
    if (this->isMissingData(*pPresentValues, searchedVars))
    {
      // 13 second on condition for 5000 MI on 200 samples
      pMissingVars = new plVariablesConjunction();
      pKnownVars = new plVariablesConjunction();
      for (i = 0,
          vi = this->allVariables.begin(),
          vi_end = this->allVariables.end();
           vi != vi_end;
           ++vi, ++i)
      {
        if ((*pPresentValues)[i])
        {
          completeLine[i] = (*pCurrentLine)[i];
          (*pKnownVars) ^= *vi;
        }
        else
        {
          (*pMissingVars) ^= *vi;
        }
      }
      pCurrentMissingProba = this->computeCurrentMissingProbabilities(
          idxLine,
          completeLine,
          *pMissingVars,
          *pKnownVars);
      if (pCurrentMissingProba)
      {
        completeLine.reset(*pMissingVars);
        i = 0;
        do
        {
          learnDistribVect.add_point_if_not_frozen(completeLine,
                                                   trueVector,
                                                   (*pCurrentMissingProba)[i++]);
        } while (completeLine.next(*pMissingVars));
        delete pMissingVars;
        delete pKnownVars;
      }
    }
    else
    {
      for (i = 0; i < n; i++)
        completeLine[i] = (*pCurrentLine)[i];
      // bottleneck > 2k seconds for 5000 MI on 200 samples
      learnDistribVect.add_point_if_not_frozen(completeLine, trueVector);
      // 4 seconds for 5000 MI on 200 samples
    }
  }
  plError::ignore_this_message(50, message_state);

  return new plJointDistribution(learnDistribVect
                                     .get_computable_object_list());
}

//=============================================================================

void pmFrequencyCounter::
    constructSearchedVariableList(const vector<plLearnObject *> &learnObjects,
                                  vector<bool> &searchedVars)
{
  unsigned int n, i;
  plLearnObject *pLearnObj;
  plVariablesConjunction allSearchedVars;
  plVariablesConjunction_const_iterator vi, vi_end;
  bool message_state = plError::message_is_ignored(62);

  plError::ignore_this_message(62, true);
  n = learnObjects.size();
  for (i = 0; i < n; i++)
  {
    const plVariablesConjunction *pVars;
    pLearnObj = learnObjects[i];
    pVars = &pLearnObj->get_variables();
    allSearchedVars ^= (*pVars);
  }
  searchedVars.resize(this->allVariables.dim());
  for (i = 0, vi = this->allVariables.begin(),
      vi_end = this->allVariables.end();
       vi != vi_end;
       ++vi, ++i)
    searchedVars[i] = allSearchedVars.contains(*vi);
  plError::ignore_this_message(62, message_state);
}

//=============================================================================

bool pmFrequencyCounter::
    isMissingData(const vector<bool> &presentValues,
                  const vector<bool> &searchedVars)
{
  unsigned i, n = this->allVariables.dim();
  for (i = 0; i < n; i++)
    if (searchedVars[i] && !presentValues[i])
      return true;
  return false;
}

//=============================================================================
// added by F. Schnitzler, 2nd December

bool pmFrequencyCounter::
    isMissingData(const vector<bool> &presentValues,
                  const std::vector<unsigned int> &SearchedVariablesIndices)
{
  unsigned num_var = 0, nbr_var = SearchedVariablesIndices.size();
  for (num_var = 0; num_var < nbr_var; num_var++)
    if (!presentValues[num_var])
      return true;
  return false;
}

//=============================================================================

const vector<plProbValue> *pmFrequencyCounter::
    computeCurrentMissingProbabilities(long unsigned int idxLine,
                                       const plValues &line,
                                       const plVariablesConjunction &search,
                                       const plVariablesConjunction &known)
{
  unsigned int i, n;
  plProbValue p;
  switch (this->estimationMethod)
  {
  case ProbabilityTable:
    if (this->missing_probability_table)
      return &((*this->missing_probability_table)[idxLine]);
    break;
  case JointDistribution:
    if (this->pJointDistribution)
    {
      this->pJointDistribution->ask(this->jdeCndDistribution, search, known);
      this->jdeCndDistribution.instantiate(this->jdeDistribution, line);
      this->jdeDistribution.compile(this->jdeCompiledDistribution);
      if (!this->jdeCompiledDistribution.is_null())
      {
        this->jdeCompiledDistribution.tabulate(this
                                                   ->currentMissingProbTable);
        return &this->currentMissingProbTable;
      }
      else
      {
      }
    }
    break;

  case Uniform:
    n = (unsigned int)search.cardinality();
    this->currentMissingProbTable.resize(n);
    p = PL_ONE_PROB / (plProbValue)n;
    for (i = 0; i < n; i++)
      this->currentMissingProbTable[i] = p;
    return &this->currentMissingProbTable;
    break;
  case IgnoreLine:
    return NULL;
    break;
  case UniformExtrapolation:
    n = (unsigned int)search.cardinality();
    this->currentMissingProbTable.resize(n);
    for (i = 0; i < n; i++)
      this->currentMissingProbTable[i] = 1.0;
    return &this->currentMissingProbTable;
    break;
  }
  return NULL;
}

void pmFrequencyCounter::updateSW(pmBayesianNetwork bn)
{
  if (this->usingSlidingWindow)
  {
    // cout << "\nFreqCache and FreqOldCache contain before updating" << endl;
    // pmSlidingWindow::Summary();

    cout << "\n\n========UPDATE CACHE and previousBN========\n\n"
         << endl;
    pmSlidingWindow::UpdateCacheAndPrevBN(bn);

    // cout << "\nFreqCache and FreqOldCache will contain now after updating" << endl;
    // pmSlidingWindow::Summary();
  }
  else
  {
    cout << "\n\nUsing SlidingWindow Boolean Variable is false, you can't use the updateSW() function\n\n"
         << endl;
  }
}

void pmFrequencyCounter::printSW_Summary()
{
  if (this->usingSlidingWindow)
  {
    pmSlidingWindow::Summary();

    cout << "pmSlidingWindow::N is ";
    cout << pmSlidingWindow::N << endl;
    cout << "pmSlidingWindow::N_old is ";
    cout << pmSlidingWindow::N_old << endl;
  }
  else
  {
    cout << "\n\nUsing SlidingWindow Boolean Variable is false, you can't use the printSW_Summary() function\n\n"
         << endl;
  }
}
