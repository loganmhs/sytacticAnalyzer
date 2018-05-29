#include"stdafx.h"
#include"declaration.h"
#include"eliminateLeftRecursion.h"
#include"arrayFunction.h"
#include<iostream>
using namespace std;
/*将产生式分成左右两部分*/
void splitLeft_Right(string gram[SIZE], int n, string(&prod)[SIZE][2]) {
	for (int i = 0; i < n; i++) {
		prod[i][0] = gram[i][0];
		for (unsigned int j = 3; j < gram[i].length(); j++) {
			prod[i][1] += gram[i][j];
		}
	}
}

/*消除产生式右部的竖线*/
void eliminateVerticalLine(string(&prod)[SIZE][2], int &n) {
	string p[SIZE][2];//新产生的没有“|”的文法
	int k = 0;//记录新生成的产生式的个数
	int begin = 0, end = 0;//记录相邻两个“|”的下标

	for (int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < prod[i][1].length(); j++) {
			if (prod[i][1][j] == '|') {
				end = j;
				p[k][0] = prod[i][0];
				p[k++][1] = prod[i][1].substr(begin, end - begin);
				begin = end + 1;
			}
		}
		end =(int) prod[i][1].length();
		p[k][0] = prod[i][0];
		p[k++][1] = prod[i][1].substr(begin, end - begin);
		begin = 0;
	}

	n = k;
	for (int i = 0; i < k; i++) {
		prod[i][0] = p[i][0];
		prod[i][1] = p[i][1];
	}
}

/*消除一切左递归*/
void eliminateLeftRecursion(string(&prod)[SIZE][2], int &n) {
	string lastNVT = prod[n - 1][0];//最后一个非终结符，不用替换
	int place = 0;//记录当前将被替换的字符在串中的位置
	string leftNVT;//产生式右部最左边的非终结符
	string leftNVT1;
	int rightPartNum = 0;//当前将要代入的产生式的右部的个数
	int rightPartIndex[SIZE];//存放含有左递归的产生式，不含左递归非终结符的那部分产生式的下标
	int recursionNum = 0;//含左递归产生式的编号

	for (int i = 0; i < n; i++) {
		rightPartNum = 0;
		for (int j = i; j < n; j++) {
			if (prod[j][0] == prod[i][0]) {
				rightPartNum++;
			}
			else {
				if (prod[i][0] != lastNVT) {
					leftNVT = prod[j][1][0];
					if (leftNVT == prod[i][0]) {
						string item[2];
						item[0] = prod[j][0];
						for (int k = 0; k < rightPartNum; k++) {
							item[1] = prod[j][1];
							if (prod[i + k][1].at(0) != '^')
								item[1].replace(0, 1, prod[i + k][1]);
							else
								item[1].replace(0, 1, "");
							insertBehind(prod, n, j + k, item);
						}
						deleteItem(prod, n, j);

					}
				}
				else {
					break;
				}
			}
		}
	}

	for (int i = 0; i < n; i++) {
		leftNVT = prod[i][1][0];
		if (leftNVT == prod[i][0]) {
			rightPartNum = 0;
			for (int j = 0; j < n; j++) {
				leftNVT1 = prod[j][1][0];
				if (prod[j][0] == leftNVT&&leftNVT1 != leftNVT) {
					rightPartIndex[rightPartNum++] = j;
				}
			}//得到了下标数组

			for (int k = 0; k < rightPartNum; k++) {
				prod[n][0] = prod[i][0];
				prod[n][1] = prod[rightPartIndex[k]][1];
				prod[n][1] += ('Z' - recursionNum);//用Z作扩展的非终结符
				n++;
			}
			prod[n][0] = ('Z' - recursionNum);
			prod[n][1] = prod[i][1].substr(1, prod[i][1].length() - 1);
			prod[n][1] += ('Z' - recursionNum);
			n++;

			prod[n][0] = ('Z' - recursionNum);
			prod[n][1] = "^";//“^”代表埃普西隆
			n++;

			int r = 0;
			for (r = 0; r < n; r++) {
				if (prod[r][0] == prod[i][0]) {
					break;
				}
			}
			for (int m = 0; m <rightPartNum + 1; m++) {//删除中间的数组元素
				deleteItem(prod, n, r);
			}
			recursionNum++;
			i--;
		}
	}

}
