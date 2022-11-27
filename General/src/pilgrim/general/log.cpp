
#include "pilgrim/general/log.h"

using namespace PILGRIM;

char Log::outLogFileName[256];
char Log::outCPCFileName[256];
char Log::outTestSummaryFileName[256];
char Log::outTimeComplxFileName[256];

Log::Log(char _logType) {
  logType = _logType;

  switch (logType) {
    case 'C':
      this->fileName = this->outCPCFileName;
      break;
    case 'c':
      this->fileName = this->outCPCFileName;
      break;
    case 'L':
      this->fileName = this->outLogFileName;
      break;
    case 'l':
      this->fileName = this->outLogFileName;
      break;
    case 'S':
      this->fileName = this->outTestSummaryFileName;
      break;
    case 's':
      this->fileName = this->outTestSummaryFileName;
      break;
    case 'T':
      this->fileName = this->outTimeComplxFileName;
      break;
    case 't':
      this->fileName = this->outTimeComplxFileName;
      break;
    default:
      std::cout << "ERROR: log type is not a valid value." << std::endl;
      break;
  }  // end switch
}

Log::~Log() {}

void Log::Write(char *logline) {
  // open a file
  this->fp = fopen(this->fileName, "a");
  fputs(logline, this->fp);
  fclose(this->fp);
}

void Log::setOutFilePrefix(char *outFileName,
                           size_t &winSize,
                           size_t k,
                           bool incrementalLearning) {
  
  int casted_winSize = static_cast<int>(winSize);
  int casted_k = static_cast<int>(k);

  switch (incrementalLearning) {
    case true: {  // later try sprintf_s() function buffer size [56]
      sprintf(outCPCFileName, "%s_CPC_W%d_k%d.txt", outFileName, casted_winSize, casted_k);
      sprintf(outTestSummaryFileName, "%s_TestSummary_W%d_k%d.txt", outFileName, casted_winSize, casted_k);
      sprintf(outLogFileName, "%s_TestLog_W%d_k%d.txt", outFileName, casted_winSize, casted_k);
      sprintf(outTimeComplxFileName, "%s_TimeComplexity_W%d_k%d.txt", outFileName, casted_winSize, casted_k);
      break;
    }
    case false: {
      sprintf(outCPCFileName, "%s_CPC_W%d.txt", outFileName, casted_winSize);
      sprintf(outTestSummaryFileName, "%s_TestSummary_W%d.txt", outFileName, casted_winSize);
      sprintf(outLogFileName, "%s_TestLog_W%d.txt", outFileName, casted_winSize);
      sprintf(outTimeComplxFileName, "%s_TimeComplexity_W%d.txt", outFileName, casted_winSize);
      break;
    }

    default: {
      break;
    }
  }  // end switch
}
