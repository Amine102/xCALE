#include "../include/genericBKTModel.h"

BKTModel::BKTModel() {
    DBN = nullptr;
}

BKTModel::~BKTModel() {

}

std::vector<Skill>& BKTModel::getSkillList() {
    return this->list_skills;
}

pmDynamicBayesianNetwork* BKTModel::getDBN() {
    return DBN;
}

void BKTModel::setDBN(pmDynamicBayesianNetwork* dbn) {
    this->DBN = dbn;
}

std::string BKTModel::getHighestMasteryValue() {
    return this->list_skills[0].getHighestMasteryValue();
}

void BKTModel::addSkillToVector(Skill& skill) {
    this->list_skills.push_back(skill);
}

void BKTModel::addSkill(const Skill& new_skill, const std::vector<double>& PInit, const std::vector<double>& PLearn,
                        double PGuess, double PSlip, double PForget, std::vector<std::vector<double>> input_Zi_table) {

    plComputableObjectList t0_tables;
    setT0Tables(t0_tables, new_skill, PInit, PGuess, PSlip, input_Zi_table);

    plComputableObjectList t_tables;
    setTTables(t_tables, new_skill, PInit, PLearn, PGuess, PSlip, PForget, input_Zi_table);


    // If the DBN hasn't been created yet, do it
    if (this->list_skills.empty()) {
        plJointDistribution jd0(t0_tables);
        plJointDistribution jdt(t_tables);
        auto BN_0 = new pmBayesianNetwork(jd0);
        auto BN_Transition = new pmBayesianNetwork(jdt);
        this->DBN = new pmDynamicBayesianNetwork(BN_0, BN_Transition);
    }

    // Else, update the JD0 and JDt
    else {
        plComputableObjectList newObj0 = this->DBN->getJointDistributionforG0().get_computable_object_list();
        newObj0 *= t0_tables;

        plComputableObjectList newObjt = this->DBN->getJointDistributionforGtransition().get_computable_object_list();
        newObjt *= t_tables;

        plJointDistribution newJd0(newObj0);
        plJointDistribution newJdt(newObjt);

        auto newBN_0 = new pmBayesianNetwork(newJd0);
        auto newBN_t = new pmBayesianNetwork(newJdt);
        this->DBN = new pmDynamicBayesianNetwork(newBN_0, newBN_t);
    }

    list_skills.push_back(new_skill);
}

