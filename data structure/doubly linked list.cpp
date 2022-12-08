#include <iostream>
#include <utility>
using namespace std;

enum Error_code{underflow,success,overflow,illegal};

template <class node_entry>
struct node
{
//data members
    node_entry entry;
    node<node_entry> *next;
    node<node_entry> *pre;
//constructors
    node();
    node(node_entry data,node<node_entry> *link_back=NULL,
                         node<node_entry> *link_next=NULL);
};

template<class LIST_entry>
class LIST
{
public:
    LIST();
    int size() const;
    //bool full() const;
    bool empty() const;
    void clear();
    //void print(LIST_entry &x)const;
    void traverse(void (*visit)(int &));
    void back_traverse(void (*visit)(int &));
    Error_code retrieve(int position, LIST_entry &x) const;
    Error_code replace(int position, const LIST_entry &x);
    Error_code remove(int position, LIST_entry &x);
    Error_code insert(int position, const LIST_entry &x);
    ~LIST();
    LIST(const LIST<LIST_entry> &copy);
    void operator =(const LIST<LIST_entry> &copy);

protected:
    int count;
    mutable int current_position;
    mutable node<LIST_entry> *current;

    void set_position(int position) const;
};
//constructors
template <class LIST_entry>
node<LIST_entry>::node()
{
    next=pre=NULL;
}
template <class LIST_entry>
node<LIST_entry>::node(LIST_entry data,
                       node<LIST_entry> *link_back,
                       node<LIST_entry> *link_next)
{
     entry=data;
     pre=link_back;
     next=link_next;
}
template<class LIST_entry>
LIST<LIST_entry>::LIST()
{
    count=0;
    current=NULL;
    current_position=-1;
}
//clear
template<class LIST_entry>
void LIST<LIST_entry>::clear()
{
    node<LIST_entry> *p,*q;

    if(current=NULL) return;
//先删current左边  再删current右边including current
    for(p=current;p;p=q){
        q=p->pre;
        delete p;
    }
    for(p=current;p;p=q){
        q=p->next;
        delete p;
    }

    count=0;
    current=NULL;
    current_position=-1;

    return;
}

template<class LIST_entry>
int LIST<LIST_entry>::size() const
{
    return count;
}

template<class LIST_entry>
bool LIST<LIST_entry>::empty() const
{
    return count<=0;
}

//traverse
template<class LIST_entry>
void LIST<LIST_entry>::traverse(void (*visit)(int &))
{
    set_position(0);
//ignore empty lists
    node<LIST_entry> *cur=current;
    while(cur){
        (*visit)(cur->entry);
        cur=cur->next;
    }
}
template<class LIST_entry>
void LIST<LIST_entry>::back_traverse(void (*visit)(int &))
{
    set_position(size()-1);

    node<LIST_entry> *cur=current;
    while(cur){
        (*visit)(cur->entry);
        cur=cur->pre;
    }
}
//insert one node at "position"
template<class LIST_entry>
Error_code LIST<LIST_entry>::insert(int position, const LIST_entry &x)
{
    node<LIST_entry> *new_node,*following,*preceding;
//illegal position
    if(position<0 || position>count) return illegal;

    if(position==0){
        if(count==0)following=NULL;//empty list
        else{
            set_position(0);
            following=current;
        }
        preceding=NULL;
    }
    else{
        set_position(position-1);//change current
        preceding=current;
        following=preceding->next;
//following是原来position位置的node
    }
    new_node=new node<LIST_entry>(x,preceding,following);

    if(preceding!=NULL)
        preceding->next=new_node;
    if(following!=NULL)
        following->pre=new_node;

    count++;
    current=new_node;
    current_position=position;

    return success;
}

//retrieve the position's entry
template<class LIST_entry>
Error_code LIST<LIST_entry>::retrieve(int position, LIST_entry &x) const
{
//illegal position
    if(position<0 || position>=count) return illegal;

    set_position(position);
    x=current->entry;
    return success;
}

//replace the position's entry
template<class LIST_entry>
Error_code LIST<LIST_entry>::replace(int position, const LIST_entry &x)
{
    if(position<0 || position>=count)
        return illegal;

    set_position(position);

    current->entry=x;
    return success;
}

//remove
template<class LIST_entry>
Error_code LIST<LIST_entry>::remove(int position, LIST_entry &x)
{
    if(count==0 || position<0 || position>=count) return illegal;

    node<LIST_entry> *old_node,*neighbor;

    set_position(position);
    old_node=current;
/*!!理解!!
这里是括号里面先赋值，再判断neighbor是不是NULL
node 1    old node(AIM)     node 2*/
/*先处理目标的前一个node1->next指向old_node->next
 如果cur是队首就不作操作*/
    if(neighbor=current->pre)
        neighbor->next=current->next;
/*再处理node2-<back指向node1
如果current是队尾也不做操作*/
    if(neighbor=current->next){
        neighbor->pre=current->pre;
        current=neighbor;
    }
//old_node=current;
    else{
        current=current->pre;
        current_position--;
    }

    x=old_node->entry;
    delete old_node;
    count--;

    return success;
}

//set position
template<class LIST_entry>
void LIST<LIST_entry>::set_position(int position) const
{
    if(current_position<=position)
        for(;current_position!=position;current_position++)
            current=current->next;
    else
        for(;current_position!=position;current_position--)
        current=current->pre;
    return;
}

template<class LIST_entry>
LIST<LIST_entry>::~LIST()
{
    clear();
}

template<class LIST_entry>
LIST<LIST_entry>::LIST(const LIST<LIST_entry> &copy)
{
    count=copy.count;
    current_position=copy.current_position;
//
    node<LIST_entry> *new_node,*old_node=copy.current;

    if(old_node == NULL)
        current=NULL;
    else{
//先拷贝current 拷贝current右边的，再拷贝current左边的
        new_node=current=new node<LIST_entry>(old_node->entry);

        while(old_node->next){
            old_node=old_node->next;
            new_node->next=new node<LIST_entry>(old_node->entry,new_node,NULL);
            new_node=new_node->next;
        }
        while(old_node->pre){
            old_node=old_node->back;
            new_node->pre=new node<LIST_entry>(old_node->entry,NULL,new_node);
            new_node=new_node->pre;
        }
    }
    return;
}

template<class LIST_entry>
void LIST<LIST_entry>::operator =(const LIST<LIST_entry> &copy)
{
    LIST new_copy(copy);
//先复制   清空原来的
    clear();

    count=new_copy.count;
    current_position=new_copy.current_position;
    current=new_copy.current;

    new_copy.count=0;
    new_copy.current_position=0;
    new_copy.current=NULL;

    return;
}
//nonono !!1const
void print(int &x)
{
    printf("%d ",x);
}


int main()
{
    LIST<int >mylist;
    int tmp,n=0;
    cin>>tmp;
    while(tmp!=-1){
        mylist.insert(n++,tmp);
        cin>>tmp;
    }
    //mylist.print();

    //mylist.traverse(print);
    int a,b;
    cin>>a>>b;
    while(a!=-1||b!=-1){
        mylist.insert(a,b);
        cin>>a>>b;
    }

    //mylist.traverse(print);
    cin>>tmp;
    while(tmp!=-1){
        int t;
        mylist.remove(tmp,t);
        cin>>tmp;
    }

    //mylist.traverse(print);
    int c,d;
    cin>>c>>d;
    while(c!=-1||d!=-1){
        mylist.replace(c,d);
        cin>>c>>d;
    }

    mylist.traverse(print);
    cout<<endl;
    mylist.back_traverse(print);
    cout<<endl;

    return 0;
}





