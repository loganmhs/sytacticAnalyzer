#include"stdafx.h"
#include<iostream>
#include<string>
#include"eliminateLeftRecursion.h"
#include"firstFollow.h"
#include"isLL1.h"

using namespace std;

/*输入：两个string字符串；输出：返回值，有相同元素返回false，否则返回true*/
bool intersect(string str1, string str2) {
	for (unsigned int i = 0; i < str1.length(); i++) {
		for (unsigned int j = 0; j < str2.length(); j++) {
			if (str1.at(i) == str2.at(j)) {
				return false;
			}
		}
	}
	return true;
}

/*输入：select集合,文法行数；输出：是否为LL1文法的真值*/
bool LL1(string select[][3],int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i != j) {//不和自己比
				if (select[j][0] == select[i][0]) {//找到和自己相同左部的产生式了
					//需要一个比较两个集合是否有相同元素的函数，是返回false，不是返回true
					if (intersect(select[j][2], select[i][2]) == false)
						return false;
				}
			}
		}
	}
	return true;
}

/*输入：原始文法，行数,新产生的文法，非终结符表，select集合；输出：是否为LL1文法的真值*/
bool isLL1(string gram[], int &n,string (&production)[SIZE][2],NVTattrs &nvtattrs,string (&select)[SIZE][3] ) {
	splitLeft_Right(gram, n, production);

	eliminateVerticalLine(production, n);

	eliminateLeftRecursion(production, n);
	

	cout << endl;
	for (int i = 0; i < n; i++) {
		cout << production[i][0] << "->" << production[i][1] << endl;
	}

	initNVTtable(nvtattrs, production, n, gram[0].at(0));

	getEpsilon(nvtattrs, production, n);

	getFisrtAssemble(nvtattrs, production, n);

	getFollowAssemble(nvtattrs, production, n);

	getSelectAssemble(nvtattrs, production, n, select);

	return LL1(select, n);
}