/*###################################-DEBUG_TOOLS-###########################################
  ##	BKTParam Lib C++20, v1.0
  ##   	Copyright (C) 2021-2024 Amine Boulahmel: <amine.boulahmel@imt-atlantique.fr>
  ##    date creation : 01/12/2022
  ##
  ##   	Licensed under the Apache License, Version 2.0 (the "License");
  ##  	you may not use this file except in compliance with the License.
  ##   	You may obtain a copy of the License at
  ##
  ##       http://www.apache.org/licenses/LICENSE-2.0
  ##
  ##   Unless required by applicable law or agreed to in writing, software
  ##   distributed under the License is distributed on an "AS IS" BASIS,
  ##   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  ##   See the License for the specific language governing permissions and
  ##   limitations under the License.
  ##
  ###########################################################################################*/

#pragma once
#ifndef BKT_PARAM_H
#define BKT_PARAM_H
#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>
#include <string>

class BKTParam
{
  private:
    float pguess_;
    std::vector<double> pinit_;
    std::vector<double> plearn_;
    float pslip_;
    float pforget_;
    std::pair<bool,float> checksum_(std::vector<double> vec);
    bool checkorder_(std::vector<double> vec, std::string order);
  public:
    BKTParam(){};
    void setPinit(std::vector<double> pinit);
    void setPlearn(std::vector<double> plearn);
    void setPguess(float pguess);
    void setPslip(float pslip);
    void setPforget(float pforget);
    std::vector<double> getPinit();
    std::vector<double> getPlearn();
    float getPguess();
    float getPslip();
    float getPforget();
};

#endif // BKT_PARAM_H
