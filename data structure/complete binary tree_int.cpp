#include <iostream>
#include <algorithm>
#include <cstdio>
#include <stack>

using namespace std;


struct tnode
{
    int data;
    tnode *left;
    tnode *right;

    tnode();
    tnode(const int &x);
};
tnode::tnode()
{
    left=right=NULL;
}
tnode::tnode(const int &x)
{
    data = x;
    left=right=NULL;
}

class btree
{
public:
    btree();
    ~btree();
    bool empty();
    int height();

    void clear();
    void insert(const int &x);

    void preorder(void (*visit)(int &));
    void inorder(void (*visit)(int &));
    void postorder(void (*visit)(int &));

protected:
    tnode *root;
    int count;

    int recursive_height(tnode *sub_root)const;
    void recursive_clear(tnode *&sub_root);
    void recursive_insert(tnode *&sub_root, const int &x);
    void recursive_preorder(tnode *&sub_root,
                            void (*visit)(int &));
    void recursive_inorder(tnode *&sub_root,
                           void (*visit)(int &));
    void recursive_postorder(tnode *&sub_root,
                             void (*visit)(int &));
};

btree::btree()
{
    root = NULL;    count =0;
}
btree::~btree()
{
    clear();
}
//EMPTY
bool btree::empty()
{
    return root==NULL;
}
//HEIGHT
int btree::height()
{
    int ans = 0;
    tnode *cur = root->left;
    while(cur){
        ans++;
        cur = cur->left;
    }
    return ans;
}
//CLEAR
void btree::clear()
{
    recursive_clear(root);  count=0;
}
void btree::recursive_clear(tnode *&sub_root)
{
    if(sub_root == NULL)    return;
    tnode *temp = sub_root;
    recursive_clear(sub_root->left);
    recursive_clear(sub_root->right);
    sub_root = NULL;
    delete temp;
}

int btree::recursive_height(tnode *sub_root)const
{
    if(sub_root == NULL)    return 0;
    int l = recursive_height(sub_root->left);
    int r = recursive_height(sub_root->right);

    return l>r?l:r;
}

//INSERT
void btree::insert(const int &x)
{
    if(root == NULL){
        root = new tnode(x);
        count=1;
        return ;
    }
/*
要获取父节点的信息: 从根节点一层层访问,
当前插入点的编号tmpcnt  开一个栈stack空间依次入栈
1.tmpcnt是左孩子还是右孩子
2.计算tmpcnt的父节点 (tmp-1)/2
3.重复1
*/
    stack<int> nums;
    int item = 0,tmpcnt = count;

    while(tmpcnt>0){
        if(tmpcnt % 2 == 0)//要插入到右边
            nums.push(2);//2表示右边
        else
            nums.push(1);//1表示左边

        tmpcnt = (tmpcnt-1) / 2;
    }

    tnode *cur = root;
//顺着路径依次出栈
    while(nums.size() > 1){
        item = nums.top();
        if(item == 1)   cur = cur->left;
        else            cur = cur->right;
        nums.pop();
    }
    item = nums.top();
    if(item == 1)   cur->left = new tnode(x);
    else            cur->right = new tnode(x);
    count++;

    return ;
}

void btree::preorder(void (*visit)(int &))
{
    recursive_preorder(root,visit);
}
void btree::recursive_preorder(tnode *&sub_root,
                            void (*visit)(int &))
{
    if(sub_root){
        (*visit)(sub_root->data);
        recursive_preorder(sub_root->left,visit);
        recursive_preorder(sub_root->right,visit);
    }
}
void btree::inorder(void (*visit)(int &))
{
    recursive_inorder(root, visit);
}
void btree::recursive_inorder(tnode *&sub_root,
                           void (*visit)(int &))
{
    if(sub_root){
        recursive_inorder(sub_root->left, visit);
        (*visit)(sub_root->data);
        recursive_inorder(sub_root->right, visit);
    }
}
void btree::postorder(void (*visit)(int &))
{
    recursive_postorder(root, visit);
}
void btree::recursive_postorder(tnode *&sub_root,
                             void (*visit)(int &))
{
    if(sub_root){
        recursive_postorder(sub_root->left, visit);
        recursive_postorder(sub_root->right, visit);
        (*visit)(sub_root->data);
    }
}

void print(int &x)
{
    cout<<x<<' ';
}

int main()
{
    int sum;    cin>>sum;

    btree mytree;

    while(sum--){
        int tmp;
        cin>>tmp;
        mytree.insert(tmp);
    }
    cout<<mytree.height()<<endl;

    mytree.preorder(print); cout<<endl;
    mytree.inorder(print); cout<<endl;
    mytree.postorder(print); cout<<endl;

    return 0;
}
