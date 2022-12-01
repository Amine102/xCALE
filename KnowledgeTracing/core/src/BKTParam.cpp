#include "../../core/include/BKTParam.h"

std::pair<bool,float> BKTParam::checksum_(std::vector<double> vec)
{
    float checksum = std::accumulate(vec.begin(), vec.end(), 0.0f);
    if(checksum != 1)
        return std::make_pair(false, checksum);
    else
        return std::make_pair(true, checksum);
}

bool BKTParam::checkorder_(std::vector<double> vec, std::string order="asc")
{
    if(order=="desc")
        std::reverse(vec.begin(), vec.end());
    if(std::is_sorted(vec.begin(), vec.end()))
         return true;
    else
        return false;
}

void BKTParam::setPinit(std::vector<double> pinit) 
{
    if(!checksum_(pinit).first)
        std::cerr << "[BKTParam.h] ERROR(Pinit, sum): probability values do not sum to 1 (checksum = "  << checksum_(pinit).second << "). Check your pinit values." << std::endl;
    if(!checkorder_(pinit, "desc"))
        std::cerr << "[BKTParam.h] ERROR(Pinit, order) : probability values should be ordered in descending order. Check your pinit value order." << std::endl;
    this->pinit_ = pinit;
}

void BKTParam::setPlearn(std::vector<double> plearn)
{
    if(!checksum_(plearn).first)
        std::cerr << "[BKTParam.h] ERROR[Plearn, sum]: probability values do not sum to 1 (checksum = " << checksum_(plearn).second << ") . Check your plearn values." << std::endl;
    if(!checkorder_(plearn, "asc"))
        std::cerr << "[BKTParam.h] ERROR(Plearn, order): probability values should be ordered in ascending order. Check your plearn value order." << std::endl;
    this->plearn_ = plearn;
}

void BKTParam::setPguess(float pguess)   { this->pguess_  = pguess ; }
void BKTParam::setPslip(float pslip)     { this->pslip_   = pslip  ; }
void BKTParam::setPforget(float pforget) { this->pforget_ = pforget; }

std::vector<double> BKTParam::getPinit()  { return this->pinit_  ; }
std::vector<double> BKTParam::getPlearn() { return this->plearn_ ; }
float BKTParam::getPguess()               { return this->pguess_ ; }
float BKTParam::getPslip()                { return this->pslip_  ; }
float BKTParam::getPforget()              { return this->pforget_; }