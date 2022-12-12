//
// Created by couland-q on 13/05/2022.
//

#ifndef KNOWLEDGE_TRACING_TOOLS_H
#define KNOWLEDGE_TRACING_TOOLS_H

#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <pl.h>

#include "exercise.h"

#define FULL_LOCATION "[" << __FILE__ << " | " << __FUNCTION__ << " | l." << __LINE__ << "]" << std::endl
#define FILE_NAME "[" << __FILE__ << "]"
#define FUNCTION_NAME "[" << __FUNCTION__ << "]"
#define LINE "l." << __LINE__

namespace Tools {
    /**
     * Checks if a string ends with a substring.
     * @param str The string to check.
     * @param suffix The substring to check.
     * @return Boolean (true if the string ends with the suffix, false otherwise).
     */
    bool endsWith(std::string const &str, std::string const &suffix);

    /**
     * Checks if a string starts with a substring.
     * @param str The string to check.
     * @param suffix The substring to check.
     * @return Boolean (true if the string starts with the prefix, false otherwise).
     */
    bool startsWith(std::string const &str, std::string const &prefix);

    /**
     * Returns an index for a 1d table from 2 indexes and a row_width (as in a 2d array).
     * @param i First index
     * @param j Second index
     * @param row_width The width of a row in the array
     * @return The corresponding 1d index
     */
    size_t get1dIdxFrom2dIdx(size_t i, size_t j, size_t row_width);

    /**
     * Strips a variable from its qualifier (Skill, Trigger, etc.) and return the skill name
     * @param var_name The variable name
     * @param delimiter The delimiter used in the skill name (usually "_")
     * @return The skill name, stripped from any qualifier
     */
    std::string getSkillNameFromVariable(const std::string& var_name, const std::string& delimiter);

    /**
     * (WARNING: legacy code) Strips a variable name regarding 3 delimiters
     * @param original_skill_name The variable name
     * @param delimiter_1 The first delimiter
     * @param delimiter_2 The second delimiter
     * @param delimiter_3 The third delimiter
     * @return The stripped name
     */
    std::string getSkillNameFromResults(const std::string& original_skill_name,
                                        const std::string& delimiter_1,
                                        const std::string& delimiter_2,
                                        const std::string& delimiter_3);

    /**
     * Checks if too variables are related to the same skill (uses @getSKillNamFromVariable)
     * @param var1 The first variable to check
     * @param var2 The second variable to check
     * @param delimiter The delimiter used (usually "_")
     * @return True if the variables are related to the same skill, false otherwise
     */
    bool isSameSkill(const std::string& var1, const std::string& var2, const std::string& delimiter);

    /**
     * Writes a vector of Exercises into a CSV file
     * @param exos The vector of Exercises
     * @param filename The output file name (without the .csv extension)
     */
    void writeExercisesToCSV(const std::vector<Exercise>& exos, const std::string& filename);

    /**
     * Loads a vector of Exercises from a CSV file
     * @param filename The input file name (without the .csv extension)
     * @param skills The skills list
     * @return A vector of Exercises
     */
    std::vector<Exercise> loadExercisesFromCSV(const std::string& filename, std::vector<Skill>& skills);

    namespace Time {
        /**
         * A struct made to hold the hyperparameters of the BKT (number of skills and number of exercises).
         */
        struct Params {
            std::pair<size_t, size_t> params;

            Params() {};

            Params(size_t param1, size_t param2) {
                params.first = param1;
                params.second = param2;
            }
        };

        /**
         * Holds multiple time measures for a specific functionnality. You should never use this directly,
         * instead, use the Times struct.
         */
        struct SpecificTime {
            std::string name;
            std::vector<std::chrono::duration<double, std::milli>> all_times;

            /**
             * Basic constructor.
             * @param name The name of the functionnality to time.
             */
            SpecificTime(const std::string& name) {
                this->name = name;
            }

            /**
             * Adds a duration to the times vector.
             * @param time_to_add The duration to add to the vector.
             */
            void addTime(std::chrono::duration<double, std::milli> time_to_add) {
                all_times.push_back(time_to_add);
            }
        };

        /**
         * A structure made to measure and hold multiple SpecificTime. Currently, it can only run one timer at a time.
         */
        struct Times {
            std::vector<SpecificTime> all_timers;

            std::chrono::time_point<std::chrono::high_resolution_clock> beg, end;
            bool is_timing = false;

            std::string running_timer;


            Times() {};

            /**
             * Starts a timer with a given name. Prints a warning and does nothing else if a timer is already running.
             * @param timer_name The name of the timer to start.
             */
            void startTimer(const std::string& timer_name) {
                if(!is_timing) {
                    beg = std::chrono::high_resolution_clock::now();
                    is_timing = true;

                    if (!timerExists(timer_name)) {
                        all_timers.emplace_back(SpecificTime(timer_name));
                    }

                    running_timer = timer_name;
                }

                else {
                    std::cout << "[TIMER WARNING (startTimer): a timer is already running.]" << std::endl;
                }
            }

