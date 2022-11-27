#include "../../core/include/debugtools.h"
using namespace debugtool_;
constexpr void UsecaseXCale::xcale_debug_(eDebug_flag_ debugflag)
{
    switch (debugflag)
    {
    case UsecaseXCale::eDebug_flag_::SCALE_CREATION_FLAG:
        break;
    case UsecaseXCale::eDebug_flag_::SCALE_CREATION_DIFFICULTY_FLAG:
        break;
    case UsecaseXCale::eDebug_flag_::SCALE_JSON_DATA_FLAG:
        break;
    case UsecaseXCale::eDebug_flag_::SKILL_CREATION_FLAG:
        break;
    case UsecaseXCale::eDebug_flag_::EXERCISE_JSON_DATA_FLAG:
        break;
    case UsecaseXCale::eDebug_flag_::EXERCISE_CREATION_FLAG:
        break;
    case UsecaseXCale::eDebug_flag_::BKT_PARAM_PINIT_CREATION_FLAG:
        break;
    case UsecaseXCale::eDebug_flag_::BKT_PARAM_PLEARN_CREATION_FLAG:
        break;
    }
}

void print_VAR_NAME__(char *name) {std::cout << name << " ";}

template <class T>
void print_VECTOR__(std::vector<T> vec, bool endline)
{
    for (typename std::vector<T>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (endline)
            std::cout << *it << std::endl;
        else
            std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void dprint_STR__(std::string expr, unsigned int delay)
{
    std::vector<char> cstr(expr.c_str(), expr.c_str() + expr.size());
    for (std::vector<char>::iterator it = cstr.begin(); it != cstr.end(); ++it)
    {
        std::cout << *it;
        fflush(stdout);
        usleep(delay);
    }
}
