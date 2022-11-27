//
// Created by stage on 03/05/22.
//

#ifndef KNOWLEDGE_TRACING_STUDENT_H
#define KNOWLEDGE_TRACING_STUDENT_H

#include <string>
#include <cstdlib>
#include <algorithm> // std::any_of in BKTParams
#include <random>
#include <set>
#include <experimental/filesystem> // used in serialize

#include <pl.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmDynamicBayesianNetwork.h>
#include <pilgrim/general/pm15dBN.h>

#include "genericBKTModel.h"
#include "exercise.h"
#include "exercisesEvaluations.h"
#include "tools.h"
#include "resultStorage.h"
#include "types.h"

// Var name -> evidence value
typedef std::pair<std::string, std::string> VarAndEvidence;

/**
 * The prediction of the BKT for the answer to an exercise. It's either None (no evidence on how the student will answer)
 * or Optimistic (the student will always answer correctly).
 */
enum class AnswersPrediction {
    None, Optimistic
};

/**
 * The metric used to compute the score of each exercise. It's either PGMean (the average level of all skills of the student);
 * or PGMin (the skill in which the student is the weakest).  
 */
enum class Metric {
    PG_min, PG_mean
};

/**
 * A structure which stores both metrics that can be calculated on the exercises (PGMin and PGMean). 
 */
struct PGscore {
    double pgmin = 0.0;
    double pgmean = 0.0;

    PGscore() = default;

    void print() const {
        std::cout << "PGmin = " << pgmin;
        std::cout << " / PGmean = " << pgmean << std::endl;
    }

    void printPGmin() const {
        std::cout << pgmin << ",";
    }

    void printPGmean() const {
        std::cout << pgmean << ",";
    }
};

class Student {
public :

    /**
     * Constructor for the Student class.
     * @params name The name of the student.
     * @params exs The exercises vector.
     */
    Student(const std::string &name, const std::vector<Exercise> &exs);

    /**
     * Getter for the BKT attribute in the Student class.
     * @return The BKT.
     */
    BKTModel &getBKT();

    /**
     * Displays the state of given skill at the current instant.
     * @params skill Skill that we are interested in.
     */
    void getActualSkill(const Skill &skill);

    /**
     * Displays the state of multiple given skills at the current instant.
     * @params skills A vector of skills that we are interested in.
     */
    void getMultipleActualSkills(const std::vector<Skill> &skills);


    /**
     * Displays the state of all skills at the current time.
     */
    void getAllActualSkills();

    void getVarsValues(const std::vector<std::string>& vars);


    /**
     * Calls both getPGMin and getPGMean, creates a new PGScore object and sets its attributes with both values given by the methods (used in the recommendExercise).
     * @params results Predictions given by the DBN used in both called methods.
     * @return Returns a PGScore object which contains the score for both metrics Mean and Min for this exercise.
     */
    PGscore getPGscore(std::vector<plComputableObjectList> &new_results, Exercise &exo);


    /**
     * Using the predictions at time t+1, calculates how the exercise will improve the skills on average and returns that score (used in the getPGscore method).
     * @params results Predictions given by the DBN for each skill of the exercise.
     * @return Returns the score calculated as a double.
     */
    double getPGmean(std::vector<plComputableObjectList> &new_results);


    /**
     * Using the predictions at time t+1, identifies the weakest skill and calculates how this skill will improve with the exercise ( * used in the getPGscore method).
     * @params future Predictions given by the DBN for each skill of the exercise
     * @return Returns the score calculated as a double.
     */
    double getPGmin(std::vector<plComputableObjectList> &future, Exercise &exo);


    /**
     * Finds the skill in the results vector (class member) of the student class and returns its indexes to display it
     * later on in the getAllActualSkills method (used in the getPGMin, getAllActualSkills methods).
     * @params skillName String of the skill that we are interested in finding.
     * @return Returns the indexes of the skill found in the vector (size = 3).
     */
    std::vector<int> getIndexes(const std::string &skillName);

    /**
     * Returns the vector of exercises left
     * @return The vector of exercises left
     */
    std::vector<Exercise>& getExercisesLeft();

