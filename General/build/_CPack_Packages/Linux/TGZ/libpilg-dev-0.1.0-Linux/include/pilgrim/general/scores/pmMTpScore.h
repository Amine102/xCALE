/*==============================================================================
 * Product : PILGRIM
 * File : pmMTpScore.h
 * Author :
 * Creation : January 2021
 * WARINING /!\ : STATUS ON PROGRESS
  *=============================================================================
 * (c) Copyright 2013, PILGRIM - all rights reserved
 *==============================================================================
 */

#ifndef _LoadMTpScore_H
#define _LoadMTpScore_H
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <pilgrim/general/pmBayesianNetwork.h>
#include "pilgrim/general/pmMTCSVDataSet.h"
#include "plVariablesConjunction.h"
#include <pilgrim/general/scores/ScoreBIC.h>
#include <pilgrim/general/scores/Score.h>
#include <pilgrim/general/scores/ScoreAIC.h>
#include <pilgrim/general/scores/ScoreBDeu.h>
#include <pilgrim/general/scores/ScoreMDL.h>
#include <pilgrim/general/scores/ScoreQNML.h>
#include <pilgrim/general/pmFrequencyCounter.h>


namespace PILGRIM {
	class pmMTpScore {
	public:
		/*=====Attributs=====*/
        std::vector<pmDecomposableScore<rowDataType> *> all_pScores; // vector to store all Scores
		std::vector<pmBayesianNetwork*> all_bn_vec; // vector of all Bns that are attached to each score
		std::vector<pmFrequencyCounter *> all_fc_vec; // vector of all frequency counters
		std::vector<pmCache *> all_cache; // all cache
        std::vector<plVariablesConjunction> all_plvars;

		/*
		=====CONSTRUCTORS=====
		*/
		pmMTpScore(pmMTCSVDataSet mt_data, std::string score_type);
		/*
		=====DESTRUCTOR=====
		*/
		//~pmMTpScore(void);
		/*
		=====PUSHERS=====
		*/
		// Push all objects concerning pScore
		void push_mtScores(int task_id, pmMTCSVDataSet& mt_data, std::string score_type);
		/*
		=====GETTERS=====
		*/
		/*
		=====PRINTERS=====
		*/
		/*
		=====UTILS=====
		*/
		/*
		=====PROTECTED ATTRIBUTES AND INIT METHOD====
		*/
	};
};
#endif
