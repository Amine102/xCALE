//
// Created by couland-q on 13/05/2022.
//

#include "../include/tools.h"

namespace Tools {
    bool endsWith(std::string const &str, std::string const &suffix) {
        if (str.length() < suffix.length()) {
            return false;
        }
        return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
    }

    bool startsWith(std::string const &str, std::string const &prefix) {
        if (str.length() < prefix.length()) {
            return false;
        }
        return str.compare(0, prefix.length(), prefix) == 0;
    }

    size_t get1dIdxFrom2dIdx(size_t i, size_t j, size_t row_width) {
        return ((i * row_width) + j);
    }

    std::string getSkillNameFromVariable(const std::string& var_name, const std::string& delimiter) {
        std::string new_var_name = var_name;
        size_t first = var_name.find(delimiter);

        new_var_name.erase(0, first + 1);
        size_t second = new_var_name.find(delimiter);

        if (second != -1) {
            new_var_name.erase(second);
        }

        return new_var_name;
    }

    std::string getSkillNameFromResults(const std::string& original_skill_name,
                                        const std::string& delimiter_1,
                                        const std::string& delimiter_2,
                                        const std::string& delimiter_3) {

        std::string new_skill_name = original_skill_name;

        size_t first = new_skill_name.find(delimiter_1);
        new_skill_name.erase(0, first + 1);

        size_t second = new_skill_name.find(delimiter_2);
        new_skill_name.erase(second);

        size_t third = new_skill_name.find(delimiter_3);
        new_skill_name.erase(0, third + 1);

        size_t fourth = new_skill_name.find(delimiter_3);

        if (fourth != -1) {
            new_skill_name.erase(fourth);
        }

        return new_skill_name;
    }

    bool isSameSkill(const std::string& var1, const std::string& var2, const std::string& delimiter) {
        return (getSkillNameFromVariable(var1, delimiter) == getSkillNameFromVariable(var2, delimiter));
    }

    void writeExercisesToCSV(const std::vector<Exercise>& exos, const std::string& filename) {

        std::string csv_filename = filename + ".csv";

        std::ofstream csv_file;
        csv_file.open(csv_filename);

        // Retrive all skills names
        std::set<std::string> all_skills;

        for (auto exo: exos) {
            for (auto& skill: exo.getSkillsUsed()) {
                all_skills.insert(skill.name);
            }
        }

        // First line: skills names + difficulty
        for (auto& skill_name: all_skills) {
            csv_file << "," + skill_name;
        }

        csv_file << ",difficulty\n";

        // For each exercise
        for (auto exo: exos) {
            // Write its name
            csv_file << exo.getName();

            // For each skill, check if the skill is used or not
            for (auto& skill_name: all_skills) {
                if (exo.isSkillUsed(skill_name)) {
                    csv_file << ",1";
                }

                else {
                    csv_file << ",0";
                }
            }

            csv_file << "," + exo.getDifficulty() + "\n";
        }

        csv_file.close();
    }

    std::vector<Exercise> loadExercisesFromCSV(const std::string& filename, std::vector<Skill>& skills) {
        std::string csv_filename = filename + ".csv";
        std::ifstream csv_file;
        csv_file.open(csv_filename);

        std::vector<Exercise> exos;

        if (csv_file.is_open()) {
            std::string line;
            std::string word;
            size_t exo_idx = 0;
            size_t word_idx = 0;
            size_t nb_skills = 0;

            // get a single line into the variable line
            while(getline(csv_file, line)) {

                // Split the line into a stream of words separated by commas
                std::stringstream str(line);
                word_idx = 0;

                if (exo_idx == 0) {
                    while(getline(str, word, ',')) {
                        nb_skills++;
                    }

                    // One for the first empty cell (exo labels), one for the difficulty column
                    nb_skills -= 2;
                }

                else {

                    Exercise new_exo;

                    while(getline(str, word, ',')) {
                        // First word: exo name
                        if (word_idx == 0) {
                            new_exo.setName(word);
                        }

                        // Everything except last col: skill mobilised or not
                        else if (word_idx <= nb_skills) {
                            if (word == "1") {
                                new_exo.linkSkill(skills[word_idx - 1]);
                            }
                        }

                        // Last col: exercise difficulty
                        else {
                            new_exo.setDifficulty(word);
                        }

                        word_idx++;
                    }

                    exos.push_back(new_exo);
                }

                exo_idx++;
            }
        }

        else {
            std::cerr << "ERROR: can't open file " << csv_filename << "." << std::endl;
        }

        return exos;
    }
}
