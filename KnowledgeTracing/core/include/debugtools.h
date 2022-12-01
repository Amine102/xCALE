/*###################################-DEBUG_TOOLS-###########################################
  ##	Debugtools Lib C++20, v1.0
  ##   	Copyright (C) 2021-2024 Amine Boulahmel: <amine.boulahmel@imt-atlantique.fr>
  ##    date creation : 25/11/2022
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
#ifndef DEBUG_TOOLS_H
#define DEBUG_TOOLS_H
#include <iostream>
#include <unistd.h>
#include <vector>

//-----------------------------------------------------------------------------
// SECTION : Global Definition of DEBUG FUNCTIONS
//-----------------------------------------------------------------------------
#define GET_VARIABLE_NAME__(name) print_VAR_NAME__(#name)                       // Debug MACRO function printing the actual name of the variable

namespace debugtool_
{
    namespace UsecaseXCale
    {
        //-----------------------------------------------------------------------------
        // SUBSECTION : Definition of DEBUG constant
        //-----------------------------------------------------------------------------
        // The debug constant are used for debug purposes only. The macros are bolean values.
        // The truth values of these debug constants are checked using the if else statement (to avoid making real MACROS and run dirty checks with #if #endif).
        static constexpr bool PL_ALWAYS_DISPLAY_WARNING           = false;    // Parameter for debugging PL library erros
        static constexpr bool SCALE_CREATION_DEBUG                = false;    // Parameter for debugging scale creation from scaleConfig.json
        static constexpr bool SCALE_CREATION_DIFFICULTY_DEBUG     = false;    // Parameter for debugging difficulty creation from scaleConfig.json
        static constexpr bool SCALE_JSON_DATA_DEBUG               = false;    // Parameter for debugging JSON scale data load (scaleConfig.json)
        static constexpr bool SKILL_CREATION_DEBUG                = false;    // Parameter for debugging skill creation from skillConfig.json file
        static constexpr bool EXERCISE_JSON_DATA_DEBUG            = false;    // Parameter for debugging JSON exercise load  from exoConfig.json file
        static constexpr bool EXERCISE_CREATION_DEBUG             = false;    // Parameter for debugging exercise creation from exoConfig.json file
        static constexpr bool BKT_PARAM_PINIT_CREATION_DEBUG      = false;    // Parameter for debugging Pinit values load from bktConfig.json file
        static constexpr bool BKT_PARAM_PLEARN_CREATION_DEBUG     = false;    // Parameter for debugging PLearn values load from bktConfig.json file
        static constexpr bool BKT_CREATION_DISPLAY_SUMMARY_DEBUG  = false;    // Parameter to print a summary of the BKT model after it's creation
        static constexpr bool LEARNER_JSON_DATA_DEBUG             = false;    // Parameter for debugging JSON learner load  from learnerData.json file

        //-----------------------------------------------------------------------------
        // SUBSECTION : Definition of DEBUG flags
        //-----------------------------------------------------------------------------
        // The debug macros are used for debug purposes only. The macros are bolean values.
        // The truth values of these macros are checked using the #if #endif directives.
        // TODO : change the comments by what these parameters actually do
        enum class eDebug_flag_
        {
            SCALE_CREATION_FLAG                                   = 0,        // The truth values of these macros are checked using the #if #endif directives.
            SCALE_CREATION_DIFFICULTY_FLAG                        = 1 << 0,   // The truth values of these macros are checked using the #if #endif directives.
            SCALE_JSON_DATA_FLAG                                  = 2 << 1,   // The truth values of these macros are checked using the #if #endif directives.
            SKILL_CREATION_FLAG                                   = 3 << 2,   // The truth values of these macros are checked using the #if #endif directives.
            EXERCISE_JSON_DATA_FLAG                               = 4 << 3,   // The truth values of these macros are checked using the #if #endif directives.
            EXERCISE_CREATION_FLAG                                = 5 << 4,   // The truth values of these macros are checked using the #if #endif directives.
            BKT_PARAM_PINIT_CREATION_FLAG                         = 6 << 5,   // The truth values of these macros are checked using the #if #endif directives.
            BKT_PARAM_PLEARN_CREATION_FLAG                        = 7 << 6    // The truth values of these macros are checked using the #if #endif directives.
        };
        //-----------------------------------------------------------------------------
        // SUBSECTION : Forward Declaration
        //-----------------------------------------------------------------------------
        // The debug macros are used for debug purposes only. The macros are bolean values.
        // The truth values of these macros are checked using the #if #endif directives.
        constexpr void xcale_debug_(eDebug_flag_ flag);
    }

}

namespace debugtool_
{
    void print_VAR_NAME__(const char *name);                                    // Debug function printing the actual name of the variable
    template<class T>   
    void print_VECTOR__(std::vector<T> vec, bool endline=true);                 // Debug function printing the actual name of the variable
    void dprint_STR__(std::string expr, unsigned int delay=121211);             // Debug function printing string in a delayed time

}
#endif // DEBUG_TOOLS_H