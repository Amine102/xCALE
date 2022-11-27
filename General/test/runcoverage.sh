#!/bin/bash

mkdir -p coverage
# We could also use gcovR but it dosnt work well on alpine (natively)
lcov --directory . --base-directory . --capture --output-file ./coverage/coverage.info
genhtml ./coverage/coverage.info -o ./coverage
