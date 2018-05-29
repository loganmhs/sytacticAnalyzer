// syntacticAnalyzer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<iostream>
#include<string>
#include"declaration.h"
#include"isLL1.h"
#include"predictingAnalysis.h"

using namespace std;

int main()
{
	int n;
	string gram[SIZE];
	NVTattrs nvtattrs;
	string prod[SIZE][2];
	string select[SIZE][3];

	cout << "input the lines of grammer:" << endl;
	cin >> n;
	cout << "input the grammer:" << endl;
	for (int i = 0; i < n; i++) {
		cin >> gram[i];
	}

	if (isLL1(gram, n,prod,nvtattrs,select))
	{
		cout << endl << "it's LL1!" << endl;
		Table *table=preAnalyTbl(nvtattrs, prod, n, select);
		
		string str;
		cout << "input the string:" << endl;
		cin >> str;
		str += '#';
		analysis(str, *table, gram[0].at(0));
	}
	else {
		cout << endl << "it's not LL1!" << endl;
	}
		
	return 0;
}
//�滻���е��ַ�����û��ʵ��
//�Ƚ����е��ķ��任��û��|���ķ�
//��һ���ķ�������ж���ݹ����ʽʱ��������ݹ��д���
//�����������һ����ݹ��ˣ��ٶ����һ�¡�
//���ڸĽ��ж��ܷ�*�Ƶ����յĺ���
//*�Ƶ���������ȷ��,���ڸĽ�first����
/*���벻��������ʱ��ʾ���ⲿ�������󡱣������ҵĴ�������ȫ��ȷ�ģ��Һ�������������������
һ�����������������ǶԽ���������д����������֪�����ĸ������������ˣ���������Ƕ���*/
//���ļ������Ѿ�����ˣ����ڵĴ���Ӧ���߼���������
//first����д���ˣ��������дfollow������
//follow����Ҳд���ˣ�����дselect�����ˡ