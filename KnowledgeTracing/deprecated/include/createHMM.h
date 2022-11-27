#pragma once
#include <string>
#include <stdlib.h>
#include <pl.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmDynamicBayesianNetwork.h>
#include <vector>
#include <utility>

typedef double ObservationElement_t;
typedef std::vector < ObservationElement_t > Observation_t;
typedef std::vector < Observation_t > ObservationSequence_t;
typedef unsigned int State_t;
typedef std::vector< State_t > StateSequence_t;
typedef std::vector<plHMM::ObservationSequence_t> ObservationSequenceArray_t;
typedef plFloat ObservationElement_t;



void split(std::string file_name);
void vectWrite( std::string file_name,
               	ObservationSequence_t& observationSequence_T
                );
void vectWriteBis(std::string file_name,
                  ObservationSequenceArray_t& observation_sequences
                	);
void displayVect( ObservationSequence_t& observationSequence_T,
                  StateSequence_t& stateSequence_T
                  );
void displayVectBis(ObservationSequenceArray_t& observation_sequences
									);
void createHMM();
void rewrite(std::string file_name);
