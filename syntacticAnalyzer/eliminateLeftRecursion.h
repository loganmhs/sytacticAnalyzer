#pragma once
#include"declaration.h"
#include<string>
#include<iostream>
using namespace std;

void splitLeft_Right(string gram[SIZE], int n, string(&prod)[SIZE][2]);

void eliminateVerticalLine(string(&prod)[SIZE][2], int &n);

void eliminateLeftRecursion(string(&prod)[SIZE][2], int &n);