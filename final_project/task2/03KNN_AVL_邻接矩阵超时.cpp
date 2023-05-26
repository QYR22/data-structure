#include <iostream>
#include <string.h>
#include <queue>
#include <fstream>
#include <time.h>
#include <windows.h>
#include <vector>
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

    void KthSmallest(int k,record &get);
protected:
    int getSize(node *cur);

    void recursive_clear(node *&sub_root);

    bool avl_insert(node *&sub_root,record &x, bool &taller);

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
    while( cur ){
        int lsum = getSize(cur->left);
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
    ifstream file("n=5000.txt");

    int sum;    file>>sum;
    AVL_tree mytree[sum+1];
    point points[sum+1];

    LARGE_INTEGER Frequ;
    LARGE_INTEGER nBegin;
    LARGE_INTEGER nEnd;

    QueryPerformanceFrequency(&Frequ);
    QueryPerformanceCounter(&nBegin);//获取开始的时刻
    //输入 计算两点之间的距离
    for(int i=0; i<sum; ++i){
        double tmp;     file>>tmp;
        points[i].x = 100*tmp;
        file>>tmp;
        points[i].y = 100*tmp;
    }

    for(int i=0; i<sum; ++i){
        for(int j=i+1; j<sum; ++j){
            record ij,ji;
            ij.index = j;   ji.index = i;
            long long tmp1 = points[i].x-points[j].x;
            long long tmp2=points[i].y-points[j].y;
            ij.dis = tmp1*tmp1 + tmp2*tmp2;
            ji.dis = ij.dis;
            mytree[i].insert(ij);
            mytree[j].insert(ji);
        }
    }

    vector<vector<bool> > edges(sum+2, vector<bool>(sum+2));

    for(int k=1; k<sum; ++k){
        int cnt = 1;
        bool visited[sum+1];
        memset(visited,false,sizeof(visited));

        queue<int> tmp_graph;
        tmp_graph.push(0);
        visited[0] = true;
        //给邻接矩阵增加边 更新邻接矩阵
        //k-1的图 一定是k的图的子图
        for(int i=0; i<sum; ++i){
            //printf("i=%d\n",i);
            record tmp,t2;
            mytree[i].KthSmallest(k,tmp);
            if(edges[i][tmp.index]) continue;
            mytree[tmp.index].KthSmallest(k,t2);
            //printf("before graph edges: %d -- %d\n",k,tmp.index,t2.index);
            if(tmp.dis <= t2.dis){
                //printf("k=%d's graph edges: %d -- %d\n",k,tmp.index,t2.index);
                edges[tmp.index][i] = true;
                edges[i][tmp.index] = true;
            }
        }

        while(!tmp_graph.empty()){
            int pnow = tmp_graph.front();
            //cout<<pnow<<"  pnow"<<endl;
            tmp_graph.pop();

            //pnow的近邻点入队
            for(int i=0; i<sum; ++i){
                if(edges[pnow][i] && !visited[i]){
                    visited[i] = true;
                    cnt++;
                    if(cnt == sum){
                        //cout<<k<<endl;
                        QueryPerformanceCounter(&nEnd);
                        //获得结束的时刻
                        double time = (nEnd.QuadPart - nBegin.QuadPart)/(double)Frequ.QuadPart;

                        printf("total time(s):%lf s\n",time);
                        return 0;
                    }
                    tmp_graph.push(i);
                }
            }
        }
    }
/*
如果这个k1不行，那就尝试 k2 = k1 + 1
*/
    return 0;
}