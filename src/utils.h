#ifndef _UTILS_H_
#define _UTILS_H_
#include"common.h"

void ParseJsonInput(const char* str, EnTaskCls& enTaskCls);
void Json2TsFusInput(const char* str, StTsFusInput& TsFusInput);
void TsFusResult2Js(StTsFusResultOutput& result, std::string& str);
void Json2TsAnaInput(const char* str, StTsAnaInput& TsAnaInput);
void TsAnaResult2Js(StTsAnaResultOutput& result, std::string& str);

#endif