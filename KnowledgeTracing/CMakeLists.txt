cmake_minimum_required (VERSION 2.8)

project (Knowledge-Tracing)

find_package (Boost 1.56 COMPONENTS system filesystem REQUIRED)

include_directories(/usr/include/probt)


find_library (PILGRIM_GENERAL pilg)
find_library (PROBT_LIBRARY probt)


IF (PILGRIM_GENERAL)
    message (STATUS "PILGRIM_GENERAL found at " ${PILGRIM_GENERAL})
ENDIF()

IF (PROBT_LIBRARY)
    message(STATUS "PROBT_LIBRARY found at " ${PROBT_LIBRARY})
ENDIF()


# Add the cpp files here
# deprecated folder is not compiled, but feel free to add it to the list
SET (APP_SRC usecases/main.cpp
        core/src/genericBKTModel.cpp
        core/src/student.cpp
        core/src/exercise.cpp
        core/src/tools.cpp
        core/src/debugtools.cpp

        core/include/resultStorage.h
        core/include/skill.h
        core/include/scale.h
        ../jsoncpp/dist/jsoncpp.cpp

        ../jsoncpp/dist/json/json.h
        ../jsoncpp/dist/json/json-forwards.h



        usecases/src/progressiveBKT.cpp
        usecases/src/studentTest.cpp
        usecases/src/scalabilityTest.cpp
        usecases/src/impacted.cpp
        usecases/src/serialize_deserialize.cpp
        usecases/src/xcaleAUTO.cpp
        usecases/src/xcale.cpp core/include/types.h usecases/include/prerequisite.h usecases/src/prerequisite.cpp)

#SET(CMAKE_CXX_FLAGS "-std=c++20 -g")

add_executable (knowledge_tracing ${APP_SRC})
target_compile_features(knowledge_tracing PRIVATE cxx_std_17)
#set_property(TARGET knowledge_tracing PROPERTY CXX_STANDARD 20)
link_libraries(stdc++fs)
target_link_libraries (knowledge_tracing LINK_PUBLIC ${PILGRIM_GENERAL} ${Boost_LIBRARIES} ${PROBT_LIBRARY} stdc++fs)
