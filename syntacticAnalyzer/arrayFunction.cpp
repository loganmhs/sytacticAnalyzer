#include"stdafx.h"
#include<string>
#include"declaration.h"
#include"arrayFunction.h"
#include<iostream>

using namespace std;
/*自己写一个静态数组的插入函数*/
void insertBehind(string(&prod)[SIZE][2], int &n, int loc, string item[2]) {
	for (int i = n; i >loc; i--) {
		prod[i][0] = prod[i - 1][0];
		prod[i][1] = prod[i - 1][1];
	}
	prod[loc + 1][0] = item[0];
	prod[loc + 1][1] = item[1];
	n++;
}

//删除一个数组元素
void deleteItem(string(&prod)[SIZE][2], int &n, int loc) {
	for (int i = loc; i <n; i++) {
		prod[i][0] = prod[i + 1][0];
		prod[i][1] = prod[i + 1][1];
	}
	n--;
}