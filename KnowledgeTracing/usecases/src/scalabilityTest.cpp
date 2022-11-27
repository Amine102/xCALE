//
// Created by couland-q on 05/05/2022.
//

#include "../include/scalabilityTest.h"


namespace UsecaseScalability {
    void ScalabilityTest() {
        plError::always_display_warning(false);

        Tools::Time::Timers timers;

        std::vector<size_t> number_of_skills{3/*, 5, 10, 20, 30*/};
        std::vector<size_t> number_of_exercises{8/*, 15, 30, 50 100*/};

        for (size_t nb_skills = 0; nb_skills < number_of_skills.size(); nb_skills++) {
            for (size_t nb_exos = 0; nb_exos < number_of_exercises.size(); nb_exos++) {

                /*std::cout << std::endl << "--------------------------------" << std::endl;
                std::cout <<              "NEW SET OF SKILLS AND EXERCISES:" << std::endl;
                std::cout <<              "--------------------------------" << std::endl;*/
                std::cout << std::to_string(number_of_skills[nb_skills]) << " Skills" << std::endl;
                std::cout << std::to_string(number_of_exercises[nb_exos]) << " Exercises" << std::endl;

                Scale scale;
                BKTModel genericBKT;
                createScalableBKT(number_of_skills[nb_skills], scale, genericBKT);
                std::vector<UsecaseProgressiveBKT::Exercise> exo_vector = getExercises(number_of_exercises[nb_exos], number_of_skills[nb_skills], scale);

                std::vector<std::string> variables_to_query;

                std::string skill_name;
                for (size_t k = 0; k < number_of_skills[nb_skills]; k++) {
                    skill_name = std::to_string(k);
                    variables_to_query.emplace_back(skill_name);
                }

                pmDynamicBayesianNetwork* DBN = genericBKT.getDBN();

                timers.addParams(Tools::Time::Params(number_of_skills[nb_skills], number_of_exercises[nb_exos]));

                // Redirects std::cout into the  V O I D
                std::streambuf* orig_buf = std::cout.rdbuf();
                std::cout.rdbuf(nullptr);
                timers.addTimes(progressiveQuery(DBN, exo_vector, variables_to_query, UsecaseProgressiveBKT::AnswersPrediction::None, UsecaseProgressiveBKT::Metric::PG_mean));
                // re-sets std::cout to console output
                std::cout.rdbuf(orig_buf);

                std::cout << "Total times: query = " << timers.getLastTime().getAllTime("query") << "s,"
                          << " search exercise = " << timers.getLastTime().getAllTime("search_exo") << "s." << std::endl;

                /*std::cout << "----" << std::endl;
                std::cout << "OVER" << std::endl;
                std::cout << "----" << std::endl;*/

            }
        }

        //timers.displayTimesForSpreadsheet();
        timers.writeToCSV("/home/couland-q/Code/C++/KnowledgeTracing/data/30_skills_100_exos_new.csv");

    }

    void createScalableBKT(size_t number_of_skills, Scale scale, BKTModel& genericBKT) {
        std::string skill_name;

        for (size_t i = 0; i < number_of_skills; i++) {
            skill_name = std::to_string(i);
            //genericBKT.addSkill(Skill(skill_name, scale), 0.1, std::vector<double>{0.95, 0.05}, 0.1, 0.05, 0);
        }
    }

    std::vector<UsecaseProgressiveBKT::Exercise> getExercises(size_t number_of_exercises, size_t number_of_skills, Scale scale) {
        std::vector<UsecaseProgressiveBKT::Exercise> exo_vector;
        std::string exo_name;

        auto random_skill = std::bind(std::uniform_int_distribution<>(0, number_of_skills - 1), std::default_random_engine());
        auto gen_skill = std::bind(std::uniform_int_distribution<>(0, scale.getNumberOfValues() - 1), std::default_random_engine());
        auto number_of_mobilised_skills = std::bind(std::uniform_int_distribution<>(1, 3), std::default_random_engine());

        for (size_t i = 0; i < number_of_exercises; i++) {
            size_t nb_skills = number_of_mobilised_skills();

            std::vector<bool> skills(number_of_skills, false);
            std::vector<int> answers(number_of_skills, -1);

            for (size_t j = 0; j < nb_skills; j++) {
                size_t idx = random_skill();
                skills[idx] = true;
                answers[idx] = gen_skill();
            }

            exo_name = "Exo_" + std::to_string(i);

            exo_vector.emplace_back(UsecaseProgressiveBKT::Exercise(exo_name, skills, answers));

        }

        return exo_vector;
    }
}
