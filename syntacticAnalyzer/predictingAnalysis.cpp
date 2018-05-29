#include"stdafx.h"
#include<iostream>
#include<string>
#include<stack>

#include"declaration.h"
#include"predictingAnalysis.h"

using namespace std;

/*输入：整个文法，文法行数；输出：返回文法中包含的终结符的个数（0号单元）,后面存储终结符，用string数组形式存放*/
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

/*输入：非终结符表，整个文法，文法行数；输出：申请表空间，初始化表头*/
Table* initTable(NVTattrs nvtattrs,string prod[][2],int n) {//动态申请一个string二维数组，初始化
	string *VT = countVT(prod, n);
	int len = atoi(VT[0].c_str());

	Table *table=new Table;
	table->height = nvtattrs.length +1;
	table->width = len+2;//加2是考虑到“#”和行表头的空间

	//初始化行表头
	for (int i = 0; i < nvtattrs.length; i++) {
		table->tbl[i + 1][0] += nvtattrs.nvtattr[i].NVT;
	}
	//初始化列表头
	for (int i = 0; i <len; i++) {
		table->tbl[0][i + 1] = VT[i + 1];
	}
	table->tbl[0][len + 1] = "#";

	return table;
}

/*输入：产生式的左部，终结符,下标类型,预测分析表，0为行号、1为列号；输出：返回下标*/
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

/*输入：非终结符表，整个文法，文法行数，select集合；输出：完善预测分析表并返回*/
Table* preAnalyTbl(NVTattrs nvtattrs,string prod[][2],int n,string select[][3]){//预测分析表
	Table *table=initTable(nvtattrs,prod,n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < select[i][2].length(); j++) {
			//需要一个函数，来通过select集合的第一列左部、第三列终结符分别得到在它们在预测分析表的行号和列号
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

/*输入：一个栈，打印顺序，0为正序，1为逆序；输出：打印整个栈的内容*/
void printStack(stack<char> stk,int flag){
	stack<char> *cp=new stack<char>;//动态申请一个栈空间
	*cp = stk;//将stk的内容复制过来
	string str;//用来存放栈的字符串内容

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

/*输入：符号串，预测分析表,文法的开始符；输出：打印分析过程*/
void analysis(string str,Table table,char start) {
	stack <char>anly;//分析栈
	stack <char>input;//剩余输入栈
	int count = 0;//步骤计数
	string result;//所有产生式或匹配
	string showResult;
	bool error = false;//记录是否出错

	/*初始化栈*/
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

			if (ana >= 'A'&&ana <= 'Z') {//如果是非终结符
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
			else {//如果是终结符
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