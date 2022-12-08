#include <iostream>
#include <utility>
using namespace std;
enum Error_code{success,underflow,overflow};

template<class node_entry>
struct node
{
    //data members
    node_entry entry;
    node<node_entry> *next;
    //constructors
    node();
    node(node_entry, node<node_entry> *link=NULL);
};

template<class List_entry>
class List
{
public:
    List();
    List(const List<List_entry> &copy);
    void operator =(const List<List_entry> &copy);
    ~List();

    int size()const;
    //bool full()const;
    bool empty()const;
    void clear();
    void traverse(void (*visit)(List_entry &));
    Error_code retrieve(int position, List_entry &x)const;
    Error_code replace(int position, const List_entry&x);
    Error_code remove(int position, List_entry &x);
    Error_code insert(int position, const List_entry &x);
protected:
    int count;
    node<List_entry> *head;
//sth new
    node<List_entry> *set_position(int position) const;
};
template<class List_entry>
node<List_entry>::node()
{ next=NULL; }

template<class List_entry>
node<List_entry>::node(List_entry data, node<List_entry> *link)
{
    entry=data;
    next=link;
}

template<class List_entry>
List<List_entry>::List()
{
    count=0;
    head=NULL;
}

//!!!remember
template<class List_entry>
void List<List_entry>::clear()
{
    node<List_entry> *cur,*cur_next;

    for(cur=head; cur; cur=cur_next){
        cur_next=cur->next;
        delete cur;
    }
    count=0;
    head=NULL;
    return;
}
//size
template<class List_entry>
int List<List_entry>::size()const
{
    return count;
}
//empty
template<class List_entry>
bool List<List_entry>::empty()const
{
    return count<=0;
}
//traverse
template<class List_entry>
void List<List_entry>::traverse(void (*visit)(List_entry &))
{
    node<List_entry> *cur;

    for(cur=head; cur; cur=cur->next)
        (*visit)(cur->entry);
    return;
}
//insert  position是0开始的下标
template<class List_entry>
Error_code List<List_entry>::insert(int position, const List_entry &x)
{
    if(position<0 || position>count)
        return overflow;

    node<List_entry> *new_node,*previous, *following;

    if(position>0){
        previous=set_position(position-1);
        following=previous->next;
    }
    else//position=0
        following=head;
    new_node=new node<List_entry>(x,following);

    if(new_node==NULL)
        return overflow;
    if(position==0)
        head=new_node;
    else
        previous->next=new_node;

    count++;
    return success;
}
//retrieve
template <class List_entry>
Error_code List<List_entry>::retrieve(int position, List_entry &x)const
{
    if(position<0 || position >=count) return overflow;

    node<List_entry> *cur;
    cur=set_position(position);
    x=cur->entry;
    return success;
}
//replace  list from 0~count-1(index)
template <class List_entry>
Error_code List<List_entry>::replace(int position, const List_entry&x)
{
    node<List_entry> *cur;

    if(position<0 || position>=count)
        return overflow;
    cur=set_position(position);
    cur->entry=x;
    return success;
}
//remove  remove LISTposition 同时x储存这个position的内容
template <class List_entry>
Error_code List<List_entry>::remove(int position, List_entry &x)
{
    node<List_entry> *prior,*cur;
    if(count==0)  return underflow;

    if(position<0 || position >=count) return overflow;

    if(position>0){
        prior=set_position(position-1);
        cur=prior->next;
        prior->next=cur->next;
    }
    else{
        cur=head;
        head=head->next;
    }

    x=cur->entry;
    delete cur;
    count--;

    return success;
}
//set position!!!!
template <class List_entry>
node<List_entry> *List<List_entry>::set_position(int position)const
{
    node<List_entry> *cur=head;
    for(int i=0;i<position;++i)
        cur=cur->next;
    return cur;
}
//拷贝构造函数
template <class List_entry>
List<List_entry>::List(const List<List_entry> &copy)
{
    count=copy.count;
    node<List_entry>*new_node,*old_node=copy.head;

    if(old_node==NULL)
        head=NULL;
    else{
        new_node=head=new node<List_entry>(old_node->entry);
        while(old_node->next){
            old_node=old_node->next;
            new_node->next=new node<List_entry>(old_node->entry);
            new_node=new_node->next;
        }
    }
}
template <class List_entry>
List<List_entry>::~List()
{
    clear();
}
template <class List_entry>
void List<List_entry>::operator =(const List<List_entry> &copy)
{
    List new_copy(copy);
    clear();
    count=new_copy.count;
    head=new_copy.head;
    new_copy.count=0;
    new_copy.head=NULL;

}
    /*
    Error_code remove(int position, List_entry &x);
    Error_code insert(int position, const List_entry &x);

*/
int main()
{
    return 0;
}
