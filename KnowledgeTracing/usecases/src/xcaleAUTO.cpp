/*###################################-XCALE_AUTO_CPP-###################################
  ##	XCALE Lib C++20, v1.0
  ##   	Copyright (C) 2021-2024 Amine Boulahmel: <amine.boulahmel@imt-atlantique.fr>
  ##
  ##
  ##   	Licensed under the Apache License, Version 2.0 (the "License");
  ##  	you may not use this file except in compliance with the License.
  ##   	You may obtain a copy of the License at
  ##
  ##       http://www.apache.org/licenses/LICENSE-2.0
  ##
  ##   Unless required by applicable law or agreed to in writing, software
  ##   distributed under the License is distributed on an "AS IS" BASIS,
  ##   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  ##   See the License for the specific language governing permissions and
  ##   limitations under the License.
  ##
  ###################################################################################*/


#include "../include/xcale.h"
#include <jsoncpp/json/json.h>                          // requires installation of third party jsoncpp library (look at jsoncpp installation procedure)
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

//-----------------------------------------------------------------------------
// SECTION : xCALE usecase implementation
//-----------------------------------------------------------------------------
namespace UsecaseXCale 
{
    int labelToDiff(std::string diffLabel)
    {
        std::ifstream scaleConfigFile("metadata/scaleConfig.json");
        Json::Reader jsonReader;
        Json::Value scaleConfigData;
        jsonReader.parse(scaleConfigFile, scaleConfigData);
        int difficulty;
        for(Json::ValueConstIterator it = scaleConfigData["difficulty_scale"].begin(); it != scaleConfigData["difficulty_scale"].end(); ++it)
        {
            if(diffLabel == (*it)["label"].asString())
            {
                difficulty = (*it)["scale"].asInt();
                break;
            }
            else
                continue;
        }
        if(difficulty == 0)
        {
            std::cerr << "Label Error : no matching difficulty for label = " << diffLabel << " in scaleConfig.json" << std::endl;
            return -1;
        }
        else
        {
            if(debugtool_::UsecaseXCale::SCALE_CREATION_DIFFICULTY_DEBUG)
                std::cout << "Difficulty Scale: " << difficulty << std::endl;                           /*FOR DEBUG PURPOSES*/
            return difficulty;
        }
    }

