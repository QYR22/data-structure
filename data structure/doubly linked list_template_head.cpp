#include <iostream>
#include <utility>
using namespace std;

enum Error_code{underflow,success,overflow,illegal,range_Error};

const int max_list=100;

template <class Node_entry>
struct node
{
    Node_entry entry;
    node<Node_entry> *back,*next;

    node();
    node(Node_entry item, node<Node_entry>* link_back=NULL,node<Node_entry> *link_next=NULL);
};
template <class Node_entry>
node<Node_entry>::node()
{
    back=next=NULL;
}
template <class Node_entry>
node<Node_entry>::node(Node_entry item, node<Node_entry>* link_back,node<Node_entry> *link_next)
{
    entry=item;
    back=link_back;
    next=link_next;
}

//doubly list
template <class List_entry>
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

    Error_code insert(int position,const List_entry&x);
    Error_code remove(int position,List_entry&x);
    Error_code retrieve(int position,List_entry&x)const;
    Error_code replace(int position,List_entry&x);

    void traverse(void(*visit)(List_entry &x));
protected:
    int count;
//还是加一个head吧，虽然没有也行，但是加了写起来..真顺!
    node<List_entry> *head;
    mutable int current_position;
    mutable node<List_entry>* current;
    void set_position(int position)const;
};
/**/
/*set_position*/
template <class List_entry>
void double_list<List_entry>::set_position(int position)const
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
template <class List_entry>
double_list<List_entry>::double_list()
{
    count=0;current_position=-1;
    head=current=NULL;
}

template <class List_entry>
double_list<List_entry>::~double_list()
{
    List_entry x;
    while(!empty())
        remove(0,x);
}

template <class List_entry>
double_list<List_entry>::double_list(const double_list &copy)
{
    count=0;
    head=NULL;
    node<List_entry> *new_node=copy.head;
    while(new_node){
        insert(size(),new_node->entry);
        new_node=new_node->next;
    }
}

template <class List_entry>
void double_list<List_entry>::operator=(const double_list &copy)
{
    List_entry x;
//if-->x=x  ↓&是取地址符号 x=y this 是指针，指向main函数等号左边的x
    if(this == &copy)
        return;
//先清空
    while(!empty())
        remove(0,x);
//再复制
    node<List_entry> *new_node=copy.head;
    while(new_node){
        insert(size(),new_node->entry);
        new_node=new_node->next;
    }
}

template <class List_entry>
int double_list<List_entry>::size()const
{
    return count;
}
template <class List_entry>
bool double_list<List_entry>::full()const
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

template <class List_entry>
bool double_list<List_entry>::empty()const
{
    return count==0;
}

template <class List_entry>
void double_list<List_entry>::clear()
{
    node<List_entry> *p,*q;

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
template <class List_entry>
Error_code double_list<List_entry>::insert(int position,const List_entry&x)
{
    if(position<0 || position>count) return range_Error;

    node<List_entry> *new_node,*previous,*following;
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

    new_node = new node<List_entry>(x,previous,following);
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

template <class List_entry>
Error_code double_list<List_entry>::remove(int position,List_entry&x)
{
    if(count==0 || position<0 || position>=count) return illegal;

    node<List_entry> *old_node,*neighbor;

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

template <class List_entry>
Error_code double_list<List_entry>::retrieve(int position,List_entry&x)const
{
    if (position < 0 || position >= count) return range_Error;
    set_position(position);
    x = current->entry;
    return success;
}

template <class List_entry>
Error_code double_list<List_entry>::replace(int position,List_entry&x)
{
    if (position < 0 || position >= count) return range_Error;

    set_position(position);
    current->entry = x;
    return success;
}
template <class List_entry>
void double_list<List_entry>::traverse(void(*visit)(List_entry &x))
{
    node<List_entry>* newptr = head;
    while (newptr) {
        (*visit)(newptr->entry);
        newptr = newptr->next;
    }
}
int main()
{
    return 0;
}
