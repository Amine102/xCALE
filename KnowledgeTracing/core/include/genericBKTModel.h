
#ifndef PILGRIM_GENERAL_GENERICBKTMODEL_H
#define PILGRIM_GENERAL_GENERICBKTMODEL_H

#include <string>
#include <cstddef> // size_t
#include <algorithm>

#include <pl.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmDynamicBayesianNetwork.h>
#include <pilgrim/general/pm15dBN.h>

#include "skill.h"
#include "scale.h"
#include "tools.h"

/**
 * Class that defines BKTModel which will later on be linked to a Student.
 * Contains all the skills that the Student will learn and will be updated as he does exercises.
 */
class BKTModel{
public:

    /**
     * Default constructor for the BKTModel class.
     */
    BKTModel();

    /**
     * Copy constructor for the BKTModel class
     * @param o_bkt The BKt to copy
     */
    BKTModel(BKTModel& o_bkt);


    /**
     * Default destructor for the BKTModel class.
     */
    ~BKTModel();


    /**
     * Getter for the List_comp attribute.
     * @return Returns the list_skills attribute of the BKTModel class.
     */
    std::vector<Skill>& getSkillList();

    /**
     * Getter for the DBN attribute.
     * @return Returns the DBN attribute of the BKTModel class.
     */
    pmDynamicBayesianNetwork* getDBN();

    void setDBN(pmDynamicBayesianNetwork* dbn);

    std::string getHighestMasteryValue();

    void addSkillToVector(Skill& skill);

    /**
     * Adds a skill to the DBN and all its linked variables (Res, Trigger) with specified parameters for the probability tables.
     * @param new_skill The Skill to add to the DBN.
     * @param PInit Initial probability of having the skill (If 2 levels of mastery then { 1-Pinit, Pinit} otherwise it needs to be a vector).
     * @param PLearn Probability to go up a level of mastery.
     * @param PGuess Probability to answer correctly to an exercise for which the student has a lower level of mastery.
     * @param PSlip Probability to answer wrong to an exercise which uses a skill for which the student has a higher level of mastery.
     * @param PForget Probability to forget the mastery of a skill (going from 2 to 1 if there is 2 levels of mastery).
     * @param input_Zi_table The Zi tables to use for the pre-requisites.
     */
    void addSkill(const Skill& new_skill, const std::vector<double>& PInit, const std::vector<double>& PLearn, double PGuess,
                  double PSlip, double PForget, std::vector<std::vector<double>> input_Zi_table = {{}});

    /**
     * Creates the variables and set their tables for T0.
     * @param t0_tables The object that will contain the tables.
     * @param new_skill The Skill to add to the DBN.
     * @param PInit Initial probability of having the skill (If 2 levels of mastery then { 1-Pinit, Pinit} otherwise it needs to be a vector).
     * @param PGuess Probability to answer correctly to an exercise for which the student has a lower level of mastery.
     * @param Pslip Probability to answer wrong to an exercise which uses a skill for which the student has a higher level of mastery.
     * @param input_Zi_table The Zi tables to use for the pre-requisites.
     */
    void setT0Tables(plComputableObjectList& t0_tables, const Skill& new_skill, const std::vector<double>& PInit,
                     double PGuess, double Pslip, std::vector<std::vector<double>> input_Zi_table = {{}});

    /**
     * Creates the variables and set their tables for T-1 and T.
     * @param t_tables The object that will contain the tables.
     * @param new_skill The Skill to add to the DBN.
     * @param PInit Initial probability of having the skill (If 2 levels of mastery then { 1-Pinit, Pinit} otherwise it needs to be a vector).
     * @param PLearn Probability to go up a level of mastery.
     * @param PGuess Probability to answer correctly to an exercise for which the student has a lower level of mastery.
     * @param Pslip Probability to answer wrong to an exercise which uses a skill for which the student has a higher level of mastery.
     * @param PForget Probability to go back to a lower level of mastery.
     * @param input_Zi_table The Zi tables to use for the pre-requisites.
     */
    void setTTables(plComputableObjectList& t_tables, const Skill& new_skill, const std::vector<double>& PInit,
                    const std::vector<double>& PLearn, double PGuess, double PSlip, double PForget,
                    std::vector<std::vector<double>> input_Zi_table = {{}});

    /**
     * Creates and returns a Res proba table with the appropriate coefficient for Pguess and Pslip, regarding the size of the table.
     * Generated coefficients are linear for now.
     * @param nb_row Number of row for the proba table
     * @param nb_col Number of column of the proba table
     * @param Pguess The original value for Pguess
     * @param Pslip The original value for Pslip
     * @return The Res proba table
     */
    plProbValue* getResTableWithCoefs(size_t nb_row, size_t nb_col, double Pguess, double Pslip);

    /**
     * Creates and returns a Plearn proba table.
     * @param nb_row Number of row for the proba table
     * @param nb_col Number of column of the proba table
     * @param Plearn Plearn value
     * @param Pforget Pforget value
     * @return The Plearn proba table
     */
    plProbValue* getPlearnTable(size_t nb_row, size_t nb_col, double Plearn, double Pforget);

    /**
     * Creates and returns an Ys proba table. Very ad-hoc for the moment.
     * @param nb_row Number of row for the proba table
     * @param nb_col Number of column of the proba table
     * @param values All the values to put in the table
     * @return The Ys table proba table
     */
    plProbValue* getYsTable(size_t nb_row, size_t nb_col, std::vector<double> values);

    /**
     * Create and returns an identity matrix.
     * @param size Size of the matrix (identity matrix are square)
     * @return A size*size identity matrix
     */
    plProbValue* getIdentityTable(size_t size);

private:
    pmDynamicBayesianNetwork* DBN;
    // TODO: list of strings, probably?
    std::vector<Skill> list_skills;

    //noisyMax code (in order to avoid c/c'ing it everywhere)
    std::string noisy_max_code =
        "void my_max(double* output, unsigned int output_size,\n"
        " double* input, unsigned int input_size)\n"
        "{\n"
        " double* vectMax = output;\n"
        " double* vectProb = input;\n"
        "\n"
        " unsigned int n_probas;\n"
        " double value_max=vectProb[0]; \n"
        " unsigned int i;\n"
        " \n"
        " n_probas = input_size; \n"
        "\n"
        " for(i=0;i<n_probas;i++) \n"
        " if (vectProb[i]>value_max) value_max=vectProb[i];\n"
        " vectMax[0]=value_max; \n"
        "}\n";
};


#endif