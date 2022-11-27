# This is a nice working alternative BUT it require the download at each cmake configure
# FetchContent_Declare(probt
#                      GIT_REPOSITORY https://gitlab.univ-nantes.fr/PILGRIM/probt.git
#                      GIT_TAG        v3.0.0)

# FetchContent_MakeAvailable(probt)

# I chose to use a git submodule because this library is nearly part of pilgrim. 
# This way you can see pilgrim as an extension of probt and not purely as a 
# client/vendor relationship. I beleive we care about having probt in our source
# tree.
# That being said and because we need the probt lib on the system (dynamic 
# linkage), I'd rather get cmake to fetch the library using find_package.

if(PILG_PROBT_SUBMODULE)
    add_subdirectory(dependencies/probt/libprobt)
else()
    find_package(probt 3.0.0 REQUIRED)
endif()
