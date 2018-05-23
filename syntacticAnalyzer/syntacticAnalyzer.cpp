// syntacticAnalyzer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<string>
using namespace std;

#define SIZE 20 
#define VTsize 20
#define NVTsize 20

struct NVTattr {//用来存放某一个非终结符的first和follow集合的元素,以及能否推出空
	char NVT;
	bool getNull;//该非终结符能否推出空
	int firstSize;
	int followSize;
	char first[VTsize];
	char follow[VTsize];
};

struct NVTattrs {
	int length;//非终结符的个数
	NVTattr nvtattr[NVTsize];
};

bool doesContainLeft(string left, string right) {//替换所有
	for (unsigned int i=0; i < right.length(); i++) {
		if (left[0] == right[i])
		{	
			
			return true;
		}
	}
	return false;
}

/*将产生式分成左右两部分*/
void splitLeft_Right(string gram[SIZE],int n,string (&prod)[SIZE][2]) {
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
				begin = end+1;
			}
		}
		end = prod[i][1].length();
		p[k][0] = prod[i][0];
		p[k++][1] = prod[i][1].substr(begin, end - begin);
		begin = 0;
	}
	
	/*
	cout << endl;
	for (int i = 0; i < k; i++) {
		cout << p[i][0] << "->" << p[i][1]<<endl;
	}*/

	//赋值回原二维数组
	n = k;
	for (int i = 0; i < k; i++) {
		prod[i][0]= p[i][0];
		prod[i][1] = p[i][1];
	}
}

string setNVTorder(string prod[][2],int n) {
	string order, lc = "";//cc为last char,上一个字符
						  //给定一个非终结符的顺序
	for (int i = 0; i < n; i++) {
		if (prod[i][0] != lc) {
			order += prod[i][0];
			lc = prod[i][0];
		}
	}
	//cout << order;
	return order;
}

/*自己写一个静态数组的插入函数*/
void insertBehind(string(&prod)[SIZE][2],int &n,int loc,string item[2]) {
	for (int i = n; i >loc; i--) {
		prod[i ][0] = prod[i - 1][0];
		prod[i ][1] = prod[i - 1][1];
	}
	prod[loc + 1][0] = item[0];
	prod[loc + 1][1] = item[1];
	n++;
}

//删除一个数组元素
void deleteItem(string(&prod)[SIZE][2], int &n, int loc) {
	for (int i = loc; i <n; i++) {
		prod[i ][0] = prod[i+1][0];
		prod[i ][1] = prod[i+1][1];
	}
	n--;
}

//验证替换操作是不是完整
void eliminateLeftRecursion(string(&prod)[SIZE][2], int& n) {
	string lastNVT = prod[n - 1][0];//最后一个非终结符，不用替换
	int place = 0;//记录当前将被替换的字符在串中的位置
	string order=setNVTorder(prod,n);
	string leftNVT;//产生式右部最左边的非终结符
	string leftNVT1;
	int rightPartNum=0;//当前将要代入的产生式的右部的个数
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
							item[1].replace(0, 1, prod[i+k][1]);
							insertBehind(prod, n, j+k, item);
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
				prod[n][1] += ('Z'-recursionNum);//用Z作扩展的非终结符
				n++;
			}
			prod[n][0] = ('Z' - recursionNum);
			prod[n][1] = prod[i][1].substr(1, prod[i][1].length() - 1);
			prod[n][1]+=('Z' - recursionNum);
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
			for (int m =0; m <rightPartNum+1;m++) {//删除中间的数组元素
				deleteItem(prod, n, r);
			}
			recursionNum++;
			i--;
		}
	}

}

/*计算当前字符能否*推导出epsilon*/
int* getSub(char nvt, string prod[][2], int n);
bool getEpsilon(char ch,string prod[][2],int n){
	//一个产生式的左部如果能*推导出空，就会写在产生式里面，不会隐含在右部的推导过程中。
	int *sub = getSub(ch, prod, n);
	for (int i = 0; i < sub[0]; i++) {
		if (prod[sub[i + 1]][1] == "^") {
			return true;
		}
	}
	return false;
}

/*计算每一个非终结符是否能*推导出空，即埃普西隆//这个算法不准确，不能求出* 推导意义下的epsilon，只能求出first集合意义下的epsilon*/
void NVTdoesGetNull(string prod[SIZE][2],int n,NVTattrs &nvtattrs) {
	string pre = "";
	for (int i = 0; i < n; i++) {
		if (prod[i][0] != pre) {
			nvtattrs.nvtattr[nvtattrs.length].NVT = prod[i][0].at(0);
			nvtattrs.length++;
			pre = prod[i][0];
		}
		if (getEpsilon(prod[i][0].at(0),prod,n)) {
			nvtattrs.nvtattr[nvtattrs.length-1].getNull = true;
		}
		//需要一个函数，输入一个字符串，输出能否*推导出空
	}
}

int* getSub(char nvt,string prod[][2],int n) {//0号元素存放元素个数，从1号位置开始存放数据
	int *sub=new int[SIZE];
	int num=0;
	sub[0] = num;
	for (int i = 0; i < n; i++) {
		if (prod[i][0].at(0)==nvt) {
			sub[++num] = i;;
			sub[0] = num;
		}
	}
	return sub;//不可能出现这种情况
}

int getSubFromAttr(char ch,NVTattrs nvtattrs) {
	for(int i=0;i<nvtattrs.length;i++){
		if (nvtattrs.nvtattr[i].NVT == ch) {
			return i;
		}
	}
	return -1;//不可能出现这种情况
}

