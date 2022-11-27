

include(CMakeFindDependencyMacro)

# Prepare the deps needed by pilgTargets

find_dependency(probt REQUIRED)

set(Boost_USE_STATIC_LIBS ON CACHE BOOL "")
set(Boost_USE_MULTITHREADED ON)
find_dependency(Boost COMPONENTS system filesystem)

# Add the targets file
include("${CMAKE_CURRENT_LIST_DIR}/pilgTargets.cmake")

# check_required_components(Pilgrim-general) # TODO(Etienne M):  should provide that macro, it does not, fix it
