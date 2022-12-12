//
// Created by couland-q on 31/08/2022.
//

#ifndef KNOWLEDGE_TRACING_RESULTSTORAGE_H
#define KNOWLEDGE_TRACING_RESULTSTORAGE_H

#include <vector>
#include <string>
#include <utility> // std::pair
#include <set>
#include <iostream>

#include <jsoncpp/json/json.h>  // data export

#include <pl.h>
#include "tools.h"

struct ResultStorage {
    // A vector of " Var A, Var B " -> results
    // Note: idx 0 contains the -1 values
    std::vector<std::pair<std::set<std::string>, std::vector<plComputableObjectList>>> results_and_vars;
    std::set<std::string> all_vars;
    std::set<std::string> vars_to_query;

    size_t getNumberOfVars() const {
        return all_vars.size();
    }

    /**
     * Adds new variables to the set of variables to query
     * @param new_vars A vector containing all the new variables to query
     */
    void addVarsToQuery(const std::vector<std::string>& new_vars) {
        for (auto& var: new_vars) {
            vars_to_query.insert(var);
        }
    }

    /**
     * Adds a new variable to the set of variables to query
     * @param new_var The name of the new variable to query
     */
    void addVarToQuery(const std::string& new_var) {
        vars_to_query.insert(new_var);
    }

    /**
     * Removes a single variable from the variables to query set.
     * @param var_to_remove The name of the variable to remove
     */
    void removeVarFromQuery(const std::string& var_to_remove) {
        vars_to_query.erase(var_to_remove);
    }

    /**
     * Clears the set of variables to query
     */
    void removeAllVarsFromQuery() {
        this->results_and_vars.clear();
    }

    /**
     * Checks if a variable name is formatted like this: P(variable)
     * @param var_name The name of the variable to check
     * @return True if the variable is formatted like P(variable), false otherwise
     */
    bool varNameHasP(const std::string& var_name) {
        if(!Tools::startsWith(var_name, "P(")) {
            return false;
        }

        if(!Tools::endsWith(var_name, ")")) {
            return false;
        }

        return true;
    }

    /**
     * Checks if the variable name has a right part (such as P(variable|right part))
     * @param var_name The name of the variable to check
     * @return True if the variable has a right part, false otherwise
     */
    bool varNamehasRightVals(const std::string& var_name) {
        return(var_name.find('|') != std::string::npos);
    }

    /**
     * Strips the name of a variable from any P() or right parts
     * @param var_name The name of the variable to strip
     * @return The stripped name (P(var|right) -> var)
     */
    std::string stripName(const std::string& var_name) {
        std::string new_name = var_name;

        if(varNameHasP(var_name)) {
            new_name = new_name.substr(2, new_name.size() - 3);
        }

        if (varNamehasRightVals(var_name)) {
            new_name = new_name.substr(0, new_name.find('|'));
        }

        return new_name;
    }

    void addResultAndVars(std::set<std::string>& vars, const std::vector<plComputableObjectList>& new_results) {
        std::set<std::string> stripped_names;

        for (auto& var: vars) {
            std::string new_name = stripName(var);
            all_vars.insert(new_name);
            stripped_names.insert(new_name);
        }

        results_and_vars.emplace_back(std::make_pair(stripped_names, new_results));
    }

    // Returns the index of the last time the skill was used (and had results). Minimum value is -1, meaning that the
    // skill was never used.
    int getIndexOfLastUsed(const std::string& skill_name) {
        for (int i = static_cast<int>(results_and_vars.size()) - 1; i >= 0; i--) {
            for (auto& skill: results_and_vars[i].first) {
                if (skill_name == skill) {
                    return i;
                }
            }
        }

        // -1 returned if not found
        return -1;
    }

    void getLastResultsForAllVars() {
        std::vector<std::string> already_displayed;


        for (auto& var: all_vars) {
            size_t idx = getIndexOfLastUsed(var);
            std::vector<plComputableObjectList> res = results_and_vars[idx].second;

            for (auto& one_res: res) {
                for(auto single_res: one_res) {

                    std::string var_name = single_res.get_print_name();

                    if (std::find(already_displayed.begin(), already_displayed.end(), var_name) == already_displayed.end()) {
                        std::cout << single_res.to_string() << std::endl;
                        already_displayed.push_back(var_name);
                    }
                }
            }
        }
    }

