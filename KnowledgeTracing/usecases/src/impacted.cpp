//
// Created by couland-q on 26/09/2022.
//

#include "../include/impacted.h"
namespace UsecaseImpacted {
    void usecase() {
        plError::always_display_warning(false);

        BKTModel genericModel;

        Scale mastery_scale(plLabelType({"1 - Insuffisant", "2 - Satisfaisant"}));
        Scale difficulty_scale(plLabelType({"1 - facile", "2 - moyen", "3 - dur"}));
        Scale speed_scale(plLabelType({"1 - nulle", "2 - lent", "3 - moyen", "4 - rapide"}));

        Skill C1("nombres", mastery_scale, difficulty_scale, speed_scale);
        Skill C2("calcul-literal", mastery_scale, difficulty_scale, speed_scale);
        Skill C3("demarche-scientifique", mastery_scale, difficulty_scale, speed_scale);

        float p01 = 0.05;
        float p02 = 0.02;
        float p03 = 0.01;
        float pl = 0.1;
        float pg = 0.1;
        float ps = 0.05;
        float pf = 0.0;

        // 3 Plearn values, because of 4 speed values, and speed == 0 is identity tables

        //link the skills to the Model
        // skill, pinit table, plearn, pguess, pslip, pforget, Zi (if any)
        genericModel.addSkill(C1, {1 - p01, p01}, {pl * 0.7, pl * 0.2, pl * 0.1}, pg, ps, pf);
        genericModel.addSkill(C2, {1 - p02, p02}, {pl * 0.7, pl * 0.2, pl * 0.1}, pg, ps, pf);
        genericModel.addSkill(C3, {1 - p03, p03}, {pl * 0.7, pl * 0.2, pl * 0.1}, pg, ps, pf);

        Exercise ex1("ex1", difficulty_scale, "2 - moyen");
        Exercise ex2("ex2", difficulty_scale, "2 - moyen");
        Exercise ex3("ex3", difficulty_scale, "2 - moyen");
        Exercise ex4("ex4", difficulty_scale, "2 - moyen");
        Exercise ex5("ex5", difficulty_scale, "2 - moyen");
        Exercise ex6("ex6", difficulty_scale, "2 - moyen");
        Exercise ex7("ex7", difficulty_scale, "2 - moyen");
        Exercise ex8("ex8", difficulty_scale, "2 - moyen");

        ex1.linkSkill(C1);
        ex2.linkSkill(C1);
        ex3.linkSkill(C1);
        ex4.linkSkills({C1, C3});
        ex5.linkSkills({C1, C2});
        ex6.linkSkills({C1, C2});
        ex7.linkSkills({C2, C3});
        ex8.linkSkill(C3);

        std::vector<Exercise> exs = {ex1, ex2, ex3, ex4, ex5, ex6, ex7, ex8};

        Learner Pierre("Pierre", exs);
        Pierre.setBKT(genericModel);

        ExercisesEvaluations evals(mastery_scale);
        evals.addEvaluation(ex1, C1, 1);
        evals.addEvaluation(ex2, C1, 1);
        evals.addEvaluation(ex3, C1, 0);
        evals.addEvaluation(ex4, {{C1, 1},
                                  {C3, 0}});
        evals.addEvaluation(ex5, {{C1, 1},
                                  {C2, 0}});
        evals.addEvaluation(ex6, {{C1, 0},
                                  {C2, 1}});
        evals.addEvaluation(ex7, {{C2, 1},
                                  {C3, 0}});
        evals.addEvaluation(ex8, C3, 0);

        std::vector<Exercise> recommended_exercices;
        // Evidences on the speed
        std::vector<VarAndEvidence> speed_ev;
        speed_ev.emplace_back(std::make_pair("Speed_nombres", "3 - moyen"));
        speed_ev.emplace_back(std::make_pair("Speed_calcul-literal", "3 - moyen"));
        speed_ev.emplace_back(std::make_pair("Speed_demarche-scientifique", "3 - moyen"));

        // Initialising the DBN
        Pierre.initDBN();

        //Pierre.getAllActualSkills();

        AnswersPrediction answer_pred = AnswersPrediction::Optimistic;
        Metric metric = Metric::PG_mean;


        for (size_t i = 0; i < 8; i++) {
            Exercise recommendedExercise = Pierre.recommendExercise(answer_pred, metric, speed_ev);
            recommended_exercices.push_back(recommendedExercise);
            std::cout << "Recommended exercise : " << recommendedExercise.getName() << std::endl;
            Pierre.doExercise(recommendedExercise, evals.getEvaluation(recommendedExercise), speed_ev);
            //Pierre.getAllActualSkills();
        }

        for (size_t i = 0; i < 8; i++) {
            if (i == 0) {
                std::cout << "L'enchainement d'exercices recommandé est le suivant : ";
            }

            std::cout << recommended_exercices[i].getName() << "-";
        }
    }
}
// Original deprecated code for reference:
/*
    plError::always_display_warning(false);

    BKTModel genericModel;
    Scale echelle;

    Skill C1("nombres", echelle);
    Skill C2("calcul-literal", echelle);
    Skill C3("demarche-scientifique", echelle);

    genericModel.addSkill(C1, 0.1, {0.05},0.1, 0.05, 0);
    genericModel.addSkill(C2, 0.1, {0.02},0.1, 0.05, 0);
    genericModel.addSkill(C3, 0.1, {0.01},0.1, 0.05, 0);

    Exercise ex1("ex1");
    Exercise ex2("ex2");
    Exercise ex3("ex3");
    Exercise ex4("ex4");
    Exercise ex5("ex5");
    Exercise ex6("ex6");
    Exercise ex7("ex7");
    Exercise ex8("ex8");




    //Pierre->setBKT(genericModel);


    ex1.linkCompetence(C1);
    ex2.linkCompetence(C1);
    ex3.linkCompetence(C1);
    ex4.linkCompetence(C1);
    ex4.linkCompetence(C3);
    ex5.linkCompetence(C1);
    ex5.linkCompetence(C2);
    ex6.linkCompetence(C1);
    ex6.linkCompetence(C2);
    ex7.linkCompetence(C2);
    ex7.linkCompetence(C3);
    ex8.linkCompetence(C3);

    std::vector<Exercise> exs = {ex1, ex2, ex3, ex4, ex5, ex6, ex7, ex8};

    Leaner Pierre("Pierre", exs);
    Pierre.setBKT(genericModel);

    std::map<std::string, int> eval1;
    eval1[C1.name] = 0;

    std::map<std::string, int> eval2;
    eval2[C1.name] = 1;

    std::map<std::string, int> eval3;
    eval3[C1.name] = 0;

    std::map<std::string, int> eval4;
    eval4[C1.name] = 0;
    eval4[C3.name] = 0;

    std::map<std::string, int> eval5;
    eval5[C1.name] = 1;
    eval5[C2.name] = 0;

    std::map<std::string, int> eval6;
    eval6[C1.name] = 0;
    eval6[C2.name] = 0;

    std::map<std::string, int> eval7;
    eval7[C2.name] = 1;
    eval7[C3.name] = 1;

    std::map<std::string, int> eval8;
    eval8[C3.name] = 1;



    ExerciseEvaluation exEval1(eval1);
    ExerciseEvaluation exEval2(eval2);
    ExerciseEvaluation exEval3(eval3);
    ExerciseEvaluation exEval4(eval4);
    ExerciseEvaluation exEval5(eval5);
    ExerciseEvaluation exEval6(eval6);
    ExerciseEvaluation exEval7(eval7);
    ExerciseEvaluation exEval8(eval8);

    std::vector<ExerciseEvaluation> exos_evals = {exEval1, exEval2, exEval3, exEval4, exEval5, exEval6, exEval7, exEval8};
    std::vector<Exercise> recommendedExercises;


    Pierre.doExercise(ex1, exEval1);
    Pierre.getAllActualSkills();

    AnswersPrediction ans = AnswersPrediction::Optimistic;
    Metric met = Metric::PG_min;

    Exercise recommendedFirst = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommendedFirst);
    std::cout << "ex recommended : " << recommendedFirst.getName() << std::endl;

    Pierre.doExercise(recommendedFirst, exos_evals[recommendedFirst.getName().back()-'0'-1]);


    Exercise recommended = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommended);
    std::cout << "ex recommended : " << recommended.getName() << std::endl;

    Pierre.getAllActualSkills();
    Pierre.doExercise(recommended, exos_evals[recommended.getName().back()-'0'-1]);



    Exercise recommended2 = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommended2);
    std::cout << "ex recommended : " << recommended2.getName() << std::endl;

    Pierre.getAllActualSkills();
    Pierre.doExercise(recommended2, exos_evals[recommended2.getName().back()-'0'-1]);



    Exercise recommended3 = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommended3);
    std::cout << "ex recommended : " << recommended3.getName() << std::endl;

    Pierre.getAllActualSkills();
    Pierre.doExercise(recommended3, exos_evals[recommended3.getName().back()-'0'-1]);



    Exercise recommended4 = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommended4);
    std::cout << "ex recommended : " << recommended4.getName() << std::endl;

    Pierre.getAllActualSkills();
    Pierre.doExercise(recommended4, exos_evals[recommended4.getName().back()-'0'-1]);



    Exercise recommended5 = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommended5);
    std::cout << "ex recommended : " << recommended5.getName() << std::endl;

    Pierre.getAllActualSkills();
    Pierre.doExercise(recommended5, exos_evals[recommended5.getName().back()-'0'-1]);



    Exercise recommended6 = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommended6);
    std::cout << "ex recommended : " << recommended6.getName() << std::endl;

    Pierre.getAllActualSkills();
    Pierre.doExercise(recommended6, exos_evals[recommended6.getName().back()-'0'-1]);



    Exercise recommended7 = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommended7);
    std::cout << "ex recommended : " << recommended7.getName() << std::endl;

    Pierre.getAllActualSkills();
    Pierre.doExercise(recommended7, exos_evals[recommended7.getName().back()-'0'-1]);

    Pierre.getAllActualSkills();

    for(size_t i=0; i<8; i++){
        if(i==0){
            std::cout << "l'enchainement d'exercice recommandés est le suivant : ";
        }
        std::cout <<  recommendedExercises[i].getName() << "-" ;
    }

*/

