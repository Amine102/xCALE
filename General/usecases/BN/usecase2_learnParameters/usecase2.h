#include <string>
#include <stdlib.h>
#include <pl.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCSVDataSet.h>
#include "pilgrim/general/scores/ScoreBIC.h"
#include "pilgrim/general/algorithms/AlgoMMHC.h"
#include <plCSVFileDataDescriptor.h>
#include "../noisyMax/Helper_Functions.h"

void usecase2_dataGen();
void usecase2();
void usecase2_incre();
void usecase2_learnObj();
void usecase2_incre_learnObj();