//
// Created by couland-q on 13/04/2022.
//

#include "../include/progressiveBKT.h"

namespace UsecaseProgressiveBKT {

    void createAndQueryProgressiveBKT() {
        plError::always_display_warning(false);

        pmDynamicBayesianNetwork *DBN = createProgressiveBKT();
        //DBN->summary();
        std::vector<Exercise> exercises = getStudent1Exercises();

        std::vector<std::string> variables_to_query;
        variables_to_query.emplace_back("nombres");
        variables_to_query.emplace_back("calcul_litteral");
        variables_to_query.emplace_back("demarche_scientifique");

        //progressiveQuery(DBN, exercises, variables_to_query, AnswersPrediction::None, Metric::PG_mean);
        progressiveQuery(DBN, exercises, variables_to_query, AnswersPrediction::Optimistic, Metric::PG_mean, true);
        //progressiveQuery(DBN, exercises, variables_to_query, AnswersPrediction::None, Metric::PG_min);
        //progressiveQuery(DBN, exercises, variables_to_query, AnswersPrediction::Optimistic, Metric::PG_min);

        delete DBN;
    }

    pmDynamicBayesianNetwork* createProgressiveBKT() {
        // A = nombres, B = calcul littéral, C = démarche scientifique
        //User-defined parameters
        double PA_INITIAL = 0.05;
        double PB_INITIAL = 0.02;
        double PC_INITIAL = 0.01;

        // All P_GUESS are the same
        double P_GUESS = 0.1;

        // All P_SLIP are the same
        double P_SLIP = 0.05;

        // All P_LEARN are the same
        double P_LEARN = 0.1;

        // All P_forget are 0 for now
        double PA_FORGET = 0;
        double PB_FORGET = 0;
        double PC_FORGET = 0;

        const std::vector<std::string> niveau_maitrise = {"1 - non acquis", "2 - acquis"};

        plVariable A_t0("Skill_nombres_t0", plLabelType(niveau_maitrise));
        plVariable B_t0("Skill_calcul_litteral_t0", plLabelType(niveau_maitrise));
        plVariable C_t0("Skill_demarche_scientifique_t0", plLabelType(niveau_maitrise));
        plVariable ResA_t0("Res_nombres_t0", plLabelType(niveau_maitrise));
        plVariable ResB_t0("Res_calcul_litteral_t0", plLabelType(niveau_maitrise));
        plVariable ResC_t0("Res_demarche_scientifique_t0", plLabelType(niveau_maitrise));

        // If we do not add these, DBN1.5 constructor fails
        plVariable Trigger_A_t0("Trigger_nombres_t0", PL_BINARY_TYPE);
        plVariable Trigger_B_t0("Trigger_calcul_litteral_t0", PL_BINARY_TYPE);
        plVariable Trigger_C_t0("Trigger_demarche_scientifique_t0", PL_BINARY_TYPE);
        plProbValue table_trigger_A_t0[] = {0.5, 0.5};
        plProbTable P_trigger_A_t0(Trigger_A_t0, table_trigger_A_t0);
        plProbValue table_trigger_B_t0[] = {0.5, 0.5};
        plProbTable P_trigger_B_t0(Trigger_B_t0, table_trigger_B_t0);
        plProbValue table_trigger_C_t0[] = {0.5, 0.5};
        plProbTable P_trigger_C_t0(Trigger_C_t0, table_trigger_C_t0);

        plProbValue tableA_t0[] = {1 - PA_INITIAL, PA_INITIAL};
        plProbTable P_A_t0(A_t0, tableA_t0);

        plProbValue tableB_t0[] = {1 - PB_INITIAL, PB_INITIAL};
        plProbTable P_B_t0(B_t0, tableB_t0);

        plProbValue tableC_t0[] = {1 - PC_INITIAL, PC_INITIAL};
        plProbTable P_C_t0(C_t0, tableC_t0);

        plProbValue tableResA_t0_knowingA_t0[] = {
                1 - P_GUESS, P_GUESS,
                P_SLIP, 1 - P_SLIP
        };

        plDistributionTable P_ResA_t0(ResA_t0, A_t0, tableResA_t0_knowingA_t0);

        plProbValue tableResB_t0_knowingB_t0[] = {
                1 - P_GUESS, P_GUESS,
                P_SLIP, 1 - P_SLIP
        };

        plDistributionTable P_ResB_t0(ResB_t0, B_t0, tableResB_t0_knowingB_t0);

        plProbValue tableResC_t0_knowingC_t0[] = {
                1 - P_GUESS, P_GUESS,
                P_SLIP, 1 - P_SLIP
        };

        plDistributionTable P_ResC_t0(ResC_t0, C_t0, tableResC_t0_knowingC_t0);

        // Joint distribution
        plJointDistribution jd0(
                A_t0 ^ B_t0 ^ C_t0 ^ ResA_t0 ^ ResB_t0 ^ ResC_t0 ^ Trigger_A_t0 ^ Trigger_B_t0 ^ Trigger_C_t0,
                P_A_t0 * P_B_t0 * P_C_t0 * P_ResA_t0 * P_ResB_t0 * P_ResC_t0 * P_trigger_A_t0 * P_trigger_B_t0 *
                P_trigger_C_t0);


        //Transition Bayesian Network

        plVariable A("Skill_nombres", plLabelType(niveau_maitrise));
        plVariable B("Skill_calcul_litteral", plLabelType(niveau_maitrise));
        plVariable C("Skill_demarche_scientifique", plLabelType(niveau_maitrise));
        plVariable ResA("Res_nombres", plLabelType(niveau_maitrise));
        plVariable ResB("Res_calcul_litteral", plLabelType(niveau_maitrise));
        plVariable ResC("Res_demarche_scientifique", plLabelType(niveau_maitrise));

        plVariable A_t("Skill_nombres_t", plLabelType(niveau_maitrise));
        plVariable B_t("Skill_calcul_litteral_t", plLabelType(niveau_maitrise));
        plVariable C_t("Skill_demarche_scientifique_t", plLabelType(niveau_maitrise));
        plVariable ResA_t("Res_nombres_t", plLabelType(niveau_maitrise));
        plVariable ResB_t("Res_calcul_litteral_t", plLabelType(niveau_maitrise));
        plVariable ResC_t("Res_demarche_scientifique_t", plLabelType(niveau_maitrise));

        plVariable Trigger_A("Trigger_nombres", PL_BINARY_TYPE);
        plVariable Trigger_B("Trigger_calcul_litteral", PL_BINARY_TYPE);
        plVariable Trigger_C("Trigger_demarche_scientifique", PL_BINARY_TYPE);
        /*plVariable Trigger_A_t("Trigger skill_nombres_t", PL_BINARY_TYPE);
        plVariable Trigger_B_t("Trigger skill_calcul_litteral_t", PL_BINARY_TYPE);
        plVariable Trigger_C_t("Trigger skill_demarche_scientifique_t", PL_BINARY_TYPE);*/

        plProbValue tableA[] = {1 - PA_INITIAL, PA_INITIAL};
        plProbTable P_A(A, tableA);

        plProbValue tableB[] = {1 - PB_INITIAL, PB_INITIAL};
        plProbTable P_B(B, tableB);

        plProbValue tableC[] = {1 - PC_INITIAL, PC_INITIAL};
        plProbTable P_C(C, tableC);

        plProbValue tableResA_knowingA[] = {
                1 - P_GUESS, P_GUESS,
                P_SLIP, 1 - P_SLIP
        };


        plDistributionTable P_ResA(ResA, A, tableResA_knowingA);

        plProbValue tableResB_knowingB[] = {
                1 - P_GUESS, P_GUESS,
                P_SLIP, 1 - P_SLIP
        };


        plDistributionTable P_ResB(ResB, B, tableResB_knowingB);

        plProbValue tableResC_knowingC[] = {
                1 - P_GUESS, P_GUESS,
                P_SLIP, 1 - P_SLIP
        };


        plDistributionTable P_ResC(ResC, C, tableResC_knowingC);

        // Probability table of table A_t
        plProbValue tableA_t_knowingA[] = {
                1 - P_LEARN, P_LEARN,
                PA_FORGET, 1 - PA_FORGET
        };

        plProbValue tableA_t_knowingA_none[] = {
                1, 0,
                0, 1
        };

        // Left, Right, right_index
        plDistributionTable P_A_t(A_t, Trigger_A ^ A, Trigger_A);
        P_A_t.push(plDistributionTable(A_t, A, tableA_t_knowingA_none), 0);
        P_A_t.push(plDistributionTable(A_t, A, tableA_t_knowingA), 1);

        // Probability table of table B_t
        plProbValue tableB_t_knowingB[] = {
                1 - P_LEARN, P_LEARN,
                PB_FORGET, 1 - PB_FORGET
        };

        plProbValue tableB_t_knowingB_none[] = {
                1, 0,
                0, 1
        };

        plDistributionTable P_B_t(B_t, Trigger_B ^ B, Trigger_B);
        P_B_t.push(plDistributionTable(B_t, B, tableB_t_knowingB_none), 0);
        P_B_t.push(plDistributionTable(B_t, B, tableB_t_knowingB), 1);

        // Probability table of table C_t
        plProbValue tableC_t_knowingC[] = {
                1 - P_LEARN, P_LEARN,
                PC_FORGET, 1 - PC_FORGET
        };

        plProbValue tableC_t_knowingC_none[] = {
                1, 0,
                0, 1
        };

        plDistributionTable P_C_t(C_t, Trigger_C ^ C, Trigger_C);
        P_C_t.push(plDistributionTable(C_t, C, tableC_t_knowingC_none), 0);
        P_C_t.push(plDistributionTable(C_t, C, tableC_t_knowingC), 1);

        plProbValue table_trigger_A[] = {0.5, 0.5};
        plProbTable P_trigger_A(Trigger_A, table_trigger_A);
        plProbValue table_trigger_B[] = {0.5, 0.5};
        plProbTable P_trigger_B(Trigger_B, table_trigger_B);
        plProbValue table_trigger_C[] = {0.5, 0.5};
        plProbTable P_trigger_C(Trigger_C, table_trigger_C);

        plProbValue tableResA_t_knowingA_t[] = {
                1 - P_GUESS, P_GUESS,
                P_SLIP, 1 - P_SLIP
        };

        plDistributionTable P_ResA_t(ResA_t, A_t, tableResA_t_knowingA_t);

        plProbValue tableResB_t_knowingB_t[] = {
                1 - P_GUESS, P_GUESS,
                P_SLIP, 1 - P_SLIP
        };


        plDistributionTable P_ResB_t(ResB_t, B_t, tableResB_t_knowingB_t);

        plProbValue tableResC_t_knowingC_t[] = {
                1 - P_GUESS, P_GUESS,
                P_SLIP, 1 - P_SLIP
        };


        plDistributionTable P_ResC_t(ResC_t, C_t, tableResC_t_knowingC_t);

        plVariable Trigger_A_t("Trigger_nombres_t", PL_BINARY_TYPE);
        plVariable Trigger_B_t("Trigger_calcul_litteral_t", PL_BINARY_TYPE);
        plVariable Trigger_C_t("Trigger_demarche_scientifique_t", PL_BINARY_TYPE);
        plProbValue table_trigger_A_t[] = {0.5, 0.5};
        plProbTable P_trigger_A_t(Trigger_A_t, table_trigger_A_t);
        plProbValue table_trigger_B_t[] = {0.5, 0.5};
        plProbTable P_trigger_B_t(Trigger_B_t, table_trigger_B_t);
        plProbValue table_trigger_C_t[] = {0.5, 0.5};
        plProbTable P_trigger_C_t(Trigger_C_t, table_trigger_C_t);

        // Joint distribution
        plJointDistribution jdt(A ^ Trigger_A ^ B ^ Trigger_B ^ C ^ Trigger_C ^
                                ResA ^ ResB ^ ResC ^
                                A_t ^ B_t ^ C_t ^ ResA_t ^ ResB_t ^ ResC_t ^
                                Trigger_A_t ^ Trigger_B_t ^ Trigger_C_t,
                                P_A * P_trigger_A * P_B * P_trigger_B * P_C * P_trigger_C *
                                P_ResA * P_ResB * P_ResC *
                                P_A_t * P_B_t * P_C_t * P_ResA_t * P_ResB_t * P_ResC_t *
                                P_trigger_A_t * P_trigger_B_t * P_trigger_C_t);

        //Creation and printing of the bayesian network
        auto *BN_0 = new pmBayesianNetwork(jd0);
        auto *BN_Transition = new pmBayesianNetwork(jdt);
        auto *DBN = new pmDynamicBayesianNetwork(BN_0, BN_Transition);

        return DBN;
    }

