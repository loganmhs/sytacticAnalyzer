#pragma once
#include<string>
using namespace std;//���ļ��еġ�δ֪��д�����������󣬼���һ�������ռ��ͺ���

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
	bool doneFirst;
	bool doneFollow;
};

struct NVTattrs {
	int length;//���ս���ĸ���
	NVTattr nvtattr[NVTsize];
};

/*��¼Ԥ�������ĸ߶ȺͿ��*/
struct Table{
	string tbl[NVTsize][VTsize];
	int height;
	int width;
};