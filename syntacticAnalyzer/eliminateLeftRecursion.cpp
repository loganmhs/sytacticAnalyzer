#include"stdafx.h"
#include"declaration.h"
#include"eliminateLeftRecursion.h"
#include"arrayFunction.h"
#include<iostream>
using namespace std;
/*������ʽ�ֳ�����������*/
void splitLeft_Right(string gram[SIZE], int n, string(&prod)[SIZE][2]) {
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
				begin = end + 1;
			}
		}
		end =(int) prod[i][1].length();
		p[k][0] = prod[i][0];
		p[k++][1] = prod[i][1].substr(begin, end - begin);
		begin = 0;
	}

	n = k;
	for (int i = 0; i < k; i++) {
		prod[i][0] = p[i][0];
		prod[i][1] = p[i][1];
	}
}

/*����һ����ݹ�*/
void eliminateLeftRecursion(string(&prod)[SIZE][2], int &n) {
	string lastNVT = prod[n - 1][0];//���һ�����ս���������滻
	int place = 0;//��¼��ǰ�����滻���ַ��ڴ��е�λ��
	string leftNVT;//����ʽ�Ҳ�����ߵķ��ս��
	string leftNVT1;
	int rightPartNum = 0;//��ǰ��Ҫ����Ĳ���ʽ���Ҳ��ĸ���
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
							if (prod[i + k][1].at(0) != '^')
								item[1].replace(0, 1, prod[i + k][1]);
							else
								item[1].replace(0, 1, "");
							insertBehind(prod, n, j + k, item);
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
				prod[n][1] += ('Z' - recursionNum);//��Z����չ�ķ��ս��
				n++;
			}
			prod[n][0] = ('Z' - recursionNum);
			prod[n][1] = prod[i][1].substr(1, prod[i][1].length() - 1);
			prod[n][1] += ('Z' - recursionNum);
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
			for (int m = 0; m <rightPartNum + 1; m++) {//ɾ���м������Ԫ��
				deleteItem(prod, n, r);
			}
			recursionNum++;
			i--;
		}
	}

}
