# Automatically generated by scripts/boost/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/functional
    REF boost-1.80.0
    SHA512 c7af977120c64b44d28b26fcc0f897731b1baa770d87659eddffcab32646e0b754a022ea5beddd930b7043f15beb083befb23303ab5363f8abd67c0ad822b592
    HEAD_REF master
)

include(${CURRENT_INSTALLED_DIR}/share/boost-vcpkg-helpers/boost-modular-headers.cmake)
boost_modular_headers(SOURCE_PATH ${SOURCE_PATH})
