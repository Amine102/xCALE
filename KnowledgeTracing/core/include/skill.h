//
// Created by couland-q on 24/10/22.
//

#ifndef KNOWLEDGE_TRACING_SKILL_H
#define KNOWLEDGE_TRACING_SKILL_H

#include <string>
#include <vector>

#include "scale.h"

/**
 * Struct that defines a skill by its level of mastery (1 or 2 for now) and its name.
 */
struct Skill{
    Scale mastery_scale;
    Scale difficulty_scale;
    Scale speed_scale;
    std::string name;
    std::vector<std::string> prerequisites;

    /**
     * Constructor for the Skill class.
     * @params scale Level of mastery of a skill (from 1 to 4 for example).
     * @params name Name of a skill (don't use '_' in the name).
     * @params prerequisites Pre-requisites names for the skills (if there are any).
     */
    Skill(std::string name, Scale& mast_scale, Scale& diff_scale, Scale& speed_scale, std::vector<std::string> prerequisites = {}):
            name(std::move(name)), mastery_scale(mast_scale), difficulty_scale(diff_scale), speed_scale(speed_scale),
            prerequisites(std::move(prerequisites))  {};

    /**
     * Constructor for the Skill class.
     * @params scale Level of mastery of a skill (from 1 to 4 for example).
     * @params name Name of a skill (don't use '_' in the name).
     * @params prerequisites Pre-requisites for the skills (if there are any).
     */
    Skill(std::string name, Scale& mast_scale, Scale& diff_scale, Scale& speed_scale, const std::vector<Skill>& prerequisites):
            name(std::move(name)), mastery_scale(mast_scale), difficulty_scale(diff_scale), speed_scale(speed_scale) {

        for (auto& prereq: prerequisites) {
            this->prerequisites.push_back(prereq.name);
        }
    };

    /**
     * Default constructor for the Skill class.
     */
    Skill() = default;

    /**
     * Returns the highest mastery value for the skill.
     * @return The label of the highest mastery possible for the skills.
     */
    std::string getHighestMasteryValue() const {
        return mastery_scale.getHighestValue();
    }

    bool operator ==(const Skill& os) {
        if (name != os.name) {
            return false;
        }

        if (prerequisites.size() != os.prerequisites.size()) {
            return false;
        }

        for (size_t i = 0; i < prerequisites.size(); i++) {
            if (prerequisites[i] != os.prerequisites[i]) {
                return false;
            }
        }

        return true;
    }


    // Useful for using it as a map key
    bool operator < (const Skill& rhs) const {
        return this->name < rhs.name;
    }
};

#endif //KNOWLEDGE_TRACING_SKILL_H
