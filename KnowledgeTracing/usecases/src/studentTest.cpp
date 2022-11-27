

#include "../include/studentTest.h"

namespace UsecaseStudent {
    void studentTest() {

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
        // WARNING: put the "real" value of the difficulty, NOT the index
        // e.g. let use a scale of 1, 2, 3 in terms of difficulty, use
        // 1 for the first difficulty (and not 0), 2 for the second, etc.
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
        evals.addEvaluation(ex1, C1, 0);
        evals.addEvaluation(ex2, C2, 1);
        evals.addEvaluation(ex3, {{C1, 1},
                                  {C3, 0}});

        std::vector<Exercise> recommendedExercises;

        // AnswersPrediction
        // None : no evidences on the results of the exercise.
        // Optimistic : the student always answers correctly on the prediction.
        AnswersPrediction ans = AnswersPrediction::Optimistic;

        // Metric
        // Mean : maximize the average level of mastery of all skills.
        // Min : maximize the lowest level of mastery of the skills.
        Metric met = Metric::PG_min;

        //instanciate the model with any exercise, it won't really be used
        Pierre.initDBN();

        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Displaying probas after initialisation:" << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        //print the initial probabilities for each skill
        Pierre.getAllActualSkills();
        //genericModel.getDBN()->summary();

        //recommend the next exercise to do
        Exercise recommendedFirst = Pierre.recommendExercise(ans, met);
        recommendedExercises.push_back(recommendedFirst);
        std::cout << "Recommended exercise is : " << recommendedFirst.getName() << std::endl;

        //Student does the recommended exercise
        Pierre.doExercise(recommendedFirst, evals.getEvaluation(recommendedFirst));

        std::cout << "----------------------------------" << std::endl;
        std::cout << "Displaying probas after first exo:" << std::endl;
        std::cout << "----------------------------------" << std::endl;
        Pierre.getAllActualSkills();

        Exercise recommended = Pierre.recommendExercise(ans, met);
        recommendedExercises.push_back(recommended);
        std::cout << "Recommended exercise is : " << recommended.getName() << std::endl;


        Pierre.doExercise(recommended, evals.getEvaluation(recommended));
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Displaying probas after second exo:" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        Pierre.getAllActualSkills();

        Exercise recommended2 = Pierre.recommendExercise(ans, met);
        recommendedExercises.push_back(recommended2);
        std::cout << "Recommended exercise is : " << recommended2.getName() << std::endl;

        Pierre.doExercise(recommended2, evals.getEvaluation(recommended2));

        std::cout << "----------------------------------" << std::endl;
        std::cout << "Displaying probas after third exo:" << std::endl;
        std::cout << "----------------------------------" << std::endl;
        Pierre.getAllActualSkills();
    }
}