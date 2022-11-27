# use
# target_link_libraries(<<my_target>>
#                       PRIVATE threads)
# to link with threads

find_package(Threads REQUIRED)

add_library(threads
            INTERFACE)

target_link_libraries(threads
                      INTERFACE ${CMAKE_THREAD_LIBS_INIT})
