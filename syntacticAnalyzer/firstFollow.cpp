#include"stdafx.h"
#include<iostream>
#include"declaration.h"
#include<string>
#include"firstFollow.h"
using namespace std;

/*输入：一个字符，非终结符表；输出：返回该字符在非终结符表中的行号*/
int getSubFromNVT(char ch,NVTattrs nvtattrs) {
	for (int i = 0; i < nvtattrs.length; i++) {
		if (nvtattrs.nvtattr[i].NVT == ch) {
			return i;
		}
	}
	return -1;//找到返回行号，未找到返回出错值-1
}

/*输入：一个非终结符表、整个文法、文法行数；输出：填写非终结符表中的所有子项的getNull信息*/
void getEpsilon(NVTattrs &nvtattrs, string prod[SIZE][2], int n) {
	for (int j = 0; j < n; j++) {
		if (prod[j][1] == "^") {
			char ch= prod[j][0].at(0);
			int sub = getSubFromNVT(ch, nvtattrs);
			nvtattrs.nvtattr[sub].getNull = true;
		}
	}
}

/*输入：一个非终结符表、一个文法数组、文法行数，文法的开始符；输出：初始化该非终结符表*/
void initNVTtable(NVTattrs &nvtattrs, string prod[][2], int n,char start) {
	char pre = ' ';//表示上一行产生式的左部
	int j = 0;
	nvtattrs.length = 0;

	for (int i = 0; i < n; i++) {
		if (prod[i][0].at(0) != pre) {
			nvtattrs.nvtattr[j].NVT = prod[i][0].at(0);
			nvtattrs.nvtattr[j].getNull = false;
			nvtattrs.nvtattr[j].firstSize = 0;
			nvtattrs.nvtattr[j].followSize = 0;
			for (int k = 0; k < VTsize; k++) {
				nvtattrs.nvtattr[j].first[k] = ' ';
				nvtattrs.nvtattr[j].follow[k] = ' ';
			}
			nvtattrs.length++;
			nvtattrs.nvtattr[j].doneFirst = false;
			nvtattrs.nvtattr[j].doneFollow = false;
			if (nvtattrs.nvtattr[j].NVT == start)
			{
				nvtattrs.nvtattr[j].follow[0] = '#';
				nvtattrs.nvtattr[j].followSize++;
			}
			j = nvtattrs.length;
			pre = prod[i][0].at(0);
		}
	}
}

/*输入：一个字符、整个文法、文法行数；输出：返回有关该字符的所有产生式的下标数组*/
int * getSubArrayFromProds(char ch,string prod[][2],int n) {
	int *sub= new int[SIZE];//0号单元存放整个数组的长度，从1号单元开始存放数据
	sub[0] = 0;
	int len = sub[0];
	for (int i = 0; i < n; i++) {
		if (prod[i][0].at(0) == ch) {
			sub[++len] = i;
			sub[0]=len;
		}
	}
	return sub;
}

/*输入：非终结符集合，非终结符行号，追加的元素；输出：填写相应的first集合，包括去重*/
void firstAppend(NVTattrs &nvtattrs,int sub,char ch) {
	int i = 0;
	int len = nvtattrs.nvtattr[sub].firstSize;

	for (i = 0; i < len; i++) {
		if (nvtattrs.nvtattr[sub].first[i] == ch) {
			break;
		}
	}
	if (i == len) {
		nvtattrs.nvtattr[sub].first[len] = ch;
		nvtattrs.nvtattr[sub].firstSize++;
	}
}

