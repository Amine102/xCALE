#include "../structureLearning/Asia_GS_structureLearningWithMwstInit.h"
#include "../structureLearning/Asia_GS_structureLearningWithRandomInit.h"
#include "../structureLearning/Asia_MWST_structureLearning.h"
#include <boost/graph/adjacency_list.hpp>
#include <string>
#include <stdlib.h>
#include <pl.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCSVDataSet.h>
#include <plCSVFileDataDescriptor.h>
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/scores/Score.h>
#include <pilgrim/general/algorithms/AlgoGS.h>
#include <pilgrim/general/scores/ScoreBIC.h>
#include <pilgrim/general/scores/Score.h>
#include <pilgrim/general/scores/ScoreAIC.h>
#include <pilgrim/general/scores/ScoreBDeu.h>
#include <pilgrim/general/scores/ScoreMDL.h>
#include <pilgrim/general/scores/ScoreQNML.h>
#include <pilgrim/general/algorithms/AlgoGS.h>
#include <pilgrim/general/algorithms/AlgoMWST.h>
#include <pilgrim/general/algorithms/AlgoMMHC.h>
#include <pilgrim/general/scores/ScoreBIC.h>

void immhc();
void immhc1(string path_to_csv);
void immhc2();


