#pragma once
#include<string>
using namespace std;//该文件中的“未知重写声明符”错误，加了一个命名空间后就好了

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
	bool doneFirst;
	bool doneFollow;
};

struct NVTattrs {
	int length;//非终结符的个数
	NVTattr nvtattr[NVTsize];
};

/*记录预测分析表的高度和宽度*/
struct Table{
	string tbl[NVTsize][VTsize];
	int height;
	int width;
};