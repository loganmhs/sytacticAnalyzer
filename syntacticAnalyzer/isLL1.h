#pragma once
#include<string>
using namespace std;

bool isLL1(string gram[], int &n, string(&production)[SIZE][2], NVTattrs &nvtattrs, string(&select)[SIZE][3]);