/*输入：非终结符集合，待追加非终结符行号，源非终结符行号；输出：填写相应的first集合，包括去重和去空*/
void firstAppend(NVTattrs &nvtattrs, int des, int res) {
	string resF = nvtattrs.nvtattr[res].first;
	int len = nvtattrs.nvtattr[res].firstSize;

	for (int i = 0; i < len; i++) {
		if (resF.at(i) == '^') {
			string str1 = resF.substr(0, i);
			string str2 = resF.substr(i + 1, len);
			resF = str1 + str2;
			len--;
			break;
		}
	}
	
	int desLen = nvtattrs.nvtattr[des].firstSize;
	for(int i=0;i<len;i++){
		int j;
		for (j = 0; j < desLen; j++) {
			if (nvtattrs.nvtattr[des].first[j] == resF.at(i)) {
				break;
			}
		}
		if (j == desLen) {
			nvtattrs.nvtattr[des].first[desLen] = resF.at(i);
			nvtattrs.nvtattr[des].firstSize++;
			desLen = nvtattrs.nvtattr[des].firstSize;
		}
	}
}

/*输入：一个字符、非终结符集合，整个文法，产生式行数；输出：填写对应的非终结符集合first集合*/
void first(char ch, NVTattrs &nvtattrs, string prod[][2], int n) {
	int subN = getSubFromNVT(ch, nvtattrs);
	int *subP = getSubArrayFromProds(ch, prod, n);
	int subPnow = 0;
	char right;

	for (int i = 0; i < subP[0]; i++) {
		subPnow = subP[i + 1];
		right = prod[subPnow][1].at(0);
		if (!(right >= 'A'&&right <= 'Z')) {
			//nvtattrs.nvtattr[subN].first[len]=//需要一个往集合里追加元素的函数，包括去掉重复项，需要两个函数，重载实现，追加一个元素和追加一个集合
			firstAppend(nvtattrs, subN, right);//第一种情况已经完成了
		}
		else {

			int subRight = getSubFromNVT(right, nvtattrs);//产生式右部首字符在非终结符表中的下标
			if (nvtattrs.nvtattr[subRight].doneFirst) {
				//去掉epsilon后追加到当前字符的first集合里
				//写追加函数，要去掉epsilon
				int subNres = getSubFromNVT(right,nvtattrs);//源first集合的字符的下标
				firstAppend(nvtattrs, subN, subNres);
			}
			else {
				first(right, nvtattrs, prod, n);
				first(ch, nvtattrs, prod, n);
			}

			int next = 0;
			char nextChar = prod[subPnow][1].at(next);
			int subNext = getSubFromNVT(nextChar, nvtattrs);
			int subLast = getSubFromNVT(right, nvtattrs);
			next++;
			while (next!=prod[subPnow][1].length())
			{
				nextChar = prod[subPnow][1].at(next);
				subNext = getSubFromNVT(nextChar, nvtattrs);
				if (subNext == -1||!nvtattrs.nvtattr[subLast].getNull)break;

				first(nextChar, nvtattrs, prod, n);
				firstAppend(nvtattrs, subN, subNext);
				subLast = subNext;//下一次循环时使用
				next++;
			}
			if (subNext == -1&&nvtattrs.nvtattr[subLast].getNull) {//如果下一位是终结符
				firstAppend(nvtattrs, subN, nextChar);
			}
		
		}
	}
	nvtattrs.nvtattr[subN].doneFirst = true;
	delete[]subP;
}

/*输入：非终结符集合、整个文法、产生式行数；输出：填写对应的非终结符的first集合*/
void getFisrtAssemble(NVTattrs &nvtattrs,string prod[][2],int n) {
	for (int i = 0; i < nvtattrs.length; i++) {
		//需要一个函数，输入一个产生式的左部，能求出它完整的first集合。消除左递归后，求first集合不存在死循环的情况,最主要的是这个函数能进行递归调用
		first(nvtattrs.nvtattr[i].NVT, nvtattrs, prod, n);//只有对小写字母的处理
	}
	cout <<endl<< "First:"<<endl;
	for (int i = 0; i < nvtattrs.length; i++) {
		cout << nvtattrs.nvtattr[i].NVT << "\t";
		for (int j = 0; j < nvtattrs.nvtattr[i].firstSize; j++) {
			cout << nvtattrs.nvtattr[i].first[j];
		}
		cout << endl;
	}
}
/************************************以上为求first集合*******************************************************/

