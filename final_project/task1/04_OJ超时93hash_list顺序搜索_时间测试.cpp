#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
#include <cstdio>

using namespace std;

/** 链式哈希表实现：
 *
 *  前两个字母作为索引table[26*26=676]都是大写字母AA=0 AB=1...ZZ=675
 *  前两个字母一样的即冲突，冲突后往后直接插入
 *
 *
 */
struct record
{
    string name,addr1,addr2;
    int index;
    record();
    record(string s1,string s2,string s3);
};
record::record()
{

}
record::record(string s1,string s2,string s3)
{
    name=s1;addr1=s2;addr2=s3;
}

struct node
{
    record entry;
    node *back,*next;
    node();
    node(record x,node *pre=NULL,node *add=NULL);
};
node::node()
{
    back=next=NULL;
}
node::node(record x,node *pre,node *add)
{
    entry=x;back=pre;next = add;
}
enum Error_code{underflow,success,overflow,illegal,range_Error};


typedef node List_entry;

//doubly list
class double_list
{
public:
    double_list();
    ~double_list();
    double_list(const double_list &copy);

    void operator=(const double_list &copy);

    int size()const;
    bool full()const;
    bool empty()const;
    void clear();

    Error_code insert(int position,const record&x);
    Error_code remove(int position,record&x);
    Error_code retrieve(int position,record&x)const;
    Error_code replace(int position,record&x);

    void traverse(void(*visit)(record &x));
protected:
    int count;
    node *head;
    mutable int current_position;
    mutable node* current;
    void set_position(int position)const;
};
/**/
/*set_position*/
void double_list::set_position(int position)const
{//current在前面
    if(current_position<=position)
        for(; current_position!=position; ++current_position)
            current=current->next;

//current在后面
    else
        for(; current_position!=position; --current_position)
            current=current->back;

}
/*constructors*/
double_list::double_list()
{
    count=0;current_position=-1;
    head=current=NULL;
}
double_list::~double_list()
{
    record x;
    while(!empty())
        remove(0,x);
}
double_list::double_list(const double_list &copy)
{
    count=0;
    head=NULL;
    node *new_node=copy.head;
    while(new_node){
        insert(size(),new_node->entry);
        new_node=new_node->next;
    }
}

void double_list::operator=(const double_list &copy)
{
    record x;
//if-->x=x  ↓&是取地址符号 x=y this 是指针，指向main函数等号左边的x
    if(this == &copy)
        return;
//先清空
    while(!empty())
        remove(0,x);
//再复制
    node *new_node=copy.head;
    while(new_node){
        insert(size(),new_node->entry);
        new_node=new_node->next;
    }
}

int double_list::size()const
{
    return count;
}
bool double_list::full()const
{
    return false;
/*//当然这是不严谨的，要是要严谨一点，要看堆满了没
    node *new_node = new node();
    if(new_node==NULL)
        return true;
//!!delete it!!! don't forget
    delete new_node;
    return false;
*/
}
bool double_list::empty()const
{
    return count==0;
}
void double_list::clear()
{
    node *p,*q;

    if(current==NULL)
        return;
    for(p=current; p; p=q){
        q=p->next;
        delete p;
    }
    for(p=current; p; p=q){
        q=p->back;
        delete p;
    }
    count=0;
    current=NULL;
    current_position=-1;

    return;
}

/* insert */
Error_code double_list::insert(int position,const record&x)
{
    if(position<0 || position>count) return range_Error;

    node *new_node,*previous,*following;
//在链表头插入
    if(position==0){
        if(count==0)
            following=NULL;
        else{
            set_position(0);
            following=current;
        }
        previous=NULL;
    }
//一般情况(包括链表尾，链表中间)
    else{
        set_position(position-1);
        previous=current;
        following=current->next;
    }

    new_node = new node(x,previous,following);
    if(new_node==NULL) return overflow;
//检查头和尾
//因为现在只给newnode的back和next赋值 previous和following的next和back还没有改正
    if(previous!=NULL)
        previous->next=new_node;
    if(following!=NULL)
        following->back=new_node;
//调整 current,count,current_position
    current=new_node;
    current_position=position;
    count++;
//!!don't forget my HEAD!orz
    if(position==0)
        head=new_node;
    return success;
}

Error_code double_list::remove(int position,record&x)
{
    if(count==0 || position<0 || position>=count) return illegal;

    node *old_node,*neighbor;

    set_position(position);
    old_node=current;
/*!!理解!!
这里是括号里面先赋值，再判断neighbor是不是NULL
node 1    old node(AIM)     node 2*/
/*先处理目标的前一个node1->next指向old_node->next
 如果cur是队首就不作操作*/
    if(neighbor=current->back)
        neighbor->next=current->next;
/*再处理node2-<back指向node1
如果current是队尾也不做操作*/
    if(neighbor=current->next){
        neighbor->back=current->back;
        current=neighbor;
    }
//old_node=current;
    else{
        current=current->back;
        current_position--;
    }

    x=old_node->entry;
    delete old_node;
    count--;

    return success;
}
Error_code double_list::retrieve(int position,record&x)const
{
    if (position < 0 || position >= count) return range_Error;
    set_position(position);
    x = current->entry;
    return success;
}
Error_code double_list::replace(int position,record&x)
{
    if (position < 0 || position >= count) return range_Error;

    set_position(position);
    current->entry = x;
    return success;
}
void double_list::traverse(void(*visit)(record &x))
{
    node* newptr = head;
    while (newptr) {
        (*visit)(newptr->entry);
        newptr = newptr->next;
    }
}
double_list table[17580];

//计算每一个地点的index(根据前两个字母)
int string2num(string str)
{
    int n1=str[0],n2=str[1],n3=str[3];
    int num = n2-'A'+(n1-'A')*26+(n3-'A')*26*26;
    return num;
}

void insert(record &data)
{
    table[data.index].insert(0,data);
}
void retrieve(const string &target)
{
    int inx=string2num(target);

    for(int i=0; i<table[inx].size(); ++i){
        record tmp;
        table[inx].retrieve(i,tmp);
        if(tmp.name == target){
            //cout<<tmp.addr1<<' '<<tmp.addr2<<endl;
            return;
        }
    }
    cout<<"NULL"<<endl;
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

    while(sum--){
        record data;
        string tmp;
        fin>>data.name>>data.addr1>>data.addr2>>tmp;
        data.index = string2num(data.name);
        insert(data);
    }

    while(qsum--){
        string target;
        fin>>target;
        retrieve(target);
    }
    QueryPerformanceCounter(&nEnd);
    //获得结束的时刻
    double time = (nEnd.QuadPart - nBegin.QuadPart)/(double)Frequ.QuadPart;

    printf("total time(s):%lf s\n",time);
    //printf("total time(ms):%lf ms\n",time*1000);

    return 0;
}