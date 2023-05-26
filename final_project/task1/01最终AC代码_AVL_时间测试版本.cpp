#include <iostream>
#include <string.h>
#include <fstream>
#include <time.h>
#include <windows.h>
using namespace std;

enum Balance_factor{left_higher,equal_height,right_higher};

struct record
{
    char name[13];
    int addr1,addr2;
    void operator=(const record &copy)
    {
        strcpy(name,copy.name);
        addr1=copy.addr1;addr2=copy.addr2;
    }
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

struct node
{
    record data;
    node *left;
    node *right;
    Balance_factor balance;
//constructos
    node();
    node(const record &x);
//factor
    void set_balance(Balance_factor b);
    Balance_factor get_balance() const;
};

node::node()
{
    left=right=NULL;    balance = equal_height;
}

node::node(const record &x)
{
    data = x;
    left=right=NULL;
    balance = equal_height;
}
void node::set_balance(Balance_factor b)
{
    balance = b;
}

Balance_factor node::get_balance() const
{
    return balance;
}

int getnum(char *str)
{
    int n1 = str[0]-'A',n2 = str[1]-'A';
    return n2+n1*26;
}

class AVL_tree
{
public:
    AVL_tree();
    bool empty() const;
    void clear();
    void retrieve(char *target);

    bool insert(record &x);

protected:

    void recursive_clear(node *&sub_root);
    bool AVL_insert(node *&sub_root, record &x, bool &taller);//

    void rotate_left(node *&sub_root);
    void rotate_right(node *&sub_root);

    void left_balance(node *&sub_root);
    void right_balance(node *&sub_root);

    node *root;
};

AVL_tree::AVL_tree()
{
    root = NULL;
}

//EMPTY
bool AVL_tree::empty()const
{
    return root == NULL;
}

//CLEAR
void AVL_tree::clear()
{
    recursive_clear(root);
}

void AVL_tree::recursive_clear(node *&sub_root)
{
    node *temp = sub_root;
    if(sub_root == NULL ) return;
    recursive_clear(sub_root->left);
    recursive_clear(sub_root->right);
    sub_root = NULL;
    delete temp;
}

//INSERT
bool AVL_tree::insert( record &x)
{
//taller means whether we the height of the avltree will +1
    bool taller;
    return AVL_insert(root, x, taller);
}
bool AVL_tree::AVL_insert(node *&sub_root, record &x, bool &taller)
{
    bool result = true;
    //if root is empty
    if(sub_root == NULL){
        sub_root = new node(x);
        taller = true;
    }
    //duplicate
    else if(x == sub_root->data){
        //sub_root->data = x;
        result = false;
        taller = false;
    }
    else if(x < sub_root->data){
    //insert 左子树
        result = AVL_insert(sub_root->left,x,taller);
        if(taller)
            switch(sub_root->get_balance())
            {
            case left_higher:
                left_balance(sub_root);
                taller = false;
                break;
            case equal_height:
                sub_root->set_balance(left_higher);
                break;
            case right_higher:
                sub_root->set_balance(equal_height);
                taller = false;
                break;
            }
    }
    else{//right tree
        result = AVL_insert(sub_root->right,x,taller);
        if(taller)
            switch(sub_root->get_balance())
            {
            case left_higher:
                sub_root->set_balance(equal_height);
                taller = false;
                break;
            case equal_height:
                sub_root->set_balance(right_higher);
                break;

            case right_higher:
                right_balance(sub_root);
                taller = false;
                break;
            }
    }
    return result;
}

void AVL_tree::retrieve(char *target)
{
    node *cur=root;
    while(cur){
        if(strcmp(target,cur->data.name)==0){
            //printf("%d %d\n",cur->data.addr1,cur->data.addr2);
            return;
        }
        if(strcmp(target,cur->data.name)>0)
            cur = cur->right;
        else
            cur = cur->left;
    }
    //printf("NULL\n");
}

/**/
//右子树高过左子树两层时
void AVL_tree::right_balance(node *&sub_root)
{
    node *&right_tree = sub_root->right;

    switch(right_tree->get_balance())
    {
    //RR
    case right_higher:
        sub_root->set_balance(equal_height);
        right_tree->set_balance(equal_height);
        rotate_left(sub_root);
        break;
    case equal_height:
        break;
    //RL
    case left_higher:
        node *sub_tree = right_tree->left;
        switch(sub_tree->get_balance())
        {
        case equal_height:
            sub_root->set_balance(equal_height);
            right_tree->set_balance(equal_height);
            break;
        case left_higher:
            sub_root->set_balance(equal_height);
            right_tree->set_balance(right_higher);
            break;
        case right_higher:
            sub_root->set_balance(equal_height);
            right_tree->set_balance(equal_height);
            break;
        }

        sub_tree->set_balance(equal_height);
        rotate_right(right_tree);
        rotate_left(sub_root);
        break;
    }
}
void AVL_tree::left_balance(node *&sub_root)
{
    node *&left_tree = sub_root->left;
    switch(left_tree->get_balance())
    {
    case left_higher:
        sub_root->set_balance(equal_height);
        left_tree->set_balance(equal_height);
        rotate_right(sub_root);
        break;
    case equal_height:
        break;
    case right_higher:
        node *sub_tree = left_tree->right;
        switch(sub_tree->get_balance())
        {
        case equal_height:
            sub_root->set_balance(equal_height);
            left_tree->set_balance(equal_height);
            break;
        case right_higher:
            sub_root->set_balance(equal_height);
            left_tree->set_balance(left_higher);
            break;
        case left_higher:
            sub_root->set_balance(right_higher);
            left_tree->set_balance(equal_height);
            break;
        }

        sub_tree->set_balance(equal_height);
        rotate_left(left_tree);
        rotate_right(sub_root);
        break;
    }
}

//左旋
void AVL_tree::rotate_left(node *&sub_root)
{
    if(sub_root==NULL || sub_root->right ==NULL)    return;
    node *right_tree = sub_root->right;
    sub_root->right = right_tree->left;
    right_tree->left = sub_root;
    sub_root = right_tree;
}

void AVL_tree::rotate_right(node *&sub_root)
{
    if(sub_root==NULL || sub_root->left ==NULL)
        return;
    node *left_tree = sub_root->left;
    sub_root->left = left_tree->right;
    left_tree->right = sub_root;
    sub_root = left_tree;
}

int main()
{
    LARGE_INTEGER Frequ;
    LARGE_INTEGER nBegin;
    LARGE_INTEGER nEnd;

    AVL_tree mytree[680];
    ifstream file("rand_800000_10.txt");
    int sum,sumq;    file>>sum>>sumq;//scanf("%d %d",&sum,&sumq);
    char target[13];
    /*
    if(!file){
        cout<<"error in opening the file";
        return 0;
    }
    */
    QueryPerformanceFrequency(&Frequ);
    QueryPerformanceCounter(&nBegin);//获取开始的时刻
    while(sum--){
        record x;
        file>>x.name>>x.addr1>>x.addr2>>target;
        //scanf("%s %d %d %s",x.name,&x.addr1,&x.addr2,target);
        int pos=getnum(x.name);
        mytree[pos].insert(x);
    }

    while(sumq--){
        file>>target;
        //scanf("%s",target);
        int pos=getnum(target);
        mytree[pos].retrieve(target);
    }

    QueryPerformanceCounter(&nEnd);
    //获得结束的时刻
    double time = (nEnd.QuadPart - nBegin.QuadPart)/(double)Frequ.QuadPart;

    printf("total time(s):%lf s\n",time);
    printf("total time(ms):%lf ms\n",time*1000);


    return 0;
}
