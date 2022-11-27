//
// Created by couland-q on 08/06/2022.
//

#include "../include/exercise.h"

Exercise::Exercise() {
    this->name = "NAME NOT SET";
    this->difficulty = "NONE";
}

Exercise::Exercise(const std::string& name, const Scale& difficulty_scale, const std::string& difficulty) {
    this->name = name;
    this->difficulty = difficulty;
    this->difficulty_scale = difficulty_scale;
}

Exercise::~Exercise() {

}

const std::string& Exercise::getName() const {
    return this->name;
}

const std::vector<Skill>& Exercise::getSkillsUsed() const {
    return this->skills_used;
}

const Skill& Exercise::getSkill(size_t idx) const {
    if(idx < this->skills_used.size())
        return this->skills_used[idx];

    return this->skills_used[0];
}

const std::string& Exercise::getDifficulty() const {
    return this->difficulty;
}

const Scale& Exercise::getDifficultyScale() const {
    return this->difficulty_scale;
}

void Exercise::setName(const std::string& new_name) {
    this->name = new_name;
}

void Exercise::setDifficulty(const std::string& difficulty) {
    this->difficulty = difficulty;
}

void Exercise::setDifficulty(int idx) {
    this->difficulty = difficulty_scale.scale_values.get_values()[idx];
}

void Exercise::linkSkill(Skill& skill) {
    this->skills_used.push_back(skill);
}

void Exercise::linkSkills(const std::vector<Skill>& v_s) {
    for (auto& S: v_s) {
        this->skills_used.push_back(S);
    }
}

bool Exercise::isSkillUsed(const std::string& skill_name) const {
    return std::any_of(skills_used.begin(), skills_used.end(),
                       [skill_name](const Skill& skill){return skill_name == skill.name;});
}

std::ostream& operator <<(std::ostream& os, const Exercise& ex) {
    os << ex.getName() << std::endl;
    os << "Difficulty: " << ex.getDifficulty() << std::endl;
    os << "(difficulty scale: ";
    for (auto& diff: ex.getDifficultyScale().scale_values.get_values()) {
        os << diff << ", ";
    }

    os << ")" << std::endl;
    os << "Skills used: ";

    for (auto& skill: ex.getSkillsUsed()) {
        os << skill.name << " ";
    }

    os << std::endl;

    return os;
}