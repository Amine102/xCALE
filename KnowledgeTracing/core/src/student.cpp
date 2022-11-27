
#include "../include/student.h"


Student::Student(const std::string& name, const std::vector<Exercise>& exs) {
    this->name = name;
    this->exercisesNotDone = exs;
}


BKTModel& Student::getBKT() {
    return this->BKT;
}


void Student::getActualSkill(const Skill& skill) {
    getMultipleActualSkills(std::vector<Skill>{skill});
}

void Student::getMultipleActualSkills(const std::vector<Skill>& skills) {
    std::vector<std::string> displayed;
    std::cout << "a l'instant t=" << this->instant << " l'état des compétences est le suivant :" << std::endl;

    std::vector<std::string> all_skills_names;
    all_skills_names.reserve(skills.size());

    for (auto& skill: skills) {
        all_skills_names.push_back(skill.name);
    }

    for (size_t i = 0; i < this->results.size(); i++) {
        for (size_t y = 0; y < this->results[i].size(); y++) {
            for (size_t k = 0; k < this->results[i][y].size(); k++) {

                std::string skillName = Tools::getSkillNameFromResults(this->results[i][y][k].get_print_name(),
                                                                       "(", ")", "_");

                if (!isDisplayed(displayed, skillName)) {
                    std::vector<int> indices = getIndexes(skillName);

                    /*if (std::find(skills_names.begin(), skills_names.end(), skillName) != skills_names.end()) {
                        std::cout << this->results[indices[0]][indices[1]][indices[2]].to_string() << std::endl;
                    }*/

                    displayed.push_back(skillName);
                }
            }
        }
    }
}

void Student::getAllActualSkills() {
    std::cout << "Etat des compétences à l'instant t=" << this->instant << std::endl;
    this->all_results.getLastResultsForAllSkills(this->instant);
}

void Student::getVarsValues(const std::vector<std::string>& vars) {
    this->all_results.getLastResultsForVars(vars);
}


PGscore Student::getPGscore(std::vector<plComputableObjectList>& new_results, Exercise& exo) {
    PGscore new_score;
    new_score.pgmin = getPGmin(new_results, exo);
    new_score.pgmean = getPGmean(new_results);
    return new_score;
}

double Student::getPGmean(std::vector<plComputableObjectList>& new_results) {
    double somme = 0;
    size_t last_idx = 0;

    for (const auto& res: new_results) {
        plHMM::Observation_t outputData;
        res[0].tabulate(outputData);

        if (last_idx == 0) {
            last_idx = outputData.size() - 1;
        }

        somme += outputData[last_idx];
    }

    double pgmean = somme / static_cast<double>(new_results.size());
    return pgmean;
}

double Student::getPGmin(std::vector<plComputableObjectList>& future, Exercise& exo) {

    std::vector<double> values;
    std::vector<std::string> skills_displayed;

    double pg_min = -1;
    size_t last_idx = 0;

    for (const auto& res: future) {
        for (auto var: res) {
            std::string skillName = Tools::getSkillNameFromResults(var.get_print_name(),
                                                                   "(", ")", "_");
            if (!isAvailable(skillName, exercisesNotDone)) {
                std::cout << skillName << " not available anymore." << std::endl;
                continue;
            }
            std::cout << skillName <<": ";
            plHMM::Observation_t outputData;
            var.tabulate(outputData);

            if (last_idx == 0) {
                last_idx = outputData.size() - 1;
            }

            std::cout << outputData[last_idx] << std::endl;

            if (pg_min < 0.0) {
                pg_min = outputData[last_idx];
            }

            else if (outputData[last_idx] < pg_min) {
                pg_min = outputData[last_idx];
            }
        }
    }

    std::cout << "PGmin = " << pg_min << std::endl;

    return pg_min;
}

std::vector<int> Student::getIndexes(const std::string& skill_name) {
    // Init vectors with 3 values set at -1
    std::vector<int> indexes(3, -1);

    for (size_t i = 0; i < this->results.size(); i++) {
        for (size_t y = 0; y < this->results[i].size(); y++) {
            for (size_t k = 0; k < this->results[i][y].size(); k++) {

                std::string to_compare = Tools::getSkillNameFromResults(this->results[i][y][k].get_print_name(),
                                                                        "(", ")", "_");
                if (to_compare == skill_name) {
                    indexes[0] = static_cast<int>(i);
                    indexes[1] = static_cast<int>(y);
                    indexes[2] = static_cast<int>(k);
                }
            }
        }
    }

    return indexes;
}