void BKTModel::setT0Tables(plComputableObjectList& t0_tables, const Skill& new_skill, const std::vector<double>& PInit,
                           double PGuess, double PSlip, std::vector<std::vector<double>> input_Zi_table) {

    size_t size_of_diff = new_skill.difficulty_scale.getNumberOfValues();
    size_t size_of_speed = new_skill.speed_scale.getNumberOfValues();
    // --------------------------------------
    // Variables and tables definition for t0
    // --------------------------------------
    // ----- Trigger S t0 -----
    plVariable Trigger_S_t0("Trigger_" + new_skill.name + "_t0", PL_BINARY_TYPE);
    plProbValue table_trigger_S_t0[] = {0.5, 0.5};
    plProbTable P_trigger_S_t0(Trigger_S_t0, table_trigger_S_t0);
    // ----- Trigger S t0 -----

    // ----- Difficulty S t0 -----
    plVariable Difficulty_S_t0("Difficulty_" + new_skill.name + "_t0", new_skill.difficulty_scale.scale_values);
    plProbValue table_difficulty_S_t0[size_of_diff];
    for (size_t i = 0; i < size_of_diff; i++) {
        table_difficulty_S_t0[i] = 1.0 / static_cast<float>(size_of_diff);
    }

    plProbTable P_Difficulty_S_t0(Difficulty_S_t0, table_difficulty_S_t0);
    // ----- Difficulty S t0 -----

    // ----- Speed S t0 -----
    // This is for the case where there's no prerequisites
    plVariable Speed_S_t0("Speed_" + new_skill.name + "_t0", new_skill.speed_scale.scale_values);

    // Tables for when there's no Ys
    plDistributionTable P_Speed_S_t0(Speed_S_t0, Trigger_S_t0 ^ Difficulty_S_t0, Trigger_S_t0);
    // If trigger = 0
    plProbValue tableSpeed_S_t0_knowing_Diff_S_t0_none[size_of_speed * size_of_diff];

    for (size_t i = 0; i < size_of_diff; i++) {
        for (size_t j = 0; j < size_of_speed; j++) {
            if (j == 0) {
                tableSpeed_S_t0_knowing_Diff_S_t0_none[(i * size_of_speed) + j] = 1;
            }

            else {
                tableSpeed_S_t0_knowing_Diff_S_t0_none[(i * size_of_speed) + j] = 0;
            }
        }
    }

    /*plProbValue tableSpeed_S_t0_knowing_Diff_S_t0_none[] = {1, 0, 0, 0,
                                                            1, 0, 0, 0,
                                                            1, 0, 0, 0};*/
    // Else
    plProbValue tableSpeed_S_t0_knowing_Diff_S_t0[size_of_speed * size_of_diff];

    for (size_t i = 0; i < size_of_diff; i++) {
        for (size_t j = 0; j < size_of_speed; j++) {
            if (j != 0) {
                if (size_of_speed - 1 - i == j) {
                    tableSpeed_S_t0_knowing_Diff_S_t0_none[(i * size_of_speed) + j] = 1;
                }
            }

            else {
                tableSpeed_S_t0_knowing_Diff_S_t0_none[(i * size_of_speed) + j] = 0;
            }
        }
    }

    /*plProbValue tableSpeed_S_t0_knowing_Diff_S_t0[] =
            {
                    0, 0, 0, 1,
                    0, 0, 1, 0,
                    0, 1, 0, 0
            };*/

    P_Speed_S_t0.push(plDistributionTable(Speed_S_t0, Difficulty_S_t0, tableSpeed_S_t0_knowing_Diff_S_t0_none), 0);
    P_Speed_S_t0.push(plDistributionTable(Speed_S_t0, Difficulty_S_t0, tableSpeed_S_t0_knowing_Diff_S_t0), 1);
    // ----- Speed S t0 -----

    // ----- Skill S t0 -----
    plVariable S_t0("Skill_" + new_skill.name + "_t0", new_skill.mastery_scale.scale_values);
    //plDistributionTable P_S_t0(S_t0, Speed_S_t0, Speed_S_t0);
    plProbValue tableS_t0[PInit.size()];
    std::copy(PInit.begin(), PInit.end(), tableS_t0);
    plProbTable P_S_t0(S_t0, tableS_t0);
    // ----- Skill S t0 -----

    // ----- Res S t0 ------
    plVariable ResS_t0("Res_" + new_skill.name + "_t0", new_skill.mastery_scale.scale_values);
    plProbValue* tableResS_t0_knowingS_t0 = getResTableWithCoefs(new_skill.mastery_scale.getNumberOfValues(),
                                                                 new_skill.mastery_scale.getNumberOfValues(),
                                                                 PGuess, PSlip);

    /*plProbValue tableResS_t0_knowingS_t0[] = {
            1 - PGuess, 0.6 * PGuess, 0.3 * PGuess, 0.1 * PGuess,
            PSlip, 1 - PSlip - PGuess, 0.8 * PGuess, 0.2 * PGuess,
            0.2 * PSlip, 0.8 * PSlip, 1 - PSlip - PGuess, PGuess,
            0.1 * PSlip, 0.3 * PSlip, 0.6 * PSlip, 1 - PSlip
    };*/
    plDistributionTable P_ResS_t0(ResS_t0, S_t0, tableResS_t0_knowingS_t0);
    // ----- Res S t0 ------


    // ComputableObjectList for t0 tables
    // Since The pre-requisites act from (t-1) to (t), t0 Ys are irrelevant
    t0_tables = plComputableObjectList(P_S_t0 * P_ResS_t0 * P_trigger_S_t0 * P_Difficulty_S_t0);

    // no pre-requisite(s)
    if(new_skill.prerequisites.empty()) {
        t0_tables *= P_Speed_S_t0;
    }

    // Skill has pre-requisite(s)
    else {
        pmBayesianNetwork gt0 = this->getDBN()->getInitial_G();
        plVariablesConjunction Zi_list_t0;

        // create the Z(Pi)S_t0 (one for each prequisite)
        for (size_t i = 0; i < new_skill.prerequisites.size(); i++) {
            plProbValue Zi_t0_table[input_Zi_table[i].size()];
            std::copy(input_Zi_table[i].begin(), input_Zi_table[i].end(), Zi_t0_table);

            plVariable insert_t0 = gt0.getVariables().get_variable_with_name("Skill_" + new_skill.prerequisites[i] + "_t0");
            plVariable Zi_t0("Z_" + new_skill.prerequisites[i] + "_" + new_skill.name + "_t0", new_skill.mastery_scale.scale_values);
            Zi_list_t0 ^= Zi_t0;
            plDistributionTable P_U0(Zi_t0, insert_t0, Zi_t0_table);

            // Add the table to the ComputableObjectList
            t0_tables *= P_U0;
        }

        plVariable Y_t0("Y_" + new_skill.name + "_t0", new_skill.mastery_scale.scale_values);

        const plExternalFunctionFromC max_fun_t0(Y_t0, Zi_list_t0, noisy_max_code, "my_max");
        const plCndDeterministic noisy_max_y_t0(Y_t0, Zi_list_t0, max_fun_t0);

        // Since there's at least one prerequisites, add the corresponding noisy max to the ComputableObjectList
        t0_tables *= noisy_max_y_t0;

        // Set the new table for P_Speed (if there are prerequisites, Speed_S depends on Trigger_S, Difficulty_S
        // and Y_S (t-1))
        // TODO: generic
        plProbValue* table_Ys0 = getYsTable(size_of_speed, size_of_diff, {0.8, 0.2});
        plProbValue* table_Ys1 = getYsTable(size_of_speed, size_of_diff, {0.4, 0.6});
        plProbValue* table_Ys2 = getYsTable(size_of_speed, size_of_diff, {0.0, 1.0});

        /*plProbValue table_Ys0[] = {
                0  , 0  , 0.8, 0.2,
                0  , 0.8, 0.2, 0  ,
                0.8, 0.2, 0  , 0
        };

        plProbValue table_Ys1[] = {
                0  , 0  , 0.4, 0.6,
                0  , 0.4, 0.6, 0  ,
                0.4, 0.6, 0  , 0
        };

        plProbValue table_Ys2[] = {
                0  , 0  , 0  , 1  ,
                0  , 0  , 1  , 0  ,
                0  , 1  , 0  , 0
        };*/

        plProbValue table_Ys3[] = {
                0  , 0  , 0  , 1  ,
                0  , 0  , 0.6, 0.4,
                0  , 0.6, 0.4, 0
        };

        plProbValue tableSpeed_S_knowing_Diff_S_none2[size_of_speed * size_of_diff];

        for (size_t i = 0; i < size_of_diff; i++) {
            for (size_t j = 0; j < size_of_speed; j++) {
                if (j == 0) {
                    tableSpeed_S_knowing_Diff_S_none2[(i * size_of_speed) + j] = 1;
                }

                else {
                    tableSpeed_S_knowing_Diff_S_none2[(i * size_of_speed) + j] = 0;
                }
            }
        }

        /*plProbValue tableSpeed_S_knowing_Diff_S_none2[] = {1, 0, 0, 0,
                                                           1, 0, 0, 0,
                                                           1, 0, 0, 0};*/

        plDistributionTable P_Speed_S_t0_2(Speed_S_t0, Trigger_S_t0 ^ Y_t0 ^ Difficulty_S_t0, Trigger_S_t0);
        plDistributionTable P_Speed_S_t0_knowing_T_equals_0(Speed_S_t0, Y_t0 ^ Difficulty_S_t0, Y_t0);
        P_Speed_S_t0_knowing_T_equals_0.push(plDistributionTable(Speed_S_t0, Difficulty_S_t0, tableSpeed_S_knowing_Diff_S_none2), 0);
        P_Speed_S_t0_knowing_T_equals_0.push(plDistributionTable(Speed_S_t0, Difficulty_S_t0, tableSpeed_S_knowing_Diff_S_none2), 1);
        P_Speed_S_t0_knowing_T_equals_0.push(plDistributionTable(Speed_S_t0, Difficulty_S_t0, tableSpeed_S_knowing_Diff_S_none2), 2);
        P_Speed_S_t0_knowing_T_equals_0.push(plDistributionTable(Speed_S_t0, Difficulty_S_t0, tableSpeed_S_knowing_Diff_S_none2), 3);

        P_Speed_S_t0_2.push(P_Speed_S_t0_knowing_T_equals_0, 0);

        plDistributionTable P_Speed_S_t0_Y_t0_2(Speed_S_t0, Y_t0 ^ Difficulty_S_t0, Y_t0);
        P_Speed_S_t0_Y_t0_2.push(plDistributionTable(Speed_S_t0, Difficulty_S_t0, table_Ys0), 0);
        P_Speed_S_t0_Y_t0_2.push(plDistributionTable(Speed_S_t0, Difficulty_S_t0, table_Ys1), 1);
        P_Speed_S_t0_Y_t0_2.push(plDistributionTable(Speed_S_t0, Difficulty_S_t0, table_Ys2), 2);
        P_Speed_S_t0_Y_t0_2.push(plDistributionTable(Speed_S_t0, Difficulty_S_t0, table_Ys3), 3);
        P_Speed_S_t0_2.push(P_Speed_S_t0_Y_t0_2, 1);

        t0_tables *= P_Speed_S_t0_2;
    }
}

