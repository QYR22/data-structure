#include <iostream>
#include <string>

using namespace std;

enum Error_code{success,duplicate_error,fail};
enum Balance_factor{l,e,r};

struct record
{
    string key;
    string other;

    record();
    record(const string &s1,const string &s2);
    void operator=(const record &copy)
    {
        key = copy.key; other = copy.other;
    }
    bool operator==(record &cmp)
    {
        return key==cmp.key;
    }
    bool operator>(record &cmp)
    {
        return key>cmp.key;
    }
    bool operator<(record &cmp)
    {
        return key<cmp.key;
    }
};
record::record()
{
    key="\0";other="\0";
}
record::record(const string &s1,const string &s2)
{
    key = s1; other=s2;
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
    left=right=NULL;    balance = e;
}

node::node(const record &x)
{
    data = x;
    left=right=NULL;
    balance = e;
}

class AVL_tree
{
public:
    AVL_tree();
    bool empty() const;
    void clear();
    void retrieve(string &target);

    Error_code insert(record &x);
    Error_code remove(record &x);

protected:

    void recursive_clear(node *&sub_root);
    Error_code avl_insert(node *&sub_root,record &x, bool &taller);//
    Error_code avl_remove(node *&sub_root,record &x, bool &shorter);//

    void rotate_left(node *&sub_root);
    void rotate_right(node *&sub_root);

    void left_balance(node *&sub_root);
    void right_balance(node *&sub_root);

    bool right_balance2(node *&sub_root);
    bool left_balance2(node *&sub_root);

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
    //sub_root = NULL;
    delete temp;
}


void AVL_tree::retrieve(string &target)
{
    node *cur=root;
    while(cur){
        cout<<cur->data.key<<' ';
        if(target == cur->data.key){
            cout<<cur->data.other<<endl;
            return;
        }
        if(target > cur->data.key)
            cur = cur->right;
        else
            cur = cur->left;
    }
    cout<<"NULL"<<endl;
}

//INSERT
Error_code AVL_tree::insert(record &x)
{
//taller means whether we the height of the avltree will +1
    bool taller=true;
    return avl_insert(root, x, taller);
}
Error_code AVL_tree::avl_insert(node *&sub_root,record &x, bool &taller)
{
    Error_code ans = success;

    if(sub_root == NULL){
        sub_root = new node(x);
    }
    else if(x == sub_root->data){
        ans = duplicate_error;
        taller = false;
    }
    //left tree
    else if(x < sub_root->data){
        ans = avl_insert(sub_root->left,x,taller);

        if(taller){
            if(sub_root->balance == l){
                left_balance(sub_root);
                taller = false;
            }
            else if(sub_root->balance == e)
                sub_root->balance = l;
            else{
                sub_root->balance=e;
                taller = false;
            }
        }
    }
    //right tree
    else{
        ans = avl_insert(sub_root->right,x,taller);
        if(taller){
            if(sub_root->balance == l){
                sub_root->balance = e;
                taller = false;
            }
            else if(sub_root->balance == e)
                sub_root->balance = r;
            else{
                right_balance(sub_root);
                taller = false;
            }
        }
    }
    return ans;
}

//REMOVE
Error_code AVL_tree::remove(record &x)
{
    //shorter树是否会变矮
    bool shorter=true;
    avl_remove(root,x,shorter);
}
Error_code AVL_tree::avl_remove(node *&sub_root,
                                record &x, bool &shorter)
{
    Error_code ans = success;

    record sub_record("...","...");

    if(sub_root==NULL){//没有可以删的
        shorter = false;
        return fail;
    }
    //删除结点的步骤和二分查找树一样
    else if(x == sub_root->data){
        node *to_delete = sub_root;
        //right tree empty
        if(sub_root->right == NULL){
            sub_root=sub_root->left;
            delete to_delete;
            return success;
        }
        //left tree empty
        else if(sub_root->left == NULL){
            sub_root=sub_root->right;
            delete to_delete;
            return success;
        }
        //左右子树都不空,找到左子树最右边的node
        else{
            to_delete = sub_root->left;
            node *parent=sub_root;
            while(to_delete->right){
                parent = to_delete;
                to_delete = to_delete->right;
            }
            //sub_root->data = to_delete->data;以前的bst的操作，但是这里不可以这样
            x=to_delete->data;
            sub_record = x;//记录删除的值
        }//只有else要进行下面的步骤，别的条件的已经结束了
/*本来要删掉m，m左子树最右下角是l，用l覆盖m的值，现在问题转换成了删除l结点*/
    }
    //x的值在上面的else里面更新了，重新调整
    if(x < sub_root->data){
        ans = avl_remove(sub_root->left,x,shorter);

        if(sub_record.key != "...")     sub_root->data=sub_record;
/*
这里删了sub_root的左节点：原来1左高--平 2平-右高 3右高-调整
*/
        if(shorter){
            if(sub_root->balance==l)
                sub_root->balance=e;

            else if(sub_root->balance==e){
                sub_root->balance=r;
                shorter=false;
            }
            else
                shorter=right_balance2(sub_root);

        }
    }
    else if(x > sub_root->data){
        ans = avl_remove(sub_root->right,x,shorter);

        if(sub_record.key != "...")    sub_root->data=sub_record;
/*
这里删了sub_root的右节点：原来1左高--调整 2平-左高 3右高-平
*/
        if(shorter){
            if(sub_root->balance == l)
                shorter = left_balance2(sub_root);

            else if(sub_root->balance == e){
                sub_root->balance = l;
                shorter = false;
            }
            else
                sub_root->balance = e;

        }
        //cout<<"root="<<root->data.key<<endl;
    }

