#pragma once
#include"declaration.h"
#include<string>
using namespace std;//��ʹ��ͷ�ļ��У�Ҳ��Ҫ��һ�仰

/*���룺һ�����ս���������ķ����ķ��������������д���ս�����е����������getNull��Ϣ*/
void getEpsilon(NVTattrs &nvtattrs, string prod[SIZE][2], int n);

void initNVTtable(NVTattrs &nvtattrs, string prod[][2], int n,char start);

void getFisrtAssemble(NVTattrs &nvtattrs, string prod[][2], int n);

void getFollowAssemble(NVTattrs &nvtattrs, string prod[][2], int n);

void getSelectAssemble(NVTattrs nvtattrs, string prod[][2], int n, string(&select)[SIZE][3]);