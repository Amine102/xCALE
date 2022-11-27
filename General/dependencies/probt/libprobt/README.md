# Probt repository


This repository contains the most recent version of probt that we own and can afford to use in Pilgrim without risking breaking everything.


## Note


- Probt is given as a shared lib, that means that if you use it you also need to have it in your path for the dynamic linker to find it !
This "problem" would not exist if it was a static lib. In the Usage section below, if you use the first 2 methods, you'll be able to build/link
but not to launch any executable build and linked with probt, because you won't have the probt lib in your path. The 3rd method fixes that but dirty your computer with development dependencies, it requires you to install the package and then you may use probt.
- We actually use 2 different versions of the library. The 3.0.0 and 3.1.0 versions.
- We can support both windows and linux under the amd64 architecture.


## Building a package:


Build the package (linux and windows):
```
$ mkdir -p build
$ cd build
$ cmake ..
$ cmake --build . --target package # On windows you may have to open msvc or use another generator, see "cmake -G" for a list of supported, but not necessarily installed  generators (I recommend Ninja)
```
At this point should have a libprobt-dev.XXXXX.deb (or *.tar.gz on windows) in the `build` directory. You
should install it.


On debian/ubuntu:
```
# the ./ is mandatory !
$ apt install ./libprobt-dev.XXXXX.deb
```
You can remove the development lib with:
```
$ apt remove libprobt-dev 
```


On windows:


You may want to extract the archive created (libprobt-dev-XXXX.tar.gz) in the `build` directory to some directory you use to store ll your development libs.


## Usage


You can trivially include the probt library this way (simplest and viable, not the best):
```
# use
# target_link_libraries(<<my_target>>
#                       PRIVATE probt)
# to link with probt


FetchContent_Declare(probt
                     GIT_REPOSITORY https://gitlab.univ-nantes.fr/PILGRIM/probt.git
                     GIT_TAG        v3.X.0)


FetchContent_MakeAvailable(probt)
```


Or as done in Pilgrim/general`, as a [git submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules):
```
# use
# target_link_libraries(<<my_target>>
#                       PRIVATE probt)
# to link with probt


add_subdirectory(probt_repo) # probt_repo is a git submodule
```


Or after installing as a package (this is the recommended way) :
```
# use
# target_link_libraries(<<my_target>>
#                       PRIVATE probt::probt) # NOTE the probt::probt is not the same as in the other methods !!
# to link with probt


find_package(probt REQUIRED)
```


## Documentation


All the documentation you need is stored in `vendor/*.7z`. It's not a part of the repository because of the size bloat of the documentation files and also because we actually need to support 2 versions of probt.


For quick access to the includes (aka interface of probt), you may want to include probt as a git submodule.


## TODO


- A release package without the headers. This would be nice for if an application where to by distributed to a client that does not need the development headers. That's why you find most libs on the debian/ubuntu repositories as both development and release. We would have `libprobt` and `libprobt-dev`.
