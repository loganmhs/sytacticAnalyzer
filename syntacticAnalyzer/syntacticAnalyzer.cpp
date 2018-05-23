// syntacticAnalyzer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<iostream>
#include<string>
using namespace std;

#define SIZE 20 
#define VTsize 20
#define NVTsize 20

struct NVTattr {//�������ĳһ�����ս����first��follow���ϵ�Ԫ��,�Լ��ܷ��Ƴ���
	char NVT;
	bool getNull;//�÷��ս���ܷ��Ƴ���
	int firstSize;
	int followSize;
	char first[VTsize];
	char follow[VTsize];
};

struct NVTattrs {
	int length;//���ս���ĸ���
	NVTattr nvtattr[NVTsize];
};

bool doesContainLeft(string left, string right) {//�滻����
	for (unsigned int i=0; i < right.length(); i++) {
		if (left[0] == right[i])
		{	
			
			return true;
		}
	}
	return false;
}

/*������ʽ�ֳ�����������*/
void splitLeft_Right(string gram[SIZE],int n,string (&prod)[SIZE][2]) {
	for (int i = 0; i < n; i++) {
		prod[i][0] = gram[i][0];
		for (unsigned int j = 3; j < gram[i].length(); j++) {
			prod[i][1] += gram[i][j];
		}
	}
}

/*��������ʽ�Ҳ�������*/
void eliminateVerticalLine(string(&prod)[SIZE][2], int &n) {
	string p[SIZE][2];//�²�����û�С�|�����ķ�
	int k = 0;//��¼�����ɵĲ���ʽ�ĸ���
	int begin = 0, end = 0;//��¼����������|�����±�

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

	//��ֵ��ԭ��ά����
	n = k;
	for (int i = 0; i < k; i++) {
		prod[i][0]= p[i][0];
		prod[i][1] = p[i][1];
	}
}

string setNVTorder(string prod[][2],int n) {
	string order, lc = "";//ccΪlast char,��һ���ַ�
						  //����һ�����ս����˳��
	for (int i = 0; i < n; i++) {
		if (prod[i][0] != lc) {
			order += prod[i][0];
			lc = prod[i][0];
		}
	}
	//cout << order;
	return order;
}

/*�Լ�дһ����̬����Ĳ��뺯��*/
void insertBehind(string(&prod)[SIZE][2],int &n,int loc,string item[2]) {
	for (int i = n; i >loc; i--) {
		prod[i ][0] = prod[i - 1][0];
		prod[i ][1] = prod[i - 1][1];
	}
	prod[loc + 1][0] = item[0];
	prod[loc + 1][1] = item[1];
	n++;
}

//ɾ��һ������Ԫ��
void deleteItem(string(&prod)[SIZE][2], int &n, int loc) {
	for (int i = loc; i <n; i++) {
		prod[i ][0] = prod[i+1][0];
		prod[i ][1] = prod[i+1][1];
	}
	n--;
}

//��֤�滻�����ǲ�������
void eliminateLeftRecursion(string(&prod)[SIZE][2], int& n) {
	string lastNVT = prod[n - 1][0];//���һ�����ս���������滻
	int place = 0;//��¼��ǰ�����滻���ַ��ڴ��е�λ��
	string order=setNVTorder(prod,n);
	string leftNVT;//����ʽ�Ҳ�����ߵķ��ս��
	string leftNVT1;
	int rightPartNum=0;//��ǰ��Ҫ����Ĳ���ʽ���Ҳ��ĸ���
	int rightPartIndex[SIZE];//��ź�����ݹ�Ĳ���ʽ��������ݹ���ս�����ǲ��ֲ���ʽ���±�
	int recursionNum = 0;//����ݹ����ʽ�ı��

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
			}//�õ����±�����
			
			for (int k = 0; k < rightPartNum; k++) {
				prod[n][0] = prod[i][0];
				prod[n][1] = prod[rightPartIndex[k]][1];
				prod[n][1] += ('Z'-recursionNum);//��Z����չ�ķ��ս��
				n++;
			}
			prod[n][0] = ('Z' - recursionNum);
			prod[n][1] = prod[i][1].substr(1, prod[i][1].length() - 1);
			prod[n][1]+=('Z' - recursionNum);
			n++;

			prod[n][0] = ('Z' - recursionNum);
			prod[n][1] = "^";//��^����������¡
			n++;

			int r = 0;
			for (r = 0; r < n; r++) {
				if (prod[r][0] == prod[i][0]) {
					break;
				}
			}
			for (int m =0; m <rightPartNum+1;m++) {//ɾ���м������Ԫ��
				deleteItem(prod, n, r);
			}
			recursionNum++;
			i--;
		}
	}

}

/*���㵱ǰ�ַ��ܷ�*�Ƶ���epsilon*/
int* getSub(char nvt, string prod[][2], int n);
bool getEpsilon(char ch,string prod[][2],int n){
	//һ������ʽ���������*�Ƶ����գ��ͻ�д�ڲ���ʽ���棬�����������Ҳ����Ƶ������С�
	int *sub = getSub(ch, prod, n);
	for (int i = 0; i < sub[0]; i++) {
		if (prod[sub[i + 1]][1] == "^") {
			return true;
		}
	}
	return false;
}

