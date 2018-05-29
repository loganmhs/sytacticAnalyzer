#include"stdafx.h"
#include<iostream>
#include<string>
#include<stack>

#include"declaration.h"
#include"predictingAnalysis.h"

using namespace std;

/*���룺�����ķ����ķ�����������������ķ��а������ս���ĸ�����0�ŵ�Ԫ��,����洢�ս������string������ʽ���*/
string* countVT(string prod[][2], int n) {
	int count = 0;
	string *VT = new string[VTsize];
	for (int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < prod[i][1].length(); j++) {
			if (!(prod[i][1].at(j) >= 'A'&&prod[i][1].at(j) <= 'Z') && prod[i][1].at(j) != '^') {
				VT[++count] += prod[i][1].at(j);
			}
		}
	}
	VT[0] += to_string(count);
	return VT;
}

/*���룺���ս���������ķ����ķ�����������������ռ䣬��ʼ����ͷ*/
Table* initTable(NVTattrs nvtattrs,string prod[][2],int n) {//��̬����һ��string��ά���飬��ʼ��
	string *VT = countVT(prod, n);
	int len = atoi(VT[0].c_str());

	Table *table=new Table;
	table->height = nvtattrs.length +1;
	table->width = len+2;//��2�ǿ��ǵ���#�����б�ͷ�Ŀռ�

	//��ʼ���б�ͷ
	for (int i = 0; i < nvtattrs.length; i++) {
		table->tbl[i + 1][0] += nvtattrs.nvtattr[i].NVT;
	}
	//��ʼ���б�ͷ
	for (int i = 0; i <len; i++) {
		table->tbl[0][i + 1] = VT[i + 1];
	}
	table->tbl[0][len + 1] = "#";

	return table;
}

/*���룺����ʽ���󲿣��ս��,�±�����,Ԥ�������0Ϊ�кš�1Ϊ�кţ�����������±�*/
int getSubFromTable(char left, char vt, int flag,Table table) {
	switch (flag)
	{
	case 0:
		for (int i = 0; i < table.height; i++) {
			if (left == table.tbl[i + 1][0].at(0)) {
				return i+1;
			}
		}
		break;
	case 1:
		for (int i = 0; i < table.width; i++) {
			if (vt == table.tbl[0][i + 1].at(0)) {
				return i + 1;
			}
		}
		break;
	default:
		break;
	}
	return -1;
}

/*���룺���ս���������ķ����ķ�������select���ϣ����������Ԥ�����������*/
Table* preAnalyTbl(NVTattrs nvtattrs,string prod[][2],int n,string select[][3]){//Ԥ�������
	Table *table=initTable(nvtattrs,prod,n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < select[i][2].length(); j++) {
			//��Ҫһ����������ͨ��select���ϵĵ�һ���󲿡��������ս���ֱ�õ���������Ԥ���������кź��к�
			int x = getSubFromTable(select[i][0].at(0), select[i][2].at(j), 0, *table);
			int y = getSubFromTable(select[i][0].at(0), select[i][2].at(j), 1, *table);
			table->tbl[x][y] = select[i][1];
		}
	}
	cout <<endl<< "predicting analysis table:" << endl;
	for (int i = 0; i < table->height; i++) {
		for (int j = 0; j < table->width; j++) {
			cout << table->tbl[i][j]<<'\t';
		}
		cout << endl;
	}
	return table;
}

/*���룺һ��ջ����ӡ˳��0Ϊ����1Ϊ�����������ӡ����ջ������*/
void printStack(stack<char> stk,int flag){
	stack<char> *cp=new stack<char>;//��̬����һ��ջ�ռ�
	*cp = stk;//��stk�����ݸ��ƹ���
	string str;//�������ջ���ַ�������

	while (!cp->empty())
	{
		str += cp->top();
		cp->pop();
	}

	switch (flag)
	{
	case 0:
		cout << str;
		break;
	case 1:
		for (int i = (int)str.length()-1; i >=0; i--) {
			cout << str.at(i);
		}
		break;
	default:
		break;
	}
	delete cp;
}

/*���룺���Ŵ���Ԥ�������,�ķ��Ŀ�ʼ�����������ӡ��������*/
void analysis(string str,Table table,char start) {
	stack <char>anly;//����ջ
	stack <char>input;//ʣ������ջ
	int count = 0;//�������
	string result;//���в���ʽ��ƥ��
	string showResult;
	bool error = false;//��¼�Ƿ����

	/*��ʼ��ջ*/
	anly.push('#');
	anly.push(start);
	for (int i =(int) str.length()-1; i >=0; i--) {
		input.push(str.at(i));
	}

	cout << "Step  Analysis  Input  Match" << endl;

	try
	{
		while (result != "acc") {
			cout << ++count << "\t";
			printStack(anly, 1);
			cout << "\t";
			printStack(input, 0);
			cout << "\t";

			char ana = anly.top();
			anly.pop();
			char inp = input.top();

			if (ana >= 'A'&&ana <= 'Z') {//����Ƿ��ս��
				int x = getSubFromTable(ana, inp, 0, table);
				int y = getSubFromTable(ana, inp, 1, table);
				result = table.tbl[x][y];
				if (result == "") {
					error = true;
					break;
				}
				showResult = table.tbl[x][0] + "->" + table.tbl[x][y];
				for (int i = (int)result.length() - 1; i >= 0; i--) {
					if (result.at(i) != '^')
						anly.push(result.at(i));
				}
				cout << showResult << endl;
			}
			else {//������ս��
				if (ana != inp) {
					error = true;
					break;
				}
				else {
					if (ana == '#') {
						result = "acc";
					}
					else {
						result = '\"';
						result += ana;
						result += '\"';
						result += "match";
						input.pop();
					}
				}
				cout << result << endl;
			}
		}
	}
	catch (const std::exception&)
	{
		error = true;
	}

	if (error == true) {
		cout <<endl<< "the sentence doesn't belong to this grammar!" << endl;
	}
	else {
		cout << "the above is the steps!" << endl;
	}
}