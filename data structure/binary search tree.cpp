#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

class record
{
public:
    record();
    record(const string&x,const string&y);
    string the_key()const;
    string get_other()const;

    void operator=(const record &copy);
private:
    string key;
    string other;
};

record::record()
{

}

record::record(const string &x,const string &y)
{
    key = x;
    other = y;
}

string record::the_key()const
{
    return key;
}

string record::get_other()const
{
    return other;
}

void record::operator=(const record &copy)
{
    key = copy.key;
    other = copy.other;
}

class Key
{
    string key;
public:
    Key();
    Key(string x);
    Key(const record &x);
    string the_key()const;
};

Key::Key()
{

}
Key::Key(string x)
{
    key = x;
}
Key::Key(const record &x)
{
    key = x.the_key();
}
string Key::the_key()const
{
    return key;
}
bool operator ==(const Key &x,const Key &y)
{
    return x.the_key()==y.the_key();
}
bool operator > (const Key &x,const Key &y)
{
    return x.the_key()>y.the_key();
}
bool operator < (const Key &x,const Key &y)
{
    return x.the_key()<y.the_key();
}
bool operator >=(const Key &x,const Key &y)
{
    return x.the_key()>=y.the_key();
}
bool operator <=(const Key &x,const Key &y)
{
    return x.the_key()<=y.the_key();
}
template<class type>
bool operator !=(const Key &x,const Key &y)
{
    return x.the_key()!=y.the_key();
}

struct node
{
    record data;
    node *left;
    node *right;

    node();
    node(const record &x);
};
node::node()
{
    left = right = NULL;
}
node::node(const record &x)
{
    left = right = NULL;
    data = x;
}

class search_tree
{
public:
    search_tree();
    ~search_tree();

    void insert(const record &new_data);
    bool retrieve(const string &target);
    bool remove(string &target);
private:
    node *root;
    node *&find_node(node *&sub_root, const string &target)const;
    void recursive_clear(node *&sub_root);
    bool remove_root(node *&sub_root);
};

search_tree::search_tree()
{
    root = NULL;
}
search_tree::~search_tree()
{
    recursive_clear(root);
}

//INSERT
void search_tree::insert(const record &new_data)
{
    if(root == NULL){
        root = new node(new_data);
        return;
    }
    node *sub_root = root, *parent;
    do{
        parent = sub_root;

        if(new_data < sub_root->data)   sub_root = sub_root->left;
        else if(new_data > sub_root->data)  sub_root = sub_root->right;
        else{
            sub_root->data = new_data;
            return;
        }

    }while(sub_root != NULL);

    sub_root = new node(new_data);
    if(new_data < parent->data)
        parent->left = sub_root;
    else
        parent->right = sub_root;

    return;
}
//CLEAR
void search_tree::recursive_clear(node *&sub_root)
{
    node *temp = sub_root;
    if(sub_root == NULL)    return;
    recursive_clear(sub_root->left);
    recursive_clear(sub_root->right);
    sub_root = NULL;
    delete temp;
}
//FIND
node *&search_tree::find_node(node *&sub_root, const string &target)const
{
    if(sub_root == NULL || sub_root->data.the_key() == target) return sub_root;
    node *&temp = find_node(sub_root->left,target);
    if(temp != NULL)    return temp;
    return find_node(sub_root->right,target);
}
//RETRIEVE
bool search_tree::retrieve(const string &target)
{
    node *cur = root;
    while(cur){
        cout<<cur->data.the_key()<<' ';
        if(target == cur->data.the_key()){
            cout<<cur->data.get_other()<<endl;
            return true;
        }
        if(target > cur->data.the_key())
            cur = cur->right;
        else
            cur = cur->left;
    }
    return false;
}
//REMOVE
bool search_tree::remove_root(node *&sub_root)
{
    if(sub_root == NULL)    return false;
    node *to_delete = sub_root;
//如果右子树是空树,可以直接让左子树上来
    if(sub_root->right == NULL) sub_root = sub_root->left;
//如果左子树是空树,同理 可以让右子树直接上来
    else if(sub_root->left == NULL) sub_root = sub_root->right;
//麻烦的是既有左子树又有右子树的情况
//找到左子树最最右边的叶子,让它来替代删除的结点
    else{
        to_delete = sub_root->left;
        node *parent = sub_root;

        while(to_delete->right){
            parent = to_delete;
            to_delete = to_delete->right;
        }

        sub_root->data = to_delete ->data;
        if(parent == sub_root)
            sub_root->left = to_delete->left;
        else
            parent->right = to_delete->left;
    }
    delete to_delete;
    return true;
}
bool search_tree::remove(string &target)
{
    node *&cur = find_node(root,target);
    if(cur == NULL)     return false;
    remove_root(cur);
    return true;
}

void print(string &x)
{
    cout<<x<<' ';
}

int main()
{
    search_tree mytree;
    int sum;            cin>>sum;
    string s1,s2;

    while(sum--){
        cin>>s1>>s2;
        record cur(s1,s2);

        mytree.insert(cur);
    }
    cin>>sum;
    while(sum--){
        cin>>s1;
        mytree.remove(s1);
    }
    cin>>s1;
    if(mytree.retrieve(s1) == false)
        cout<<"NULL"<<endl;
    cin>>s2;
    if(mytree.retrieve(s2) == false)
        cout<<"NULL"<<endl;

    return 0;
}
