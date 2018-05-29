// syntacticAnalyzer.cpp : 定义控制台应用程序的入口点。
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
//替换所有的字符串还没有实现
//先将所有的文法变换成没有|的文法
//当一个文法中最后含有多个递归产生式时，消除左递归有错误。
//好像可以消除一切左递归了，再多测试一下。
//现在改进判断能否*推导出空的函数
//*推导好像是正确的,现在改进first集合
/*编译不报错，运行时提示“外部解析错误”，可是我的代码是完全正确的，我后来进行了两个操作，
一是清理解决方案，二是对解决方案运行代码分析，不知道是哪个操作起作用了，反正结果是对了*/
//分文件工作已经完成了，现在的代码应该逻辑很清晰了
//first集合写好了，下面可以写follow集合了
//follow集合也写好了，可以写select集合了。