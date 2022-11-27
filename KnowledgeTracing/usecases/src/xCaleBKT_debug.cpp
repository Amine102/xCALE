//
// Created by Boulahmel-A on 15/09/2022.
//

#include "../include/xcale.h"


/* Find a C++ library to connect to database phpmydmin, and automatically create 
   student class inside the code*/

/*Retracer les appels de fonctions sur son exemple pour comprendre les Z*/
 
void xCale_BKT_debug() {

    plError::always_display_warning(false);

    BKTModel xcaleModel;  // Creation of the BKT model
    //MasteryLevel echelle({"1 - M_FRG", "2 - M_SAT", "3 - M_TB"});   // Creation of the skill mastery level. in our case, the skill can be mastered from 1 to 3 (1: M_FRG, 2: M_SAT, 3: M_TB)
    Scale mastery_scale(plLabelType({"1 - insuffisant", "2 - fragile", "3 - satisfaisant", "4 - très bien"}));  // Creation of the skill mastery level. in our case, the skill can be mastered from 1 to 3 (1: M_FRG, 2: M_SAT, 3: M_TB)
    Scale difficulty_scale(plLabelType({"1 - facile", "2 - moyen", "3 - dur"}));
    Scale speed_scale(plLabelType({"1 - nulle", "2 - lent", "3 - moyen", "4 - rapide"}));

    // NOTE : as created skills requires pre-requisites, the order of variable creation would be from the less dependent skill to the most dependent

    Skill C1("S1", mastery_scale, difficulty_scale, speed_scale);

    /*link the skills to the Model : in our setting :
    -----------------------------------------------------------------------------------------------------------------------------------------------------------------
        + PG    = 0.10                   - Initializing Probability of Guessing
        + PInit = (0.80, 0.10,0.08,0.02) - Initializing Prior Probabilities for the four Mastery level
        + PL    = (0.60, 0.10, 0.10)     - Initializing Probabilities of Learning. In this case, all PLs are the same for all mastery level
        + PS    = 0.05                   - Initializing Probability of Slip
        + PF    = 0                      - Initializing Probability of Forgetting Skill. In this setting, we assume that a learner can't forget a skill once learned
       --------------------------------------------------------------------------------------------------------------------------------------------------------------
        + flatten   = 0                  - Initializing Probability of Forgetting Skill. In this setting, we assume that a learner can't forget a skill once learned
    */ 
   
    // Pull la librairie sur Master
    // Je rajoute toutes les compétences dans le modèle 
    // Créer des profils étudiants 
    // 

    xcaleModel.addSkill(C1, {0.8f, 0.1f, 0.08f, 0.02f}, {0.6f, 0.4f, 0.1f}, 0.1f, 0.05f, 0.0f);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
    //create the exercises
    Exercise ex1_1("Melodie_1", difficulty_scale,    "1 - facile");
    Exercise ex2_1("Alternance_1", difficulty_scale, "1 - facile");
    

    // Link the competences with their exercices

    /*Exercice Melodie*/
    ex1_1.linkSkill(C1); /*Observed*/

    /*Exercice Alternance*/
    ex2_1.linkSkill(C1);

    // create the student and liking its genericBKTmodel
    Student Learner_461("461",{ex1_1, ex2_1});
    Learner_461.setBKT(xcaleModel);

    //xcaleModel.getDBN()->summary();

    // create the evaluations for each exercise : 0 for fail, >0 represents the level of skill in which the learner is currently in
    ExercisesEvaluations evals_461(mastery_scale);
    evals_461.addEvaluation(ex1_1, C1, 1);
    evals_461.addEvaluation(ex2_1, C1, 1);

    //std::vector<ExerciseEvaluation> evals = {exEval1, exEval2}; // 2 exercices évalué : MelodieV2, AlternanceV2

    //instanciate the model with any exercise, it won't really be used
    Learner_461.initDBN();
    Learner_461.doExercise(ex1_1, evals_461.getEvaluation(ex1_1));
    std::cout << "------------LEARNER 461----------------" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Displaying probas after first exercice:" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    //print the initial probabilities for each skill
    Learner_461.getAllActualSkills();

    Learner_461.doExercise(ex2_1, evals_461.getEvaluation(ex2_1));
    std::cout << "------------LEARNER 461----------------"  << std::endl;
    std::cout << "---------------------------------------"  << std::endl;
    std::cout << "Displaying probas after second exercice:" << std::endl;
    std::cout << "---------------------------------------"  << std::endl;
    //print the  probabilities for each skill
    Learner_461.getAllActualSkills();

    // Creating a vector of learners
    // ----------------------------------------Learner 162:--------------------------------------------------
    /*Create our learner and link the exercices*/
    Student Learner_162("162",{ex1_1, ex2_1});
    Learner_162.setBKT(xcaleModel);
    /*create the evaluations for each exercise 1 for succeed, 0 for fail*/
    ExercisesEvaluations evals_162(mastery_scale);
    evals_162.addEvaluation(ex1_1, C1, 1);
    evals_162.addEvaluation(ex2_1, C1, 1);

    Learner_162.initDBN();
    Learner_162.doExercise(ex1_1, evals_162.getEvaluation(ex1_1));
    std::cout << "------------LEARNER 162----------------" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Displaying probas after first exercice:" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    //print the initial probabilities for each skill
    Learner_162.getAllActualSkills();
    Learner_162.doExercise(ex2_1, evals_162.getEvaluation(ex2_1));
    std::cout << "------------LEARNER 162----------------" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Displaying probas after second exercice:"<< std::endl;
    std::cout << "---------------------------------------" << std::endl;
    //print the  probabilities for each skill
    Learner_162.getAllActualSkills();

}