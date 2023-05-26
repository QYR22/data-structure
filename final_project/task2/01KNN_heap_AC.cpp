#include <iostream>
#include <string.h>
#include <queue>
#include <fstream>
#include <time.h>
#include <windows.h>
#include <vector>
using namespace std;

struct record
{
    long long dis;
    int index;

    record();
    void operator=(const record &copy)
    {
        dis = copy.dis; index = copy.index;
    }
    bool operator==(record &cmp)
    {
        return dis==cmp.dis;
    }
    bool operator>(record &cmp)
    {
        return dis>cmp.dis;
    }
    bool operator<(record &cmp)
    {
        return dis<cmp.dis;
    }
    bool operator!=(record &cmp)
    {
        return dis!=cmp.dis;
    }
    bool operator>=(record &cmp)
    {
        return dis>=cmp.dis;
    }
    bool operator<=(record &cmp)
    {
        return dis<=cmp.dis;
    }
};
record::record()
{
    dis = -1; index= -1;
}

class slist
{
public:
    void heapsort();

    void insert_heap( record &current, int low, int high);
    void build_heap();
//protected:
    vector<record> entry;
    int count;

    void print()const
    {
        for(int i=0; i<count; ++i){
            cout<<entry[i].dis<<"  ";
        }
        cout<<endl;
    }
};

void slist::insert_heap( record &current, int low, int high)
{
    int large = 2*low+1;

    while(large <= high){
        if(large<high && entry[large]<entry[large+1])
            large++;
        if(current >= entry[large])
            break;
        else{
            entry[low] = entry[large];
            low = large;
            large = 2*low+1;
        }
    }
    entry[low] = current;
}

void slist::build_heap()
{
    int low;
    //最后一个头
    for(low=count/2-1; low>=0; --low){
        record current = entry[low];
        insert_heap(current,low,count-1);
    }
}

void slist::heapsort()
{
    record current;
    int last_unsorted;
    build_heap();

    for(last_unsorted=count-1; last_unsorted>0; --last_unsorted){
        current = entry[last_unsorted];
        entry[last_unsorted] = entry[0];
        insert_heap(current, 0, last_unsorted-1);
//不用清空头头entry[0] 自然会在insert_heap函数调用之后被挤掉的
    }
}

struct point
{
    long long x,y;
};

int main()
{
    int sum;    cin>>sum;

    slist myheap[sum];
    point points[sum+1];

    //输入 计算两点之间的距离
    for(int i=0; i<sum; ++i){
        double tmp;     cin>>tmp;
        points[i].x = 100*tmp;
        cin>>tmp;
        points[i].y = 100*tmp;
        //printf("point i 'x=%ld  point i'y= %ld    ",points[i].x,points[i].y);
    }

    for(int i=0; i<sum; ++i){
        for(int j=i+1; j<sum; ++j){
            record ij,ji;
            ij.index = j;   ji.index = i;
            long long tmp1 = points[i].x-points[j].x;
            long long tmp2=points[i].y-points[j].y;
            ij.dis = tmp1*tmp1 + tmp2*tmp2;
            ji.dis = ij.dis;

            myheap[i].entry.push_back(ij);
            myheap[j].entry.push_back(ji);
        }
        myheap[i].count = sum-1;
        myheap[i].heapsort();
        //myheap[i].print();
    }

    vector<vector<bool> > edges(sum+1, vector<bool>(sum+1));

    for(int k=1; k<sum; ++k){
        int cnt = 1;
        bool visited[sum+1]={};
        //memset(visited,false,sizeof(visited));
        queue<int> tmp_graph;
        tmp_graph.push(0);
        visited[0] = true;

        //给邻接矩阵增加边 更新邻接矩阵
        //k-1的图 一定是k的图的子图
        for(int i=0; i<sum; ++i){
            record tmp = myheap[i].entry[k-1];
            if( !edges[i][tmp.index] ){
                record t2 = myheap[tmp.index].entry[k-1];
                if(tmp.dis <= t2.dis){
                    edges[tmp.index][i] = true;
                    edges[i][tmp.index] = true;
                }
            }
        }

        while(!tmp_graph.empty()){
            int pnow = tmp_graph.front();
            //cout<<pnow<<"  pnow"<<endl;
            tmp_graph.pop();

            //pnow的近邻点入队
            for(int i=0; i<sum; ++i){
                if(edges[pnow][i] && !visited[i]){

                    cnt++;
                    if(cnt == sum){
                        cout<<k<<endl;
                        return 0;
                    }
                    tmp_graph.push(i);
                    visited[i] = true;
                    //cout<<"i="<<i<<endl;
                }
            }
        }
    }
    return 0;
}