    Tools::Time::Times progressiveQuery(pmDynamicBayesianNetwork* DBN, std::vector<Exercise>& exercises,
                                        std::vector<std::string>& variables_to_query,
                                        AnswersPrediction answersPrediction,
                                        Metric metric, bool spreadsheet_display, bool time_it) {

        if (answersPrediction == AnswersPrediction::Optimistic) {
            std::cout << "Answer prediction is OPTIMISTIC (always good answer(s) for potential next exercise)."
                      << std::endl;
        } else if (answersPrediction == AnswersPrediction::None) {
            std::cout << "Answer prediction is NONE (no evidence for potential next exercise)." << std::endl;
        }

        if (metric == Metric::PG_mean) {
            std::cout << "Metric is PG_Mean (the mean value of all skills)." << std::endl;
        } else if (metric == Metric::PG_min) {
            std::cout << "Metric is PG_min (the min value of all skills)." << std::endl;
        }

        const unsigned int nb_slices = exercises.size();
        std::vector<std::string> niveau_maitrise;

        niveau_maitrise.emplace_back("1 - non acquis");
        niveau_maitrise.emplace_back("2 - acquis");

        std::vector<plValues> all_evidences;
        std::vector<int> instants;
        std::vector<plVariablesConjunction> all_targets;

        pm15dBN *dbn15;
        bool dbn15_has_been_instanciated = false;

        plValues evidences_t0;

        std::pair<PGscore, Exercise> pgmin_mean_and_exo;

        Tools::Time::Times times;

        size_t i = 0;

        // nb_slices = nb_exercises, so == while there are still exercises to do
        // For every new exercise to test
        while (!exercises.empty()) {

            // The rest happens here
            // exercises will come back with one less exercise
            // If it's t0, query Ginit
            if (i == 0) {
                pgmin_mean_and_exo = firstStep(DBN, exercises, variables_to_query, niveau_maitrise,
                                               answersPrediction, metric, spreadsheet_display, times);

                // Needed to seed the DBN 1.5 with t0 evidences
                evidences_t0 = getTrueT0Evidences(pgmin_mean_and_exo.second, DBN->getInitial_G(), variables_to_query,
                                                  niveau_maitrise, true);
            } else {
                // Only instantiate it once
                if (!dbn15_has_been_instanciated) {
                    times.startTimer("update_model_0");
                    dbn15 = new pm15dBN(DBN, evidences_t0);
                    times.stopTimer("update_model_0");
                    dbn15_has_been_instanciated = true;
                    //dbn15->summary_1_5();
                }

                instants.clear();
                instants.emplace_back(i);

                if (!spreadsheet_display) {
                    std::cout << "---------------" << std::endl;
                    std::cout << "New instant: " << i << std::endl;
                    std::cout << "---------------" << std::endl;
                }

                pgmin_mean_and_exo = nextStep(dbn15, DBN, i, variables_to_query, all_targets, instants, all_evidences,
                                              exercises, niveau_maitrise, answersPrediction, metric,
                                              spreadsheet_display,
                                              times);
            }

            removeExerciseFromVector(pgmin_mean_and_exo.second, exercises);
            i++;
        }

        delete dbn15;

        return times;
    }


