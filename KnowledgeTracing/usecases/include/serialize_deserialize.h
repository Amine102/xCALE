//
// Created by couland-q on 17/10/22.
//

#ifndef KNOWLEDGE_TRACING_SERIALIZE_DESERIALIZE_H
#define KNOWLEDGE_TRACING_SERIALIZE_DESERIALIZE_H

#include <string>
#include "../../core/include/student.h"

namespace UsecaseSerializeDeserialize {
    void usecase();

    void serialize(Student& student, const std::string& filename);
    void deserialize(Student& student, const std::string& filename);

    Student createStudent(Scale& mastery_scale, Scale& difficulty_scale, Scale& speed_scale);
    std::vector<Exercise> getExos(Scale difficulty_scale, std::vector<Skill> skills);
    ExercisesEvaluations getEvals(Scale mastery_scale, std::vector<Skill> skills, std::vector<Exercise> exos);
}

#endif //KNOWLEDGE_TRACING_SERIALIZE_DESERIALIZE_H
