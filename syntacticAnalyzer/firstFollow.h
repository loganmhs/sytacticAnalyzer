#pragma once
#include"declaration.h"
#include<string>
using namespace std;//即使在头文件中，也需要这一句话

/*输入：一个非终结符表、整个文法、文法行数；输出：填写非终结符表中的所有子项的getNull信息*/
void getEpsilon(NVTattrs &nvtattrs, string prod[SIZE][2], int n);

void initNVTtable(NVTattrs &nvtattrs, string prod[][2], int n,char start);

void getFisrtAssemble(NVTattrs &nvtattrs, string prod[][2], int n);

void getFollowAssemble(NVTattrs &nvtattrs, string prod[][2], int n);

void getSelectAssemble(NVTattrs nvtattrs, string prod[][2], int n, string(&select)[SIZE][3]);