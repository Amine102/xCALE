//
// Created by couland-q on 17/10/22.
//

#include "../include/serialize_deserialize.h"

namespace UsecaseSerializeDeserialize {
    void usecase() {
        plError::always_display_warning(false);

        std::string filename = "dirty_test";

        Scale mastery_scale(plLabelType({"1 - insuffisant", "2 - fragile", "3 - satisfaisant", "4 - tres bien"}));
        Scale difficulty_scale(plLabelType({"1 - facile", "2 - moyen", "3 - dur"}));
        Scale speed_scale(plLabelType({"1 - nulle", "2 - lent", "3 - moyen", "4 - rapide"}));

        Student pierre = createStudent(mastery_scale, difficulty_scale, speed_scale);

        serialize(pierre, filename);

        std::vector<Exercise> exo_deserialized = Tools::loadExercisesFromCSV("dirty_test", pierre.getBKT().getSkillList());
        std::cout << "Deserialized exo: " << std::endl;
        for (auto& exo: exo_deserialized) {
            std::cout << exo << std::endl;
        }

        Student new_pierre("New Pierre", exo_deserialized);
        deserialize(new_pierre, filename);

        new_pierre.initDBN();

        //std::vector<Exercise> exos = getExos(difficulty_scale, new_pierre.getBKT().getSkillList());
        ExercisesEvaluations evals = getEvals(mastery_scale, new_pierre.getBKT().getSkillList(), exo_deserialized);

        new_pierre.doExercise(exo_deserialized[0], evals.getEvaluation(exo_deserialized[0]));
        new_pierre.doExercise(exo_deserialized[1], evals.getEvaluation(exo_deserialized[1]));
        new_pierre.doExercise(exo_deserialized[2], evals.getEvaluation(exo_deserialized[2]));


    }

    void serialize(Student& student, const std::string& filename) {
        student.serialize(filename);
    }

    void deserialize(Student& student, const std::string& filename) {
        student.deserialize(filename);
    }

    // Code taken from testEtudiant.cpp
    Student createStudent(Scale& mastery_scale, Scale& difficulty_scale, Scale& speed_scale) {
        //create the genericBKTModel
        BKTModel genericModel;

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

        Tools::writeExercisesToCSV({ex1, ex2, ex3}, "dirty_test");

        //create the student and liking its genericBKTmodel
        Student Pierre("Pierre", {ex1, ex2, ex3});
        Pierre.setBKT(genericModel);

        Pierre.initDBN();

        //create the evaluations for each exercise 1 for succeed, 0 for fail
        //create the evaluations for each exercise 1 for succeed, 0 for fail
        ExercisesEvaluations evals(mastery_scale);
        evals.addEvaluation(ex1, C1, 0);
        evals.addEvaluation(ex2, C2, 1);
        evals.addEvaluation(ex3, {{C1, 1}, {C3, 0}});

        Pierre.doExercise(ex1, evals.getEvaluation(ex1));
        Pierre.doExercise(ex2, evals.getEvaluation(ex2));
        Pierre.doExercise(ex3, evals.getEvaluation(ex3));

        return Pierre;
    }

    std::vector<Exercise> getExos(Scale difficulty_scale, std::vector<Skill> skills) {
        Exercise ex1("ex1", difficulty_scale, difficulty_scale.scale_values.get_values()[0]);
        Exercise ex2("ex2", difficulty_scale, difficulty_scale.scale_values.get_values()[1]);
        Exercise ex3("ex3", difficulty_scale, difficulty_scale.scale_values.get_values()[2]);
        ex1.linkSkill(skills[0]);
        ex2.linkSkill(skills[1]);
        ex3.linkSkills({skills[0], skills[2]});

        return {ex1, ex2, ex3};
    }

    ExercisesEvaluations getEvals(Scale mastery_scale, std::vector<Skill> skills, std::vector<Exercise> exos) {
        ExercisesEvaluations evals(mastery_scale);
        evals.addEvaluation(exos[0], skills[0], 0);
        evals.addEvaluation(exos[1], skills[1], 1);
        evals.addEvaluation(exos[2], {{skills[0], 1}, {skills[2], 0}});

        return evals;
    }
}