/*����ÿһ�����ս���Ƿ���*�Ƶ����գ���������¡//����㷨��׼ȷ���������* �Ƶ������µ�epsilon��ֻ�����first���������µ�epsilon*/
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
		//��Ҫһ������������һ���ַ���������ܷ�*�Ƶ�����
	}
}

int* getSub(char nvt,string prod[][2],int n) {//0��Ԫ�ش��Ԫ�ظ�������1��λ�ÿ�ʼ�������
	int *sub=new int[SIZE];
	int num=0;
	sub[0] = num;
	for (int i = 0; i < n; i++) {
		if (prod[i][0].at(0)==nvt) {
			sub[++num] = i;;
			sub[0] = num;
		}
	}
	return sub;//�����ܳ����������
}

int getSubFromAttr(char ch,NVTattrs nvtattrs) {
	for(int i=0;i<nvtattrs.length;i++){
		if (nvtattrs.nvtattr[i].NVT == ch) {
			return i;
		}
	}
	return -1;//�����ܳ����������
}

/*��Ҫһ�����ϸ��ƺ�������ǰһ��first�����������׷�ӵ���һ��first������*/
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

/*ȥ����ǰ�ַ���first�����е�epsilon*/
string cutOffEpsilon(char ch,NVTattrs nvtattrs) {
	string first;
	
	int sub = getSubFromAttr(ch, nvtattrs);
	first = nvtattrs.nvtattr[sub].first;
	for (int i = 0; i < nvtattrs.nvtattr[sub].firstSize; i++) {
		if (first.at(i) == '^') {
			string temp1 = first.substr(0, i);
			string temp2 = first.substr(i + 1, first.length());
			first = temp1 + temp2;
			break;//���ֻ��һ��epsilon
		}
	}
	return first;
}

void First(string str, NVTattrs &nvtattrs,int sub,string prod[][2],int n,bool done[]) {
	//Ҫ��������ķ��ս�������в���ʽ���б���
	
	if (!(str.at(0) >= 'A'&& str.at(0) <= 'Z')) {
		nvtattrs.nvtattr[sub].first[nvtattrs.nvtattr[sub].firstSize] = str.at(0);
		nvtattrs.nvtattr[sub].firstSize++;
	}
	else {
		int *sub1 = getSub(str.at(0), prod,n);//��Ӧ��ֻ�Ǽ򵥵صõ�һ�������±꣬Ӧ�õõ�����ͬ�����������±�
		for (int i = 0; i < sub1[0]; i++) {
			int sub2 = getSubFromAttr(prod[sub1[i+1]][1].at(0), nvtattrs);
			if (sub2!=-1&&done[sub2]) {//����ȴ�����ķ��ս����first�����Ѿ������ˣ���ô��ֱ�����������㣬������еݹ����
				//�����ǰ�ַ����Ƴ�epsilon
				if (getEpsilon(prod[sub1[i + 1]][1].at(0), prod, n)) {
					string str = cutOffEpsilon(prod[sub1[i + 1]][1].at(0),nvtattrs);

					//������ȥ����ǰ�ַ���first�����е�epsilon
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
				if (sub3!=-1&&done[sub3]) {//����Ҳ���һ���ַ�Ϊ���ս������Ϊ��Ҫ�ȴ���״̬,��������ɵݹ�����´����Ҳ�
					First(str, nvtattrs, sub, prod, n, done);
				}
			}
		}
		done[sub] = true;//��ʾ�÷��ս����first�Ѿ�������
	}
}

void getFisrt(NVTattrs &nvtattrs,string prod[][2],int n) {//ͨ��һ���ַ�������������first����,�����ֿ��ܣ�һ�����ַ�Ϊ�ս�����������ַ�Ϊ���ս�������ķ�����Ϊ�����Ҳ���
	string str="";
	bool done[NVTsize] = {false};

	for (int i = 0; i <nvtattrs.length; i++) {//�����еķ��ս����first����
		str = "";
		str += nvtattrs.nvtattr[i].NVT;
		int sub = getSubFromAttr(str.at(0), nvtattrs);
		if (sub != -1 && !done[sub]) {
			First(str, nvtattrs,i,prod,n,done);
		}
	}
}

void FirstFollow(string prod[][2],int n) {//����first��follow����
	NVTattrs nvtattrs;
	nvtattrs.length = 0;
	for (int i = 0; i < NVTsize; i++) {
		nvtattrs.nvtattr[i].getNull = false;
		nvtattrs.nvtattr[i].firstSize = 0;
		nvtattrs.nvtattr[i].followSize = 0;
		for (int j = 0; j < VTsize; j++) {
			nvtattrs.nvtattr[i].first[j]='`';//���ϳ�ʼ��Ϊ��`��
			nvtattrs.nvtattr[i].follow [j]= '`';
		}
	}//�ṹ���ʼ��


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
//�滻���е��ַ�����û��ʵ��
//�Ƚ����е��ķ��任��û��|���ķ�
//��һ���ķ�������ж���ݹ����ʽʱ��������ݹ��д���
//�����������һ����ݹ��ˣ��ٶ����һ�¡�
//���ڸĽ��ж��ܷ�*�Ƶ����յĺ���
//*�Ƶ���������ȷ��,���ڸĽ�first����