void BKTModel::setTTables(plComputableObjectList& t_tables, const Skill& new_skill, const std::vector<double>& PInit,
                          const std::vector<double>& PLearn, double PGuess, double PSlip, double PForget,
                          std::vector<std::vector<double>> input_Zi_table) {

    size_t size_of_diff = new_skill.difficulty_scale.getNumberOfValues();
    size_t size_of_speed = new_skill.speed_scale.getNumberOfValues();
    // ---------------------------------------
    // Variables and tables definition for t-1
    // ---------------------------------------
    // ----- S -----
    plVariable S("Skill_" + new_skill.name, new_skill.mastery_scale.scale_values);
    plProbValue tableS[PInit.size()];
    std::copy(PInit.begin(), PInit.end(), tableS);
    plProbTable P_S(S, tableS);
    // ----- S -----

    // ----- Res S -----
    plVariable ResS("Res_" + new_skill.name, new_skill.mastery_scale.scale_values);
    plProbTable P_ResS(ResS);
    // ----- Res S -----


    // ----- Trigger S -----
    plVariable Trigger_S("Trigger_" + new_skill.name, PL_BINARY_TYPE);
    plProbTable P_trigger_S(Trigger_S);
    // ----- Trigger S -----

    // ----- Difficulty S -----
    plVariable Difficulty_S("Difficulty_" + new_skill.name, new_skill.difficulty_scale.scale_values);
    plProbTable P_Difficulty_S(Difficulty_S);
    // ----- Difficulty S -----

    // ----- Speed S -----
    // This is for the case where there's no prerequisites
    plVariable Speed_S("Speed_" + new_skill.name, new_skill.speed_scale.scale_values);
    plProbTable P_Speed_S(Speed_S);
    // ----- Speed S -----

    // -------------------------------------
    // Variables and tables definition for t
    // -------------------------------------

    // ----- Trigger St -----
    plVariable Trigger_S_t("Trigger_" + new_skill.name + "_t", PL_BINARY_TYPE);
    plProbValue table_trigger_S_t[] = {0.5, 0.5};
    plProbTable P_trigger_S_t(Trigger_S_t, table_trigger_S_t);
    // ----- Trigger St -----

    // ----- Difficulty St -----
    plVariable Difficulty_S_t("Difficulty_" + new_skill.name +"_t", new_skill.difficulty_scale.scale_values);
    plProbValue table_difficulty_S_t[size_of_diff];
    for (size_t i = 0; i < size_of_diff; i++) {
        table_difficulty_S_t[i] = 1.0 / static_cast<float>(size_of_diff);
    }
    plProbTable P_Difficulty_S_t(Difficulty_S_t, table_difficulty_S_t);
    // ----- Difficulty St -----

    // ----- Speed St -----
    // This is for the case where there's no prerequisites
    plVariable Speed_S_t("Speed_" + new_skill.name + "_t", new_skill.speed_scale.scale_values);

    // Tables for when there's no Ys
    plDistributionTable P_Speed_S_t(Speed_S_t, Trigger_S_t ^ Difficulty_S_t, Trigger_S_t);
    // If trigger = 0, speed stays at 0
    plProbValue tableSpeed_S_t_knowing_Diff_S_t_none[size_of_speed * size_of_diff];

    for (size_t i = 0; i < size_of_diff; i++) {
        for (size_t j = 0; j < size_of_speed; j++) {
            if (j == 0) {
                tableSpeed_S_t_knowing_Diff_S_t_none[(i * size_of_speed) + j] = 1;
            }

            else {
                tableSpeed_S_t_knowing_Diff_S_t_none[(i * size_of_speed) + j] = 0;
            }
        }
    }

    /*plProbValue tableSpeed_S_t_knowing_Diff_S_t_none[] = {1, 0, 0, 0,
                                                      1, 0, 0, 0,
                                                      1, 0, 0, 0};*/

    // Else, the lower the difficulty is, the higher the speed is
    // Else
    plProbValue tableSpeed_S_t_knowing_Diff_S_t[size_of_speed * size_of_diff];

    for (size_t i = 0; i < size_of_diff; i++) {
        for (size_t j = 0; j < size_of_speed; j++) {
            if (j != 0) {
                if (size_of_speed - 1 - i == j) {
                    tableSpeed_S_t_knowing_Diff_S_t[(i * size_of_speed) + j] = 1;
                }
            }

            else {
                tableSpeed_S_t_knowing_Diff_S_t[(i * size_of_speed) + j] = 0;
            }
        }
    }

    /*plProbValue tableSpeed_S_t_knowing_Diff_S_t[] =
            {
                    0, 0, 0, 1,
                    0, 0, 1, 0,
                    0, 1, 0, 0
            };*/

    P_Speed_S_t.push(plDistributionTable(Speed_S_t, Difficulty_S_t, tableSpeed_S_t_knowing_Diff_S_t_none), 0);
    P_Speed_S_t.push(plDistributionTable(Speed_S_t, Difficulty_S_t, tableSpeed_S_t_knowing_Diff_S_t), 1);
    // ----- Speed St -----

    // ----- St -----
    plVariable S_t("Skill_" + new_skill.name + "_t", new_skill.mastery_scale.scale_values);
    size_t size_of_skill = new_skill.mastery_scale.getNumberOfValues();

    // If Speed = 0, identity matrix
    plProbValue* tableS_t_knowingS_none = getIdentityTable(size_of_skill);

    /*plProbValue tableS_t_knowingS_none[] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };*/

    plDistributionTable P_S_t(S_t, Speed_S_t ^ S, Speed_S_t);

    P_S_t.push(plDistributionTable(S_t, S, tableS_t_knowingS_none), 0);

    // 1 + nb_plearn tables (speed = 0 is identity matrix, see above)
    // type is int because there's an ambigious call if it's size_t
    for (int i = 0; i < PLearn.size(); i++) {
        plProbValue* tableS_t_knowingS_current_speed = getPlearnTable(size_of_skill, size_of_skill, PLearn[i], PForget);
        /*plProbValue tableS_t_knowingS_current_speed[] = {
                1 - PLearn[i], PLearn[i], 0, 0,
                PForget, 1 - PLearn[i] - PForget, PLearn[i], 0,
                0, PForget, 1 - PLearn[i] - PForget, PLearn[i],
                0, 0, PForget, 1 - PForget
        };*/

        P_S_t.push(plDistributionTable(S_t, S, tableS_t_knowingS_current_speed), i+1);
    }

    // ----- Res St ------
    plVariable ResS_t("Res_" + new_skill.name + "_t", new_skill.mastery_scale.scale_values);

    plProbValue* tableResS_t_knowingS_t = getResTableWithCoefs(new_skill.mastery_scale.getNumberOfValues(),
                                                               new_skill.mastery_scale.getNumberOfValues(),
                                                               PGuess, PSlip);
    /*plProbValue tableResS_t_knowingS_t[] = {
            1 - PGuess, 0.6 * PGuess, 0.3 * PGuess, 0.1 * PGuess,
            PSlip, 1 - PSlip - PGuess, 0.8 * PGuess, 0.2 * PGuess,
            0.2 * PSlip, PSlip, 1 - PSlip - PGuess, PGuess,
            0.1 * PSlip, 0.3 * PSlip, 0.6 * PSlip, 1 - PSlip
    };*/

    plDistributionTable P_ResS_t(ResS_t, S_t, tableResS_t_knowingS_t);
    // ----- Res St ------

    // ComputableObjectList for t-1 and t tables
    // Speed_S and Speed_S_t are added later because they might be changed (if there is at least one prerequisites)
    t_tables = plComputableObjectList (P_S   * P_ResS   * P_trigger_S   * P_Difficulty_S   *
                                       P_S_t * P_ResS_t * P_trigger_S_t * P_Difficulty_S_t);

    // If the skill has no pre-requisites, add the tables
    if(new_skill.prerequisites.empty()) {
        t_tables *= P_Speed_S * P_Speed_S_t;
    }

        // If the skill has prequisites
    else {

        pmBayesianNetwork gt = this->getDBN()->getTransition_G();
        // t-1
        plVariablesConjunction Zi_list;
        // t
        plVariablesConjunction Zi_list_t;

        // create the Z(Pi)S and Z(Pi)S_t (one for each prequisite)
        for (size_t i = 0; i < new_skill.prerequisites.size(); i++) {

            plProbValue Zi_table[input_Zi_table[i].size()];
            std::copy(input_Zi_table[i].begin(), input_Zi_table[i].end(), Zi_table);

            plProbValue Zi_t_table[input_Zi_table[i].size()];
            std::copy(input_Zi_table[i].begin(), input_Zi_table[i].end(), Zi_t_table);

            plVariable insert = gt.getVariables().get_variable_with_name("Skill_" + new_skill.prerequisites[i]);
            plVariable Zi("Z_" + new_skill.prerequisites[i] + "_" + new_skill.name, new_skill.mastery_scale.scale_values);
            Zi_list ^= Zi;
            //plDistributionTable P_U(Zi, insert, Zi_table);
            plProbTable P_U(Zi, Zi_table);

            plVariable insert_t = gt.getVariables().get_variable_with_name("Skill_" + new_skill.prerequisites[i] + "_t");
            plVariable Zi_t("Z_" + new_skill.prerequisites[i] + "_" + new_skill.name + "_t", new_skill.mastery_scale.scale_values);
            Zi_list_t ^= Zi_t;
            plDistributionTable P_U_t(Zi_t, insert_t, Zi_t_table);

            t_tables *= P_U * P_U_t;
        }

        plVariable Y("Y_" + new_skill.name, new_skill.mastery_scale.scale_values);

        const plExternalFunctionFromC max_fun(Y, Zi_list, noisy_max_code, "my_max");
        const plCndDeterministic noisy_max_y(Y, Zi_list, max_fun);

        plVariable Y_t("Y_" + new_skill.name + "_t", new_skill.mastery_scale.scale_values);

        const plExternalFunctionFromC max_fun_t(Y_t, Zi_list_t, noisy_max_code, "my_max");
        const plCndDeterministic noisy_max_y_t(Y_t, Zi_list_t, max_fun_t);


        // Since there's at least one prerequisites, add the corresponding noisy max to the ComputableObjectList
        t_tables *= noisy_max_y * noisy_max_y_t;

        // Set the new table for P_Speed (if there are prerequisites, Speed_S depends on Trigger_S, Difficulty_S
        // and Y_S (t-1))
        // TODO: generic
        plProbValue* table_Ys0 = getYsTable(size_of_speed, size_of_diff, {0.8, 0.2});
        plProbValue* table_Ys1 = getYsTable(size_of_speed, size_of_diff, {0.4, 0.6});
        plProbValue* table_Ys2 = getYsTable(size_of_speed, size_of_diff, {0.0, 1.0});

        /*plProbValue table_Ys0[] = {
                0  , 0  , 0.8, 0.2,
                0  , 0.8, 0.2, 0  ,
                0.8, 0.2, 0  , 0
        };

        plProbValue table_Ys1[] = {
                0  , 0  , 0.4, 0.6,
                0  , 0.4, 0.6, 0  ,
                0.4, 0.6, 0  , 0
        };

        plProbValue table_Ys2[] = {
                0  , 0  , 0  , 1  ,
                0  , 0  , 1  , 0  ,
                0  , 1  , 0  , 0
        };*/

        plProbValue table_Ys3[] = {
                0  , 0  , 0  , 1  ,
                0  , 0  , 0.6, 0.4,
                0  , 0.6, 0.4, 0
        };

        plProbValue tableSpeed_S_knowing_Diff_S_none2[size_of_speed * size_of_diff];

        for (size_t i = 0; i < size_of_diff; i++) {
            for (size_t j = 0; j < size_of_speed; j++) {
                if (j == 0) {
                    tableSpeed_S_knowing_Diff_S_none2[(i * size_of_speed) + j] = 1;
                }

                else {
                    tableSpeed_S_knowing_Diff_S_none2[(i * size_of_speed) + j] = 0;
                }
            }
        }

        /*plProbValue tableSpeed_S_knowing_Diff_S_none2[] = {1, 0, 0, 0,
                                                           1, 0, 0, 0,
                                                           1, 0, 0, 0};*/

        plProbTable P_Speed_S_2(Speed_S);

        // Speed St
        plDistributionTable P_Speed_S_t_2(Speed_S_t, Trigger_S_t ^ Y ^ Difficulty_S_t, Trigger_S_t);
        plDistributionTable P_Speed_S_t_knowing_T_equals_0(Speed_S_t, Y ^ Difficulty_S_t, Y);
        // TODO: generic regarding the number of speed values
        P_Speed_S_t_knowing_T_equals_0.push(plDistributionTable(Speed_S_t, Difficulty_S_t, tableSpeed_S_knowing_Diff_S_none2), 0);
        P_Speed_S_t_knowing_T_equals_0.push(plDistributionTable(Speed_S_t, Difficulty_S_t, tableSpeed_S_knowing_Diff_S_none2), 1);
        P_Speed_S_t_knowing_T_equals_0.push(plDistributionTable(Speed_S_t, Difficulty_S_t, tableSpeed_S_knowing_Diff_S_none2), 2);
        P_Speed_S_t_knowing_T_equals_0.push(plDistributionTable(Speed_S_t, Difficulty_S_t, tableSpeed_S_knowing_Diff_S_none2), 3);

        P_Speed_S_t_2.push(P_Speed_S_t_knowing_T_equals_0, 0);

        plDistributionTable P_Speed_S_t_Y_t_2(Speed_S_t, Y ^ Difficulty_S_t, Y);
        P_Speed_S_t_Y_t_2.push(plDistributionTable(Speed_S_t, Difficulty_S_t, table_Ys0), 0);
        P_Speed_S_t_Y_t_2.push(plDistributionTable(Speed_S_t, Difficulty_S_t, table_Ys1), 1);
        P_Speed_S_t_Y_t_2.push(plDistributionTable(Speed_S_t, Difficulty_S_t, table_Ys2), 2);
        P_Speed_S_t_Y_t_2.push(plDistributionTable(Speed_S_t, Difficulty_S_t, table_Ys3), 3);
        P_Speed_S_t_2.push(P_Speed_S_t_Y_t_2, 1);

        t_tables *= P_Speed_S_2 * P_Speed_S_t_2;
    }
}

