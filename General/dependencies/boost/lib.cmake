# use
# target_link_libraries(<<my_target>>
#                       PRIVATE specific_boost_dependencies)
# to link with the boost modules needs by pilgrim general

# For now it'll be a system dependency, later it may be better to have it as a gitsubmodule, 
# maybe ?

set(Boost_USE_STATIC_LIBS ON CACHE BOOL "")
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS system filesystem REQUIRED)

add_library(specific_boost_dependencies
            INTERFACE)

target_link_libraries(specific_boost_dependencies
                      INTERFACE ${Boost_LIBRARIES})
