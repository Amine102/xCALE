//
// Created by couland-q on 24/10/22.
// Updated by Amine-n   on 25/11/22:
    // + Adding new include : debugtools.h
    // + Adding new useCase method : xcaleAUTO

#ifndef KNOWLEDGE_TRACING_XCALE_H
#define KNOWLEDGE_TRACING_XCALE_H

#include <pl.h>
#include "../../core/include/debugtools.h"
#include "../../core/include/genericBKTModel.h"
#include "../../core/include/scale.h"
#include "../../core/include/skill.h"
#include "../../core/include/student.h"

//-----------------------------------------------------------------------------
// SECTION : Typedefs 
//-----------------------------------------------------------------------------
typedef int learnerID;
typedef int exoDifficulty;
typedef std::string exoLabel;


namespace UsecaseXCale {
    void xCale_BKT_debug();
    void xcaleAUTO();
}

#endif //KNOWLEDGE_TRACING_XCALE_H