plProbValue* BKTModel::getResTableWithCoefs(size_t nb_row, size_t nb_col, double pg, double ps) {
    /* We want to get this kind of table
     * plProbValue tableResS_t_knowingS_t[] = {
            1 - PGuess, 0.6 * PGuess, 0.3 * PGuess, 0.1 * PGuess,
            PSlip, 1 - PSlip - PGuess, 0.8 * PGuess, 0.2 * PGuess,
            0.2 * PSlip, PSlip, 1 - PSlip - PGuess, PGuess,
            0.1 * PSlip, 0.3 * PSlip, 0.6 * PSlip, 1 - PSlip
    };*/

    auto new_table = new plProbValue[nb_row * nb_col];
    std::vector<float> coefs;

    // If the table is 2x2, it's faster to do this I guess
    if (nb_row * nb_col == 4) {
        new_table[0] = 1 - pg;
        new_table[1] = pg;
        new_table[2] = ps;
        new_table[3] = 1 - ps;
        // Putting a return here to avoid nested conditions later
        return new_table;
    }

    // Steps are :
    // 1) create the coefficients for the current column
    //        -> Diagonal values are at 0 (they are skipped later anyway)
    // 2) put the right value in the table (pg or pg * coef)
    for (size_t i = 0; i < nb_col; i++) {
        // 1) ------------------------------------
        coefs.clear();

        // formula is : nb_coef_pg = nb_rows - (i + 1)
        //              nb_coef_ps = i
        size_t nb_coef_ps = i;
        size_t nb_coef_pg = nb_row - (i + 1);

        float sum_of_elem_ps = 0.0;
        float sum_of_elem_pg = 0.0;


        // Doing the ps first, because in the prob table, ps is always above the diagonal
        // c as in " coefficient " (smart boy)
        for (size_t c = 0; c < nb_coef_ps; c++) {
            coefs.push_back(static_cast<float>(c + 1));
            sum_of_elem_ps += static_cast<float>(c + 1);
        }

        // diagonal value
        coefs.push_back(0);

        // No need to do it for the last column (not sure about appending empty vectors and stuff)
        if (nb_coef_pg > 0) {
            // needs to be reversed
            std::vector<float> pg_coefs;

            for (size_t c = 0; c < nb_coef_pg; c++) {
                pg_coefs.push_back(static_cast<float>(c + 1));
                sum_of_elem_pg += static_cast<float>(c + 1);
            }

            std::reverse(pg_coefs.begin(), pg_coefs.end());
            coefs.insert(coefs.end(), pg_coefs.begin(), pg_coefs.end());
        }

        // Divide each value by the corresponding sum of coefs
        for (size_t c = 0; c < nb_coef_ps + nb_coef_pg + 1; c++) {
            if (c < nb_coef_ps) {
                coefs[c] = coefs[c] / sum_of_elem_ps;
            }

            // Avoid edge case for last column (0 / 0)
            else if (nb_coef_pg > 0){
                coefs[c] = coefs[c] / sum_of_elem_pg;
            }
        }

        // 2) ------------------------------------
        // the value that'll be put in the table (either : coef * ps / coef * ps / 1 - pg / 1 - ps / 1 - ps - pg)
        double val;

        for (size_t j = 0; j < nb_row; j++) {
            // this is the diagonal case (it's either 1 - pg, 1 - pg - ps or 1 - ps)
            if (i == j) {
                // First column
                if (i == 0) {
                    val = 1 - pg;
                }

                // Last column
                else if (i == nb_col - 1) {
                    val = 1 - ps;
                }

                // diagonal value other than the first and the last
                else {
                    val = 1 - pg - ps;
                }
            }

            else {
                if (j < nb_coef_ps) {
                    val = ps * coefs[j];
                }

                else {
                    val = pg * coefs[j];
                }

            }

            new_table[Tools::get1dIdxFrom2dIdx(i, j, nb_row)] = val;
        }
    }

    return new_table;
}

