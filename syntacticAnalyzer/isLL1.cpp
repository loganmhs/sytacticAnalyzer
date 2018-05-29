#include"stdafx.h"
#include<iostream>
#include<string>
#include"eliminateLeftRecursion.h"
#include"firstFollow.h"
#include"isLL1.h"

using namespace std;

/*���룺����string�ַ��������������ֵ������ͬԪ�ط���false�����򷵻�true*/
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

/*���룺select����,�ķ�������������Ƿ�ΪLL1�ķ�����ֵ*/
bool LL1(string select[][3],int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i != j) {//�����Լ���
				if (select[j][0] == select[i][0]) {//�ҵ����Լ���ͬ�󲿵Ĳ���ʽ��
					//��Ҫһ���Ƚ����������Ƿ�����ͬԪ�صĺ������Ƿ���false�����Ƿ���true
					if (intersect(select[j][2], select[i][2]) == false)
						return false;
				}
			}
		}
	}
	return true;
}

/*���룺ԭʼ�ķ�������,�²������ķ������ս����select���ϣ�������Ƿ�ΪLL1�ķ�����ֵ*/
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