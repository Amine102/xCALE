//
// Created by couland-q on 13/04/2022.
// This (big) usecase is an example about how to make a progressive BKT
// for exercise recommendation and skill progression assessment between exercises.
//

#ifndef PILGRIM_GENERAL_PROGRESSIVEBKT_H
#define PILGRIM_GENERAL_PROGRESSIVEBKT_H

#include <string>
#include <cstdlib>
#include <algorithm> // std::any_of in BKTParams
#include <random>

#include <pl.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmDynamicBayesianNetwork.h>
#include <pilgrim/general/pm15dBN.h>

#include "../../core/include/tools.h"

namespace UsecaseProgressiveBKT {
    #define MY_DEBUG false

    enum class AnswersPrediction {
        None, Optimistic
    };
    enum class Metric {
        PG_min, PG_mean
    };

    struct PGscore {
        // -1.0 means it hasn't been set
        double pgmin = -1.0;
        double pgmean = -1.0;
        std::string exo_name = "NAME NOT SET";

        void print() const {
            std::cout << "Exercise \"" << exo_name << "\": " << std::endl;
            std::cout << "PGmin = " << pgmin;
            std::cout << " / PGmean = " << pgmean << std::endl;
        }

        void printPGmin() const {
            std::cout << pgmin << ",";
        }

        void printPGmean() const {
            std::cout << pgmean << ",";
        }

        bool isEmpty() {
            return (pgmin < 0.0 && pgmean < 0.0);
        }
    };

    struct Exercise {
        std::vector<bool> comps;
        std::vector<int> res;
        std::string name;

        Exercise() {};

        Exercise(const std::string& name) : name(name) {};

        Exercise(const std::string& name, const std::vector<bool>&comps) : name(name), comps(comps) {};

        Exercise(const std::string& name, const std::vector<bool>& comps, const std::vector<int>& res) : 
            name(name), comps(comps), res(res) {};

        Exercise(unsigned int idx_comp) {
            for (size_t i = 0; i < idx_comp - 1; i++) {
                comps.push_back(false);
            }

            comps.push_back(true);
        }

        bool hasSameComp(Exercise& other_e) const {
            return (comps == other_e.comps);
        }

        bool hasComp(unsigned int idx) {
            if (comps.size() < idx) {
                return false;
            }

            return comps[idx];
        }

        void print() const {
            std::cout << name << " ";
            for (bool comp: comps) {
                std::cout << (!comp ? "0 " : " 1");
            }

            std::cout << std::endl;
        }

    };

    // TODO: continue this lol
    struct BKTResults {
        size_t nb_exos;
        std::vector<PGscore> all_pg_scores;
        std::vector<std::string> skills_names;
        std::vector<float> skills_vals;
        std::vector<float> skills_t_vals;
    };

    /** Main function used to create and run the BKT with specific parameters. **/
    void createAndQueryProgressiveBKT();

    /** Creates the BKT. **/
    pmDynamicBayesianNetwork* createProgressiveBKT();


    /** Makes successive queries to the BKT. **/
    Tools::Time::Times progressiveQuery(pmDynamicBayesianNetwork* DBN, std::vector<Exercise>& exercises,
                                        std::vector<std::string>& variables_to_query,
                                        AnswersPrediction answersPrediction,
                                        Metric metric, bool spreadsheet_display = false, bool time_it = false);


    /** First query of the BKT (uses the G_init graph). **/
    std::pair<PGscore, Exercise> firstStep(pmDynamicBayesianNetwork* DBN, std::vector<Exercise>& exo_vector,
                                           std::vector<std::string>& variables_to_query_t0,
                                           std::vector<std::string>& masteries,
                                           AnswersPrediction answersPrediction, Metric metric, bool spreadsheet_display,
                                           Tools::Time::Times& times);

    /** Queries the 1.5 DBN for every exercise in the vector. **/
    std::pair<PGscore, Exercise> nextStep(pm15dBN* DBN15, pmDynamicBayesianNetwork* DBN, size_t nb_loops,
                                          std::vector<std::string>& variables_to_query,
                                          std::vector<plVariablesConjunction>& all_targets, std::vector<int>& instants,
                                          std::vector<plValues>& all_evidences, std::vector<Exercise>& exo_vector,
                                          std::vector<std::string>& masteries, AnswersPrediction answersPrediction,
                                          Metric metric, bool spreadsheet_display, Tools::Time::Times& times);