plProbValue* BKTModel::getPlearnTable(size_t nb_row, size_t nb_col, double pl, double pf) {
    /* We want to get this kind of table
         plProbValue tableS_t_knowingS_current_speed[] = {
                1 - PLearn[i], PLearn[i], 0, 0,
                PForget, 1 - PLearn[i] - PForget, PLearn[i], 0,
                0, PForget, 1 - PLearn[i] - PForget, PLearn[i],
                0, 0, PForget, 1 - PForget
        };
     */

    // This table is a square one : P(S(t) | S(t-1), Speed = 0)
    auto new_table = new plProbValue[nb_row * nb_col];

    // The table should be square
    if (nb_row != nb_col) {
        std::cout << "ERROR: table should be square (P(S(t) | S(t-1), Speed = 0)). Return un-initialised table (size = "
                  << nb_row << "x" << nb_col << " = " << (nb_row * nb_col) << "). Expect unexpected behaviour."
                  << std::endl;
        return new_table;
    }

    // Faster to do it this way if table = 2x2
    if (nb_row * nb_col == 4) {
        new_table[0] = 1 - pl;
        new_table[1] = pl;
        new_table[2] = pf;
        new_table[3] = 1 - pf;
        return new_table;
    }

    // We gon' do a little trick here (since we only need to fill the diagonal and the value right above and below)
    // Fill the table with 0s
    for (size_t i = 0; i < nb_col * nb_row; i++) {
        new_table[i] = 0;
    }

    // Then, fill the diagonal and above and below values if applicable
    for (size_t i = 0; i < nb_col; i++) {
        // first col (= no pf value)
        if (i == 0) {
            new_table[Tools::get1dIdxFrom2dIdx(i, i, nb_row)] = 1 - pl;
            new_table[Tools::get1dIdxFrom2dIdx(i, i+1, nb_row)] = pl;
        }

        // last col (= no pl value)
        else if (i == nb_col - 1) {
            new_table[Tools::get1dIdxFrom2dIdx(i, i, nb_row)] =  1 - pf;
            new_table[Tools::get1dIdxFrom2dIdx(i, i-1, nb_row)] = pf;
        }

        else {
            new_table[Tools::get1dIdxFrom2dIdx(i, i-1, nb_row)] = pf;
            new_table[Tools::get1dIdxFrom2dIdx(i, i, nb_row)] = 1 - pl - pf;
            new_table[Tools::get1dIdxFrom2dIdx(i, i+1, nb_row)] = pl;
        }
    }

    /*std::cout << "Plearn = " << pl << " Pforget = " << pf << std::endl;
    for (size_t i = 0; i < nb_col * nb_row; i++) {
        std::cout << new_table[i] << " ";
        if ((i + 1) % nb_col == 0) {
            std::cout << std::endl;
        }
    }*/

    return new_table;
}