    std::pair<PGscore, Exercise> firstStep(pmDynamicBayesianNetwork* DBN, std::vector<Exercise>& exo_vector,
                                           std::vector<std::string>& variables_to_query_t0,
                                           std::vector<std::string>& masteries,
                                           AnswersPrediction answersPrediction, Metric metric, bool spreadsheet_display,
                                           Tools::Time::Times& times) {

        std::pair<PGscore, Exercise> pgmin_mean_and_exercise;
        PGscore current_pg_score;

        pmBayesianNetwork g_init = DBN->getInitial_G();
        plVariablesConjunction variables_init = g_init.getVariables();

        std::vector<plVariable> skills_t0 = getVariablesWithName(variables_to_query_t0, variables_init, "Skill_", true,
                                                                 false);
        std::vector<plVariable> res_t0 = getVariablesWithName(variables_to_query_t0, variables_init, "Res_", true,
                                                              false);

        std::vector<plDistribution> best_result_of_query;
        best_result_of_query.resize(res_t0.size());

        //std::vector<Exercise> reduced_exercises_vector = getExercisesWithoutRepetition(exo_vector);

        times.startTimer("reco_exo_0");
        for (Exercise exo: exo_vector) {
            if (MY_DEBUG) {
                std::cout << "----------NEW EXO----------" << std::endl;
                std::cout << "Skills: ";
                for (size_t i = 0; i < exo.comps.size(); i++) {
                    unsigned char letter = static_cast<unsigned char>(i) + 65;
                    std::cout << " " << letter << ": " << (exo.comps[i] ? "Yes" : "No");
                }

                std::cout << std::endl;
            }


            plValues evidences_t0 = getT0Evidences(exo, g_init, variables_to_query_t0, masteries, answersPrediction, true);

            std::vector<plDistribution> result_of_query;
            result_of_query.resize(res_t0.size());

            if (evidences_t0.size() != 0) {
                for (size_t i = 0; i < variables_to_query_t0.size(); i++) {
                    result_of_query[i] = g_init.query(skills_t0[i], evidences_t0);
                }
            } else {
                for (size_t i = 0; i < variables_to_query_t0.size(); i++) {
                    result_of_query[i] = g_init.query(skills_t0[i]);
                }
            }

            current_pg_score = getPGscore(result_of_query, exo_vector);
            current_pg_score.exo_name = exo.name;

            if (metric == Metric::PG_mean) {
                current_pg_score.printPGmean();
            } else if (metric == Metric::PG_min) {
                current_pg_score.printPGmin();
            }


            if (MY_DEBUG) {
                current_pg_score.print();
            }


            if (metric == Metric::PG_mean) {
                if (current_pg_score.pgmean > pgmin_mean_and_exercise.first.pgmean) {
                    pgmin_mean_and_exercise.first.pgmean = current_pg_score.pgmean;
                    pgmin_mean_and_exercise.second = exo;
                    pgmin_mean_and_exercise.first.exo_name = exo.name;

                    for (size_t i = 0; i < result_of_query.size(); i++) {
                        best_result_of_query[i] = result_of_query[i];
                    }
                }
            } else if (metric == Metric::PG_min) {
                if (current_pg_score.pgmin > pgmin_mean_and_exercise.first.pgmin) {
                    pgmin_mean_and_exercise.first.pgmin = current_pg_score.pgmin;
                    pgmin_mean_and_exercise.second = exo;
                    pgmin_mean_and_exercise.first.exo_name = exo.name;

                    for (size_t i = 0; i < result_of_query.size(); i++) {
                        best_result_of_query[i] = result_of_query[i];
                    }
                }
            }


            if (MY_DEBUG) {
                if (metric == Metric::PG_mean) {
                    std::cout << "Exercise " << exo.name << " got pgmean = " << current_pg_score.pgmean << std::endl
                              << std::endl;
                } else if (metric == Metric::PG_min) {
                    std::cout << "Exercise " << exo.name << " got pgmin = " << current_pg_score.pgmin << std::endl
                              << std::endl;
                }

            }

        }

        if (MY_DEBUG) {
            if (metric == Metric::PG_mean) {
                std::cout << "First exercise to do: " << pgmin_mean_and_exercise.second.name
                          << " (PGmean = " << pgmin_mean_and_exercise.first.pgmean << ")" << std::endl;
            } else if (metric == Metric::PG_min) {
                std::cout << "First exercise to do: " << pgmin_mean_and_exercise.second.name
                          << " (PGmin = " << pgmin_mean_and_exercise.first.pgmin << ")" << std::endl;
            }
        }

        times.stopTimer("reco_exo_0");

        std::vector<std::vector<std::pair<plValues, plProbValue>>> all_res_final;
        all_res_final.resize(res_t0.size());

        for (size_t i = 0; i < res_t0.size(); i++) {
            best_result_of_query[i].sorted_tabulate(all_res_final[i]);
        }
        //pgmin_mean_and_exercise.second = getExerciseWithSameSkills(pgmin_mean_and_exercise.second, exo_vector, true);

        std::cout << std::endl << pgmin_mean_and_exercise.second.name << std::endl;

        for (auto &res: all_res_final) {
            std::cout << res[1].second << ",";
        }
        std::cout << std::endl << std::endl << std::endl;

        // Timing " estimation des compétences "
        times.startTimer("estim_comps_0");
        for (size_t i = 0; i < variables_to_query_t0.size(); i++) {
            g_init.query(skills_t0[i]);
        }
        times.stopTimer("estim_comps_0");

        return pgmin_mean_and_exercise;
    }

