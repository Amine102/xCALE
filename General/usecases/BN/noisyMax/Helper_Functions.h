#include <string>
#include <stdlib.h>
#include <pl.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCSVDataSet.h>
#include "pilgrim/general/scores/ScoreBIC.h"
#include "pilgrim/general/algorithms/AlgoMMHC.h"
#include <plCSVFileDataDescriptor.h>


plValues get_Label_Index(plVariable x, string s);

void split_Data(string data_file, int numberOfParts);
