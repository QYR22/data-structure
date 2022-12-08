#include <iostream>
using namespace std;

enum Error_code{underflow,success,overflow,illegal,range_Error};

const int max_list=100;

typedef int List_entry;
typedef int Node_entry;

struct node
{
    Node_entry entry;
    node *back,*next;

    node();
    node(Node_entry item, node* link_back=NULL,node *link_next=NULL);
};
node::node()
{
    entry=0;
    back=next=NULL;
}
node::node(Node_entry item, node* link_back,node *link_next)
{
    entry=item;
    back=link_back;
    next=link_next;
}

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

    node *get_current();
    Error_code insert(int position,const List_entry&x);
    Error_code remove(int position,List_entry&x);
    Error_code retrieve(int position,List_entry&x)const;
    Error_code replace(int position,List_entry&x);

    void traverse(void(*visit)(List_entry &x));
protected:
    int count;
//还是加一个head吧，虽然没有也行，但是加了写起来..真顺!
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
    List_entry x;
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
    List_entry x;
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

node* double_list::get_current()
{
    return current;
}
/* insert */
Error_code double_list::insert(int position,const List_entry&x)
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

Error_code double_list::remove(int position,List_entry&x)
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
Error_code double_list::retrieve(int position,List_entry&x)const
{
    if (position < 0 || position >= count) return range_Error;
    set_position(position);
    x = current->entry;
    return success;
}
Error_code double_list::replace(int position,List_entry&x)
{
    if (position < 0 || position >= count) return range_Error;

    set_position(position);
    current->entry = x;
    return success;
}

void double_list::traverse(void(*visit)(List_entry &x))
{
    node* newptr = head;
    while (newptr) {
        (*visit)(newptr->entry);
        newptr = newptr->next;
    }
}
int main()
{
    double_list mylist;
    int tmp,q=0;
    cin>>tmp;
    while(tmp+1){
        mylist.insert(q++,tmp);
        cin>>tmp;
    }
//前向
    cout<<"0"<<" ";
    for(int i=1; i<q; ++i){
        mylist.retrieve(i,tmp);
        bool get=false;
        for(int t=i-1; t>=0; --t){
            int pre_min;
            mylist.retrieve(t,pre_min);
            if(pre_min<tmp){
                printf("%d ",pre_min);
                get=true;
                break;
            }
        }
        if(get==false){
            printf("0 ");
        }
    }
    cout<<endl;
    for(int i=0; i<q-1; ++i){
        mylist.retrieve(i,tmp);
        bool get=false;
        for(int t=i+1; t<q; ++t){
            int next_max;
            mylist.retrieve(t,next_max);
            if(next_max>tmp){
                printf("%d ",next_max);
                get=true;
                break;
            }
        }
        if(get==false){
            printf("0 ");
        }
    }
    printf("0\n");
    return 0;
}
