//
// Created by couland-q on 08/06/2022.
//

#ifndef KNOWLEDGE_TRACING_EXERCISESEVALUATIONS_H
#define KNOWLEDGE_TRACING_EXERCISESEVALUATIONS_H

#include <vector>
#include <map>
#include <cstddef>
#include "genericBKTModel.h"
#include "scale.h"
#include "skill.h"
#include "types.h"

/**
 * Struct that defines how an exercise is graded.
 */
struct ExercisesEvaluations {

    /** @brief Bunch of "exercise name: <skill_name: evaluation>" */
    std::map<std::string, ExoEval> exos_evals;
    Scale mastery_scale;

    ExercisesEvaluations() = default;

    ExercisesEvaluations(const Scale& mastery_scale):
            mastery_scale(mastery_scale) {}

    /**
     * Constructor for the ExerciseEvaluation class
     * @params exs_evals Contains the answers for each skill assessed in the exercise
     * @params mastery_scale The mastery scale
     */
    ExercisesEvaluations(const std::map<std::string, ExoEval>& exs_evals, const Scale& mastery_scale):
            exos_evals(exs_evals), mastery_scale(mastery_scale) {}

    ExercisesEvaluations(const std::string& exo_name, const std::string& skill_name, const std::string& eval,
                         const Scale& mastery_scale) {

        ExoEval exo_eval;
        exo_eval.insert(std::make_pair(skill_name, eval));
        this->exos_evals.insert(std::make_pair(exo_name, exo_eval));
        this->mastery_scale = mastery_scale;
    }

    /**
     * Getter for the evaluation attribute of the ExerciseEvaluation class.
     * @return Returns the whole vector of evaluation attribute of the ExerciseEvaluation class.
     */
    std::map<std::string, ExoEval>& getEvaluations() {
        return this->exos_evals;
    }

    /**
     * Returns a single exercise evaluation from an exercise name
     * @param exo_name The name of the exercise's evaluation we want to get
     * @return The exercise evaluation
     */
    ExoEval getEvaluation(const std::string& exo_name) {
        if(this->exos_evals.find(exo_name) != this->exos_evals.end()) {
            return this->exos_evals[exo_name];
        }

        else {
            std::cerr << "[ExercisesEvaluations.h] ERROR: exercise " << exo_name << " not found in the evaluations map."
                      << " Returning \"EXERCISE_NOT_FOUND\" as evaluation (expect crashes or weird behaviours)." << std::endl;

            ExoEval exo_eval;
            exo_eval.insert(std::make_pair("exo_name", "EXERCISE_NOT_FOUND"));
            return exo_eval;
        }
    }

    /**
     * Returns the evaluation of an Exercise
     * @param exo The exercise from which we want to get the evaluation
     * @return The exercise evaluation
     */
    ExoEval getEvaluation(const Exercise& exo) {
        return getEvaluation(exo.getName());
    }

    /**
     * Adds an evaluation to the list
     * @param exo_name The exercise name
     * @param skill_name The skill name
     * @param eval The evaluation of the exercise
     */
    void addEvaluation(const std::string& exo_name, const std::string& skill_name, const std::string& eval) {
        auto ret = this->exos_evals[exo_name].insert(std::make_pair(skill_name, eval));

        if (!ret.second) {
            std::cerr << "[exercisesEvaluations.h] WARNING: exercise " << exo_name << " already exists in list. "
                      << "New value (\"" << eval << "\") has not been set (keeping old values)." << std::endl;
        }
    }

    /**
     * Adds an evaluation to the list
     * @param exo The exercise
     * @param skill The skill
     * @param eval_idx The index of the evaluation in the mastery scale
     */
    void addEvaluation(const Exercise& exo, const Skill& skill, size_t eval_idx) {
        addEvaluation(exo.getName(), skill.name, this->mastery_scale.getValueFromIdx(eval_idx));
    }

    /**
     * Adds an evaluation to the list
     * @param exo The exercise
     * @param skill The skill
     * @param eval The evaluation of the skill
     */
    void addEvaluation(const Exercise& exo, const Skill& skill, const std::string& eval) {
        addEvaluation(exo.getName(), skill.name, eval);
    }

    /**
     * Adds a list of evaluations to the list
     * @param exo_name The exercise name
     * @param all_evals All the evaluations to add
     */
    void addEvaluation(const std::string& exo_name, const ExoEval& all_evals) {
        for (auto& eval: all_evals) {
            addEvaluation(exo_name, eval.first, eval.second);
        }
    }

    /**
     * Adds a list of evaluations to the list
     * @param exo_name The exercise name
     * @param all_evals All the evaluations to add (map of Skill and eval as string)
     */
    void addEvaluation(const Exercise& exo_name, const std::map<Skill, std::string>& all_evals) {
        for (auto& eval: all_evals) {
            addEvaluation(exo_name, eval.first, eval.second);
        }
    }

    /**
     * Adds a list of evaluations to the list
     * @param exo_name The exercise name
     * @param all_evals All the evaluations to add (map of Skill and eval index)
     */
    void addEvaluation(const Exercise& exo_name, const std::map<Skill, size_t>& all_evals) {
        for (auto& eval: all_evals) {
            addEvaluation(exo_name, eval.first, eval.second);
        }
    }
};

#endif //KNOWLEDGE_TRACING_EXERCISESEVALUATIONS_H
