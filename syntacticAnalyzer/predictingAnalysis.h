#pragma once
#include<string>
#include"declaration.h"
using namespace std;

Table* preAnalyTbl(NVTattrs nvtattrs, string prod[][2], int n, string select[][3]);

void analysis(string str, Table table, char start);