    std::pair<PGscore, Exercise> nextStep(pm15dBN* DBN15, pmDynamicBayesianNetwork* DBN, size_t nb_loops,
                                          std::vector<std::string>& variables_to_query,
                                          std::vector<plVariablesConjunction>& all_targets, std::vector<int>& instants,
                                          std::vector<plValues>& all_evidences, std::vector<Exercise>& exo_vector,
                                          std::vector<std::string>& masteries, AnswersPrediction answersPrediction,
                                          Metric metric, bool spreadsheet_display, Tools::Time::Times& times) {

        std::pair<PGscore, Exercise> pgmin_mean_and_exercise;
        PGscore current_pg_scores;
        plValues evidence_to_keep;
        std::vector<PGscore> all_pg_scores;
        plComputableObjectList res_query;
        plComputableObjectList res_query_to_keep;

        pmBayesianNetwork gt = DBN->getTransition_G();
        plVariablesConjunction variables_t = gt.getVariables();
        std::vector<plVariable> vars_skills = getVariablesWithName(variables_to_query, variables_t, "Skill_", false,
                                                                   false);
        std::vector<plVariable> vars_skills_t = getVariablesWithName(variables_to_query, variables_t, "Skill_", false,
                                                                     true);
        std::vector<plVariable> vars_res_t = getVariablesWithName(variables_to_query, variables_t, "Res_", false, true);
        std::vector<plVariable> vars_trigger = getVariablesWithName(variables_to_query, variables_t, "Trigger_", false,
                                                                    false);

        //std::vector<Exercise> reduced_exercises_vector = getExercisesWithoutRepetition(exo_vector);

        all_targets.clear();
        plVariablesConjunction new_target;
        for (auto &target: vars_skills) {
            new_target.push_back(target);
        }

        for (auto &target: vars_skills_t) {
            new_target.push_back(target);
        }

        all_targets.push_back(new_target);



        // Test each exercise
        std::string timer_name_reco = "reco_exo_" + std::to_string(nb_loops);
        times.startTimer(timer_name_reco);
        for (Exercise exo: exo_vector) {

            plValues new_evidences;

            // Size is the same for skills, res and triggers
            for (size_t i = 0; i < vars_res_t.size(); i++) {
                if (answersPrediction == AnswersPrediction::Optimistic && exo.comps[i]) {
                    new_evidences = new_evidences.add(vars_res_t[i], masteries[1]);

                    if (MY_DEBUG) {
                        std::cout << "new_evidences[" << vars_res_t[i].name() << "] = " << new_evidences[vars_res_t[i]]
                                  << std::endl;
                    }
                }

                new_evidences = new_evidences.add(vars_trigger[i], exo.comps[i]);

                if (MY_DEBUG) {
                    std::cout << "Trigger " << vars_trigger[i].name() << "] = " << new_evidences[vars_trigger[i]]
                              << std::endl;
                }
            }

            std::vector<plComputableObjectList> results;

            /*std::string timer_name = "query_" + std::to_string(nb_loops);

            times.startTimer(timer_name);*/
            for (auto &target: all_targets) {
                res_query = DBN15->query_JD(target, new_evidences);
                results.emplace_back(res_query);
            }
            //times.stopTimer(timer_name);

            current_pg_scores = getPGscore(results, all_targets, exo_vector);
            all_pg_scores.push_back(current_pg_scores);
            current_pg_scores.exo_name = exo.name;

            if (metric == Metric::PG_mean) {
                current_pg_scores.printPGmean();
            } else if (metric == Metric::PG_min) {
                current_pg_scores.printPGmin();
            }

            if (MY_DEBUG) {
                current_pg_scores.print();
            }

            if (metric == Metric::PG_mean) {
                if (current_pg_scores.pgmean > pgmin_mean_and_exercise.first.pgmean) {
                    pgmin_mean_and_exercise.first.pgmean = current_pg_scores.pgmean;
                    pgmin_mean_and_exercise.second = exo;
                    pgmin_mean_and_exercise.first.exo_name = exo.name;
                    evidence_to_keep = new_evidences;
                    res_query_to_keep = res_query;
                }
            } else if (metric == Metric::PG_min) {
                if (current_pg_scores.pgmin > pgmin_mean_and_exercise.first.pgmin) {
                    pgmin_mean_and_exercise.first.pgmin = current_pg_scores.pgmin;
                    pgmin_mean_and_exercise.second = exo;
                    pgmin_mean_and_exercise.first.exo_name = exo.name;
                    evidence_to_keep = new_evidences;
                    res_query_to_keep = res_query;
                }
            }
        }
        times.stopTimer(timer_name_reco);

        //pgmin_mean_and_exercise.second = getExerciseWithSameSkills(pgmin_mean_and_exercise.second, exo_vector, true);


        if (!spreadsheet_display) {
            if (metric == Metric::PG_mean) {
                std::cout << "Exercise chosen: " << pgmin_mean_and_exercise.second.name << " (PGMean = "
                          << pgmin_mean_and_exercise.first.pgmean << ")" << std::endl;
            } else if (metric == Metric::PG_min) {
                std::cout << "Exercise chosen: " << pgmin_mean_and_exercise.second.name << " (PGMin = "
                          << pgmin_mean_and_exercise.first.pgmin << ")" << std::endl;
            }
        }

        all_evidences.push_back(evidence_to_keep);

        //std::vector<plComputableObjectList> best_res = DBN15->predict(all_targets, instants, all_evidences);

        std::string timer_name_update = "update_model_" + std::to_string(nb_loops);
        times.startTimer(timer_name_update);
        DBN15->updateGT(evidence_to_keep);
        times.stopTimer(timer_name_update);

        std::vector<plComputableObjectList> yo{res_query};
        if (spreadsheet_display) {
            printForSpreadsheet(yo, all_targets, pgmin_mean_and_exercise.second, all_pg_scores);
            //printForSpreadsheetOnlyValues(best_res, all_targets, all_pg_scores, metric);
        } else {
            //printProgressiveBKTResult(best_res, instants, all_targets, pgmin_mean_and_exercise.second, false, false);
        }

        // Timing " estimation des compétences "
        std::string timer_name_comps = "estim_comps_" + std::to_string(nb_loops);
        times.startTimer(timer_name_comps);
        for (auto &target: all_targets) {
            DBN15->query_JD(target);
        }
        times.stopTimer(timer_name_comps);

        return pgmin_mean_and_exercise;
    }