    /** Returns the PG scores (PGmean and PGmin) from a set of plDistribution. **/
    PGscore getPGscore(std::vector<plDistribution>& variables, std::vector<Exercise>& exo_vector);

    /** Returns the PG scores (PGmean and PGmin) from the result of a query on the BKT. **/
    PGscore getPGscore(std::vector<plComputableObjectList>& results, std::vector<plVariablesConjunction>& all_targets,
                       std::vector<Exercise>& exo_vector);

    /** Returns the PGmean from a set of plDistribution. **/
    double getPGmean(std::vector<plDistribution>& variables);

    /** Returns the PGmean from the result of a query on the BKT. **/
    double getPGmean(std::vector<plComputableObjectList>& results, std::vector<plVariablesConjunction>& all_targets);

    /** Returns the PGmin from a set of plDistribution. **/
    double getPGmin(std::vector<plDistribution>& variables, std::vector<Exercise>& exo_vector);

    /** Returns the PGmin from the result of a query on the BKT. **/
    double getPGmin(std::vector<plComputableObjectList>& results, std::vector<plVariablesConjunction>& all_targets,
                    std::vector<Exercise>& exo_vector);


    /** Returns the T0 evidences, depending on the chosen predictor. **/
    plValues getT0Evidences(Exercise& exo, pmBayesianNetwork& g_init, std::vector<std::string>& variables_to_query_t0,
                            std::vector<std::string>& masteries, AnswersPrediction answersPrediction,
                            bool triggers = false);

    /** Returns the T0 evidences, from the student's true answers. **/
    plValues
    getTrueT0Evidences(Exercise& exo, pmBayesianNetwork& g_init, std::vector<std::string>& variables_with_evidences,
                       std::vector<std::string>& masteries, bool triggers);

    /** Returns a variable with a specified name from a plVariable vector. **/
    plVariable &getVariableWithName(const std::string& name, std::vector<plVariable>& variables);

    /** Returns a vector of variables from all specified names from a plVariable vector. **/
    std::vector<plVariable> getVariablesWithName(std::vector<std::string>& names, plVariablesConjunction& all_variables,
                                                 const std::string& prefix = "", bool is_t0 = false, bool get_next_t = false);

    std::vector<std::string> getVariablesNamesWithName(std::vector<std::string>& names, plVariablesConjunction& all_variables,
                                                       const std::string&prefix = "", bool is_t0 = false, bool get_next_t = false);

    /** Returns a vector of exercises (specific to student 1) **/
    std::vector<Exercise> getStudent1Exercises();

    /** Returns a vector of exercises (specific to student 2) **/
    std::vector<Exercise> getStudent2Exercises();

    /** Returns a vector of exercises (specific to student 3) **/
    std::vector<Exercise> getStudent3Exercises();

    /** Removes an exercise from the exercises vector. **/
    void removeExerciseFromVector(Exercise& exo_to_remove, std::vector<Exercise>& exo_vector);

    /** Returns a vector of exercises without any duplicates regarding the used skills (optimisation for the predition phase). **/
    std::vector<Exercise> getExercisesWithoutRepetition(std::vector<Exercise>& original_exo_vector);

    Exercise &getExerciseWithSameSkills(Exercise& exo, std::vector<Exercise>& exo_vector, bool random = false);

    /** Returns a vector indicating which skill is present or not in the exercises left (used for the new PGmin metric). **/
    bool isSkillStillInExercises(size_t skill_idx, std::vector<Exercise>& exo_vector);

    /** Prints the results for a BKT step. **/
    void printProgressiveBKTResult(std::vector<plComputableObjectList>& results, std::vector<int>& instants,
                                   std::vector<plVariablesConjunction>& all_targets, Exercise& exo,
                                   bool formatted_for_spreadsheet = true, bool only_true = false);

    /** Prints the results in a convenient way to copy/paste them into a spreadsheet. **/
    void
    printForSpreadsheet(std::vector<plComputableObjectList>& results, std::vector<plVariablesConjunction>& all_targets,
                        Exercise& exo, std::vector<PGscore>& all_pg_scores);

    void printForSpreadsheetOnlyValues(std::vector<plComputableObjectList>& results,
                                       std::vector<plVariablesConjunction>& all_targets,
                                       std::vector<PGscore>& all_pg_scores, Metric metric);
}
#endif // PILGRIM_GENERAL_PROGRESSIVEBKT_H
