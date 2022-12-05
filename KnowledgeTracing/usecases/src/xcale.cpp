/*###################################-XCALE_CPP-#######################################
  ##	XCALE Lib C++20, v1.0
  ##   	Copyright (C) 2022-2024 Amine Boulahmel: <amine.boulahmel@imt-atlantique.fr>
  ##                            Quentin Couland: <quentin.coulandl@univ-nantes.fr>
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
//
// Created by Couland-Q   on 24/10/22.
//
// Created by Boulahmel-A on 15/09/2022.
// 
//

#include "../include/xcale.h"
#include "../include/serialize_deserialize.h"

//-----------------------------------------------------------------------------
// SECTION : xCALE usecase implementation
//-----------------------------------------------------------------------------
namespace UsecaseXCale 
{
    void parseAndExport(std::string filepath)
    {
        std::cout << "------------------------" << std::endl;
        std::cout << "BEGIN PARSING PROCESS"    << std::endl;
        std::cout << "------------------------" << std::endl;
        std::ifstream result("result.txt");
        if(result.is_open())
        {
            std::string line;
            while(std::getline(result, line))
            {
                std::cout << line << std::endl;
            }
        }
        else
            std::cerr << "ifstream Error : cannot open file " << filepath << ": No such file or directory" << std::endl;
    }

    void displayLearnerSkills(Student learner, Exercise ex, bool display=true)
    {
        if(display)
        {
            std::string exerciseName = ex.getName();
            std::string exerciseDiff = ex.getDifficulty();
            std::string learnerName  = learner.getName();

            std::cout << "--------------------------LEARNER " << learnerName << "--------------------------" << std::endl;
            std::cout << "---------------------------------------------------------------" << std::endl;
            std::cout << "Displaying probas after exercice "  << exerciseName << " with difficulty = " << exerciseDiff << std::endl;
            std::cout << "---------------------------------------------------------------" << std::endl;
            learner.getAllActualSkills();
        }
    }
    void xcaleMANUAL() 
    {

        plError::always_display_warning(debugtool_::UsecaseXCale::PL_ALWAYS_DISPLAY_WARNING);                        // PL library warning Debug
        //-----------------------------------------------------------------------------
        // SUBSECTION : Manual creation of scales
        //-----------------------------------------------------------------------------
        Scale mastery_scale(plLabelType({"0 - invalide", "1 - insuffisant", "2 - fragile", "3 - satisfaisant"}));      // Creation of the skill mastery level. in our case, the skill can be mastered from 1 to 3 (1: M_FRG, 2: M_SAT, 3: M_TB)
        Scale difficulty_scale(plLabelType({"1 - facile", "2 - moyen", "3 - dur"}));                                 // Creation of the difficulty scale. For this use case, three difficulty will be created
        Scale speed_scale(plLabelType({"0 - nulle", "1 - lent", "2 - moyen", "3 - rapide"}));                        // Creation of the speed scale. 

        //-----------------------------------------------------------------------------
        // SUBSECTION : Manual creation of skill topology
        //-----------------------------------------------------------------------------
        Skill C01("C01", mastery_scale, difficulty_scale, speed_scale);                                              // For this usecase, only three skills will be created
        Skill C02("C02", mastery_scale, difficulty_scale, speed_scale);                                              // For this usecase, only three skills will be created
        Skill C03("C03", mastery_scale, difficulty_scale, speed_scale);                                              // For this usecase, only three skills will be created

        //-----------------------------------------------------------------------------------------
        // SUBSECTION : Manual Creation & Configuration of BKT model
        //-----------------------------------------------------------------------------------------
        // NOTE : as created skills requires pre-requisites, the order of variable creation would be from the less dependent skill to the most dependent
        /* Add the skills to the Model : in our setting :
        -----------------------------------------------------------------------------------------------------------------------------------------------------------------
            + PG    = 0.10                   - Initializing Probability of Guessing
            + PInit = (0.80, 0.10,0.08,0.02) - Initializing Prior Probabilities for the four Mastery level
            + PL    = (0.60, 0.10, 0.10)     - Initializing Probabilities of Learning. In this case, all PLs are the same for all mastery level
            + PS    = 0.05                   - Initializing Probability of Slip
            + PF    = 0                      - Initializing Probability of Forgetting Skill. In this setting, we assume that a learner can't forget a skill once learned
        -----------------------------------------------------------------------------------------------------------------------------------------------------------------
        */
    
        BKTModel xcaleModel;                                                                                           // Creation of the BKT model
        BKTParam param;                                                                                                // Creation of our model parameters
        param.setPguess(0.10f);
        param.setPinit({0.80f, 0.10f, 0.08f, 0.02f});                                                                  // ! BEWARE ! sum(pinit) =1 AND pinit(n)  > pinit(n+1)
        param.setPlearn({0.10f, 0.30f, 0.60f});                                                                        // ! BEWARE ! sum(plearn)=1 AND plearn(n) < plearn(n+1) [The plearn vector size is proportional to the speed scale size - 1]
        param.setPslip(0.05f);
        param.setPforget(0.0f);
        xcaleModel.addSkill(C01, param.getPinit(), param.getPlearn(), param.getPguess(), param.getPslip(), param.getPforget());                 // Configuration of the BKT Model + Skill C01 linkage
        xcaleModel.addSkill(C02, param.getPinit(), param.getPlearn(), param.getPguess(), param.getPslip(), param.getPforget());                 // Configuration of the BKT Model + Skill C02 linkage
        xcaleModel.addSkill(C03, param.getPinit(), param.getPlearn(), param.getPguess(), param.getPslip(), param.getPforget());                 // Configuration of the BKT Model + Skill C11 linkage
        if(debugtool_::UsecaseXCale::BKT_CREATION_DISPLAY_SUMMARY_DEBUG)                                               // PRINT FOR DEBUG PURPOSES
            xcaleModel.getDBN()->summary();
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
        //-----------------------------------------------------------------------------------------
        // SUBSECTION : Manual Creation & configuration of exercises 
        //-----------------------------------------------------------------------------------------
        Exercise E01_01("Melodie#1",    difficulty_scale,    "1 - facile");                                            // Exercise Melodie of version 2 -> Difficulty 1
        Exercise E01_02("Melodie#2",    difficulty_scale,    "2 - moyen");                                             // Exercise Melodie of version 3 -> Difficulty 2
        Exercise E01_03("Melodie#3",    difficulty_scale,    "3 - dur");                                               // Exercise Melodie of version 4 -> Difficulty 3

        Exercise E02_01("Alternance#1", difficulty_scale,    "1 - facile");                                            // Exercise Alternance of version 2 -> Difficulty 1
        Exercise E02_02("Alternance#2", difficulty_scale,    "2 - moyen");                                             // Exercise Alternance of version 3 -> Difficulty 2
        Exercise E02_03("Alternance#3", difficulty_scale,    "3 - dur");                                               // Exercise Alternance of version 4 -> Difficulty 3


        //-----------------------------------------------------------------------------------------
        // SUBSECTION : Manual Skill linking for each exercise
        //-----------------------------------------------------------------------------------------
        E01_01.linkSkill(C01);                                                                                       // E01_01 mobilizes only skill C01
        E01_02.linkSkill(C02);                                                                                       // E01_02 mobilizes only skill C02
        E01_03.linkSkill(C02);                                                                                       // E01_03 mobilizes also skill C02

        E02_01.linkSkill(C01);                                                                                       // E02_01 mobilizes      skill C01
        E02_01.linkSkill(C02);                                                                                       // E02_01 mobilizes also skill C02
        E02_01.linkSkill(C03);                                                                                       // E02_01 mobilizes also skill C03

        E02_02.linkSkill(C01);                                                                                       // E02_02 mobilizes      skill C01
        E02_02.linkSkill(C02);                                                                                       // E02_02 mobilizes also skill C02
        E02_02.linkSkill(C03);                                                                                       // E02_02 mobilizes also skill C03

        E02_03.linkSkill(C01);                                                                                       // E02_03 mobilizes      skill C01
        E02_03.linkSkill(C02);                                                                                       // E02_03 mobilizes also skill C02
        E02_03.linkSkill(C03);                                                                                       // E02_03 mobilizes also skill C03

        //-----------------------------------------------------------------------------------------
        // SUBSECTION : Manual Creation learners and link them with their exercises
        //-----------------------------------------------------------------------------------------
        Student Learner_162("162",{E01_01, E01_02, E01_03, E02_01, E02_02});                                         // Learner 162 -> index: 0 : made Exercise Melodie in three difficulties, and Alternance in only two difficulties
        Student Learner_167("167",{E01_01, E01_02, E01_03, E02_01, E02_02, E02_03});                                 // Learner 167 -> index: 1 : made Exercise Melodie and Alternance in three difficulties
        Student Learner_173("173",{E01_01, E01_02, E01_03, E02_01, E02_02, E02_03});                                 // Learner 173 -> index: 2 : made Exercise Melodie and Alternance in three difficulties

        //-----------------------------------------------------------------------------------------
        // SUBSECTION : Manual model linking with their learners
        //-----------------------------------------------------------------------------------------
        Learner_162.setBKT(xcaleModel);                                                                              // Linking Learner 162 with the xcaleModel
        Learner_167.setBKT(xcaleModel);                                                                              // Linking Learner 167 with the xcaleModel
        Learner_173.setBKT(xcaleModel);                                                                              // Linking Learner 173 with the xcaleModel

        //-----------------------------------------------------------------------------------------
        // SUBSECTION : Manual Creation of evaluations for each Learner along with their exercisess
        //-----------------------------------------------------------------------------------------
        ExercisesEvaluations eval_162(mastery_scale);                                                                // Exercise Evaluation of learner 162
        ExercisesEvaluations eval_167(mastery_scale);                                                                // Exercise Evaluation of learner 167
        ExercisesEvaluations eval_173(mastery_scale);                                                                // Exercise Evaluation of learner 173

        //-----------------------------------------------------------------------------------------
        // SUBSECTION : Manual evaluation of the observed skill for each learner
        //-----------------------------------------------------------------------------------------
        //-----------------------------------------------------------------------------------------
        /* link the skills to the Model : in our setting :
        // NOTE : In this use case, the evaluation model goes as follow : if a learner perform on an exercise of difficulty n, then the eval = n
        -----------------------------------------------------------------------------------------------------------------------------------------------------------------
            + E01_01 : [C01]                 - Exercise Melodie,    with Difficulty diff=1 observes skill C01
            + E01_02 : [C02]                 - Exercise Melodie,    with Difficulty diff=2 observes skill C02
            + E01_03 : [C02]                 - Exercise Melodie,    with Difficulty diff=3 observes skill C03
            + E02_01 : [C03]                 - Exercise Alternance, with Difficulty diff=1 observes skill C03
            + E02_02 : [C03]                 - Exercise Alternance, with Difficulty diff=2 observes skill C03
            + E02_03 : [C03]                 - Exercise Alternance, with Difficulty diff=3 observes skill C03
        -----------------------------------------------------------------------------------------------------------------------------------------------------------------
        /* link the skills to the Model : in our setting :
        // NOTE : In this use case, the evaluation model goes as follow : if a learner perform on an exercise of difficulty n, then the eval = n
        -----------------------------------------------------------------------------------------------------------------------------------------------------------------
            + eval  = 0                      - Learner have not acquired any skill
            + eval  = 1                      - Learner have performed and validated an exercise of difficulty 1, therefore eval = 1
            + eval  = 2                      - Learner have performed and validated an exercise of difficulty 2, therefore eval = 2
            + eval  = 3                      - Learner have performed and validated an exercise of difficulty 3, therefore eval = 3
        -----------------------------------------------------------------------------------------------------------------------------------------------------------------
        */ 

        eval_162.addEvaluation(E01_01, C01, 1);                                                                      // E01_01 Observes skill C01, AND learner 162 validated the exercise  -------x CHECK
        eval_162.addEvaluation(E01_02, C02, 2);                                                                      // E01_02 Observes skill C02, AND learner 162 validated the exercise  -------x CHECK
        eval_162.addEvaluation(E01_03, C02, 3);                                                                      // E01_03 Observes skill C02, AND learner 162 validated the exercise  -------x CHECK
        eval_162.addEvaluation(E02_01, C03, 1);                                                                      // E02_01 Observes skill C03, AND learner 162 validated the exercise  -------x CHECK
        eval_162.addEvaluation(E02_02, C03, 2);                                                                      // E02_02 Observes skill C03, AND learner 162 validated the exercise  -------x CHECK
        /*eval_162.addEvaluation(E02_03, C03, 3);*/                                                                  // E02_03 Observes skill C03, AND learner 162 didn't do the exxercise -------x CHECK

        eval_167.addEvaluation(E01_01, C01, 0);                                                                      // E01_01 Observes skill C01, AND learner 167 didn't validate the exercise --x CHECK
        eval_167.addEvaluation(E01_02, C02, 0);                                                                      // E01_02 Observes skill C02, AND learner 167 didn't validate the exercise --x CHECK
        eval_167.addEvaluation(E01_03, C02, 3);                                                                      // E01_03 Observes skill C02, AND learner 167 validated the exercise  -------x CHECK
        eval_167.addEvaluation(E02_01, C03, 0);                                                                      // E02_01 Observes skill C03, AND learner 167 validated the exercise  -------x CHECK
        eval_167.addEvaluation(E02_02, C03, 2);                                                                      // E02_02 Observes skill C03, AND learner 167 validated the exercise  -------x CHECK
        eval_167.addEvaluation(E02_03, C03, 0);                                                                      // E02_03 Observes skill C03, AND learner 167 didn't validate the exercise --x CHECK

        eval_173.addEvaluation(E01_01, C01, 1);                                                                      // E01_01 Observes skill C01, AND learner 173 validated the exercise --------x CHECK
        eval_173.addEvaluation(E01_02, C02, 2);                                                                      // E01_02 Observes skill C02, AND learner 173 validated the exercise --------x CHECK
        eval_173.addEvaluation(E01_03, C02, 0);                                                                      // E01_03 Observes skill C02, AND learner 173 validated the exercise --------x CHECK
        eval_173.addEvaluation(E02_01, C03, 1);                                                                      // E02_01 Observes skill C03, AND learner 173 validated the exercise --------x CHECK
        eval_173.addEvaluation(E02_02, C03, 2);                                                                      // E02_02 Observes skill C03, AND learner 173 validated the exercise --------x CHECK
        eval_173.addEvaluation(E02_03, C03, 3);                                                                      // E02_03 Observes skill C03, AND learner 173 validated the exercise --------x CHECK

        //------------------------------------------------------------------------------------------
        // SUBSECTION : + Manual initialization of each learners with their DBN 
        //              + (in this case, all learners will have the same model)
        //              + Shouldn't the initDBN() method be renamed to initBKT() ?
        //------------------------------------------------------------------------------------------
        Learner_162.initDBN();                                                                                       // Initialize BKT Model for learner 162
        Learner_167.initDBN();                                                                                       // Initialize BKT Model for learner 167
        Learner_173.initDBN();                                                                                       // Initialize BKT Model for learner 173

        //-----------------------------------------------------------------------------------------
        // SUBSECTION : Manual Vizualisations of learners' skills after doing exercises
        //-----------------------------------------------------------------------------------------
        Learner_162.doExercise(E01_01, eval_162.getEvaluation(E01_01));                                             // Do Exercise Melodie of Diff=1 for learner 162 and get it's evaluation
        displayLearnerSkills(Learner_162, E01_01, false);                                                           // Display the actual skills of learner 162 after doing Melodie of Diff=1
        Learner_162.doExercise(E01_02, eval_162.getEvaluation(E01_02));                                             // Do Exercise Melodie of Diff=2 for learner 162 and get it's evaluation
        displayLearnerSkills(Learner_162, E01_02, false);                                                           // Display the actual skills of learner 162 after doing Melodie of Diff=2
        Learner_162.doExercise(E01_03, eval_162.getEvaluation(E01_03));                                             // Do Exercise Melodie of Diff=3 for learner 162 and get it's evaluation
        displayLearnerSkills(Learner_162, E01_03, false);                                                           // Display the actual skills of learner 162 after doing Melodie of Diff=3
        Learner_162.doExercise(E02_01, eval_162.getEvaluation(E02_01));                                             // Do Exercise Alternance of Diff=1 for learner 162 and get it's evaluation
        displayLearnerSkills(Learner_162, E02_01, false);                                                           // Display the actual skills of learner 162 after doing Alternance of Diff=3
        Learner_162.doExercise(E02_02, eval_162.getEvaluation(E02_02));                                             // Do Exercise Alternance of Diff=2 for learner 162 and get it's evaluation
        displayLearnerSkills(Learner_162, E02_02, false);                                                           // Display the actual skills of learner 162 after doing Alternance of Diff=2

        Learner_167.doExercise(E01_01, eval_167.getEvaluation(E01_01));                                             // Do Exercise Melodie of Diff=1 for learner 167 and get it's evaluation    
        displayLearnerSkills(Learner_167, E01_01, false);                                                           // Display the actual skills of learner 167 after doing Melodie of Diff=1   
        Learner_167.doExercise(E01_02, eval_167.getEvaluation(E01_02));                                             // Do Exercise Melodie of Diff=2 for learner 167 and get it's evaluation   
        displayLearnerSkills(Learner_167, E01_02, false);                                                           // Display the actual skills of learner 167 after doing Melodie of Diff=2   
        Learner_167.doExercise(E01_03, eval_167.getEvaluation(E01_03));                                             // Do Exercise Melodie of Diff=3 for learner 167 and get it's evaluation    
        displayLearnerSkills(Learner_167, E01_03, false);                                                           // Display the actual skills of learner 167 after doing Melodie of Diff=3   
        Learner_167.doExercise(E02_01, eval_167.getEvaluation(E02_01));                                             // Do Exercise Alternance of Diff=1 for learner 167 and get it's evaluation 
        displayLearnerSkills(Learner_167, E02_01, false);                                                           // Display the actual skills of learner 167 after doing Alternance of Diff=1
        Learner_167.doExercise(E02_02, eval_167.getEvaluation(E02_02));                                             // Do Exercise Alternance of Diff=2 for learner 167 and get it's evaluation 
        displayLearnerSkills(Learner_167, E02_02, false);                                                           // Display the actual skills of learner 167 after doing Alternance of Diff=2 
        Learner_167.doExercise(E02_03, eval_167.getEvaluation(E02_03));                                             // Do Exercise Alternance of Diff=3 for learner 167 and get it's evaluation  
        displayLearnerSkills(Learner_167, E02_03, false);                                                           // Display the actual skills of learner 167 after doing Alternance of Diff=3

        Learner_173.doExercise(E01_01, eval_173.getEvaluation(E01_01));                                             // Do Exercise Melodie of Diff=1 for learner 173 and get it's evaluation    
        displayLearnerSkills(Learner_173, E01_01, true);                                                           // Display the actual skills of learner 173 after doing Melodie of Diff=1   
        Learner_173.doExercise(E01_02, eval_173.getEvaluation(E01_02));                                             // Do Exercise Melodie of Diff=2 for learner 173 and get it's evaluation    
        displayLearnerSkills(Learner_173, E01_02, true);                                                           // Display the actual skills of learner 173 after doing Melodie of Diff=2   
        Learner_173.doExercise(E01_03, eval_173.getEvaluation(E01_03));                                             // Do Exercise Melodie of Diff=3 for learner 173 and get it's evaluation    
        displayLearnerSkills(Learner_173, E01_03, true);                                                           // Display the actual skills of learner 173 after doing Melodie of Diff=3   
        Learner_173.doExercise(E02_01, eval_173.getEvaluation(E02_01));                                             // Do Exercise Alternance of Diff=1 for learner 173 and get it's evaluation 
        displayLearnerSkills(Learner_173, E02_01, true);                                                           // Display the actual skills of learner 173 after doing Alternance of Diff=1
        Learner_173.doExercise(E02_02, eval_173.getEvaluation(E02_02));                                             // Do Exercise Alternance of Diff=2 for learner 173 and get it's evaluation 
        displayLearnerSkills(Learner_173, E02_02, true);                                                           // Display the actual skills of learner 173 after doing Alternance of Diff=2
        Learner_173.doExercise(E02_03, eval_173.getEvaluation(E02_03));                                             // Do Exercise Alternance of Diff=3 for learner 173 and get it's evaluation 
        displayLearnerSkills(Learner_173, E02_03, true);                                                           // Display the actual skills of learner 173 after doing Alternance of Diff=3

        //-----------------------------------------------------------------------------------------
        // SUBSECTION : Export all of the probability results into a csv file
        //-----------------------------------------------------------------------------------------
        //parseAndExport("result.txt");
    }
    
}