    std::string diffToLabel(int difficulty)
    {
        std::ifstream scaleConfigFile("metadata/scaleConfig.json");
        Json::Reader jsonReader;
        Json::Value scaleConfigData;
        jsonReader.parse(scaleConfigFile, scaleConfigData);
        std::string diffLabel;
        for(Json::ValueConstIterator it = scaleConfigData["difficulty_scale"].begin(); it != scaleConfigData["difficulty_scale"].end(); ++it)
        {
            if(difficulty == (*it)["scale"].asInt())
            {
                diffLabel = (*it)["label"].asString();
                break;
            }
            else
                continue;
        }
        if(diffLabel.empty())
        {
            std::cerr << "Scale Error : no matching difficulty for scale = " << difficulty << " in scaleConfig.json" << std::endl;
            return "\0";
        }
        else
        {
            if(debugtool_::UsecaseXCale::SCALE_CREATION_DIFFICULTY_DEBUG)
                std::cout << "Difficulty Label: " << diffLabel << std::endl;                           /*FOR DEBUG PURPOSES*/
            return diffLabel;
        }

    }
    void xcaleAUTO()
    {
        plError::always_display_warning(debugtool_::UsecaseXCale::PL_ALWAYS_DISPLAY_WARNING);
        //-----------------------------------------------------------------------------
        // SUBSECTION : Automatic creation of scales from scaleConfig.json file
        //-----------------------------------------------------------------------------
        std::ifstream scaleConfigFile("metadata/scaleConfig.json");                                     /*Reading scale metadata in scaleConfig.json*/
        Json::Reader jsonReader;                                                                        /*Creating a JSON parser from using the jsoncpp library*/
        Json::Value scaleConfigData;                                                                    /*metadata scale config file containing our scales setup */
        jsonReader.parse(scaleConfigFile, scaleConfigData);                                             /*jsonReader reads & parse the scaleConfig JSON file, then store the results in the scaleConfigData variable*/
        if(debugtool_::UsecaseXCale::SCALE_JSON_DATA_DEBUG)
            std::cout << "scale data: " << std::endl << scaleConfigData << std::endl;                   /* PRINT FOR DEBUG PURPOSES */
        std::vector<std::string> difficultyScaleVec;                                                    /*creation of a difficulty level vector to store all difficulty scales*/
        std::vector<std::string> masteryScaleVec;                                                       /*creation of a mastery level vector to store all mastery scales*/
        std::vector<std::string> speedScaleVec;                                                         /*creation of a speed scale vector to store all speed scales*/
        for(Json::ValueConstIterator it = scaleConfigData["difficulty_scale"].begin(); it != scaleConfigData["difficulty_scale"].end(); ++it)   /*Iterating over skillConfigData and loading skills into skilLVec*/
            difficultyScaleVec.push_back((*it)["label"].asString()); /*Pushing back the difficulty labels from the skillConfig.json to skillVec*/
        for(Json::ValueConstIterator it = scaleConfigData["mastery_level"].begin(); it != scaleConfigData["mastery_level"].end(); ++it)         /*Iterating over skillConfigData and loading skills into skilLVec*/
            masteryScaleVec.push_back((*it)["label"].asString());   /*Pushing back the mastery level labels from the skillConfig.json to skillVec*/
        for(Json::ValueConstIterator it = scaleConfigData["speed_scale"].begin(); it != scaleConfigData["speed_scale"].end(); ++it)             /*Iterating over skillConfigData and loading skills into skilLVec*/
            speedScaleVec.push_back((*it)["label"].asString());   /*Pushing back the speed scale labels from the skillConfig.json to skillVec*/
        /*WARNING : constructor calls must be between "{}" (uniform initialization) instead of "()" (classic initialization) to avoid C++'s annoying ambuiguous parsing phenomenon : Most Vexing Parse ! */
        Scale difficulty_scale{plLabelType(difficultyScaleVec)};
        Scale mastery_scale{plLabelType(masteryScaleVec)};  // Creation of the skill mastery level. in our case, the skill can be mastered from 1 to 3 (1: M_FRG, 2: M_SAT, 3: M_TB)
        Scale speed_scale{plLabelType(speedScaleVec)};
        // NOTE : as created skills requires pre-requisites, the order of variable creation would be from the less dependent skill to the most dependent

        //-----------------------------------------------------------------------------
        // SUBSECTION : Automatic creation of skill topology from skillConfig.json file
        //-----------------------------------------------------------------------------
        std::ifstream skillConfigFile("metadata/skillConfig.json");                                     /*reading skill metadata in skillConfig.json*/
        Json::Value skillConfigData;                                                                    /*metadata skill config file containing our skill topology */
        jsonReader.parse(skillConfigFile, skillConfigData);                                             /*jsonReader reads & parse the skillConfig JSON file, then store the results in the skillConfigData variable*/
        if(debugtool_::UsecaseXCale::SKILL_CREATION_DEBUG)
            std::cout << "skill data: " << std::endl << skillConfigData<< std::endl;                    /* PRINT FOR DEBUG PURPOSES*/
        std::map<std::string, Skill> skillMap;                                                                    /*creation of a skill vector to store all skills*/
        for(Json::ValueConstIterator it = skillConfigData.begin(); it != skillConfigData.end(); ++it)   /*Iterating over skillConfigData and loading skills into skilLVec*/
            skillMap.emplace((*it)["name"].asString(), Skill((*it)["name"].asString(),  mastery_scale, difficulty_scale, speed_scale)); /*Pushing back the skills from the skillConfig.json to skillMap*/
        
        //-----------------------------------------------------------------------------------------
        // SUBSECTION : Creation and Automatic configuration of BKT model from bktConfig.json file
        //-----------------------------------------------------------------------------------------
        BKTModel xcaleModel;                                                                             /* Creation of the BKT model*/
        std::ifstream bktConfigFile("metadata/bktConfig.json");                                          /*reading BKT Config metadata in bktConfig.json*/
        Json::Value bktConfigData;                                                                       /*metadata BKT config file containing our BKT setting and parameters */
        jsonReader.parse(bktConfigFile, bktConfigData);                                                  /*jsonReader reads & parse the bktConfig JSON file, then store the results in the bktConfigData variable*/
        //std::cout << "BKT data: " << std::endl << bktConfigData<< std::endl;                           /* PRINT FOR DEBUG PURPOSES */
        std::vector<double> PInit;                                                                       /*Creating a vector to store Pinit values*/
        std::vector<double> PLearn;                                                                      /*Creating a vector to store PLearn values*/
        for(Json::ValueConstIterator it = bktConfigData["Bkt_params"]["pinit"].begin(); it != bktConfigData["Bkt_params"]["pinit"].end(); ++it)   /*Iterating over bktConfig's pinit list and loading pinit values into PInit vector*/
            PInit.push_back(it->asDouble());                                                             /*Pushing back the PInit values into PInit Vector*/
        if(debugtool_::UsecaseXCale::BKT_PARAM_PINIT_CREATION_DEBUG)
            debugtool_::print_VECTOR__<double>(PInit, false);
        /*Iterating over bktConfig's plearn list and loading PL values into PLearn vector*/
        for(Json::ValueConstIterator it = bktConfigData["Bkt_params"]["plearn"].begin(); it != bktConfigData["Bkt_params"]["plearn"].end(); ++it)   
            PLearn.push_back(it->asDouble());                                                           /*Pushing back the PInit values into PInit Vec*/
        if(debugtool_::UsecaseXCale::BKT_PARAM_PLEARN_CREATION_DEBUG)
            debugtool_::print_VECTOR__<double>(PLearn, false);
        for(std::map<std::string, Skill>::iterator it = skillMap.begin(); it != skillMap.end(); ++it)                                                          /*Iterating over the vector of skills and adding each skills to the BKT xcaleModel*/
        {
            /*¨Adding each skill of skillVec into xcaleModel, with it's parameters from the bktConfig.json file*/
            xcaleModel.addSkill(it->second, PInit, PLearn, 
            bktConfigData["Bkt_params"]["pguess"].asDouble(), 
            bktConfigData["Bkt_params"]["pslip"].asDouble(), 
            bktConfigData["Bkt_params"]["pforget"].asDouble());
        }
        if(debugtool_::UsecaseXCale::BKT_CREATION_DISPLAY_SUMMARY_DEBUG)
            xcaleModel.getDBN()->summary();                                                               /*PRINT THE BKT SUMMARY FOR DEBUG PURPOSES*/

        //-----------------------------------------------------------------------------------------
        // SUBSECTION : Automatic creation of Exercises from exoConfig.json file
        //-----------------------------------------------------------------------------------------
        std::ifstream exoConfigFile("metadata/exoConfig.json");                                           /*reading exercises metadata in bktConfig.json*/
        Json::Value exoConfigData;                                                                        /*metadata Exercises config file containing our exercises' structures*/
        jsonReader.parse(exoConfigFile, exoConfigData);                                                   /*jsonReader reads & parse the exoConfig JSON file, then store the results in the exoConfigData variable*/
        if(debugtool_::UsecaseXCale::EXERCISE_JSON_DATA_DEBUG)
            std::cout << "Exo data: " << std::endl << exoConfigData<< std::endl;                          /* PRINT FOR DEBUG PURPOSES */
        std::vector<Exercise> exoVec;
        for(Json::ValueConstIterator it = exoConfigData.begin(); it != exoConfigData.end(); ++it)   
        {
            if(debugtool_::UsecaseXCale::EXERCISE_CREATION_DEBUG)
                std::cout << "Exo Name: " << (*it)["name"] << ", Exo Difficulty: " << (*it)["difficulty"] << std::endl; /*FOR DEDBUG PURPOSES*/
            exoVec.push_back(Exercise((*it)["name"].asString(), difficulty_scale, diffToLabel((*it)["difficulty"].asInt())));
        }

        //-----------------------------------------------------------------------------------------
        // SUBSECTION : Automatic Skill linking to each exercises in exoVec
        //-----------------------------------------------------------------------------------------
        for(size_t i = 0; i < exoVec.size(); i++)
            for(Json::ValueConstIterator it = exoConfigData.begin(); it != exoConfigData.end(); ++it)
                if(exoVec[i].getName() == (*it)["name"].asString() && exoVec[i].getDifficulty() == diffToLabel((*it)["difficulty"].asInt()))
                    for(Json::ValueConstIterator it2 = (*it)["mobilised_skills"].begin(); it2 != (*it)["mobilised_skills"].end(); ++it2)
                        exoVec[i].linkSkill(skillMap[it2->asString()]);

        //-----------------------------------------------------------------------------------------
        // SUBSECTION : Automatic creation of learners from learner.json data file
        //-----------------------------------------------------------------------------------------
        std::ifstream learnerDataFile("data/learnerData.json");                                     /*Reading learner data in learnerData.json*/
        Json::Value learnerData;                                                                    /*learner Data variable containing our learner traces*/
        jsonReader.parse(learnerDataFile, learnerData);                                             /*jsonReader reads & parse the learnerData JSON file, then store the results in the learnerData variable*/
        if(debugtool_::UsecaseXCale::LEARNER_JSON_DATA_DEBUG)
            std::cout << "Learner data: " << std::endl << learnerData<< std::endl;                  /* PRINT FOR DEBUG PURPOSES */
        std::map<learnerID, Student> learnerMap;                                                    /*Create a maps of learners of vectors */
        std::vector<Student> learnerVec; 
        std::vector<Exercise> exoVecTemp;
        for(Json::ValueConstIterator it = learnerData.begin(); it != learnerData.end(); ++it)
        {
            if(learnerMap.find((*it)["LEARNER"].asInt()) != learnerMap.end()) /* Check if key already exists in map to avoid overwriting it */
            {
                for(int i = 0; i < exoVec.size(); i++)
                    if(exoVec[i].getName() == (*it)["SUJET"].asString() && exoVec[i].getDifficulty() == diffToLabel((*it)["VER"].asInt() - 1))
                        learnerMap[(*it)["LEARNER"].asInt()].setExercise(exoVec[i]);
            }
            else
            {
                for(int i = 0; i < exoVec.size(); i++)
                    if(exoVec[i].getName() == (*it)["SUJET"].asString() && exoVec[i].getDifficulty() == diffToLabel((*it)["VER"].asInt() - 1))
                        exoVecTemp.push_back(exoVec[i]);
                for(int i=0; i<exoVecTemp.size(); i++)
                    std::cout << exoVecTemp[i] << std::endl;
                if(!exoVecTemp.empty())
                {
                    std::cout << (*it)["LEARNER"].asInt() << std::endl;
                    Student learner((*it)["LEARNER"].asString(), exoVecTemp);
                    learnerVec.push_back(Student{(*it)["LEARNER"].asString(), exoVecTemp});
                    //learnerMap.emplace((*it)["LEARNER"].asInt(),  Student{(*it)["LEARNER"].asString(), exoVecTemp});           /* THROW A PlError 128*/
                    exoVecTemp.clear();
                    learner.setBKT(xcaleModel);
                    ExercisesEvaluations evals(mastery_scale);
                    for(int i=0; i<exoVec.size(); i++)
                    {
                        // evals.addEvaluation(exoVec[i], skillMap[exoVec[i].getName()], labelToDiff(exoVec[i].getDifficulty()));
                        // learner.initDBN();
                        // learner.doExercise(exoVec[i], evals.getEvaluation(exoVec[i])); /* THROW A PlError 120 */
                        std::cout << "------------LEARNER " << (*it)["LEARNER"].asString() << "----------------" << std::endl;
                        std::cout << "---------------------------------------" << std::endl;
                        std::cout << "Displaying probas after first exercice:" << std::endl;
                        std::cout << "---------------------------------------" << std::endl;
                        //print the initial probabilities for each skill
                        //learner.getAllActualSkills();

                    }

                }
            }
        }

    }
}