    PGscore getPGscore(std::vector<plDistribution>& variables, std::vector<Exercise>& exo_vector) {
        PGscore new_score;
        new_score.pgmin = getPGmin(variables, exo_vector);
        new_score.pgmean = getPGmean(variables);
        return new_score;
    }

    PGscore getPGscore(std::vector<plComputableObjectList>& results, std::vector<plVariablesConjunction>& all_targets,
                       std::vector<Exercise>& exo_vector) {
        PGscore new_score;
        new_score.pgmin = getPGmin(results, all_targets, exo_vector);
        new_score.pgmean = getPGmean(results, all_targets /*variables, variables_to_query*/);
        return new_score;
    }

    double getPGmean(std::vector<plDistribution>& variables) {
        double pgmean = 0.0;
        std::vector<std::vector<std::pair<plValues, plProbValue>>> all_res;
        all_res.resize(variables.size());

        for (size_t i = 0; i < variables.size(); i++) {
            variables[i].sorted_tabulate(all_res[i]);
        }


        for (auto &res: all_res) {
            pgmean += res[1].second;
        }

        pgmean = pgmean / static_cast<double>(all_res.size());

        return pgmean;
    }

    double getPGmean(std::vector<plComputableObjectList>& results, std::vector<plVariablesConjunction>& all_targets) {
        double pgmean = 0.0;
        int nb = 0;

        std::string suffix = "_t";

        for (size_t i = 0; i < results.size(); ++i) {
            plHMM::Observation_t outputData;
            for (size_t y = 0; y < results[i].size(); ++y) {
                if (Tools::endsWith(all_targets[i][y].name(), suffix)) {

                    results[i][y].tabulate(outputData);
                    pgmean += outputData[1];
                    ++nb;
                }
            }
        }

        pgmean = pgmean / nb;

        return pgmean;
    }