plProbValue* BKTModel::getYsTable(size_t nb_row, size_t nb_col, std::vector<double> values) {
    auto new_table = new plProbValue[nb_row * nb_col];

    // If we have more values to put than the number of rows, there's a problem
    if (values.size() > nb_row) {
        std::cout << "ERROR (Ys table): too much values vs number of rows (nb values = " << values.size()
                  << " vs nb rows = " << nb_row << "). Return un-initialised table (size = "
                  << nb_row << "x" << nb_col << " = " << (nb_row * nb_col) << "). Expect unexpected behaviour."
                  << std::endl;

        return new_table;
    }

    for (size_t i = 0; i < nb_col; i++) {
        for (size_t j = 0; j < nb_row; j++) {
            new_table[Tools::get1dIdxFrom2dIdx(i, j, nb_row)] = 0.0;
        }
    }

    // only iterates on the columns
    for (int i = static_cast<int>(nb_col - 1); i >= 0; i--) {

        // If there is enough "space" in the column to put all the tables, do it
        if (values.size() >= (nb_col - 1 - i)) {
            for (size_t k = 0; k < values.size(); k++) {
                new_table[Tools::get1dIdxFrom2dIdx(i, (nb_col - 1 - i) + k, nb_row)] = values[k];
            }
        }

        // else, merge the X first values into one
        else {
            // merge the values into one
            size_t nb_val_to_merge = (nb_col - i) - values.size();
            double merged_values = 0.0;
            for (size_t k = 0; k < nb_val_to_merge; k++) {
                merged_values += values[k];
            }

            // put the value and the rest of the vector in the column
            for (size_t k = 0; k < values.size() - nb_val_to_merge; k++) {
                new_table[Tools::get1dIdxFrom2dIdx(i, i + k, nb_row)] = values[k];
            }

        }

    }

    /*for (size_t i = 0; i < nb_col; i++) {
        for (size_t j = 0; j < nb_row; j++) {
            std::cout << new_table[Tools::get1dIdxFrom2dIdx(i, j, nb_row)] << " ";
        }

        std::cout << std::endl;
    }*/

    return new_table;
}

plProbValue* BKTModel::getIdentityTable(size_t size) {
    auto new_table = new plProbValue[size * size];

    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            if (i == j) {
                new_table[Tools::get1dIdxFrom2dIdx(i, j, size)] = 1;
            }

            else {
                new_table[Tools::get1dIdxFrom2dIdx(i, j, size)] = 0;
            }
        }
    }

    return new_table;
}