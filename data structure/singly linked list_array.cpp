#include <iostream>
#include <utility>
//utility头文件定义重载的关系运算符
#define max_list 150
using namespace std;

enum Error_code{success,underflow,range_error,overflow};

template <class List_entry>
class List
{
public:
    List();
    //~List();
    List(const List &copy);
    void operator=(const List &copy);
    int size()const;
    bool full()const;
    bool empty()const;
    void clear();
    void print()const;
    void traverse(void (*visit)(List_entry &));
    Error_code retrieve(int position,List_entry &x)const;
    Error_code replace(int position,const List_entry &X);
    Error_code remove(int position,List_entry &x);
    Error_code insert(int position,const List_entry &x);

protected:
    int count;
    List_entry entry[max_list];
};

template <class List_entry>
List<List_entry>::List()
{ count=0; }

//List::~List();
List<List_entry>::List(const List &copy)
{

}

void List::operator=(const List &copy)
{

}

template <class List_entry>
int List<List_entry>::size()const
{ return count; }

template <class List_entry>
bool List<List_entry>::full()const
{ return count>=max_list; }

template <class List_entry>
bool List<List_entry>::empty()const
{ return count<=0; }

template <class List_entry>
void List<List_entry>::clear()
{ count=0; }

template <class List_entry>
/*函数指针是这个函数的名字
main函数里面可以写
List mylist;
mylist.traverse(print);
mylist.traverse(函数名);
*/
void List<List_entry>::traverse(void (*visit)(List_entry &))
{
    for(int i=0;i<count;++i)
        (*visit)(entry[i]);
}

template <class List_entry>
Error_code List<Lisst_entry>::retrieve(int position,List_entry &x)const
{
    if(position<0 || position>=count)
        return range_error;
    
    x=entry[position];
    return success;
}

template <List_entry>
Error_code List<List_entry>::replace(int position,const List_entry &X)
{
    if(position<0 || position>=count)
        return range_error;
        
    entry[position]=x;
    return success;
}

template <List_entry>
Error_code List<List_entry>::remove(int position,List_entry &x)
{
    if(count==0)
        return underflow;
    if(position<0 || position>=count)
        return range_error;
    
    x=entry[position];
    count--;
    while(position<count){
        entry[position]=entry[position+1];
        position++;
    }
    return success;
}
template <class List_entry>
void List<List_entry>::print()const
{
    for(int i=0;i<count-1;++i)
        printf("%d ",entry[i]);
    printf("%d\n",entry[count-1]);
    return 0;
}


template <class List_entry>
Error_code List<List_entry>::insert(int position,const List_entry &x)
{
    if(full())
        return overflow;
    if(position<0 || position>count)
        return range_error;
    
    for(int i=count-1;i>=position;--i)
        entry[i+1]=entry[i];
        
    entry[position]=x;
    count++;
    return success;
}


int main()
{
    List<int>mylist;

    int tmp=0,a,b;
    cin>>tmp;
    while(tmp!=-1){
        mylist.insert(mylist.size(),tmp);
        cin>>tmp;
    }
    mylist.print();

    cin>>a>>b;
    while(a!=-1 && b!=-1){
        mylist.insert(a,b);
        cin>>a>>b;
    }

    mylist.print();

    cin>>tmp;
    while(tmp!=-1){
        int num;
        mylist.remove(tmp,num);
        cin>>tmp;
    }

    mylist.print();

    cin>>a>>b;
    while(a!=-1 && b!=-1){
        mylist.replace(a,b);
        cin>>a>>b;
    }

    mylist.print();

    return 0;
}