/************************************以下为求follow集合*******************************************************/
/*输入：一个字符，整个文法，产生式行数；输出：返回右部含有该字符的产生式的下标数组*/
int ** getSubArrayFromProdsRight(char ch, string prod[][2], int n) {
	int **sub = new int*[SIZE];//第一列表示产生式的下标，第二列表示该产生式右部中该字符在字符串中的下标
	for (int i = 0; i < SIZE; i++) {
		sub[i] = new int[2];
	}
	sub[0][0] = 0;//返回的下标个数
	sub[0][1] = 0;//不表示任何意义，仅做简单的初始化
	int len = sub[0][0];
	for (int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < prod[i][1].length(); j++) {
			if (prod[i][1].at(j) == ch) {
				sub[++len][0] = i;
				sub[len][1] = j;
				sub[0][0] = len;
				break;
			}
		}
	}
	return sub;
}

/*输入：目的行号，源行号，非终结符集合；输出：填写相应的follow集合*/
void appendFfromF(NVTattrs &nvtattrs, int des, int res) {
	for (int i = 0; i < nvtattrs.nvtattr[res].followSize; i++) {
		int len = nvtattrs.nvtattr[des].followSize;
		int j = 0;
		for (j = 0; j < len; j++) {
			if (nvtattrs.nvtattr[des].follow[j] == nvtattrs.nvtattr[res].follow[i]) {
				break;
			}
		}
		if (j == len) {
			nvtattrs.nvtattr[des].follow[len] = nvtattrs.nvtattr[res].follow[i];
			nvtattrs.nvtattr[des].followSize++;
			len = nvtattrs.nvtattr[des].followSize;
		}
	}
}

/*输入：目的行号，源字符，非终结符结合；输出：填写相应的follow集合*/
void appendFfromChar(NVTattrs &nvtattrs, int des, char res) {
	int len = nvtattrs.nvtattr[des].followSize;
	int i;
	for (i = 0; i < len; i++) {
		if (nvtattrs.nvtattr[des].follow[i] == res) {
			break;
		}
	}
	if (i == len) {
		nvtattrs.nvtattr[des].follow[len] = res;
		nvtattrs.nvtattr[des].followSize++;
	}
}

/*输入：目的行号，源行号，非终结符集合*/
void appendFfromFirst(NVTattrs &nvtattrs, int des, int res) {
	for (int i = 0; i < nvtattrs.nvtattr[res].firstSize; i++) {
		int len = nvtattrs.nvtattr[des].followSize;
		int j;
		for (j = 0; j < len; j++) {
			if (nvtattrs.nvtattr[des].follow[j] == nvtattrs.nvtattr[res].first[i]) {
				break;
			}
		}
		if (j == len&&nvtattrs.nvtattr[res].first[i] != '^') {
			nvtattrs.nvtattr[des].follow[len] = nvtattrs.nvtattr[res].first[i];
			nvtattrs.nvtattr[des].followSize++;
		}
	}
}

