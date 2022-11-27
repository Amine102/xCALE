//
// Created by Boulahmel-A on 15/09/2022.
//

#include "../include/xCaleBKT.h"


/* Find a C++ library to connect to database phpmydmin, and automatically create 
   student class inside the code*/

/*Retracer les appels de fonctions sur son exemple pour comprendre les Z*/
 
void xCale_BKT() {

    plError::always_display_warning(false);

    BKTModel xcaleModel;  // Creation of the BKT model
    //MasteryLevel echelle({"1 - M_FRG", "2 - M_SAT", "3 - M_TB"});   // Creation of the skill mastery level. in our case, the skill can be mastered from 1 to 3 (1: M_FRG, 2: M_SAT, 3: M_TB)
    MasteryLevel echelle;   // Creation of the skill mastery level. in our case, the skill can be mastered from 1 to 3 (1: M_FRG, 2: M_SAT, 3: M_TB)

    // NOTE : as created skills requires pre-requisites, the order of variable creation would be from the less dependent skill to the most dependent

    /*Dependency 0 : Creation of skills that do not need any pre-requisites*/
    //Skill C1("savoir appeler une fonction sans argument"    , echelle, {});
    Skill C1("S1"    , echelle, {});
    Skill C8("Savoir utiliser les opérateurs arithmetiques" , echelle, {});
    Skill C9("savoir utiliser les operateurs de comparaison", echelle, {});
    Skill C10("savoir utiliser les operateurs logiques"     , echelle, {});
    Skill C11("Savoir utiliser une variable"                , echelle, {});

    /*Dependency 1 : Creation of skills that needs pre-requisite*/    
    Skill C2("savoir appeler une fonction avec argument"    , echelle, {/*C1, C11*/});
    Skill C3("savoir ecrire une boucle repeter x fois"      , echelle, {/*C11*/});
    Skill C4("savoir ecrire une boucle tant-que"            , echelle, {/*C9, C11*/});   
    Skill C6("savoir utiliser un if"                        , echelle, {/*C9, C10*/});
    Skill C12("Savoir incrementer une variable"             , echelle, {/*C8, C11*/});

    /*Dependency 2 : Creation of skills that needs pre-requisites from Dependency 1 skills*/
    Skill C5("savoir imbriquer deux boucles"                , echelle, {/*C3, C4*/});
    Skill C7("Savoir utiliser un if-else"                   , echelle, {/*C6*/});

    /*link the skills to the Model : in our setting :
    -----------------------------------------------------------------------------------------------------------------------------------------------------------------
        + PG    = 0.10               - Initializing Probability of Guessing
        + PInit = (0.80, 0.10, 0.08) - Initializing Prior Probabilities for the three Mastery level
        + PL    = (0.10, 0.10, 0.10) - Initializing Probabilities of Learning. In this case, all PLs are the same for all mastery level
        + PS    = 0.05               - Initializing Probability of Slip
        + PF    = 0                  - Initializing Probability of Forgetting Skill. In this setting, we assume that a learner can't forget a skill once learned
    -----------------------------------------------------------------------------------------------------------------------------------------------------------------
        + flatten   = 0              - Initializing Probability of Forgetting Skill. In this setting, we assume that a learner can't forget a skill once learned
    */ 
   
    // Pull la librairie sur Master
    // Je rajoute toutes les compétences dans le modèle 
    // Créer des profils étudiants 
    // 

    xcaleModel.addSkill(C1, 0.1, {0.80, 0.10, 0.08, 0.02}, {0.1, 0.1, 0.1, 0.1}, 0.05, 0, {{1, 0, 0, 0},
                                                                                             {0, 1, 0, 0},
                                                                                             {0, 0, 1, 0},
                                                                                             {0, 0, 0, 1}});
    // xcaleModel.addSkill(C2, 0.1, {0.80, 0.10, 0.08, 0.02}, {0.1, 0.1, 0.1, 0.1}, 0.05, 0, {{1, 0, 0, 0},
    //                                                                                          {0, 1, 0, 0},
    //                                                                                          {0, 0, 1, 0},
    //                                                                                          {0, 0, 0, 1}});                                                                                      


    // xcaleModel.addSkill(C1, 0.1, {0.80, 0.10, 0.08}, {0.1, 0.1, 0.1}, 0.05, 0, {{1, 0, 0},
    //                                                                             {0, 1, 0},
    //                                                                             {0, 0, 1}});


    // xcaleModel.addSkill(C2, 0.1, {0.80, 0.10, 0.08}, {0.1, 0.1, 0.1}, 0.05, 0, {{1, 0, 0},
    //                                                                               {0, 1, 0},
    //                                                                               {0, 0, 1}});
    // xcaleModel.addSkill(C3, 0.1, {0.80, 0.10, 0.08}, {0.1, 0.1, 0.1}, 0.05, 0, {{1, 0, 0},
    //                                                                               {0, 1, 0},
    //                                                                               {0, 0, 1}});
    // xcaleModel.addSkill(C4, 0.1, {0.80, 0.10, 0.08}, {0.1, 0.1, 0.1}, 0.05, 0, {{1, 0, 0},
    //                                                                               {0, 1, 0},
    //                                                                               {0, 0, 1}});
    // xcaleModel.addSkill(C5, 0.1, {0.80, 0.10, 0.08}, {0.1, 0.1, 0.1}, 0.05, 0, {{1, 0, 0},
    //                                                                               {0, 1, 0},
    //                                                                               {0, 0, 1}});
    // xcaleModel.addSkill(C6, 0.1, {0.80, 0.10, 0.08}, {0.1, 0.1, 0.1}, 0.05, 0, {{1, 0, 0},
    //                                                                               {0, 1, 0},
    //                                                                               {0, 0, 1}});
    // xcaleModel.addSkill(C7, 0.1, {0.80, 0.10, 0.08}, {0.1, 0.1, 0.1}, 0.05, 0, {{1, 0, 0},
    //                                                                               {0, 1, 0},
    //                                                                               {0, 0, 1}});                                                                                                                                                                                                                                          
    // xcaleModel.addSkill(C8, 0.1, {0.80, 0.10, 0.08}, {0.1, 0.1, 0.1}, 0.05, 0, {{1, 0, 0},
    //                                                                               {0, 1, 0},
    //                                                                               {0, 0, 1}});
    // xcaleModel.addSkill(C9, 0.1, {0.80, 0.10, 0.08}, {0.1, 0.1, 0.1}, 0.05, 0, {{1, 0, 0},
    //                                                                               {0, 1, 0},
    //                                                                               {0, 0, 1}});            
    // xcaleModel.addSkill(C10, 0.1, {0.80, 0.10, 0.08}, {0.1, 0.1, 0.1}, 0.05, 0, {{1, 0, 0},
    //                                                                               {0, 1, 0},
    //                                                                               {0, 0, 1}});
    // xcaleModel.addSkill(C11, 0.1, {0.80, 0.10, 0.08}, {0.1, 0.1, 0.1}, 0.05, 0, {{1, 0, 0},
    //                                                                               {0, 1, 0},
    //                                                                               {0, 0, 1}}); 
    // xcaleModel.addSkill(C12, 0.1, {0.80, 0.10, 0.08}, {0.1, 0.1, 0.1}, 0.05, 0, {{1, 0, 0},
    //                                                                               {0, 1, 0},
    //                                                                               {0, 0, 1}});                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
    //create the exercises
    Exercise ex1_1("Melodie_1");
    Exercise ex1_2("Melodie_2");
    Exercise ex1_3("Melodie_3");

    Exercise ex2_1("Alternance_1");
    Exercise ex2_2("Alternance_2");
    Exercise ex2_3("Alternance_3");

    Exercise ex3_1("Show_Lumineux_1");
    Exercise ex3_2("Show_Lumineux_2");
    Exercise ex3_3("Show_Lumineux_3");

    Exercise ex4_1("Quelle_Direction_1");
    Exercise ex4_2("Quelle_Direction_2");
    Exercise ex4_3("Quelle_Direction_3");

    Exercise ex5_1("Instrument_1");
    Exercise ex5_2("Instrument_2");
    Exercise ex5_3("Instrument_3");

    Exercise ex6_1("Show_Lumineux2_1");
    Exercise ex6_2("Show_Lumineux2_2");
    Exercise ex6_3("Show_Lumineux2_3");

    Exercise ex7_1("Avertisseur_1");
    Exercise ex7_2("Avertisseur_2");
    Exercise ex7_3("Avertisseur_3");

    Exercise ex8_1("Serveur_Chronometre_1");
    Exercise ex8_2("Serveur_Chronometre_2");
    Exercise ex8_3("Serveur_Chronometre_3");

    Exercise ex9_1("Sommeil_Agreable_1");
    Exercise ex9_2("Sommeil_Agreable_2");
    Exercise ex9_3("Sommeil_Agreable_3");

    Exercise ex10_1("Reveil_matin_1");
    Exercise ex10_2("Reveil_matin_2");
    Exercise ex10_3("Reveil_matin_3");

    Exercise ex11_1("Secousses_1");
    Exercise ex11_2("Secousses_2");
    Exercise ex11_3("Secousses_3");

    Exercise ex12_1("Detection_mouvement_1");
    Exercise ex12_2("Detection_mouvement_2");
    Exercise ex12_3("Detection_mouvement_3");

    Exercise ex13_1("Sirene_1");
    Exercise ex13_2("Sirene_2");
    Exercise ex13_3("Sirene_3");

    Exercise ex14_1("Applaudimetre_1");
    Exercise ex14_2("Applaudimetre_2");
    Exercise ex14_3("Applaudimetre_3");

    Exercise ex15_1("Binaire_1");
    Exercise ex15_2("Binaire_2");
    Exercise ex15_3("Binaire_3");

    Exercise ex16_1("Bille_1");
    Exercise ex16_2("Bille_2");
    Exercise ex16_3("Bille_3");


    

    // Link the skills with their exercices

    /*Exercice Melodie*/
    ex1_1.linkSkill(C1); /*Observed*/
    ex1_2.linkSkill(C2); /*Observed*/
    ex1_3.linkSkill(C3); /*Observed*/

    /*Exercice Alternance*/
    ex2_1.linkSkill(C1);
    // ex2_1.linkSkill(C2);
    //ex2_1.linkSkill(C3); /*Observed*/
    // ex2_2.linkSkill(C1); 
    // ex2_2.linkSkill(C2); 
    ex2_2.linkSkill(C3); /*Observed*/
    // ex2_3.linkSkill(C1); 
    // ex2_3.linkSkill(C2); 
    ex2_3.linkSkill(C3); /*Observed*/
    
    /*Exercice Show Lumineux*/
    // ex3_1.linkSkill(C1); 
    // ex3_1.linkSkill(C2);
    ex3_1.linkSkill(C3); /*Observed*/
    // ex3_2.linkSkill(C1); 
    // ex3_2.linkSkill(C2); 
    ex3_2.linkSkill(C3); /*Observed*/
    // ex3_3.linkSkill(C1); 
    // ex3_3.linkSkill(C2); 
    ex3_3.linkSkill(C3); /*Observed*/

    /*Exercice Quelle Direction*/
    // ex4_1.linkSkill(C1);
    // ex4_1.linkSkill(C2);
    // ex4_1.linkSkill(C4);
    ex4_1.linkSkill(C6); /*Observed*/
    // ex4_2.linkSkill(C1);
    // ex4_2.linkSkill(C2);
    // ex4_2.linkSkill(C4);
    ex4_2.linkSkill(C6); /*Observed*/
    // ex4_3.linkSkill(C1);
    // ex4_3.linkSkill(C2);
    // ex4_3.linkSkill(C4);
    ex4_3.linkSkill(C6); /*Observed*/

    /*Exercice Instrument*/
    // ex5_1.linkSkill(C1);
    // ex5_1.linkSkill(C2);
    // ex5_1.linkSkill(C4);
    // ex5_1.linkSkill(C6);
    ex5_1.linkSkill(C7); /*Observed*/
    // ex5_2.linkSkill(C1);
    // ex5_2.linkSkill(C2);
    // ex5_2.linkSkill(C4);
    // ex5_2.linkSkill(C6);
    ex5_2.linkSkill(C7); /*Observed*/
    // ex5_3.linkSkill(C1);
    // ex5_3.linkSkill(C2);
    // ex5_3.linkSkill(C4);
    // ex5_3.linkSkill(C6);
    ex5_3.linkSkill(C7); /*Observed*/

    /*Exercice Show Lumineux 2*/
    // ex6_1.linkSkill(C2); 
    ex6_1.linkSkill(C5); /*Observed*/
    // ex6_1.linkSkill(C6);
    // ex6_2.linkSkill(C2);
    ex6_2.linkSkill(C5); /*Observed*/
    // ex6_2.linkSkill(C6);
    // ex6_3.linkSkill(C2);
    ex6_3.linkSkill(C5); /*Observed*/
    // ex6_3.linkSkill(C6);
     
    /*Exercice Avertisseur*/
    // ex7_1.linkSkill(C1);
    // ex7_1.linkSkill(C2);
    // ex7_1.linkSkill(C3);
    // ex7_1.linkSkill(C4);
    ex7_1.linkSkill(C6); /*Observed*/
    // ex7_1.linkSkill(C7);
    // ex7_2.linkSkill(C1);
    // ex7_2.linkSkill(C2);
    // ex7_2.linkSkill(C3);
    // ex7_2.linkSkill(C4);
    ex7_2.linkSkill(C6); /*Observed*/
    // ex7_2.linkSkill(C7);
    // ex7_3.linkSkill(C1);
    // ex7_3.linkSkill(C2);
    // ex7_3.linkSkill(C3);
    // ex7_3.linkSkill(C4);
    ex7_3.linkSkill(C6); /*Observed*/
    // ex7_3.linkSkill(C7);

    /*Exercice Servo Chronometre*/
    // ex8_1.linkSkill(C1);
    // ex8_1.linkSkill(C2);
    // ex8_1.linkSkill(C3);
    // ex8_1.linkSkill(C4);
    // ex8_1.linkSkill(C6); 
    ex8_1.linkSkill(C7); /*Observed*/
    // ex8_2.linkSkill(C2);
    // ex8_2.linkSkill(C3);
    // ex8_2.linkSkill(C4);
    // ex8_2.linkSkill(C6);
    ex8_2.linkSkill(C7); /*Observed*/
    // ex8_3.linkSkill(C2);
    // ex8_3.linkSkill(C3);
    // ex8_3.linkSkill(C4);
    // ex8_3.linkSkill(C6);
    ex8_3.linkSkill(C7); /*Observed*/

    /*Exercice Sommeil Agreable*/
    // ex9_1.linkSkill(C2);
    // ex9_1.linkSkill(C4);
    // ex9_1.linkSkill(C6);
    ex9_1.linkSkill(C7); /*Observed*/
    // ex9_1.linkSkill(C10);
    // ex9_2.linkSkill(C2);
    // ex9_2.linkSkill(C4);
    // ex9_2.linkSkill(C6);
    ex9_2.linkSkill(C7); /*Observed*/
    // ex9_2.linkSkill(C10);
    // ex9_3.linkSkill(C2);
    // ex9_3.linkSkill(C4);
    // ex9_3.linkSkill(C6);
    ex9_3.linkSkill(C7); /*Observed*/
    // ex9_3.linkSkill(C10);

    /*Exercice Reveil matin*/
    // ex10_1.linkSkill(C1);
    // ex10_1.linkSkill(C2);
    ex10_1.linkSkill(C4); /*Observed*/
    // ex10_1.linkSkill(C9); 
    // ex10_2.linkSkill(C1);
    // ex10_2.linkSkill(C2);
    ex10_2.linkSkill(C4); /*Observed*/
    // ex10_2.linkSkill(C9); 
    // ex10_2.linkSkill(C10); 
    // ex10_3.linkSkill(C1);
    // ex10_3.linkSkill(C2);
    // ex10_3.linkSkill(C4); 
    // ex10_3.linkSkill(C9); 
    // ex10_3.linkSkill(C10);

    /*Exercice Secousses*/
    // ex11_1.linkSkill(C1);
    // ex11_1.linkSkill(C2);
    // ex11_1.linkSkill(C4);
    // ex11_1.linkSkill(C6);
    // ex11_1.linkSkill(C7);
    // ex11_1.linkSkill(C8);
    // ex11_1.linkSkill(C9);
    ex11_1.linkSkill(C11); /*Observed*/
    // ex11_1.linkSkill(C12);
    // ex11_2.linkSkill(C1);
    // ex11_2.linkSkill(C2);
    // ex11_2.linkSkill(C4);
    // ex11_2.linkSkill(C6);
    // ex11_2.linkSkill(C7);
    // ex11_2.linkSkill(C8);
    // ex11_2.linkSkill(C9);
    // ex11_2.linkSkill(C10);
    ex11_2.linkSkill(C11); /*Observed*/
    // ex11_2.linkSkill(C12);
    // ex11_3.linkSkill(C1);
    // ex11_3.linkSkill(C2);
    // ex11_3.linkSkill(C4);
    // ex11_3.linkSkill(C6);
    // ex11_3.linkSkill(C7);
    // ex11_3.linkSkill(C8);
    // ex11_3.linkSkill(C9);
    // ex11_3.linkSkill(C10);
    ex11_3.linkSkill(C11); /*Observed*/
    // ex11_3.linkSkill(C12);

    /*Exercice Detecteur de Mouvement*/
    // ex12_1.linkSkill(C1);
    // ex12_1.linkSkill(C2);
    // ex12_1.linkSkill(C4);
    // ex12_1.linkSkill(C6);
    // ex12_1.linkSkill(C7);
    // ex12_1.linkSkill(C8);
    // ex12_1.linkSkill(C9);
    ex12_1.linkSkill(C11); /*Observed*/
    // ex12_1.linkSkill(C12);
    // ex12_2.linkSkill(C1);
    // ex12_2.linkSkill(C2);
    // ex12_2.linkSkill(C4);
    // ex12_2.linkSkill(C6);
    // ex12_2.linkSkill(C7);
    // ex12_2.linkSkill(C8);
    // ex12_2.linkSkill(C9);
    // ex12_2.linkSkill(C10);
    ex12_2.linkSkill(C11); /*Observed*/
    // ex12_2.linkSkill(C12);
    // ex12_3.linkSkill(C1);
    // ex12_3.linkSkill(C2);
    // ex12_3.linkSkill(C4);
    // ex12_3.linkSkill(C6);
    // ex12_3.linkSkill(C7);
    // ex12_3.linkSkill(C8);
    // ex12_3.linkSkill(C9);
    // ex12_3.linkSkill(C10);
    ex12_3.linkSkill(C11); /*Observed*/
    // ex12_3.linkSkill(C12);

    /*Exercice Sirene*/
    // ex13_1.linkSkill(C2);
    // ex13_1.linkSkill(C4);
    // ex13_1.linkSkill(C6);
    // ex13_1.linkSkill(C7);
    // ex13_1.linkSkill(C8);
    // ex13_1.linkSkill(C9);
    ex13_1.linkSkill(C11); /*Observed*/
    // ex13_2.linkSkill(C2);
    // ex13_2.linkSkill(C4);
    // ex13_2.linkSkill(C6);
    // ex13_2.linkSkill(C7);
    // ex13_2.linkSkill(C8);
    // ex13_2.linkSkill(C9);
    // ex13_2.linkSkill(C10);
    ex13_2.linkSkill(C11); /*Observed*/
    // ex13_3.linkSkill(C2);
    // ex13_3.linkSkill(C4);
    // ex13_3.linkSkill(C6);
    // ex13_3.linkSkill(C7);
    // ex13_3.linkSkill(C8);
    // ex13_3.linkSkill(C9);
    // ex13_3.linkSkill(C10);
    ex13_3.linkSkill(C11); /*Observed*/

    /*Exercice Applaudimètre*/
    // ex14_1.linkSkill(C1);
    // ex14_1.linkSkill(C2);
    // ex14_1.linkSkill(C4);
    // ex14_1.linkSkill(C6);
    // ex14_1.linkSkill(C7);
    // ex14_1.linkSkill(C9);
    // ex14_1.linkSkill(C10);
    ex14_1.linkSkill(C11); /*Observed*/
    // ex14_2.linkSkill(C1);
    // ex14_2.linkSkill(C2);
    // ex14_2.linkSkill(C3);
    // ex14_2.linkSkill(C6);
    // ex14_2.linkSkill(C7);
    // ex14_2.linkSkill(C9);
    // ex14_2.linkSkill(C10);
    ex14_2.linkSkill(C11); /*Observed*/
    // ex14_3.linkSkill(C1);
    // ex14_3.linkSkill(C2);
    // ex14_3.linkSkill(C3);
    // ex14_3.linkSkill(C4);
    // ex14_3.linkSkill(C6);
    // ex14_3.linkSkill(C7);
    // ex14_3.linkSkill(C8);
    // ex14_3.linkSkill(C9);
    // ex14_3.linkSkill(C10);
    ex14_3.linkSkill(C11); /*Observed*/

    /*Exercice Binaire\{C5,C12}*/
    // ex15_1.linkSkill(C1);
    // ex15_1.linkSkill(C2);
    // ex15_1.linkSkill(C3);
    // ex15_1.linkSkill(C4);
    // ex15_1.linkSkill(C6);
    // ex15_1.linkSkill(C7);
    // ex15_1.linkSkill(C8);
    // ex15_1.linkSkill(C9);
    // ex15_1.linkSkill(C10);
    ex15_1.linkSkill(C11); /*Observed*/
    // ex15_2.linkSkill(C1);
    // ex15_2.linkSkill(C2);
    // ex15_2.linkSkill(C3);
    // ex15_2.linkSkill(C4);
    // ex15_2.linkSkill(C6);
    // ex15_2.linkSkill(C7);
    // ex15_2.linkSkill(C8);
    // ex15_2.linkSkill(C9);
    // ex15_2.linkSkill(C10);
    ex15_2.linkSkill(C11); /*Observed*/
    // ex15_3.linkSkill(C1);
    // ex15_3.linkSkill(C2);
    // ex15_3.linkSkill(C3);
    // ex15_3.linkSkill(C4);
    // ex15_3.linkSkill(C6);
    // ex15_3.linkSkill(C7);
    // ex15_3.linkSkill(C8);
    // ex15_3.linkSkill(C9);
    // ex15_3.linkSkill(C10);
    ex15_3.linkSkill(C11); /*Observed*/

    /*Exercice Bille\{C4, C12}*/
    // ex16_1.linkSkill(C1);
    // ex16_1.linkSkill(C2);
    // ex16_1.linkSkill(C3);
    // ex16_1.linkSkill(C5);
    // ex16_1.linkSkill(C6);
    // ex16_1.linkSkill(C7);
    // ex16_1.linkSkill(C8);
    // ex16_1.linkSkill(C9);
    // ex16_1.linkSkill(C10);
    ex16_1.linkSkill(C11); /*Observed*/
    // ex16_2.linkSkill(C1);
    // ex16_2.linkSkill(C2);
    // ex16_2.linkSkill(C3);
    // ex16_2.linkSkill(C5);
    // ex16_2.linkSkill(C6);
    // ex16_2.linkSkill(C7);
    // ex16_2.linkSkill(C8);
    // ex16_2.linkSkill(C9);
    // ex16_2.linkSkill(C10);
    ex16_2.linkSkill(C11); /*Observed*/
    // ex16_3.linkSkill(C1);
    // ex16_3.linkSkill(C2);
    // ex16_3.linkSkill(C3);
    // ex16_3.linkSkill(C5);
    // ex16_3.linkSkill(C6);
    // ex16_3.linkSkill(C7);
    // ex16_3.linkSkill(C8);
    // ex16_3.linkSkill(C9);
    // ex16_3.linkSkill(C10);
    ex16_3.linkSkill(C11); /*Observed*/



    // create the student and liking its genericBKTmodel
    // 

    std::vector<Student> learners;
    Student Learner_461("461",{ex1_1, ex2_1});
    //Student Learner_4691("4691",{ex1_1, ex_1_2, ex1_3});
    //Student Learner_4686("4686",{});
    //Student Learner_4674("4674",{});
    //Student Learner_4673("4673",{});
    //Student Pierre("Pierre", {ex1, ex2, ex3});    
    Learner_461.setBKT(xcaleModel);

    //xcaleModel.getDBN()->summary();

    //create the evaluations for each exercise 1 for succeed, 0 for fail
    // We need to create a file with all the evaluation for each learners to measure their skills
    

    //std::vector<std::map<std::string, int>> evalVec;
    //evalVec.push_back(eval1[C1.name])

    std::map<std::string, int> eval1;   // L'apprenant a validé l'exo Melodie version 2 (c'est un indexe de l'échelle : 1 -> insuffisant)
                                        // 
    eval1[C1.name] = 0;
    std::map<std::string, int> eval2;   // L'apprenant a validé l'exo Alternance version 2
    eval2[C1.name] = 3;
    //std::map<std::string, int> eval3;
    //eval3[C1.name] = 0;
    //std::map<std::string, int> eval4;
    //eval4[C1.name] = 1;
    ExerciseEvaluation exEval1(eval1);
    ExerciseEvaluation exEval2(eval2);
    //ExerciseEvaluation exEval3(eval3);
    //ExerciseEvaluation exEval4(eval4);

    std::vector<ExerciseEvaluation> evals = {exEval1, exEval2}; // 2 exercices évalué : MelodieV2, AlternanceV2
    //std::vector<Exercise> recommendedExercises;

    // AnswersPrediction
    // None : no evidences on the results of the exercise.
    // Optimistic : the student always answers correctly on the prediction.
    //AnswersPrediction ans = AnswersPrediction::Optimistic;

    // Metric
    // Mean : maximize the average level of mastery of all skills.
    // Min  : maximize the lowest level of mastery of the skills.
    //Metric met = Metric::PG_min;

    //instanciate the model with any exercise, it won't really be used
    Learner_461.doExercise(ex1_1, exEval1);

    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Displaying probas after first exercice:" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    //print the initial probabilities for each skill
    Learner_461.getAllActualSkills();

    Learner_461.doExercise(ex2_1, exEval2);

    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Displaying probas after second exercice:" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    //print the initial probabilities for each skill
    Learner_461.getAllActualSkills();


    // Learner_461.doExercise(ex1_1, exEval3);

    // std::cout << "---------------------------------------" << std::endl;
    // std::cout << "Displaying probas after initialisation:" << std::endl;
    // std::cout << "---------------------------------------" << std::endl;
    // //print the initial probabilities for each skill
    // Learner_461.getAllActualSkills();

    // Learner_461.doExercise(ex1_1, exEval4);

    // std::cout << "---------------------------------------" << std::endl;
    // std::cout << "Displaying probas after initialisation:" << std::endl;
    // std::cout << "---------------------------------------" << std::endl;
    // //print the initial probabilities for each skill
    // Learner_461.getAllActualSkills();


    //xcaleModel.getDBN()->summary();

    //recommend the next exercise to do
    //Exercise recommendedFirst = Pierre.recommendExercise(ans, met);
    //recommendedExercises.push_back(recommendedFirst);
    //std::cout << "Recommended exercise is : " << recommendedFirst.getName() << std::endl;

    //Student does the recommended exercise
    //Pierre.doExercise(recommendedFirst, evals[recommendedFirst.getName().back() - '0' - 1]);

    //std::cout << "----------------------------------" << std::endl;
    //std::cout << "Displaying probas after first exo:" << std::endl;
    //std::cout << "----------------------------------" << std::endl;
    //Pierre.getAllActualSkills();

    //Exercise recommended = Pierre.recommendExercise(ans, met);
    //recommendedExercises.push_back(recommended);
    //std::cout << "Recommended exercise is : " << recommended.getName() << std::endl;


    //Pierre.doExercise(recommended, evals[recommended.getName().back() - '0' - 1]);
    //std::cout << "-----------------------------------" << std::endl;
    //std::cout << "Displaying probas after second exo:" << std::endl;
    //std::cout << "-----------------------------------" << std::endl;
    //Pierre.getAllActualSkills();

    //Exercise recommended2 = Pierre.recommendExercise(ans, met);
    //recommendedExercises.push_back(recommended2);
    //std::cout << "Recommended exercise is : " << recommended2.getName() << std::endl;

    //Pierre.doExercise(recommended2, evals[recommended2.getName().back() - '0' - 1]);

    //std::cout << "----------------------------------" << std::endl;
    //std::cout << "Displaying probas after third exo:" << std::endl;
    //std::cout << "----------------------------------" << std::endl;
    //Pierre.getAllActualSkills();

    /*
    plError::always_display_warning(false);

    BKTModel xcaleModel;
    MasteryLevel echelle;

    Skill C1("nombres", echelle);
    Skill C2("calcul-literal", echelle);
    Skill C3("demarche-scientifique", echelle);

    xcaleModel.addSkill(C1, 0.1, {0.05},0.1, 0.05, 0);
    xcaleModel.addSkill(C2, 0.1, {0.02},0.1,0.05, 0);
    xcaleModel.addSkill(C3, 0.1, {0.01},0.1, 0.05, 0);

    Exercise ex1("ex1");
    Exercise ex2("ex2");
    Exercise ex3("ex3");
    Exercise ex4("ex4");
    Exercise ex5("ex5");
    Exercise ex6("ex6");
    Exercise ex7("ex7");
    Exercise ex8("ex8");




    //Pierre->setBKT(xcaleModel);


    ex1.linkSkill(C1);
    ex2.linkSkill(C1);
    ex3.linkSkill(C1);
    ex4.linkSkill(C1);
    ex4.linkSkill(C3);
    ex5.linkSkill(C1);
    ex5.linkSkill(C2);
    ex6.linkSkill(C1);
    ex6.linkSkill(C2);
    ex7.linkSkill(C2);
    ex7.linkSkill(C3);
    ex8.linkSkill(C3);

    std::vector<Exercise> exs = {ex1, ex2, ex3, ex4, ex5, ex6, ex7, ex8};

    Student Pierre("Pierre", exs);
    Pierre.setBKT(xcaleModel);

    std::map<std::string, int> eval1;
    eval1[C1.name] = 0;

    std::map<std::string, int> eval2;
    eval2[C1.name] = 1;

    std::map<std::string, int> eval3;
    eval3[C1.name] = 0;

    std::map<std::string, int> eval4;
    eval4[C1.name] = 0;
    eval4[C3.name] = 0;

    std::map<std::string, int> eval5;
    eval5[C1.name] = 1;
    eval5[C2.name] = 0;

    std::map<std::string, int> eval6;
    eval6[C1.name] = 0;
    eval6[C2.name] = 0;

    std::map<std::string, int> eval7;
    eval7[C2.name] = 1;
    eval7[C3.name] = 1;

    std::map<std::string, int> eval8;
    eval8[C3.name] = 1;



    ExerciseEvaluation exEval1(eval1);
    ExerciseEvaluation exEval2(eval2);
    ExerciseEvaluation exEval3(eval3);
    ExerciseEvaluation exEval4(eval4);
    ExerciseEvaluation exEval5(eval5);
    ExerciseEvaluation exEval6(eval6);
    ExerciseEvaluation exEval7(eval7);
    ExerciseEvaluation exEval8(eval8);

    std::vector<ExerciseEvaluation> evals = {exEval1, exEval2, exEval3, exEval4, exEval5, exEval6, exEval7, exEval8};
    std::vector<Exercise> recommendedExercises;


    Pierre.doExercise(ex1, exEval1);
    Pierre.getAllActualSkills();

    AnswersPrediction ans = AnswersPrediction::Optimistic;
    Metric met = Metric::PG_min;

    Exercise recommendedFirst = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommendedFirst);
    std::cout << "ex recommended : " << recommendedFirst.getName() << std::endl;

    Pierre.doExercise(recommendedFirst, evals[recommendedFirst.getName().back()-'0'-1]);


    Exercise recommended = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommended);
    std::cout << "ex recommended : " << recommended.getName() << std::endl;

    Pierre.getAllActualSkills();
    Pierre.doExercise(recommended, evals[recommended.getName().back()-'0'-1]);



    Exercise recommended2 = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommended2);
    std::cout << "ex recommended : " << recommended2.getName() << std::endl;

    Pierre.getAllActualSkills();
    Pierre.doExercise(recommended2, evals[recommended2.getName().back()-'0'-1]);



    Exercise recommended3 = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommended3);
    std::cout << "ex recommended : " << recommended3.getName() << std::endl;

    Pierre.getAllActualSkills();
    Pierre.doExercise(recommended3, evals[recommended3.getName().back()-'0'-1]);



    Exercise recommended4 = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommended4);
    std::cout << "ex recommended : " << recommended4.getName() << std::endl;

    Pierre.getAllActualSkills();
    Pierre.doExercise(recommended4, evals[recommended4.getName().back()-'0'-1]);



    Exercise recommended5 = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommended5);
    std::cout << "ex recommended : " << recommended5.getName() << std::endl;

    Pierre.getAllActualSkills();
    Pierre.doExercise(recommended5, evals[recommended5.getName().back()-'0'-1]);



    Exercise recommended6 = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommended6);
    std::cout << "ex recommended : " << recommended6.getName() << std::endl;

    Pierre.getAllActualSkills();
    Pierre.doExercise(recommended6, evals[recommended6.getName().back()-'0'-1]);



    Exercise recommended7 = Pierre.recommendExercise( ans, met);
    recommendedExercises.push_back(recommended7);
    std::cout << "ex recommended : " << recommended7.getName() << std::endl;

    Pierre.getAllActualSkills();
    Pierre.doExercise(recommended7, evals[recommended7.getName().back()-'0'-1]);

    Pierre.getAllActualSkills();

    for(size_t i=0; i<8; i++){
        if(i==0){
            std::cout << "l'enchainement d'exercice recommandés est le suivant : ";
        }
        std::cout <<  recommendedExercises[i].getName() << "-" ;
    }

*/


}