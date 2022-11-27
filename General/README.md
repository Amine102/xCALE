# PILGRIM General

[![pipeline status](https://gitlab.univ-nantes.fr/PILGRIM/General/badges/master/pipeline.svg)](https://gitlab.univ-nantes.fr/PILGRIM/General/commits/)
[![coverage report](https://gitlab.univ-nantes.fr/PILGRIM/General/badges/master/coverage.svg)](https://gitlab.univ-nantes.fr/PILGRIM/General/commits/)


## Satisfying The Dependencies

Obtain the system dependencies:
```
$ sudo apt install build-essential cmake git libboost-system-dev libboost-filesystem-dev
```

Start by clonning the repo:
```
# Over ssh (strongly recommended)
$ git clone git@gitlab.univ-nantes.fr:PILGRIM/General.git
# Over https (strongly discouraged)
$ git clone https://gitlab.univ-nantes.fr/PILGRIM/General.git
```

Pull the requiered depdencies:
```
$ git submodule update --init --recursive
```

Install libprobt, on Ubuntu or Debian it can be done like this:
```
$ cd dependencies/probt/libprobt
$ mkdir -p build && cd build
$ cmake .. && make package
$ sudo apt install ./libprobt-dev*.deb
```


## Installing Pilgrim General

After satisfying the dependencies, run:
```
# Assuming you are at the root of the Pilgrim General repo
$ mkdir -p build && cd build
$ cmake .. && make -j 8  && make package
$ sudo apt install ./libpilg-dev*.deb
```

## Usage Example

See the [sample project](https://gitlab.univ-nantes.fr/PILGRIM/Applications/PM-General-App-Example) or the multiple
usecase specified in `usecases/`.

When Pilgrim General is built, it'll produce an executable named
`pilg_usecases`, this execrable represents the compilation of all the use cases
and you may run it. It's located in your `build` repository (where you ran 
`cmake ..`).

## Contributing Changes

Please read and understand the whole [CONTRIBUTING.md](CONTRIBUTING.md) 
document.


## Documentation

Install doxygen:
```
$ sudo apt install doxygen graphviz
```
Build the doc:
```
$ mkdir -p build && cd build
$ cmake .. && make doc
```
The doc should new be built and is available in build/doc.


A version of the documentation is available at 
[https://pilgrim.univ-nantes.io/General/](https://pilgrim.univ-nantes.io/General/)
and examples are provided in the `usecases` directory.


## Releasing Pilgrim General On Gitlab

Please read the [release.md](devops/ci/gitlab/howto/release.md) document.