/*输入：一个字符，非终结符集合、整个文法，产生式行数，该递归函数被调用的次数；输出：填写对应的非终结符集合的follow集合*/
void follow(char ch, NVTattrs &nvtattrs, string prod[][2], int n,int &count) {
	int **subP = getSubArrayFromProdsRight(ch, prod, n);
	int subNVT = getSubFromNVT(ch, nvtattrs);//该字符在非终结符表的下标
	int back=0;//记录follow是正常退出0，还是从死循环中跳出-1

	count++;
	if (count > nvtattrs.length) {//错误在这****************************************************************************
		//nvtattrs.nvtattr[subNVT].doneFollow = true;
		count = 0;
		return;//异常结束
	}

	for (int i = 0; i < subP[0][0]; i++) {
		int subNow = subP[i + 1][0];//当前产生式的下标
		int subStr = subP[i + 1][1];//该字符在字符串中的下标
		int subLeft = getSubFromNVT(prod[subNow][0].at(0), nvtattrs);//产生式左部字符在非终结符表的下标

		//每一次递归调用follow函数时，都应该判断该非终结符的follow集合是否完成了
		if (subStr == prod[subNow][1].length() - 1) {//如果当前字符后面没有其他字符了
			if (!nvtattrs.nvtattr[subLeft].doneFollow)
			{
				if(ch!=prod[subNow][0].at(0))
					follow(prod[subNow][0].at(0), nvtattrs, prod, n,count);/*******************************自己等待自己的follow集合，有点问题*******************************/
			}
			else {
				appendFfromF(nvtattrs, subNVT, subLeft);//把产生式左部字符的follow集合加进来
			}
		}
		else {//还有其他字符
			char next = prod[subNow][1].at(subStr + 1);
			if (!(next >= 'A'&&next <= 'Z')) {
				if(next!='^')
					appendFfromChar(nvtattrs, subNVT, next);//需要一个函数，实现加一个字符进入follow集合，但是得去重
			}
			else {//下一个字符是非终结符
				int j = subStr+1;
				int subNext;

				do {
					next = prod[subNow][1].at(j++);
					subNext = getSubFromNVT(next, nvtattrs);
					if (subNext == -1)break;
					appendFfromFirst(nvtattrs, subNVT, subNext);
				} while (nvtattrs.nvtattr[subNext].getNull && j != prod[subNow][1].length());

				if (subNext == -1) {//如果下一字符是终结符
					appendFfromChar(nvtattrs, subNVT, next);
				}
				else if (j == prod[subNow][1].length()&&nvtattrs.nvtattr[subNext].getNull) {//产生式右部所有非终结符依次都能推出epsilon，所以得把产生式左部的follow集合追加进来
					if (nvtattrs.nvtattr[subLeft].doneFollow) {//如果产生式左部的follow集合已经做完了，就直接加上它的follow集合
						appendFfromF(nvtattrs, subNVT, subLeft);
					}
					else {
						if(prod[subNow][0].at(0)!=ch)
							follow(prod[subNow][0].at(0), nvtattrs, prod, n,count);
					}

					if (subNVT != subLeft) {
						appendFfromF(nvtattrs, subNVT, subLeft);
					}
				}
			}
		}
	}

	
	nvtattrs.nvtattr[subNVT].doneFollow = true;
	count = 0;
	for (int i = 0; i < SIZE; i++) {
		delete[] subP[i];
	}
	delete[]subP;
}

/*输入：非终结符集合、整个文法，产生式行数；输出：填写对应的非终结符的follow集合*/
void getFollowAssemble(NVTattrs &nvtattrs, string prod[][2], int n) {
	int count = 0;
	for (int i = 0; i < nvtattrs.length; i++) {
		if(!nvtattrs.nvtattr[i].doneFollow)
			follow(nvtattrs.nvtattr[i].NVT,nvtattrs,prod, n,count);
	}

	//如果碰到从死循环里跳出的情况，那么第一次求的follow集合就不完善，需要第二次求follow集合来完善这个集合
	count = 0;
	for (int i = 0; i < nvtattrs.length; i++) {
		follow(nvtattrs.nvtattr[i].NVT, nvtattrs, prod, n, count);
	}

	cout <<endl<< "Follow:" << endl;
	for (int i = 0; i < nvtattrs.length; i++) {
		cout << nvtattrs.nvtattr[i].NVT << '\t';
		for (int j = 0; j < nvtattrs.nvtattr[i].followSize; j++) {
			cout << nvtattrs.nvtattr[i].follow[j];
		}
		cout << endl;
	}
}