    void getLastResultsForVars(const std::vector<std::string>& vars) {
        std::vector<std::string> already_displayed;

        for (auto& var: vars) {
            std::string new_var_name = var;

            int idx = getIndexOfLastUsed(new_var_name);

            if (idx == -1) {
                for (auto& elem: this->results_and_vars[0].first) {
                    std::cout << elem << std::endl;
                }

                std::cout << "WARNING: variable " << var << " hasn't been queried yet. Checking if there's a t0 value." << std::endl;
                idx = getIndexOfLastUsed(var + "_t0");

                if (idx == -1) {
                    std::cout << "WARNING: variable " << var << " hasn't been queried at all (either at t > 0 or t = 0)." << std::endl;
                    continue;
                }

                else {
                    new_var_name.append("_t0");
                }
            }

            std::vector<plComputableObjectList> res = results_and_vars[idx].second;

            for (auto& one_res: res) {
                for (auto single_res: one_res) {

                    std::string var_name = single_res.get_print_name();

                    // Bug here: strip var from any right values P(S1_t|_______)
                    if (stripName(var_name) == new_var_name) {
                        if (std::find(already_displayed.begin(), already_displayed.end(), var_name) == already_displayed.end()) {
                            std::cout << single_res.to_string() << std::endl;
                            already_displayed.push_back(var_name);
                        }
                    }
                }
            }
        }
    }

    void getLastResults() {
        std::vector<std::string> already_displayed;

        for (auto& skill: all_vars) {

            for (auto& one_res: this->results_and_vars.back().second) {
                for (auto single_res: one_res) {

                    std::string skillName = single_res.get_print_name();

                    if (std::find(already_displayed.begin(), already_displayed.end(), skillName) == already_displayed.end()) {
                        std::cout << single_res.to_string() << std::endl;
                        already_displayed.push_back(skillName);
                    }
                }
            }
        }
    }

    void getLastResultsForAllSkills(int instant) {
        if (instant < 2) {
            std::vector<std::string> vars_to_query_t0;
            for (auto& var: vars_to_query) {
                if (Tools::endsWith(var, "_t0")) {
                    vars_to_query_t0.push_back(var);
                }
            }

            getLastResultsForVars(vars_to_query_t0);
        }

        else {
            std::vector<std::string> vars_to_query_t;
            for (auto& var: vars_to_query) {
                if (!Tools::endsWith(var, "_t0")) {
                    vars_to_query_t.push_back(var);
                }
            }

            getLastResultsForVars(vars_to_query_t);
        }
    }

    void outputToFile(const std::string& export_type, const std::string& filename, const std::string& path = "") {
        std::string final_filename = filename;
        if (!Tools::endsWith(filename, export_type)) {
            final_filename.append("." + export_type);
        }

        if (export_type == "csv") {
            csvExport(final_filename, path);
        }

        if (export_type == "json") {
            jsonExport(final_filename, path);
        }

        else {
            std::cerr << FULL_LOCATION /*<< WARNING*/ << "WARNING: filetype for export (" << export_type << ") isn't supported. "
                      << "Supported types are csv, json and xml." << std::endl;
        }
    }

    void csvExport(const std::string& filename, const std::string& path) {

        std::ofstream o_file;
        o_file.open(path + filename);

        int instant = 0;

        for (auto& val: results_and_vars) {
            for(auto& name: val.first) {
                std::cout << name << std::endl;
            }

            for (auto& val2: val.second) {
                std::cout << val2.to_string() << std::endl;
            }

            ++instant;
        }

        o_file.close();
    }

    void jsonExport(const std::string& filename, const std::string& path) {
    //     using json = nlohmann::json;

    //     json json_object;

    //     int instant = 0;

    //     for (auto& val: results_and_vars) {
    //         json var_names = json::array();

    //         for(auto& name: val.first) {
    //             var_names.emplace_back(name);
    //         }

    //         json to_strings = json::array();

    //         for (auto& val2: val.second) {
    //             to_strings.push_back(val2.to_string());
    //         }

    //         json merged = json::object();
    //         merged["vars"] = var_names;
    //         merged["to_string"] = to_strings;

    //         json_object[std::to_string(instant)] = merged;

    //         ++instant;
    //     }

    //     std::ofstream o_file;
    //     o_file.open(path + filename);
    //     o_file << json_object;
    //     o_file.close();
    }
};

#endif //KNOWLEDGE_TRACING_RESULTSTORAGE_H