    return ans;
}
/**/
//右子树高过左子树两层时
void AVL_tree::right_balance(node *&sub_root)
{
    node *&right_tree = sub_root->right;
    //判断是RR型还是RL型
    switch(right_tree->balance)
    {
    //RR
    case r:
        sub_root->balance=e;
        right_tree->balance=e;
        rotate_left(sub_root);
        break;
    case e:
        break;
    //RL
    case l:
        node *sub_tree = right_tree->left;
        switch(sub_tree->balance)
        {
        case e:
            sub_root->balance=e;
            right_tree->balance=e;
            break;
        case l:
            sub_root->balance=e;
            right_tree->balance=r;
            break;
        case r:
            sub_root->balance=e;
            right_tree->balance=e;
            break;
        }

        sub_tree->balance=e;
        rotate_right(right_tree);
        rotate_left(sub_root);
        break;
    }
}
void AVL_tree::left_balance(node *&sub_root)
{
    node *&left_tree = sub_root->left;
    if(left_tree->balance == l){
                sub_root->balance=e;
        left_tree->balance=e;
        rotate_right(sub_root);
    }
    else if(left_tree->balance == r){
        node *sub_tree = left_tree->right;

        if(sub_tree->balance == e)
            sub_root->balance=left_tree->balance=e;

        else if(sub_tree->balance == r){
            sub_root->balance=e;
            left_tree->balance=l;
        }
        else{
            sub_root->balance=r;
            left_tree->balance=e;
        }
        sub_tree->balance=e;
        rotate_left(left_tree);
        rotate_right(sub_root);
    }
}

bool AVL_tree::right_balance2(node *&sub_root)
{
    bool shorter = true;
    node *right_tree=sub_root->right;
/*针对原来sub_root已经右高了的同时，又删除了一个左边的叶子
所以要知道 right tree 的平衡因子
*/
    if(right_tree->balance == r){
        sub_root->balance=right_tree->balance=e;
        rotate_left(sub_root);
        //shorter =false;
    }
    else if(right_tree->balance == e){
        right_tree->balance=l;
        rotate_left(sub_root);
        shorter = false;
    }
    else{
        node *sub_tree=right_tree->left;

        if(sub_tree->balance == e)
            sub_root->balance=right_tree->balance=e;

        else if(sub_tree->balance == l){
            sub_root->balance=e;
            right_tree->balance=r;
        }
        else{
            sub_root->balance=l;
            right_tree->balance=e;
        }
        sub_tree->balance=e;
        rotate_right(right_tree);
        rotate_left(sub_root);
    }
    return shorter;
}
bool AVL_tree::left_balance2(node *&sub_root)
{
    bool shorter = true;
    node *left_tree=sub_root->left;
/*针对原来sub_root已经左高了的同时，又删除了一个右边的叶子
所以要知道 left tree 的平衡因子
*/  //ll的情况
    if(left_tree->balance == l){
        sub_root->balance=left_tree->balance=e;
        rotate_right(sub_root);
         //shorter =false;
    }
    else if(left_tree->balance == e){
        shorter = false;
        left_tree->balance=r;
        rotate_right(sub_root);
    }
    //lr的情况
    else{
        node *sub_tree = left_tree->right;

        if(sub_tree->balance == e)
            sub_root->balance = left_tree->balance = e;

        else if(sub_tree->balance == r){
            sub_root->balance = e;
            left_tree->balance = l;
        }
        else{
            sub_root->balance = r;
            left_tree->balance = e;
        }
        sub_tree->balance = e;
        //rotate_left(left_tree);

        if(sub_root->left==NULL || sub_root->left->right ==NULL)
            return shorter;
        node *right_tree = sub_root->left->right;
        sub_root->left->right = right_tree->left;
        right_tree->left = sub_root->left;
        sub_root->left = right_tree;
        //cout<<"!!!!!left-tree-data "<<left_tree->data.key<<endl;
        //cout<<"!!!!!left-tree-data "<<sub_root->left->data.key<<endl;
        //上面都是对的
        rotate_right(sub_root);
        //cout<<"now subroot= "<<sub_root->data.key<<endl;
    }
    return shorter;
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
    if(sub_root==NULL || sub_root->left ==NULL)     return;
    node *left_tree = sub_root->left;
    //cout<<"left-tree-data "<<left_tree->data.key<<endl;
    sub_root->left = left_tree->right;
    left_tree->right = sub_root;
    sub_root = left_tree;
}

int main()
{
    AVL_tree mytree;
    int sum;    cin>>sum;

    while(sum--){
        string s1,s2;
        cin>>s1>>s2;
        record x(s1,s2);
        mytree.insert(x);
    }

    //delete--有问题
    cin>>sum;
    string x;
    while(sum--){
        cin>>x;

        record target(x,x);
        mytree.remove(target);

    }
    string aa;

    cin>>aa;
    mytree.retrieve(aa);

    cin>>aa;
    mytree.retrieve(aa);

    return 0;
}