    double getPGmin(std::vector<plDistribution>& variables, std::vector<Exercise>& exo_vector) {
        double pgmin = 1.1; // Since 1 is the maximum...

        std::vector<std::vector<std::pair<plValues, plProbValue>>> all_res;
        all_res.resize(variables.size());

        for (size_t i = 0; i < variables.size(); i++) {
            variables[i].sorted_tabulate(all_res[i]);
        }

        for (size_t i = 0; i < variables.size(); i++) {
            if (isSkillStillInExercises(i, exo_vector)) {
                if (all_res[i][1].second < pgmin) {
                    pgmin = all_res[i][1].second;
                }
            }
        }

        return pgmin;
    }

    double getPGmin(std::vector<plComputableObjectList>& results, std::vector<plVariablesConjunction>& all_targets,
                    std::vector<Exercise>& exo_vector) {

        double pgmin = 1.1;
        // Because there are also A, B, C in the results and targets
        // (so can't just use i as an index)
        size_t current_skill = 0;

        std::string suffix = "_t";

        for (size_t i = 0; i < results.size(); ++i) {
            plHMM::Observation_t outputData;

            for (size_t y = 0; y < results[i].size(); ++y) {
                if (Tools::endsWith(all_targets[i][y].name(), suffix)) {
                    if (isSkillStillInExercises(current_skill, exo_vector)) {

                        results[i][y].tabulate(outputData);
                        if (outputData[1] < pgmin) {
                            pgmin = outputData[1];
                        }
                    }

                    ++current_skill;
                }
            }
        }

        return pgmin;
    }


    plValues getT0Evidences(Exercise& exo, pmBayesianNetwork& g_init, std::vector<std::string>& variables_with_evidences,
                            std::vector<std::string>& masteries, AnswersPrediction answersPrediction, bool triggers) {

        plValues evidences_t0;
        plVariablesConjunction variables_init = g_init.getVariables();
        std::vector<plVariable> variables_t0 = getVariablesWithName(variables_with_evidences, variables_init, "Res_",
                                                                    true, false);
        std::vector<plVariable> triggers_t0 = getVariablesWithName(variables_with_evidences, variables_init,
                                                                   "Trigger_", true, false);

        if (answersPrediction == AnswersPrediction::Optimistic) {
            for (size_t i = 0; i < variables_t0.size(); i++) {
                if(exo.comps[i]) {
                    evidences_t0 = evidences_t0.add(variables_t0[i], masteries[1]);
                }

                if (MY_DEBUG) {
                    std::cout << "evidences_t0[" << variables_t0[i].name() << "] = " << evidences_t0[variables_t0[i]]
                              << std::endl;
                }
            }
        }

        if (triggers) {
            for (size_t i = 0; i < triggers_t0.size(); i++) {
                evidences_t0 = evidences_t0.add(triggers_t0[i], exo.comps[i]);
                if (MY_DEBUG) {
                    std::cout << triggers_t0[i].name() << ": " << exo.comps[0] << std::endl;
                }
            }
        }

        return evidences_t0;
    }

    plValues getTrueT0Evidences(Exercise& exo, pmBayesianNetwork& g_init, std::vector<std::string>& variables_with_evidences,
                                std::vector<std::string>& masteries, bool triggers) {
        plValues true_evidences;

        plVariablesConjunction variables_init = g_init.getVariables();
        std::vector<plVariable> res_t0 = getVariablesWithName(variables_with_evidences, variables_init, "Res_", true,
                                                              false);

        for (size_t i = 0; i < res_t0.size(); i++) {
            if (exo.comps[i]) {
                true_evidences = true_evidences.add(res_t0[i], exo.res[i]);
            }
        }

        if (triggers) {
            std::vector<plVariable> triggers_t0 = getVariablesWithName(variables_with_evidences, variables_init,
                                                                       "Trigger_", true, false);
            for (size_t i = 0; i < triggers_t0.size(); i++) {
                true_evidences = true_evidences.add(triggers_t0[i], exo.comps[i]);
            }
        }

        return true_evidences;
    }

    plVariable& getVariableWithName(const std::string& name, std::vector<plVariable>& variables) {
        for (plVariable &v: variables) {
            if (v.name() == name) {
                return v;
            }
        }

        std::cout << "[ERROR: variable " << name
                  << " not found in vector. Returning first variable of the vector. Expect weird behaviour/crash.]"
                  << std::endl;
        return variables[0];
    }


    std::vector<plVariable> getVariablesWithName(std::vector<std::string>& names, plVariablesConjunction& all_variables,
                                                 const std::string& prefix, bool is_t0, bool get_next_t) {

        std::vector<plVariable> variables_to_query;

        if (!is_t0) {
            if (!get_next_t) {
                std::string skill_name;
                for (std::string &name: names) {
                    skill_name = prefix + name;
                    variables_to_query.push_back(all_variables.get_variable_with_name(skill_name));
                }
            } else {
                std::string skill_t_name;
                for (std::string &name: names) {
                    skill_t_name = prefix + name + "_t";
                    variables_to_query.push_back(all_variables.get_variable_with_name(skill_t_name));
                }
            }
        } else {
            std::string skill_t0_name;
            for (std::string &name: names) {
                skill_t0_name = prefix + name + "_t0";
                variables_to_query.push_back(all_variables.get_variable_with_name(skill_t0_name));
            }
        }

        return variables_to_query;
    }

