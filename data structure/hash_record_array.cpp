#include <iostream>
#include <string.h>
using namespace std;

template <class type>
class record
{
public:
    record();
    record(const type&x,const type &y);
    type the_key()const;
    type get_other()const;

    void operator=(const record<type> &copy);
private:
    type key;
    type other;
};
template <class type>
record<type>::record()
{

}

template <class type>
record<type>::record(const type&x,const type &y)
{
    key = x;
    other = y;
}

template <class type>
type record<type>::the_key()const
{
    return key;
}
template <class type>
type record<type>::get_other()const
{
    return other;
}
template <class type>
void record<type>::operator=(const record<type> &copy)
{
    key = copy.key;
    other = copy.other;
}

template<class type>
class Key
{
    type key;
public:
    Key();
    Key(type x);
    Key(const record<type> &x);
    type the_key()const;
};

template<class type>
Key<type>::Key()
{

}
template<class type>
Key<type>::Key(type x)
{
    key = x;
}
template<class type>
Key<type>::Key(const record<type> &x)
{
    key = x.the_key();
}
template<class type>
type Key<type>::the_key()const
{
    return key;
}

template<class type>
bool operator ==(const Key<type> &x,const Key<type> &y)
{
    return x.the_key()==y.the_key();
}

template<class type>
bool operator !=(const Key<type> &x,const Key<type> &y)
{
    return x.the_key()!=y.the_key();
}

template<class type>
class hashh
{
public:
    hashh();
    void get_size();
    bool insert(record<type> &r);
    bool retrieve(Key<type> &target);

private:
    record<type> table[2003];
    bool used[2003];
    int hash_size;
};
template<class type>
hashh<type>::hashh()
{
    memset(used,false,sizeof(used));
}
template<class type>
void hashh<type>::get_size()
{
    cin>>hash_size;
}

template<class type>
bool hashh<type>::insert(record<type> &r)
{
    int cnt=0,
    position = r.the_key() % hash_size,
    inc=1;

    while(used[position]==true &&
          cnt < (hash_size+1)/2 &&
          table[position].the_key() != r.the_key()){
        cnt++;
        position = (position+inc) % hash_size;
        inc+=2;
    }
    if(!used[position]){
        used[position] = true;
        table[position] = r;
        return true;
    }
    return false;
}
/*是一样的
    int cnt=0,inc=1,position = target.the_key() % hash_size;
    int pp=position;
    while(used[position]==true &&
          table[position].the_key()!=target.the_key()
          && cnt < (hash_size+1) /2){
        cnt++;
        cout<<position<<' ';
        position = (pp + cnt*cnt) % hash_size;
        inc+=2;
    }
*/

template<class type>
bool hashh<type>::retrieve(Key<type> &target)
{
    int cnt=0,inc=1,position = target.the_key() % hash_size;

    while(used[position]==true &&
          table[position].the_key()!=target.the_key()
          && cnt < (hash_size+1) /2){
        cnt++;
        cout<<position<<' ';
        position = (position+inc) % hash_size;
        inc+=2;
    }
    if(cnt < (hash_size+1) /2)
        cout<<position<<' ';

    if(table[position].the_key() == target.the_key()){
        cout<<table[position].the_key()<<' '<<table[position].get_other()<<endl;
        return true;
    }

    return false;
}

int main()
{
    hashh<int> myhash;
    myhash.get_size();

    int pairs;
    cin>>pairs;
    while(pairs--){
        int a,b;
        cin>>a>>b;
        record<int> tmp(a,b);

        myhash.insert(tmp);
    }

    int tt;
    cin>>tt;
    Key<int> target1(tt);
    if(myhash.retrieve(target1) == false)
        cout<<"NULL"<<endl;


    cin>>tt;
    Key<int> target2(tt);
    if(myhash.retrieve(target2) == false)
        cout<<"NULL"<<endl;

    return 0;
}

/*
13 6
16 16 3 3 29 29 26 26 39 39 52 52
52
20
*/