std::vector<Exercise>& Student::getExercisesLeft() {
    return this->exercisesNotDone;
}

ResultStorage& Student::getResultStorage() {
    return this->all_results;
}

void Student::setBKT(BKTModel& new_BKT) {
    this->BKT = new_BKT;

    for (auto& var: new_BKT.getSkillList()) {
        this->all_results.addVarToQuery("Skill_" + var.name + "_t0");
        this->all_results.addVarToQuery("Skill_" + var.name + "_t");
    }
}

bool Student::isDisplayed(std::vector<std::string>& list, const std::string& skill_name) {
    return std::any_of(list.begin(), list.end(),
                        [skill_name](const std::string& elem) {return elem == skill_name;});
}

int Student::triggerIsUsed(const plVariable& trigger, std::vector<plVariable>& res) {

    std::string triggerName = Tools::getSkillNameFromVariable(trigger.name(), "_");

    for (const auto& single_res: res) {
        std::string resName = Tools::getSkillNameFromVariable(single_res.name(), "_");

        if (resName == triggerName) {
            return 1;
        }
    }

    return 0;
}

bool Student::isAvailable(const std::string& skill_name, std::vector<Exercise>& new_exercisesNotDone){

    for(auto& exo: new_exercisesNotDone){
        for(auto& skill: exo.getSkillsUsed()){
            if(skill_name == skill.name){
                return true;
            }
        }
    }
    return false;
}

int Student::exerciseVectorIndex(const Exercise& ex, std::vector<Exercise>& new_exercisesNotDone) {

    for(int k = 0; k < new_exercisesNotDone.size(); k++) {
        if(ex.getName() == new_exercisesNotDone[k].getName()) {
            return k;
        }
    }

    return -1;
}

void Student::initDBN() {
    std::vector<plComputableObjectList> res_t0;
    plComputableObjectList comp_obj_lst = this->BKT.getDBN()->getInitial_G().getComputableObjectList();
    plComputableObjectList t0_values;
    std::set<std::string> all_skills;

    for (const auto& comp_obj: comp_obj_lst) {
        std::string comp_obj_name = comp_obj.get_print_name();

        // exclude Res, Triggers and Difficulties from the results
        if ((comp_obj_name.find("Res") == std::string::npos) && (comp_obj_name.find("Trigger") == std::string::npos) &&
            (comp_obj_name.find("Difficulty") == std::string::npos)){
            t0_values = t0_values.push_back(comp_obj);
            //std::cout << "[student.cpp l.262] SKILL: " << comp_obj_name << std::endl;
            all_skills.insert(comp_obj_name);
        }
    }

    res_t0.push_back(t0_values);
    this->results.push_back(res_t0);
    all_results.addResultAndVars(all_skills, res_t0);

    this->instant++;
}

