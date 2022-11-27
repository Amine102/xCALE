//
// Created by couland-q on 08/06/2022.
//

#ifndef KNOWLEDGE_TRACING_EXERCISE_H
#define KNOWLEDGE_TRACING_EXERCISE_H

#include <string>
#include <vector>

#include "skill.h"


class Exercise {
public :

    /**
     * Default constructor for the Exercise class.
     */
    Exercise();

    /**
     * Constructor for the Exercise class.
     * @params name Name of the exercise.
     */
    explicit Exercise(const std::string& name, const Scale& difficulty_scale, const std::string& difficulty = "None");

    /**
     * Destructor of the Exercise class.
     */
    ~Exercise();

    /**
     * Getter for the name attribute of the Exercise class.
     * @return Returns the name attribute of the Exercise class.
     */
    const std::string& getName() const;

    /**
     * Getter for the skills_used attribute of the Exercise class.
     * @return Returns the skills_used attribute of the Exercise class.
     */
    const std::vector<Skill>& getSkillsUsed() const;

    /**
     * Get a specific skill from the list of mobilized skills in the exercise.
     * @params idx Index of the skill to return.
     * @return Returns the skill at the given index.
     */
    const Skill& getSkill(size_t idx) const;

    /**
     * Getter for the difficulty level of the exercise
     * @return Returns the difficulty level of the exercise.
     */
    const std::string& getDifficulty() const;

    const Scale& getDifficultyScale() const;

    /**
     * Setter for the name attribute of the Exercise class.
     * @params Sets the name attribute of the Exercise class.
     */
    void setName(const std::string& name);

    void setDifficulty(const std::string& difficulty);

    void setDifficulty(int idx);

    /**
     * Links a skill to an exercise. Linked skills can be assessed later on using doExercise().
     * @params skill Skill to link.
     */
    void linkSkill(Skill& skill);

    /**
      * Links a skill to an exercise. Linked skills can be assessed later on using doExercise().
      * @params S Skill to link.
      */
    void linkSkills(const std::vector<Skill>& v_s);

    bool isSkillUsed(const std::string& skill_name) const;

    friend std::ostream& operator <<(std::ostream& os, const Exercise& ex);

private:
    std::string name;
    std::vector<Skill> skills_used;
    std::string difficulty;
    Scale difficulty_scale;
};


#endif //KNOWLEDGE_TRACING_EXERCISE_H