/*需要一个集合复制函数，将前一个first集合里的内容追加到后一个first集合里*/
void appendFirst(char ch,string str,NVTattrs nvtattrs) {
	int sub = getSubFromAttr(ch, nvtattrs);
	for (int i = 0; i < str.length(); i++) {
		int j = 0;
		for (j = 0; j < nvtattrs.nvtattr[sub].firstSize; j++) {
			if (str.at(i) == nvtattrs.nvtattr[sub].first[j]) {
				break;
			}
		}
		if (j == nvtattrs.nvtattr[sub].firstSize) {

		}
	}
}

/*去掉当前字符的first集合中的epsilon*/
string cutOffEpsilon(char ch,NVTattrs nvtattrs) {
	string first;
	
	int sub = getSubFromAttr(ch, nvtattrs);
	first = nvtattrs.nvtattr[sub].first;
	for (int i = 0; i < nvtattrs.nvtattr[sub].firstSize; i++) {
		if (first.at(i) == '^') {
			string temp1 = first.substr(0, i);
			string temp2 = first.substr(i + 1, first.length());
			first = temp1 + temp2;
			break;//最多只有一个epsilon
		}
	}
	return first;
}

void First(string str, NVTattrs &nvtattrs,int sub,string prod[][2],int n,bool done[]) {
	//要对输进来的非终结符的所有产生式进行遍历
	
	if (!(str.at(0) >= 'A'&& str.at(0) <= 'Z')) {
		nvtattrs.nvtattr[sub].first[nvtattrs.nvtattr[sub].firstSize] = str.at(0);
		nvtattrs.nvtattr[sub].firstSize++;
	}
	else {
		int *sub1 = getSub(str.at(0), prod,n);//不应该只是简单地得到一个数组下标，应该得到左部相同的所有数组下标
		for (int i = 0; i < sub1[0]; i++) {
			int sub2 = getSubFromAttr(prod[sub1[i+1]][1].at(0), nvtattrs);
			if (sub2!=-1&&done[sub2]) {//如果等待加入的非终结符的first集合已经做完了，那么就直接做集合运算，否则进行递归调用
				//如果当前字符能推出epsilon
				if (getEpsilon(prod[sub1[i + 1]][1].at(0), prod, n)) {
					string str = cutOffEpsilon(prod[sub1[i + 1]][1].at(0),nvtattrs);

					//函数，去掉当前字符的first集合中的epsilon
				}
				for (int j = 0; j < nvtattrs.nvtattr[sub2].firstSize; j++) {
					sub = getSubFromAttr(str.at(0), nvtattrs);
					nvtattrs.nvtattr[sub].first[nvtattrs.nvtattr[sub].firstSize] = nvtattrs.nvtattr[sub2].first[j];
					nvtattrs.nvtattr[sub].firstSize++;
				}
			}
			else {
				First(prod[sub1[i + 1]][1], nvtattrs, sub, prod, n, done);
				int sub3= getSubFromAttr(prod[sub1[i + 1]][1].at(0), nvtattrs);
				if (sub3!=-1&&done[sub3]) {//如果右部第一个字符为非终结符，即为需要等待的状态,则在它完成递归后重新处理右部
					First(str, nvtattrs, sub, prod, n, done);
				}
			}
		}
		done[sub] = true;//表示该非终结符的first已经算完了
	}
}

void getFisrt(NVTattrs &nvtattrs,string prod[][2],int n) {//通过一个字符串，计算它的first集合,有两种可能，一是首字符为终结符，二是首字符为非终结符（找文法，左部为它，右部）
	string str="";
	bool done[NVTsize] = {false};

	for (int i = 0; i <nvtattrs.length; i++) {//对所有的非终结符求first集合
		str = "";
		str += nvtattrs.nvtattr[i].NVT;
		int sub = getSubFromAttr(str.at(0), nvtattrs);
		if (sub != -1 && !done[sub]) {
			First(str, nvtattrs,i,prod,n,done);
		}
	}
}

void FirstFollow(string prod[][2],int n) {//计算first和follow集合
	NVTattrs nvtattrs;
	nvtattrs.length = 0;
	for (int i = 0; i < NVTsize; i++) {
		nvtattrs.nvtattr[i].getNull = false;
		nvtattrs.nvtattr[i].firstSize = 0;
		nvtattrs.nvtattr[i].followSize = 0;
		for (int j = 0; j < VTsize; j++) {
			nvtattrs.nvtattr[i].first[j]='`';//集合初始化为“`”
			nvtattrs.nvtattr[i].follow [j]= '`';
		}
	}//结构体初始化


	NVTdoesGetNull(prod, n, nvtattrs);

	getFisrt(nvtattrs, prod, n);
	
}

bool isLL1(string gram[],int n) {
	string production[SIZE][2];

	/*for (int i=0; i < n; i++) {
		cout << gram[i]<<endl;
	}*/
	splitLeft_Right(gram,n,production);
	
	eliminateVerticalLine(production, n);
	/*for (int i = 0; i < n; i++) {
	cout << production[i][0] <<"->"<<production[i][1]<<endl;
	}*/

	eliminateLeftRecursion(production, n);

	cout << endl;
	for (int i = 0; i < n; i++) {
		cout << production[i][0] << "->" << production[i][1] << endl;
	}

	FirstFollow(production, n);


	return true;
}

int main()
{
	int n;
	string gram[SIZE];
	cout << "input the lines of grammer:" << endl;
	cin >> n;
	cout << "input the grammer:" << endl;
	for (int i = 0; i < n; i++) {
		cin >> gram[i];
	}
	isLL1(gram,n);
    return 0;
}
//替换所有的字符串还没有实现
//先将所有的文法变换成没有|的文法
//当一个文法中最后含有多个递归产生式时，消除左递归有错误。
//好像可以消除一切左递归了，再多测试一下。
//现在改进判断能否*推导出空的函数
//*推导好像是正确的,现在改进first集合