void Student::doExercise(const Exercise& ex, const ExoEval& evaluation, const std::vector<VarAndEvidence>& add_evidences) {

    std::cout << std::endl << std::endl;
    std::cout << "Instant = " << this->instant << std::endl << std::endl;

    // t = 0
    if (this->instant == 0) {
        plValues evidences_t0;
        pmBayesianNetwork g_init = this->BKT.getDBN()->getInitial_G();
        plVariablesConjunction variables_init = g_init.getVariables();
        std::vector<plVariable> triggers;
        std::vector<plVariable> res;
        std::vector<plVariable> diffs;
        std::vector<plVariable> skills;
        std::vector<plVariable> Yis;

        std::set<std::string> all_skills;

        for (auto var: variables_init) {
            if (Tools::startsWith(var.name(), "Trigger")) {
                triggers.push_back(variables_init.get_variable_with_name(var.name()));
            }
        }

        for (size_t i = 0; i < ex.getSkillsUsed().size(); i++) {
            res.push_back(variables_init.get_variable_with_name("Res_" + ex.getSkill(i).name + "_t0"));
            skills.push_back(variables_init.get_variable_with_name("Skill_" + ex.getSkill(i).name + "_t0"));
            diffs.push_back(variables_init.get_variable_with_name("Difficulty_" + ex.getSkill(i).name + "_t0"));
            all_skills.insert("Skill_" + ex.getSkill(i).name + "_t0");
            //std::cout << "[student.cpp l.295] SKILL = " << ex.getSkill(i).name << std::endl;


            for(size_t j = 0; j < ex.getSkill(i).prerequisites.size(); j++){
                // TODO: probably a fixed bug here (reverse prereq and skill)
                Yis.push_back(variables_init.get_variable_with_name("Y_" + ex.getSkill(i).prerequisites[j] + "_" + ex.getSkill(i).name + "_t0"));
            }
        }

        for (auto& trigger: triggers) {
            int estPresent = triggerIsUsed(trigger, res);
            evidences_t0 = evidences_t0.add(trigger, estPresent);
        }

        // Adding " manual " evidences if there are any (useful for Impacted usecase for now)
        for (auto& add_ev: add_evidences) {
            plVariable var = variables_init.get_variable_with_name(add_ev.first + "_t0");
            evidences_t0 = evidences_t0.add(var, add_ev.second);
        }


        for(auto& it: evaluation) {
            evidences_t0 = evidences_t0.add(variables_init.get_variable_with_name("Res_" + it.first + "_t0"), it.second);
        }

        for(auto& diff: diffs) {
            evidences_t0 = evidences_t0.add(diff, ex.getDifficulty());
        }

        /*plVariablesConjunction vars = evidences_t0.get_variables();

        std::cout << "evidences_t0.size() = " << evidences_t0.size() << std::endl;
        for (size_t i = 0; i < evidences_t0.size(); i++) {
            std::cout << vars[i] << " -> " << evidences_t0[i] << std::endl;
        }*/

        std::vector<plComputableObjectList> res_t0;
        res_t0.reserve(skills.size());

        for (auto& skill: skills) {
            //std::cout << "Query on " << skill.name() << std::endl;
            res_t0.emplace_back(plComputableObjectList(g_init.query(skill, evidences_t0)));
        }

        this->results.push_back(res_t0);
        this->all_results.addResultAndVars(all_skills, res_t0);
        this->dbn_15 = new pm15dBN(this->BKT.getDBN(), evidences_t0);

        //this->dbn_15.summary_1_5();

        int pos = exerciseVectorIndex(ex, this->exercisesNotDone);
        this->exercisesNotDone.erase(exercisesNotDone.begin() + pos);

    }

    // t > 0
    else {

        plVariablesConjunction variables_t = this->dbn_15.getVariables();
        plValues evidences;
        std::vector<plVariable> triggers;
        std::vector<plVariable> res;
        std::vector<plVariable> skills;
        std::vector<plVariable> diffs;

        std::set<std::string> all_skills;

        for (auto& skill: this->BKT.getSkillList()) {
            skills.push_back(variables_t.get_variable_with_name("Skill_" + skill.name + "_t"));
        }

        for (auto var: variables_t) {
            if (Tools::startsWith(var.name(), "Trigger"))
                triggers.push_back(variables_t.get_variable_with_name(var.name()));
        }

        for (size_t i = 0; i < ex.getSkillsUsed().size(); i++) {
            res.push_back(variables_t.get_variable_with_name("Res_" + ex.getSkill(i).name + "_t"));
            //skills.push_back(variables_t.get_variable_with_name("Skill_" + ex.getSkill(i).name + "_t"));
            diffs.push_back(variables_t.get_variable_with_name("Difficulty_" + ex.getSkill(i).name + "_t"));
            //std::cout << "[student.cpp l.357] SKILL = " << ("Skill_" + ex.getSkill(i).name) << std::endl;

        }

        for (auto& trigger: triggers) {
            //est mobilisé ou pas
            int triggerPresent = triggerIsUsed(trigger, res);
            evidences = evidences.add(trigger, triggerPresent);
        }
        //std::cout << "res.size : " << res.size() << std::endl;

        for(auto& it: evaluation){
            evidences = evidences.add(variables_t.get_variable_with_name("Res_" + it.first + "_t"), it.second);
        }

        for(auto& diff: diffs) {
            evidences = evidences.add(diff, ex.getDifficulty());
        }

        // Adding " manual " evidences if there are any (useful for Impacted usecase for now)
        for (auto& add_ev: add_evidences) {
            plVariable var = variables_t.get_variable_with_name(add_ev.first + "_t");
            evidences = evidences.add(var, add_ev.second);
        }

        /*plVariablesConjunction vars = evidences.get_variables();

        std::cout << "evidences_t.size() = " << evidences.size() << std::endl;
        for (size_t i = 0; i < evidences.size(); i++) {
            std::cout << vars[i] << " -> " << evidences[i] << std::endl;
        }*/

        std::vector<plComputableObjectList> res_t;

        for (auto& skill: skills) {
            //std::cout << "Query on " << skill.name() << std::endl;
            all_skills.insert(skill.name());
            plComputableObjectList resQuery = this->dbn_15.query_JD(skill, evidences);
            res_t.push_back(resQuery);
        }

        this->results.push_back(res_t);
        this->all_results.addResultAndVars(all_skills, res_t);
        this->dbn_15.updateGT(evidences);


        int pos = exerciseVectorIndex(ex, this->exercisesNotDone);
        this->exercisesNotDone.erase(exercisesNotDone.begin() + pos);
    }

    this->instant++;
}

