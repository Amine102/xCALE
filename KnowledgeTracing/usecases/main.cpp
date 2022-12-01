/*! \file main.cpp
    \brief Main entry point for the application
*/

#include <vector>
#include <string>

//#include "include/useCaseImpactedGeneric.h"
#include "include/progressiveBKT.h"
#include "include/scalabilityTest.h"
#include "include/studentTest.h"
#include "include/impacted.h"
#include "include/serialize_deserialize.h"
#include "include/xcale.h"
#include "include/prerequisite.h"



std::vector<std::string> getAllUsecases() {
    std::vector<std::string> all_usecases;
    all_usecases.emplace_back("1 - Generic BKT model creation (removed)");
    all_usecases.emplace_back("2 - Progressive BKT query");
    all_usecases.emplace_back("3 - BKT scalability test");
    all_usecases.emplace_back("4 - Impacted usecase re-done");
    all_usecases.emplace_back("5 - Etudiant test");
    all_usecases.emplace_back("6 - Serialize / deserialize test");
    all_usecases.emplace_back("7 - XCale usecase");
    all_usecases.emplace_back("8 - Prerequisite test");
    all_usecases.emplace_back("9 - xcaleAUTO");
    return all_usecases;
}

void usecaseSelection() {
    std::vector<std::string> all_usecases = getAllUsecases();
    for (std::string& usecase: all_usecases) {
        std::cout << usecase << std::endl;
    }


    unsigned int choice = 0;
    std::cin >> choice;

    switch (choice) {
        case 1:
            //impactedUseCaseWithClasses();
            break;

        case 2:
            UsecaseProgressiveBKT::createAndQueryProgressiveBKT();
            break;

        case 3:
            UsecaseScalability::ScalabilityTest(); // TODO: Currently segfaulting
            break;

        case 4:
            UsecaseImpacted::usecase();
            break;

        case 5:
            UsecaseStudent::studentTest();
            break;

        case 6:
            UsecaseSerializeDeserialize::usecase();
            break;

        case 7:
            UsecaseXCale::xcaleMANUAL();
            break;

        case 8:
            UsecasePrerequisite::prerequisite();

        case 9:
            UsecaseXCale::xcaleAUTO();


        default:
            break;
    }
}


int main() {
    usecaseSelection();
    return 0;
}