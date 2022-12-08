#include <iostream>

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
    //Key::comparisons++;
    return x.the_key()==y.the_key();
}
template<class type>
bool operator > (const Key<type> &x,const Key<type> &y)
{
    return x.the_key()>y.the_key();
}
template<class type>
bool operator < (const Key<type> &x,const Key<type> &y)
{
    return x.the_key()<y.the_key();
}
template<class type>
bool operator >=(const Key<type> &x,const Key<type> &y)
{
    return x.the_key()>=y.the_key();
}
template<class type>
bool operator <=(const Key<type> &x,const Key<type> &y)
{
    return x.the_key()<=y.the_key();
}
template<class type>
bool operator !=(const Key<type> &x,const Key<type> &y)
{
    return x.the_key()!=y.the_key();
}

int main()
{

    return 0;
}