Exercise& Student::recommendExercise(AnswersPrediction decision, Metric pedagogy, const std::vector<VarAndEvidence>& add_evidences) {
    plVariablesConjunction variables_t = this->dbn_15.getVariables();

    std::pair<PGscore, Exercise> pgmin_mean_and_exercise;
    plValues evidence_to_keep;
    std::vector<plComputableObjectList> best_res;
    std::vector<PGscore> all_pg_scores;
    std::vector<plVariable> all_skills;
    std::set<plVariable> triggers;

    if(this->instant == 0){

        pmBayesianNetwork g_init = this->BKT.getDBN()->getInitial_G();
        plVariablesConjunction variables_init = g_init.getVariables();

        for (auto& skill: this->BKT.getSkillList()) {
            all_skills.push_back((variables_init.get_variable_with_name("Skill_" + skill.name + "_t0")));
        }

        for (auto var: variables_init) {
            if(Tools::startsWith(var.name(), "Trigger"))
                triggers.insert(variables_init.get_variable_with_name(var.name()));
        }

        for (auto& exo: this->exercisesNotDone) {
            std::vector<plVariable> res;
            std::vector<plVariable> skills;
            plValues evidences_t0;

            for (auto& skill: exo.getSkillsUsed()) {
                res.push_back(variables_init.get_variable_with_name("Res_" + skill.name + "_t0"));
                skills.push_back(variables_init.get_variable_with_name("Skill_" + skill.name + "_t0"));
            }

            for (auto& trigger: triggers) {
                // Add trigger value to evidence (0 if skill is not used, 1 otherwise)
                evidences_t0 = evidences_t0.add(trigger, triggerIsUsed(trigger, res));
            }

            for (auto& single_res: res) {
                if (decision == AnswersPrediction::Optimistic) {
                    evidences_t0 = evidences_t0.add(single_res, this->getBKT().getHighestMasteryValue());
                }
            }

            // Adding manual evidences if they exist
            for (auto& add_ev: add_evidences) {
                plVariable new_ev = variables_init.get_variable_with_name(add_ev.first + "_t0");
                evidences_t0 = evidences_t0.add(new_ev, add_ev.second);
            }

            /*plVariablesConjunction vars = evidences_t0.get_variables();

            std::cout << "evidences_t0.size() = " << evidences_t0.size() << std::endl;
            for (size_t i = 0; i < evidences_t0.size(); i++) {
                std::cout << vars[i] << " -> " << evidences_t0[i] << std::endl;
            }*/

            std::vector<plComputableObjectList> res_t0;

            for (auto& skill: all_skills) {
                //std::cout << "Skill queried: " << skill << std::endl;
                plComputableObjectList resQuery = plComputableObjectList(g_init.query(skill, evidences_t0));
                res_t0.push_back(resQuery);
            }

            /*for (auto& res_0: res_t0) {
                std::cout << res_0 << std::endl;
            }*/

            std::cout << "Ex " << exo.getName() << std::endl;
            PGscore current_pg_scores = getPGscore(res_t0, exo);
            //current_pg_scores.print();
            all_pg_scores.push_back(current_pg_scores);

        }
    }

    else {

        for (auto& comp: this->BKT.getSkillList()) {
            all_skills.push_back((variables_t.get_variable_with_name("Skill_" + comp.name + "_t")));
        }

        for (size_t j = 0; j < variables_t.size(); j++) {
            std::string entier = variables_t.at(j).name();
            std::string delimiter = "_";

            size_t first = entier.find(delimiter);
            entier.erase(0, first + 1);

            size_t second = entier.find(delimiter);
            if (second != -1) {
                entier.erase(second);
            }

            triggers.insert(variables_t.get_variable_with_name("Trigger_" + entier + "_t"));
        }

        for (auto& exo: this->exercisesNotDone) {

            std::vector<plVariable> res;
            std::vector<plVariable> skills;
            plValues evidences;
            //std::cout << "skills_used.name() " << this->exercisesNotDone[0].skills_used[0].name << std::endl;

            for (size_t j = 0; j < exo.getSkillsUsed().size(); j++) {
                res.push_back(variables_t.get_variable_with_name("Res_" + exo.getSkill(j).name + "_t"));
                skills.push_back(variables_t.get_variable_with_name("Skill_" + exo.getSkill(j).name + "_t"));
            }

            for (auto& trigger: triggers) {
                //est mobilisé ou pas
                int triggerPresent = triggerIsUsed(trigger, res);
                evidences = evidences.add(trigger, triggerPresent);
            }

            for (auto& single_res: res) {
                if (decision == AnswersPrediction::Optimistic) {
                    evidences = evidences.add(single_res, this->getBKT().getHighestMasteryValue());
                }
            }

            for (auto& add_ev: add_evidences) {
                // If the evidence should be used (-> if the trigger for the skill is at 1)
                bool skill_is_used = false;
                for (auto& trigger: triggers) {
                    if (Tools::isSameSkill(add_ev.first, trigger.name(), "_") && triggerIsUsed(trigger, res)) {
                        skill_is_used = true;
                        break;
                    }
                }

                if (skill_is_used) {
                    std::cout << "Skill " << Tools::getSkillNameFromVariable(add_ev.first, "_") << " is used." << std::endl;
                    plVariable new_ev = variables_t.get_variable_with_name(add_ev.first + "_t");
                    evidences = evidences.add(new_ev, add_ev.second);
                }
            }

            std::vector<plComputableObjectList> res_t;
            plComputableObjectList resQuery;

            /*for (auto var: evidences.get_variables()) {
                std::cout << var.name() << ": " << evidences[var] << std::endl;
            }*/

            for (auto& skill: all_skills) {
                resQuery.clear();
                resQuery = this->dbn_15.query_JD(skill, evidences);
                res_t.push_back(resQuery);
            }

            std::cout << "Ex " << exo.getName() << std::endl;
            PGscore current_pg_scores = getPGscore(res_t, exo);
            all_pg_scores.push_back(current_pg_scores);

        }
    }


    double pg_max = 0;
    double current_pg = 0;
    size_t index = 0;

    for(size_t i = 0; i < all_pg_scores.size(); i++) {
        if(pedagogy == Metric::PG_mean) {
            current_pg = all_pg_scores[i].pgmean;
        }

        else if(pedagogy == Metric::PG_min) {
            current_pg = all_pg_scores[i].pgmin;
        }

        if(current_pg > pg_max){
            pg_max = current_pg;
            index = i;
        }
    }

    return this->exercisesNotDone[index];
}

