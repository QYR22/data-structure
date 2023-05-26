#include <iostream>
#include <string.h>
#include <queue>

using namespace std;

enum Balance_factor{l,e,r};

struct record
{
    long long dis;
    int index;

    record();
    void operator=(const record &copy)
    {
        dis = copy.dis; index = copy.index;
    }
    bool operator==(record &cmp)
    {
        return dis==cmp.dis;
    }
    bool operator>(record &cmp)
    {
        return dis>cmp.dis;
    }
    bool operator<(record &cmp)
    {
        return dis<cmp.dis;
    }
    bool operator!=(record &cmp)
    {
        return dis!=cmp.dis;
    }
};
record::record()
{
    dis = -1; index= -1;
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
    //void retrieve(string &target);

    void insert(record &x);
    void remove(record &x);


    void KthSmallest(int k,record &get);
protected:
    int getSize(node *cur);

    void recursive_clear(node *&sub_root);

    bool avl_insert(node *&sub_root,record &x, bool &taller);//
    bool avl_remove(node *&sub_root,record &x, bool &shorter);//

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
    sub_root = NULL;
    delete temp;
}

//INSERT
void AVL_tree::insert(record &x)
{
//taller means whether we the height of the avltree will +1
    bool taller=true;
    avl_insert(root, x, taller);
}
bool AVL_tree::avl_insert(node *&sub_root,record &x, bool &taller)
{
    bool ans = true;

    if(sub_root == NULL){
        sub_root = new node(x);
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
void AVL_tree::remove(record &x)
{
    //shorter树是否会变矮
    bool shorter=true;
    avl_remove(root,x,shorter);
}
bool AVL_tree::avl_remove(node *&sub_root,record &x, bool &shorter)
{
    bool ans = true;

    record sub_record;

    if(sub_root==NULL){//没有可以删的
        shorter = false;
        return false;
    }
    //删除结点的步骤和二分查找树一样
    else if(x == sub_root->data){
        node *to_delete = sub_root;
        //right tree empty
        if(sub_root->right == NULL){
            sub_root=sub_root->left;
            delete to_delete;
            return true;
        }
        //left tree empty
        else if(sub_root->left == NULL){
            sub_root=sub_root->right;
            delete to_delete;
            return true;
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
/*key*/
        if(sub_record.dis != -1)     sub_root->data=sub_record;
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

        if(sub_record.dis != -1)    sub_root->data=sub_record;
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

        if(sub_root->left==NULL || sub_root->left->right ==NULL)
            return shorter;
        node *right_tree = sub_root->left->right;
        sub_root->left->right = right_tree->left;
        right_tree->left = sub_root->left;
        sub_root->left = right_tree;
        rotate_right(sub_root);
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
    sub_root->left = left_tree->right;
    left_tree->right = sub_root;
    sub_root = left_tree;
}

int AVL_tree::getSize(node *cur)
{
    if(cur == NULL) return 0;
    if(cur->left == NULL && cur->right == NULL) return 1;
    return getSize(cur->left) + getSize(cur->right) +1;
}

void AVL_tree::KthSmallest(int k,record &get)
{
    node *cur = root;
/*
    while(cur->left)
        cur=cur->left;
    record temp =cur->left->data;
    cout<<temp.index<<endl;
    */
    while( cur ){
        int lsum = getSize(cur->left);
        //cout<<"lsum=="<<lsum<<endl;
        //cout<<"cur->left->index->dis"<<cur->left->data.dis<<' '<<cur->left->data.index<<"        lsum   ="<<lsum<<endl;
        if(lsum < k-1){
            cur = cur->right;
            k = k-lsum-1;
        }
        else if(lsum == k-1)
            break;
        else
            cur = cur->left;
    }
    get = cur->data;
}

struct point
{
    long long x,y;
};

int main()
{
    int sum;    cin>>sum;
    AVL_tree mytree[sum+1];
    point points[sum+1];

    for(int i=0; i<sum; ++i){
        double tmp;     cin>>tmp;
        points[i].x = 100*tmp;
        cin>>tmp;
        points[i].y = 100*tmp;
        //printf("point i 'x=%ld  point i'y= %ld    ",points[i].x,points[i].y);
    }

    for(int i=0; i<sum; ++i){
        for(int j=i+1; j<sum; ++j){
            record ij,ji;
            ij.index = j;   ji.index = i;
            long long tmp1 = points[i].x-points[j].x;
            long long tmp2=points[i].y-points[j].y;
            ij.dis = tmp1*tmp1 + tmp2*tmp2;
            ji.dis = ij.dis;
            //printf("i=%d j=%d\n",i,j);
            //printf("ij.index=%d  ji.index=%d\n",ij.index,ji.index);
            //printf("%lld\n",ij.dis);
            mytree[i].insert(ij);
            mytree[j].insert(ji);
        }
    }
    //int a=1;
    //int b = mytree[5].KthSmallest(a);

    for(int k=2; k<sum; ++k){
        //cout<<"now k=="<<k<<endl;
        int cnt = 1;
        bool visited[sum+1];
        memset(visited,false,sizeof(visited));

        queue<int> tmp_graph;
        tmp_graph.push(0);
        visited[0] = true;

        while(!tmp_graph.empty()){
            int pnow = tmp_graph.front();
            tmp_graph.pop();
            //visited[pnow] = true;
            //cout<<"pnow"<<pnow<<' ';

            //现在路径到了的点是pnow

            //pnow的k近邻点入队
            for(int i=1; i<=k; ++i){
                record now;
                mytree[pnow].KthSmallest(i,now);

                if(visited[now.index])    continue;

                //cout<<"before now "<<now<<' ';

                //确定是不是对于now来说  pnow也是他的近邻点
                //是的话才能让now入队 不是的话那就没有这条路

                record tempo;
                mytree[now.index].KthSmallest(k,tempo);
                //printf("tempo.dis=%lld\n",tempo.dis);
                if(tempo.dis >= now.dis){
                    visited[now.index] = true;
                    cnt++;
                    if(cnt==sum){
                        printf("%d\n",k);
                        return 0;
                    }
                    tmp_graph.push(now.index);
                }
            }
        }
    }

    return 0;
}
