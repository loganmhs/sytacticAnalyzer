#include"stdafx.h"
#include<iostream>
#include"declaration.h"
#include<string>
#include"firstFollow.h"
using namespace std;

/*���룺һ���ַ������ս������������ظ��ַ��ڷ��ս�����е��к�*/
int getSubFromNVT(char ch,NVTattrs nvtattrs) {
	for (int i = 0; i < nvtattrs.length; i++) {
		if (nvtattrs.nvtattr[i].NVT == ch) {
			return i;
		}
	}
	return -1;//�ҵ������кţ�δ�ҵ����س���ֵ-1
}

/*���룺һ�����ս���������ķ����ķ��������������д���ս�����е����������getNull��Ϣ*/
void getEpsilon(NVTattrs &nvtattrs, string prod[SIZE][2], int n) {
	for (int j = 0; j < n; j++) {
		if (prod[j][1] == "^") {
			char ch= prod[j][0].at(0);
			int sub = getSubFromNVT(ch, nvtattrs);
			nvtattrs.nvtattr[sub].getNull = true;
		}
	}
}

/*���룺һ�����ս����һ���ķ����顢�ķ��������ķ��Ŀ�ʼ�����������ʼ���÷��ս����*/
void initNVTtable(NVTattrs &nvtattrs, string prod[][2], int n,char start) {
	char pre = ' ';//��ʾ��һ�в���ʽ����
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

/*���룺һ���ַ��������ķ����ķ�����������������йظ��ַ������в���ʽ���±�����*/
int * getSubArrayFromProds(char ch,string prod[][2],int n) {
	int *sub= new int[SIZE];//0�ŵ�Ԫ�����������ĳ��ȣ���1�ŵ�Ԫ��ʼ�������
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

/*���룺���ս�����ϣ����ս���кţ�׷�ӵ�Ԫ�أ��������д��Ӧ��first���ϣ�����ȥ��*/
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

/*���룺���ս�����ϣ���׷�ӷ��ս���кţ�Դ���ս���кţ��������д��Ӧ��first���ϣ�����ȥ�غ�ȥ��*/
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

/*���룺һ���ַ������ս�����ϣ������ķ�������ʽ�������������д��Ӧ�ķ��ս������first����*/
void first(char ch, NVTattrs &nvtattrs, string prod[][2], int n) {
	int subN = getSubFromNVT(ch, nvtattrs);
	int *subP = getSubArrayFromProds(ch, prod, n);
	int subPnow = 0;
	char right;

	for (int i = 0; i < subP[0]; i++) {
		subPnow = subP[i + 1];
		right = prod[subPnow][1].at(0);
		if (!(right >= 'A'&&right <= 'Z')) {
			//nvtattrs.nvtattr[subN].first[len]=//��Ҫһ����������׷��Ԫ�صĺ���������ȥ���ظ����Ҫ��������������ʵ�֣�׷��һ��Ԫ�غ�׷��һ������
			firstAppend(nvtattrs, subN, right);//��һ������Ѿ������
		}
		else {

			int subRight = getSubFromNVT(right, nvtattrs);//����ʽ�Ҳ����ַ��ڷ��ս�����е��±�
			if (nvtattrs.nvtattr[subRight].doneFirst) {
				//ȥ��epsilon��׷�ӵ���ǰ�ַ���first������
				//д׷�Ӻ�����Ҫȥ��epsilon
				int subNres = getSubFromNVT(right,nvtattrs);//Դfirst���ϵ��ַ����±�
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
				subLast = subNext;//��һ��ѭ��ʱʹ��
				next++;
			}
			if (subNext == -1&&nvtattrs.nvtattr[subLast].getNull) {//�����һλ���ս��
				firstAppend(nvtattrs, subN, nextChar);
			}
		
		}
	}
	nvtattrs.nvtattr[subN].doneFirst = true;
	delete[]subP;
}

/*���룺���ս�����ϡ������ķ�������ʽ�������������д��Ӧ�ķ��ս����first����*/
void getFisrtAssemble(NVTattrs &nvtattrs,string prod[][2],int n) {
	for (int i = 0; i < nvtattrs.length; i++) {
		//��Ҫһ������������һ������ʽ���󲿣��������������first���ϡ�������ݹ����first���ϲ�������ѭ�������,����Ҫ������������ܽ��еݹ����
		first(nvtattrs.nvtattr[i].NVT, nvtattrs, prod, n);//ֻ�ж�Сд��ĸ�Ĵ���
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
/************************************����Ϊ��first����*******************************************************/

/************************************����Ϊ��follow����*******************************************************/
/*���룺һ���ַ��������ķ�������ʽ����������������Ҳ����и��ַ��Ĳ���ʽ���±�����*/
int ** getSubArrayFromProdsRight(char ch, string prod[][2], int n) {
	int **sub = new int*[SIZE];//��һ�б�ʾ����ʽ���±꣬�ڶ��б�ʾ�ò���ʽ�Ҳ��и��ַ����ַ����е��±�
	for (int i = 0; i < SIZE; i++) {
		sub[i] = new int[2];
	}
	sub[0][0] = 0;//���ص��±����
	sub[0][1] = 0;//����ʾ�κ����壬�����򵥵ĳ�ʼ��
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

/*���룺Ŀ���кţ�Դ�кţ����ս�����ϣ��������д��Ӧ��follow����*/
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

/*���룺Ŀ���кţ�Դ�ַ������ս����ϣ��������д��Ӧ��follow����*/
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

/*���룺Ŀ���кţ�Դ�кţ����ս������*/
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

/*���룺һ���ַ������ս�����ϡ������ķ�������ʽ�������õݹ麯�������õĴ������������д��Ӧ�ķ��ս�����ϵ�follow����*/
void follow(char ch, NVTattrs &nvtattrs, string prod[][2], int n,int &count) {
	int **subP = getSubArrayFromProdsRight(ch, prod, n);
	int subNVT = getSubFromNVT(ch, nvtattrs);//���ַ��ڷ��ս������±�
	int back=0;//��¼follow�������˳�0�����Ǵ���ѭ��������-1

	count++;
	if (count > nvtattrs.length) {//��������****************************************************************************
		//nvtattrs.nvtattr[subNVT].doneFollow = true;
		count = 0;
		return;//�쳣����
	}

	for (int i = 0; i < subP[0][0]; i++) {
		int subNow = subP[i + 1][0];//��ǰ����ʽ���±�
		int subStr = subP[i + 1][1];//���ַ����ַ����е��±�
		int subLeft = getSubFromNVT(prod[subNow][0].at(0), nvtattrs);//����ʽ���ַ��ڷ��ս������±�

		//ÿһ�εݹ����follow����ʱ����Ӧ���жϸ÷��ս����follow�����Ƿ������
		if (subStr == prod[subNow][1].length() - 1) {//�����ǰ�ַ�����û�������ַ���
			if (!nvtattrs.nvtattr[subLeft].doneFollow)
			{
				if(ch!=prod[subNow][0].at(0))
					follow(prod[subNow][0].at(0), nvtattrs, prod, n,count);/*******************************�Լ��ȴ��Լ���follow���ϣ��е�����*******************************/
			}
			else {
				appendFfromF(nvtattrs, subNVT, subLeft);//�Ѳ���ʽ���ַ���follow���ϼӽ���
			}
		}
		else {//���������ַ�
			char next = prod[subNow][1].at(subStr + 1);
			if (!(next >= 'A'&&next <= 'Z')) {
				if(next!='^')
					appendFfromChar(nvtattrs, subNVT, next);//��Ҫһ��������ʵ�ּ�һ���ַ�����follow���ϣ����ǵ�ȥ��
			}
			else {//��һ���ַ��Ƿ��ս��
				int j = subStr+1;
				int subNext;

				do {
					next = prod[subNow][1].at(j++);
					subNext = getSubFromNVT(next, nvtattrs);
					if (subNext == -1)break;
					appendFfromFirst(nvtattrs, subNVT, subNext);
				} while (nvtattrs.nvtattr[subNext].getNull && j != prod[subNow][1].length());

				if (subNext == -1) {//�����һ�ַ����ս��
					appendFfromChar(nvtattrs, subNVT, next);
				}
				else if (j == prod[subNow][1].length()&&nvtattrs.nvtattr[subNext].getNull) {//����ʽ�Ҳ����з��ս�����ζ����Ƴ�epsilon�����ԵðѲ���ʽ�󲿵�follow����׷�ӽ���
					if (nvtattrs.nvtattr[subLeft].doneFollow) {//�������ʽ�󲿵�follow�����Ѿ������ˣ���ֱ�Ӽ�������follow����
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

/*���룺���ս�����ϡ������ķ�������ʽ�������������д��Ӧ�ķ��ս����follow����*/
void getFollowAssemble(NVTattrs &nvtattrs, string prod[][2], int n) {
	int count = 0;
	for (int i = 0; i < nvtattrs.length; i++) {
		if(!nvtattrs.nvtattr[i].doneFollow)
			follow(nvtattrs.nvtattr[i].NVT,nvtattrs,prod, n,count);
	}

	//�����������ѭ�����������������ô��һ�����follow���ϾͲ����ƣ���Ҫ�ڶ�����follow�����������������
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

/*���룺�ַ��������ս�����ϣ����������һ���ܷ�õ�epsilon��boolֵ*/
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
		else {//����Ƿ��ս��
			char ch = right.at(i);
			int subNVT = getSubFromNVT(ch, nvtattrs);
			if (!nvtattrs.nvtattr[subNVT].getNull) {
				return false;
			}
		}
	}
	return true;
}

/*���룺���ս�����ϣ����ս�������±꣬select���ϣ�select�����±꣬flag��0Ϊ��first�����󲢼���1Ϊ��follow�����󲢼����������дselect����*/
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

/*���룺���ռ������ϣ�select���ϣ�select�����±ꣻ�������д��Ӧ��select����*/
void firstStr(NVTattrs nvtattrs,string (&select)[SIZE][3],int i) {
	unsigned int j = 0;
	while (j < select[i][1].length()) {
		int res = getSubFromNVT(select[i][1].at(j), nvtattrs);
		if (res == -1) {//������ַ�Ϊ�ս��
			if(select[i][1].at(j)!='^')
				select[i][2] += select[i][1].at(j);
			break;
		}
		SetUnion(nvtattrs, res, select, i,0);
		if (nvtattrs.nvtattr[res].getNull) {//�����ǰ���ս�����Ƴ�epsilon
			j++;
		}
		else {//�����ǰ���ս�������Ƴ�epsilon
			break;
		}
	}
}

/*���룺���ս�����ϡ������ķ�������ʽ������select���ϣ��������д��Ӧ��select����*/
void getSelectAssemble(NVTattrs nvtattrs, string prod[][2], int n, string (&select)[SIZE][3]) {
	for (int i = 0; i < n; i++) {
		select[i][0] += prod[i][0];// ����ʽ��
		select[i][1] += prod[i][1];//����ʽ�Ҳ�
		char left = prod[i][0].at(0);
		int subLeft = getSubFromNVT(left, nvtattrs);
		//��Ҫһ��������ͨ��һ���ַ����ж����ܲ���*�Ƶ���epsilon
		if (!getEpsilon(select[i][1], nvtattrs)) {//����ò���ʽ�Ҳ������Ƴ�epsilon
			//select���ϵ���first��select[i][1])
			//��Ҫʵ��һ�������ཻ�������ӷ��ս�����ϵ�first�������ó������뵽select���ϵĵ�����
			firstStr(nvtattrs, select, i);
		}
		else {//����ò���ʽ�Ҳ����Ƴ���
			//select���ϵ���first��select[i][1]��-��epsilon������follow��select[i][0]��;		
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