void Student::serialize(const std::string& filename) {

    std::string xml_filename = filename + ".xml";

    // If t < 0, the DBN is not initialized yet, only serialize exercises and skills and such
    if (this->instant < 0) {
        std::cout << "WARNING: DBN has not been initialized (current instant = " << this->instant << "), no DBN will be serialized." << std::endl;
    }

    // the plSerializer stops the program if the file does not exists, so we can't use a try/catch block for this part
    plSerializer serializer;

    // If the file exists, load the existing content
    if(std::experimental::filesystem::exists(xml_filename)) {
        serializer.load(xml_filename);
    }

    if (this->instant >= 0) {
        this->BKT.getDBN()->serialize(serializer);
    }

    // 1.5 DBN has only been created if instant > 0
    if (this->instant > 0) {
        // If t = 0 (-> the DBN has been created but no exercises have been done yet), the 1.5dbn has not been created yet
        // Add the new 1.5DBN
        this->dbn_15.serialize(serializer, "jd15", "Oi15");
    }

    // Add all the scales for the skills
    // Ok so weird bug here. If you don't capitalize the first letter of each object,
    // Mastery and Difficulty are written after the DBB/1.5DBN objects in the XML. This results
    // in the values of the plLabelType not being written in the XML. Don't ask me why, because
    // "pre_req" and "present_futur" are written after them, and they still have their values.
    // It makes no sense.
    serializer.add_object("Mastery_scale", this->BKT.getSkillList()[0].mastery_scale.scale_values);
    serializer.add_object("Difficulty_scale", this->BKT.getSkillList()[0].difficulty_scale.scale_values);
    serializer.add_object("Speed_scale", this->BKT.getSkillList()[0].speed_scale.scale_values);

    // Add the pre-requisites matrix
    plFloatMatrix skills_prerequisites(this->BKT.getSkillList().size());
    // for each skill in the BKT skill's list
    for (size_t i = 0; i < this->BKT.getSkillList().size(); i++) {
        for (size_t j = 0; j < this->BKT.getSkillList().size(); j++) {
            // checks if other skills are a prerequisites or not
            if(std::find(this->BKT.getSkillList()[i].prerequisites.begin(),
                         this->BKT.getSkillList()[i].prerequisites.end(),
                         this->BKT.getSkillList()[j].name) != this->BKT.getSkillList()[i].prerequisites.end()) {

                skills_prerequisites[i][j] = 1;
            }
        }
    }

    plLabelType skills_names;
    for (auto skill: this->BKT.getSkillList()) {
        skills_names.add_label(skill.name);
    }

    serializer.add_object("Skills_names", skills_names);

    serializer.add_object("Pre_req", skills_prerequisites);

    // Write the plSerializer content to a file
    std::ofstream serial_file(xml_filename);

    if(serial_file) {
        try {
            serializer.save(serial_file);
        }

        catch(const std::exception &e) {
            std::cerr << "Error while writing in serialization file : " << e.what();
        }

    }

    else {
        std::cerr << "Error : cannot create serialization file.\n ";
    }
}

