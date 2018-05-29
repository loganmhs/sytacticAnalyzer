#pragma once
#include"declaration.h"
#include<string>
#include<iostream>
using namespace std;

void insertBehind(string(&prod)[SIZE][2], int &n, int loc, string item[2]);

void deleteItem(string(&prod)[SIZE][2], int &n, int loc);