    std::vector<std::string> getVariablesNamesWithName(std::vector<std::string>& names, plVariablesConjunction& all_variables,
                                                       const std::string& prefix, bool is_t0, bool get_next_t) {

        std::vector<std::string> variables_to_query;

        if (!is_t0) {
            if (!get_next_t) {
                std::string skill_name;
                for (std::string &name: names) {
                    skill_name = prefix + name;
                    variables_to_query.push_back(skill_name);
                }
            } else {
                std::string skill_t_name;
                for (std::string &name: names) {
                    skill_t_name = prefix + name + "_t";
                    variables_to_query.push_back(skill_t_name);
                }
            }
        } else {
            std::string skill_t0_name;
            for (std::string &name: names) {
                skill_t0_name = prefix + name + "_t0";
                variables_to_query.push_back(skill_t0_name);
            }
        }

        return variables_to_query;
    }

    std::vector<Exercise> getStudent1Exercises() {
        std::vector<Exercise> exercises;
        exercises.emplace_back(
                Exercise("E1 - fractions",
                         std::vector<bool>{true, false, false},
                         std::vector<int>{1, -1, -1}
                ));

        exercises.emplace_back(
                Exercise("E2 - evolution proportion",
                         std::vector<bool>{true, false, false},
                         std::vector<int>{1, -1, -1}
                ));

        exercises.emplace_back(
                Exercise("E3 - proportionnalite",
                         std::vector<bool>{true, false, false},
                         std::vector<int>{0, -1, -1}
                ));

        exercises.emplace_back(
                Exercise("E4 - vente livres",
                         std::vector<bool>{true, false, true},
                         std::vector<int>{1, -1, 0}
                ));

        exercises.emplace_back(
                Exercise("E5 - conjecture",
                         std::vector<bool>{true, true, false},
                         std::vector<int>{1, 0, -1}
                ));

        exercises.emplace_back(
                Exercise("E6 - programmes calcul",
                         std::vector<bool>{true, true, false},
                         std::vector<int>{0, 1, -1}
                ));

        exercises.emplace_back(
                Exercise("E7 - devinette math",
                         std::vector<bool>{false, true, true},
                         std::vector<int>{-1, 1, 0}
                ));

        exercises.emplace_back(
                Exercise("E8 - eau de pluie",
                         std::vector<bool>{false, false, true},
                         std::vector<int>{-1, -1, 0}
                ));

        return exercises;
    }

    std::vector<Exercise> getStudent2Exercises() {
        std::vector<Exercise> exercises;
        exercises.emplace_back(
                Exercise("E1 - fractions",
                         std::vector<bool>{true, false, false},
                         std::vector<int>{0, -1, -1}
                ));

        exercises.emplace_back(
                Exercise("E2 - evolution proportion",
                         std::vector<bool>{true, false, false},
                         std::vector<int>{0, -1, -1}
                ));

        exercises.emplace_back(
                Exercise("E3 - proportionnalite",
                         std::vector<bool>{true, false, false},
                         std::vector<int>{1, -1, -1}
                ));

        exercises.emplace_back(
                Exercise("E4 - vente livres",
                         std::vector<bool>{true, false, true},
                         std::vector<int>{1, -1, 1}
                ));

        exercises.emplace_back(
                Exercise("E5 - conjecture",
                         std::vector<bool>{true, true, false},
                         std::vector<int>{1, 0, -1}
                ));

        exercises.emplace_back(
                Exercise("E6 - programmes calcul",
                         std::vector<bool>{true, true, false},
                         std::vector<int>{1, 1, -1}
                ));

        exercises.emplace_back(
                Exercise("E7 - devinette math",
                         std::vector<bool>{false, true, true},
                         std::vector<int>{-1, 1, 1}
                ));

        exercises.emplace_back(
                Exercise("E8 - eau de pluie",
                         std::vector<bool>{false, false, true},
                         std::vector<int>{-1, -1, 0}
                ));

        return exercises;
    }

    std::vector<Exercise> getStudent3Exercises() {
        std::vector<Exercise> exercises;
        exercises.emplace_back(
                Exercise("E1 - fractions",
                         std::vector<bool>{true, false, false},
                         std::vector<int>{0, -1, -1}
                ));

        exercises.emplace_back(
                Exercise("E2 - evolution proportion",
                         std::vector<bool>{true, false, false},
                         std::vector<int>{1, -1, -1}
                ));

        exercises.emplace_back(
                Exercise("E3 - proportionnalite",
                         std::vector<bool>{true, false, false},
                         std::vector<int>{0, -1, -1}
                ));

        exercises.emplace_back(
                Exercise("E4 - vente livres",
                         std::vector<bool>{true, false, true},
                         std::vector<int>{0, -1, 0}
                ));

        exercises.emplace_back(
                Exercise("E5 - conjecture",
                         std::vector<bool>{true, true, false},
                         std::vector<int>{1, 0, -1}
                ));

        exercises.emplace_back(
                Exercise("E6 - programmes calcul",
                         std::vector<bool>{true, true, false},
                         std::vector<int>{0, 0, -1}
                ));

        exercises.emplace_back(
                Exercise("E7 - devinette math",
                         std::vector<bool>{false, true, true},
                         std::vector<int>{-1, 1, 1}
                ));

        exercises.emplace_back(
                Exercise("E8 - eau de pluie",
                         std::vector<bool>{false, false, true},
                         std::vector<int>{-1, -1, 1}
                ));

        return exercises;
    }

    void removeExerciseFromVector(Exercise& exo_to_remove, std::vector<Exercise>& exo_vector) {
        int final_idx = -1;

        for (size_t i = 0; i < exo_vector.size(); i++) {
            if (exo_to_remove.name == exo_vector[i].name) {
                final_idx = static_cast<int>(i);
                break;
            }
        }

        if (final_idx > -1) {
            exo_vector.erase(exo_vector.begin() + final_idx);
            if (MY_DEBUG) {
                std::cout << "Exercise \"" << exo_to_remove.name << "\" removed from vector" << std::endl;
            }


        } else {
            std::cout << "WARNING: exercise " << exo_to_remove.name << " not found in vector." << std::endl;
        }
    }