void Student::deserialize(const std::string& filename) {

    std::string xml_filename = filename + ".xml";

    if(!std::experimental::filesystem::exists(xml_filename)) {
        std::cerr << "Error: file " << xml_filename << " does not exist." << std::endl;
        return;
    }

    // Retrieving DBN
    auto dbn = new pmDynamicBayesianNetwork();
    dbn = dbn->load_from_xml(xml_filename);
    this->BKT.setDBN(dbn);

    // Retrieving 1.5 DBN
    this->dbn_15 = pm15dBN();
    this->dbn_15 = pm15dBN::unserialize(xml_filename, "jd15", "oi15");

    plSerializer serial_load("serial_l");
    serial_load.load(xml_filename);

    // get the skills prereq
    plFloatMatrix prereq;
    serial_load.get_object("Pre_req", prereq);

    // Get the scales
    plLabelType scale;
    serial_load.get_object("Mastery_scale", scale);
    Scale mastery_scale(scale);
    serial_load.get_object("Difficulty_scale", scale);
    Scale difficulty_scale(scale);
    serial_load.get_object("Speed_scale", scale);
    Scale speed_scale(scale);

    plLabelType skills_names;
    serial_load.get_object("Skills_names", skills_names);

    for (size_t i = 0; i < skills_names.get_values().size(); i++) {
        std::cout << skills_names.get_values()[i] << std::endl;

        Skill new_skill(skills_names.get_values()[i], mastery_scale, difficulty_scale, speed_scale);
        std::vector<std::string> skill_prereq;

        for (size_t j = 0; j < prereq.cols(); j++) {
            if (prereq[i][j] == 1) {
                skill_prereq.push_back(skills_names.get_values()[j]);
            }
        }

        new_skill.prerequisites = skill_prereq;
        this->BKT.addSkillToVector(new_skill);
    }
}