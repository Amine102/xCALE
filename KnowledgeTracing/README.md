# Knowledge Tracing
Bayesian Knowledge Tracing made easier.

## Building the project

###  Using CMake

* Pre-requisite:
  * Pilgrim (use the `develop` branch)
  * ProBT


* Adapt the `CMakeLists.txt` to your needs.

* Build the project as follows (starting from the root of the project):

``` 
mkdir build && cd build
cmake ..
cmake --build .
```

Optionally, you can use `make -j 8` instead of `cmake --build .` to speed up the project (`8` being the number of cores your computer have).

