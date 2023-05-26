#include <iostream>
#include <string.h>
#include <fstream>
#include <time.h>
#include <cstdio>
#include <vector>
#include <windows.h>
#include <string>

using namespace std;

enum Error_code{success,duplicate_error,fail};
enum Balance_factor{left_higher,equal_height,right_higher};

class record
{
public:
    record();
    record(const string &x,const string &y1,const string &y2);
    void operator=(const record &copy);
    bool operator==(const record &copy);
    bool operator<(const record &copy);
    bool operator>(const record &copy);
    string key,addr1,addr2;
};
record::record()
{
}

record::record(const string &x,const string &y1,const string &y2)
{
    key = x;
    addr1=y1;addr2=y2;
}

void record::operator=(const record &copy)
{
    key = copy.key;
    addr1 = copy.addr1;
    addr2 = copy.addr2;
}
bool record::operator<(const record &copy)
{
    return key<copy.key;
}
bool record::operator>(const record &copy)
{
    return key>copy.key;
}
bool record::operator==(const record &copy)
{
    return key==copy.key;
}

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

class AVL_tree
{
public:
    AVL_tree();
    bool empty() const;
    void clear();
    void retrieve(string &target);

    Error_code insert( record &x);
    Error_code remove(record &x);

protected:

    void recursive_clear(node *&sub_root);
    Error_code AVL_insert(node *&sub_root, record &x, bool &taller);//
    Error_code AVL_remove(node *&sub_root,record &x, bool &shorter);//

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
Error_code AVL_tree::insert( record &x)
{
//taller means whether we the height of the avltree will +1
    bool taller;
    return AVL_insert(root, x, taller);
}
Error_code AVL_tree::AVL_insert(node *&sub_root, record &x, bool &taller)
{
    Error_code result = success;
    //if root is empty
    if(sub_root == NULL){
        sub_root = new node(x);
        taller = true;
    }
    //duplicate
    else if(x == sub_root->data){
        //sub_root->data = x;
        result = duplicate_error;
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

void AVL_tree::retrieve(string &target)
{
    node *cur=root;
    while(cur){
        if(target == cur->data.key){
            //cout<<cur->data.addr1<<' '<<cur->data.addr2<<endl;
            return;
        }
        if(target > cur->data.key)
            cur = cur->right;
        else
            cur = cur->left;
    }
    cout<<"NULL"<<endl;
}

//右子树高过左子树两层时
void AVL_tree::right_balance(node *&sub_root)
{
    node *&right_tree = sub_root->right;
    //判断是RR型还是RL型
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

    ifstream fin("rand_800000_10.txt");
    AVL_tree mytree;
    int sum,sumq;    fin>>sum>>sumq;

    QueryPerformanceFrequency(&Frequ);
    QueryPerformanceCounter(&nBegin);//获取开始的时刻

    while(sum--){
        string s1,s2,s3,s4;
        fin>>s1>>s2>>s3>>s4;
        record x(s1,s2,s3);
        mytree.insert(x);
    }

    while(sumq--){
        string tmp;
        fin>>tmp;
        mytree.retrieve(tmp);
    }

    QueryPerformanceCounter(&nEnd);
    //获得结束的时刻
    double time = (nEnd.QuadPart - nBegin.QuadPart)/(double)Frequ.QuadPart;

    printf("total time(s):%lf s\n",time);

    return 0;
}
