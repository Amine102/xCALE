//
// Created by couland-q on 05/05/2022.
//

#ifndef KNOWLEDGE_TRACING_SCALABILITYTEST_H
#define KNOWLEDGE_TRACING_SCALABILITYTEST_H

#include <cstddef> // size_t
#include <random>

#include "../../core/include/genericBKTModel.h"
#include "progressiveBKT.h"
#include"../../core/include/tools.h"

namespace UsecaseScalability {
    void ScalabilityTest();

    void createScalableBKT(size_t number_of_skills, Scale scale, BKTModel& genericBKT);
    std::vector<UsecaseProgressiveBKT::Exercise> getExercises(size_t number_of_exercises, size_t number_of_skills, Scale scale);
}

#endif //KNOWLEDGE_TRACING_SCALABILITYTEST_H