    std::vector<Exercise> getExercisesWithoutRepetition(std::vector<Exercise>& original_exo_vector) {
        std::vector<Exercise> exo_without_rep;
        bool already_inside = false;

        for (Exercise &exo: original_exo_vector) {
            already_inside = false;

            for (Exercise &exo_in_new: exo_without_rep) {
                if (exo.hasSameComp(exo_in_new)) {
                    already_inside = true;
                    break;
                }
            }

            if (!already_inside) {
                exo_without_rep.emplace_back(exo);
            }
        }

        return exo_without_rep;
    }

    Exercise& getExerciseWithSameSkills(Exercise& exo, std::vector<Exercise>& exo_vector, bool random) {
        if (!random) {
            for (Exercise &exo_in_vec: exo_vector) {
                if (exo.hasSameComp(exo_in_vec)) {
                    return exo_in_vec;
                }
            }
        } else {
            std::vector<size_t> idx_to_chose_from;
            for (size_t i = 0; i < exo_vector.size(); i++) {
                if (exo.hasSameComp(exo_vector[i])) {
                    idx_to_chose_from.push_back(i);
                }
            }

            if (idx_to_chose_from.size() == 1) {
                return exo_vector[idx_to_chose_from[0]];
            }

            if (MY_DEBUG) {
                if (idx_to_chose_from.size() > 1) {
                    std::cout << std::endl << "DEBUG: exo " << exo.name << " has more than 1 to chose from (good): ";
                    for (size_t idx: idx_to_chose_from) {
                        std::cout << exo_vector[idx].name << ") / ";
                    }

                    std::cout << std::endl;
                }
            }

            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<std::mt19937::result_type> dist(0, idx_to_chose_from.size());
            size_t choosen = dist(rng);
            return exo_vector[choosen];
        }

        // Should never happen but CLion keep bothering me
        std::cout << "[WARNING in getExerciseWithSameSkills(): this should not happen.]" << std::endl;
        return exo_vector[0];
    }

    bool isSkillStillInExercises(size_t skill_idx, std::vector<Exercise>& exo_vector) {
        for (Exercise &exo: exo_vector) {
            if (exo.comps[skill_idx]) {
                //std::cout << "[INFO: skill index " << skill_idx << " is still in exercises.]" << std::endl;
                return true;
            }
        }

        //std::cout << "[INFO: skill index " << skill_idx << " is not in exercises anymore.]" << std::endl;
        return false;
    }


    void printProgressiveBKTResult(std::vector<plComputableObjectList>& results, std::vector<int>& instants,
                                   std::vector<plVariablesConjunction>& all_targets,
                                   Exercise& exo, bool formatted_for_spreadsheet, bool only_true) {

        std::cout << "Instant " << instants[0] << std::endl;

        for (size_t i = 0; i < results.size(); i++) {
            plHMM::Observation_t outputData;

            for (size_t y = 0; y < results[i].size(); y++) {
                results[i][y].tabulate(outputData);

                if (!formatted_for_spreadsheet) {
                    std::cout << all_targets[i][y].name() << ": [";
                }

                if (formatted_for_spreadsheet && only_true) {
                    std::cout << outputData[1] << ",";
                } else {
                    for (size_t k = 0; k < outputData.size(); k++) {
                        if (formatted_for_spreadsheet) {
                            std::cout << outputData[k] << (k < outputData.size() - 1 ? "," : " ");
                        } else {
                            std::cout << k << ": " << outputData[k] << (k < outputData.size() - 1 ? "," : " ");
                        }
                    }
                }

                if (!formatted_for_spreadsheet) {
                    std::cout << "]" << (y < results[i].size() - 1 ? "," : "");
                }
            }

            std::cout << std::endl;
            std::cout << "Exercise name: " << exo.name << std::endl << std::endl << std::endl;
        }
    }

    void printForSpreadsheet(std::vector<plComputableObjectList>&results, std::vector<plVariablesConjunction>& all_targets,
                             Exercise& exo, std::vector<PGscore>& all_pg_scores) {

        std::cout << "PGmin " << std::endl;
        for (PGscore &pgscore: all_pg_scores) {
            std::cout << pgscore.pgmin << ",";
        }
        std::cout << std::endl;

        std::cout << "PGmean" << std::endl;
        for (PGscore &pgscore: all_pg_scores) {
            std::cout << pgscore.pgmean << ",";
        }
        std::cout << std::endl;

        std::cout << "1 values:" << std::endl;

        for (size_t i = 0; i < results.size(); i++) {
            plHMM::Observation_t outputData;

            for (size_t y = 0; y < results[i].size(); y++) {
                results[i][y].tabulate(outputData);

                std::cout << outputData[1] << ",";
            }

            std::cout << std::endl;

            std::cout << "Exercise name: " << exo.name << std::endl << std::endl << std::endl;
        }
    }

    void printForSpreadsheetOnlyValues(std::vector<plComputableObjectList>& results,
                                       std::vector<plVariablesConjunction>& all_targets,
                                       std::vector<PGscore>& all_pg_scores, Metric metric) {

        /*for (PGscore& pgscore: all_pg_scores) {
            if (metric == Metric::PG_min) {
                std::cout << pgscore.pgmin << ",";
            }

            else if (metric == Metric::PG_mean) {
                std::cout << pgscore.pgmean << ",";
            }
        }*/

        for (size_t i = 0; i < results.size(); i++) {
            plHMM::Observation_t outputData;

            for (size_t y = 0; y < results[i].size(); y++) {
                results[i][y].tabulate(outputData);

                std::cout << outputData[1] << ",";
            }

            std::cout << std::endl;
        }
    }
}
