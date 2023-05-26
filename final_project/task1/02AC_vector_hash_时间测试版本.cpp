#include <iostream>
#include <string.h>
#include <fstream>
#include <time.h>
#include <cstdio>
#include <vector>
#include <windows.h>
using namespace std;

/** 链式哈希表实现：
 *
 *  前两个字母作为索引table[26*26=676]都是大写字母AA=0 AB=1...ZZ=675
 *  前两个字母一样的即冲突，冲突后往后直接插入
 *
 *   26^3=17576  676
 */
struct record
{
    char name[13];
    int addr1,addr2;
    bool operator<(const record &cmp)
    {
        return strcmp(name,cmp.name)<0;
    }
    bool operator>(const record &cmp)
    {
        return strcmp(name,cmp.name)>0;
    }
    bool operator==(const record &cmp)
    {
        return strcmp(name,cmp.name)==0;
    }
};

int s_getnum(char *str)
{
    int n1 = str[0]-'A',n2 = str[1]-'A';
    return n2+n1*26;
}

int l_getnum(char *str)
{
    int n1 = str[0]-'A',n2 = str[1]-'A',n3 = str[2]-'A';
    return n3+n2*26+n1*26*26;
}
//16900+650+25=17575
void insert(vector<record > &chart,record data)
{
    if(chart.size() == 0){
        chart.push_back(data);
        return;
    }

    if(data< chart[0]){
        chart.insert(chart.begin(),data);
        return;
    }

    if(data > chart.back()){
        chart.push_back(data);
        return;
    }

    vector<record>::iterator ite=chart.begin();
    vector<record>::iterator temp;

    for(; ite != chart.end(); ++ite){
        temp = (ite+1);
        if(*ite<data && data < *temp){
            chart.insert(ite+1,data);
            return;
        }
    }
}
char target[13];
void recursive_search(vector<record > &chart,int bottom,int top)
{
    if(bottom<=top){
        int mid = (bottom+top)/2;

        if(strcmp(target,chart[mid].name) == 0){
            //printf("%d %d\n",chart[mid].addr1,chart[mid].addr2);
            //cout<<chart[mid].addr1<<' '<<chart[mid].addr2<<endl;
            return;
        }
        if(strcmp(target,chart[mid].name)>0)
            return recursive_search(chart,mid+1,top);
        else
            return recursive_search(chart,bottom,mid-1);
    }
    else
        printf("NULL\n");
}

int main()
{
    LARGE_INTEGER Frequ;
    LARGE_INTEGER nBegin;
    LARGE_INTEGER nEnd;

    ifstream fin("rand_800000_10.txt");
        int sum,qsum;   fin>>sum>>qsum;

    QueryPerformanceFrequency(&Frequ);
    QueryPerformanceCounter(&nBegin);//获取开始的时刻
    vector<vector<record >> table(17580);
    while(sum--){
        record new_one;
        fin>>new_one.name>>new_one.addr1>>new_one.addr2>>target;
        int pos=l_getnum(new_one.name);
        insert(table[pos],new_one);
    }
    while(qsum--){
        fin>>target;
        int pos = l_getnum(target);
        recursive_search(table[pos],0,table[pos].size());
    }
    QueryPerformanceCounter(&nEnd);
    //获得结束的时刻
    double time = (nEnd.QuadPart - nBegin.QuadPart)/(double)Frequ.QuadPart;

    printf("total time(s):%lf s\n",time);
    //printf("total time(ms):%lf ms\n",time*1000);

    return 0;
}