            /**
             * Stops a running timer. Prints a warning and does nothing else if either 1) no timer is running, or
             * 2/ the running timer has another name.
             * @param timer_name The name of the timer to stop.
             */
            void stopTimer(const std::string& timer_name) {
                if(is_timing && timer_name == running_timer) {
                    end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> res = end - beg;
                    all_timers[findTimerIdx(timer_name)].addTime(res);
                    is_timing = false;
                    running_timer = "";
                    beg = end;
                }

                else {
                    if(!is_timing) {
                        std::cout << "[TIMER WARNING (stopTimer): no timer is currently running.]" << std::endl;
                    }

                    if(timer_name != running_timer) {
                        std::cout << "[TIMER WARNING (stopTimer): no timer with the name " << timer_name << " is currently running.]" << std::endl;
                    }
                }
            }

            /**
             * Finds the index of a given timer.
             * @param timer_to_return The name of the timer to return.
             * @return The index of the timer. Returns the last timer if the timer is not found.
             */
            size_t findTimerIdx(const std::string& timer_to_return) {

                for (size_t i = 0; i < all_timers.size(); i++) {
                    if(all_timers[i].name == timer_to_return) {
                        return i;
                    }
                }

                std::cout << "[WARNING: no timer found with the name " << timer_to_return <<". Returning last value, expect unexpected behaviour.]" << std::endl;
                return (all_timers.size() - 1);
            }

            /**
             * Checks if a timer exists.
             * @param timer_name The name of the timer to find.
             * @return Boolean, true if found, false otherwise.
             */
            bool timerExists(const std::string& timer_name) {
                if(std::any_of(all_timers.begin(), all_timers.end(),
                               [timer_name](SpecificTime& timer)
                                   {return timer_name == timer.name;}
                )) {
                    return true;
                }

                return false;
            }

            double getAllTime(const std::string& timer_name, const std::string& unit="s") {
                double sum = 0.0;
                SpecificTime specific_timer = all_timers[findTimerIdx(timer_name)];

                for (auto& val: specific_timer.all_times) {
                    sum += val.count();
                }

                if (unit == "s") {
                    return sum / 1000;
                }

                    // default is ms
                else {
                    return sum;
                }
            }

            void displayLastT(const std::string& timer_name, const std::string& unit="s") {

                SpecificTime specific_timer = all_timers[findTimerIdx(timer_name)];

                if (unit == "s") {
                    std::cout << (specific_timer.all_times.back().count() / 1000) << "s" << std::endl;
                }

                else {
                    std::cout << specific_timer.all_times.back().count() << "ms" << std::endl;
                }
            }

            void displayAllT(const std::string& timer_name, const std::string& unit="s") {

                SpecificTime specific_timer = all_timers[findTimerIdx(timer_name)];

                for (auto& val: specific_timer.all_times) {
                    if (unit == "s") {
                        std::cout << (val.count() / 1000) << "s" << std::endl;
                    }

                    else {
                        std::cout << val.count() << "ms" << std::endl;
                    }
                }
            }
        };

        /**
         * A high-level structure to hold multiple pairs of Params/Timers.
         */
        struct Timers {
            // Vector of pairs (params, durations)
            std::vector<std::pair<Params, Times>> durations;

            void addParams(Params params) {
                std::pair<Params, Times> new_times;
                new_times.first = params;
                durations.emplace_back(new_times);
            }

            void addTimes(Times times) {
                durations.back().second = times;
            }

            Times& getLastTime() {
                return durations.back().second;
            }

            void displayTimesForSpreadsheet() {
                // first line = params
                for (auto& pairs: durations) {
                    std::cout << pairs.first.params.first << "," << pairs.first.params.second << ",";
                }

                std::cout << std::endl;

                // second line = total times
                for (auto& pairs: durations) {
                    std::cout << pairs.second.getAllTime("s") << ",";
                }

                std::cout << std::endl;
            }

            /**
             * Prints all the values in a CSV formatted file (comma separated).
             * All values will be displayed in milliseconds.
            **/
            void writeToCSV(const std::string& filename) {
                std::ofstream output_file;

                std::string final_filename = filename;

                if (!Tools::endsWith(final_filename, ".csv")) {
                    final_filename = final_filename + ".csv";
                }

                output_file.open(final_filename);

                for (auto& params_and_times: durations) {
                    // Write the params first
                    output_file << "parameters,";
                    output_file << params_and_times.first.params.first << ",";
                    output_file << params_and_times.first.params.second << "\n";

                    for(auto& timer: params_and_times.second.all_timers) {
                        output_file << timer.name << ",";
                        for (auto& val: timer.all_times) {
                            output_file << val.count() << ",";
                        }

                        // delete the last comma
                        long pos = output_file.tellp();
                        output_file.seekp(pos - 1);
                        output_file.write(" ", 1);

                        output_file << "\n";
                    }

                    output_file << "\n";
                }

                output_file.close();

            }
        };
    }
}

#endif //KNOWLEDGE_TRACING_TOOLS_H
