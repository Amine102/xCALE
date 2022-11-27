#include <string>
#include <stdlib.h>
#include <pl.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCSVDataSet.h>
#include "pilgrim/general/scores/ScoreBIC.h"
#include "pilgrim/general/algorithms/AlgoMMHC.h"
#include <plCSVFileDataDescriptor.h>


void splitData(string data_file, int numberOfParts=2);

int GetNumberOfSplitsFromUser();

bool AskUserIfChangingForgetFactor();

double getForgettingFactorFromUser();

plComputableObject getComputableObject(string headSignature, plComputableObjectList compObject_List);

void exportVectorOfProba(plComputableObjectList co_list, string headSignature, int id);