/*输入：字符串，非终结符集合；输出：返回一个能否得到epsilon的bool值*/
bool getEpsilon(string right, NVTattrs nvtattrs) {
	for (unsigned int i = 0;i<right.length(); i++) {
		if (!(right.at(i) >= 'A'&&right.at(i) <= 'Z')) {
			if (right.at(i) == '^') {
				return true;
			}
			else {
				return  false;
			}
		}
		else {//如果是非终结符
			char ch = right.at(i);
			int subNVT = getSubFromNVT(ch, nvtattrs);
			if (!nvtattrs.nvtattr[subNVT].getNull) {
				return false;
			}
		}
	}
	return true;
}

/*输入：非终结符集合，非终结符集合下标，select集合，select集合下标，flag中0为对first集合求并集，1为对follow集合求并集；输出：填写select集合*/
void SetUnion(NVTattrs nvtattrs, int res, string (&select)[SIZE][3], int des,int flag) {
	switch (flag)
	{
	case 0:
		for (int i = 0; i < nvtattrs.nvtattr[res].firstSize; i++) {
			unsigned int j;
			for (j = 0; j < select[des][2].length(); j++) {
				if (nvtattrs.nvtattr[res].first[i] == select[des][2].at(j)) {
					break;
				}
			}
			if (j == select[des][2].length()) {
				if (nvtattrs.nvtattr[res].first[i] != '^')
					select[des][2] += nvtattrs.nvtattr[res].first[i];
			}
		}
		break;
	case 1:
		for (int i = 0; i < nvtattrs.nvtattr[res].followSize; i++) {
			unsigned int j;
			for (j = 0; j < select[des][2].length(); j++) {
				if (nvtattrs.nvtattr[res].follow[i] == select[des][2].at(j)) {
					break;
				}
			}
			if (j == select[des][2].length()) {
				select[des][2] += nvtattrs.nvtattr[res].follow[i];
			}
		}
		break;
	default:
		break;
	}
	
}

/*输入：非终极符集合，select集合，select数组下标；输出：填写对应的select集合*/
void firstStr(NVTattrs nvtattrs,string (&select)[SIZE][3],int i) {
	unsigned int j = 0;
	while (j < select[i][1].length()) {
		int res = getSubFromNVT(select[i][1].at(j), nvtattrs);
		if (res == -1) {//如果该字符为终结符
			if(select[i][1].at(j)!='^')
				select[i][2] += select[i][1].at(j);
			break;
		}
		SetUnion(nvtattrs, res, select, i,0);
		if (nvtattrs.nvtattr[res].getNull) {//如果当前非终结符能推出epsilon
			j++;
		}
		else {//如果当前非终结符不能推出epsilon
			break;
		}
	}
}

/*输入：非终结符集合、整个文法，产生式行数，select集合；输出：填写对应的select集合*/
void getSelectAssemble(NVTattrs nvtattrs, string prod[][2], int n, string (&select)[SIZE][3]) {
	for (int i = 0; i < n; i++) {
		select[i][0] += prod[i][0];// 产生式左部
		select[i][1] += prod[i][1];//产生式右部
		char left = prod[i][0].at(0);
		int subLeft = getSubFromNVT(left, nvtattrs);
		//需要一个函数，通过一个字符串判断它能不能*推导出epsilon
		if (!getEpsilon(select[i][1], nvtattrs)) {//如果该产生式右部不能推出epsilon
			//select集合等于first（select[i][1])
			//需要实现一个集合相交函数，从非终结符集合的first集合里拿出来加入到select集合的第三列
			firstStr(nvtattrs, select, i);
		}
		else {//如果该产生式右部能推出空
			//select集合等于first（select[i][1]）-｛epsilon｝并上follow（select[i][0]）;		
			firstStr(nvtattrs, select, i);
			SetUnion(nvtattrs, subLeft, select, i, 1);
		}
	}

	cout << endl << "Select:" << endl;
	for (int i = 0; i < n; i++) {
		cout << select[i][0] << "->" << select[i][1]<<"\t";
		for (unsigned int j = 0; j < select[i][2].length(); j++) {
			cout << select[i][2].at(j) << " ";
		}
		cout << endl;
	}
}