    /**
     * Returns the results
     * @return the ResultStorage object
     */
    ResultStorage& getResultStorage();


    /**
     * Sets the BKT for the student, and adds all skills to the ResultStorage (for displaying purpose)
     * @params BKT The new BKT.
     */
    void setBKT(BKTModel &new_BKT);


    /**
     * Finds out if the given skill is already displayed in the getAllActualSkills method (used in the getAllActualSkills method).
     * @params list Vector of Strings containing all the already displayed skills.
     * @params skillName Name of the skill to compare to the other ones of the list.
     * @return Returns a boolean.
     */
    bool isDisplayed(std::vector<std::string> &list, const std::string &skillName);


    /**
     * When creating evidences for the doExercise method one needs to add evidences for the Trigger variable. This method
     * determines if the skill is mobilized in the exercise. If mobilized the evidence is created with 1 value, 0 else.
     * @params trigger Trigger variable to check.
     * @params res Vector of all the mobilized skills in the exercise.
     * @return Returns 1 if mobilized, 0 else.
     */
    int triggerIsUsed(const plVariable& trigger, std::vector<plVariable> &res);


    /**
     * Checks if there if still an exercise not done using the given skill (used in the getPGMin method).
     * @params skill_name Skill to check.
     * @params exercisesNotDone Vector of all the exercises available.
     * @return Returns a boolean.
     */
    bool isAvailable(const std::string &skill_name, std::vector<Exercise> &exercisesNotDone);


    /**
     * Finds the index of the exercise in the vector to be later on erased from the vector of available exercises
     * (used in doExercise).
     * @params ex Exercise to find and later on remove from the vector.
     * @params exercisesNotDone Vector of available exercises.
     * @return Returns the index of the exercise in the vector.
     */
    int exerciseVectorIndex(const Exercise &ex, std::vector<Exercise> &exerciseNotDone);


    void initDBN();

    /**
     * Updates the skills in the 1.5dbn linked to a Student by giving them an exercise, answers to that exercise and
     * stores the results in a vector. Calling this method also increments the time variable and removes the given
     * exercise from the vector of available exercises.
     * @params ex Exercise to do.
     * @params evaluation Answers to the Exercise for each assessed skill (all the skills of the exercise are not
     * necessarily assessed)
     * @return
     */
    void doExercise(const Exercise& ex, const ExoEval& eval, const std::vector<VarAndEvidence>& add_evidences = {});


    /**
     * Recommends the next exercise available to do for a student based on Metric and Answers prediction.
     * @params decision The prediction made on the answer (Optimistic or None).
     * @params pedagogy The computed metric for the exercises (PGMin or PGMean).
     * @return Returns the exercise to be given next.
     */
    Exercise &recommendExercise(AnswersPrediction decision, Metric pedagogy, const std::vector<VarAndEvidence>& add_evidences = {});

    /**
     * Serializes the Student's DBN, 1.5dbn and exercise/skills used to an XML file
     * @param filename The desired output filename
     */
    void serialize(const std::string& filename);

    /**
     * Loads a Student (name, DBN, 1.5dbn and exercise/skills used) from an XML file
     * @param filename the XMl file's name
     */
    void deserialize(const std::string& filename);


private:
    /** @brief Name of the student */
    std::string name;

    /** @brief the BKT model associated to the student */
    BKTModel BKT;

    /** @brief the 1.5 DBN created from the BKT's DBN */
    pm15dBN dbn_15;

    /** @brief current instant for the student (-1 = DBN is not initialised) */
    int instant = -1;

    /** @brief contains all the results of all the queries done for each instant */
    ResultStorage all_results;

    /** @brief contains the results for the queries (should be unused in the current version of the library) */
    // TODO: replace with the ResultStorage object
    std::vector<std::vector<plComputableObjectList>> results;

    std::vector<Exercise> all_exercises;

    /** @brief Vector of exercises still not done by the student */
    std::vector<Exercise> exercisesNotDone;
};

#endif //KNOWLEDGE_TRACING_STUDENT_H