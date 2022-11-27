//
// Created by couland-q on 08/11/22.
//

#include "../include/prerequisite.h"

namespace UsecasePrerequisite {
    //TODO: multiple exercise that mobilize the same skill, which must be a prerequisite for another skill
    void prerequisite() {

        plError::always_display_warning(false);

        //create the genericBKTModel
        BKTModel genericModel;
        Scale mastery_scale(plLabelType({"1 - insuffisant", "2 - fragile", "3 - satisfaisant", "4 - très bien"}));
        Scale difficulty_scale(plLabelType({"1 - facile", "2 - moyen", "3 - dur"}));
        Scale speed_scale(plLabelType({"1 - nulle", "2 - lent", "3 - moyen", "4 - rapide"}));

        Skill C1("S1", mastery_scale, difficulty_scale, speed_scale);
        Skill C2("S2", mastery_scale, difficulty_scale, speed_scale, {C1});
        Skill C3("S3", mastery_scale, difficulty_scale, speed_scale, {C2});

        std::vector<double> p_init = {0.80, 0.10, 0.08, 0.02};
        std::vector<double> p_learn = {0.1, 0.3, 0.6};
        double pg = 0.1;
        double ps = 0.05;
        double pf = 0.0;
        std::vector<double> Zi{1, 0, 0, 0,
                               0, 1, 0, 0,
                               0, 0, 1, 0,
                               0, 0, 0, 1};

        //link the skills to the Model
        genericModel.addSkill(C1, p_init, p_learn, pg, ps, pf);
        genericModel.addSkill(C2, p_init, p_learn, pg, ps, pf, {Zi});
        genericModel.addSkill(C3, p_init, p_learn, pg, ps, pf, {Zi});

        // Create the exercises and link the skills for each exercise
        Exercise ex1("ex1", difficulty_scale, difficulty_scale.scale_values.get_values()[0]);
        Exercise ex2("ex2", difficulty_scale, difficulty_scale.scale_values.get_values()[1]);
        Exercise ex3("ex3", difficulty_scale, difficulty_scale.scale_values.get_values()[2]);
        ex1.linkSkill(C1);
        ex2.linkSkill(C2);
        ex3.linkSkills({C1, C3});

        //create the student and liking its genericBKTmodel
        Student Pierre("Pierre", {ex1, ex2, ex3});
        Pierre.setBKT(genericModel);

        //genericModel.getDBN()->summary();

        //create the evaluations for each exercise 1 for succeed, 0 for fail
        ExercisesEvaluations evals(mastery_scale);
        evals.addEvaluation(ex1, C1, mastery_scale.scale_values.get_values()[0]);
        evals.addEvaluation(ex2, C2, mastery_scale.scale_values.get_values()[3]);
        evals.addEvaluation(ex3, {{C1, mastery_scale.scale_values.get_values()[1]},
                                  {C3, mastery_scale.scale_values.get_values()[2]}});

        std::vector<Exercise> recommendedExercises;

        //instanciate the model
        Pierre.initDBN();

        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Displaying probas after initialisation:" << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        //print the initial probabilities for each skill
        Pierre.getAllActualSkills();
        //genericModel.getDBN()->summary();

        //Student does the recommended exercise
        Pierre.doExercise(ex1, evals.getEvaluation(ex1));

        std::cout << "----------------------------------" << std::endl;
        std::cout << "Displaying probas after first exo:" << std::endl;
        std::cout << "----------------------------------" << std::endl;
        Pierre.getAllActualSkills();


        Pierre.doExercise(ex2, evals.getEvaluation(ex2));
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Displaying probas after second exo:" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        Pierre.getAllActualSkills();

        Pierre.doExercise(ex3, evals.getEvaluation(ex3));

        std::cout << "----------------------------------" << std::endl;
        std::cout << "Displaying probas after third exo:" << std::endl;
        std::cout << "----------------------------------" << std::endl;
        Pierre.getAllActualSkills();
    }
}