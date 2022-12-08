#include <iostream>
#include <string.h>
#include <utility>
using namespace std;

class String
{
public:
    String();
    ~String();
//copy constructor
    String(const String &copy);
//conversion from C-string类型转换
    String(const char *copy);
//conversion from List
    String(List<char>&copy);
//等号重载
    void operator =(const String &copy);
//conversion to C-style string
    const char *c_str()const;

protected:
    char *entries;
    int length;    
};
//重定义输入流
String read_in(istream &input, int &terminator)
{
    List<char>temp;
    int size=0;
    
    while((terminator = input.peek())!=EOF &&
          (terminator = input.get())!='\n')
        temp.insert(size++,terminator);
    
    String answer(temp);
    return answer;
}

String read_in(istream &input)
{
    List<char> temp;
    int size=0;
    
    char c;
    while((c=input.peek())!=EOF && (c=input.get())!='\n')
        temp.insert(size++,c);
    
    String answer(temp);
    return answer;
}
void write(String &s)
{
    cout<<s.c_str()<<endl;
}


void strcpy(String &copy,const String &original);
void strncpy(String &copy,const String &original,unsigned len);
int strstr(const String &text,const String &target);

bool operator==(const String&first, const String &second);
bool operator> (const String&first, const String &second);
bool operator< (const String&first, const String &second);
bool operator>=(const String&first, const String &second);
bool operator<=(const String&first, const String &second);
bool operator!=(const String&first, const String &second);

String::String()
{
    length=0;
    entries=new char[1];
    entries[0]='\0';
}

String::~String()
{
    delete []entries;
}

//copy constructor
String::String(const String &copy)
{
    length=strlen(const String &copy);
    entries=new char[length+1];
    strcpy(entries,copy.entries);
}

//conversion from C-string类型转换
String::String(const char *copy)
{
    length=strlen(copy);
    entries=new char[length+1];
    strcpy(entries,copy);
}
//conversion from List
String::String(List<char>&copy)
{
    length=copy.size();
    entries=new char[length+1];
    for(int i=0;i<length;++i)
        copy.retrieve(i,entries[i]);
    entries[length]='\0';
}
/*等号重载
先判断是不是 x=x  如果是,不操作
如果不是，先清空x再复制
*/
void String::operator =(const String &copy)
{
    if(strcmp(entries,copy.entries)!=0){
        delete []entries;
        length=strlen(copy.entries);
        entries=new char[length+1];
        strcpy(entries,copy.entries);
    }
}
//conversion to C-style string type:const char*
const char *String::c_str()const
{
    return (const char *)entries;
}

void strcpy(String &copy,const String &original)
{//因为已经有等号重载函数了
    copy=original;
}
void strncpy(String &copy,const String &original,unsigned len)
{
    const char *temp=original.c_str();
    char *s=new char[len+1];
//这是C++库里面的strncpy
    strncpy(s,temp,len);
    s[len]='\0';
/*①编译器检测到两边一个String一个char*,调用Stirng(const char *copy)构造函数转换类型
②利用等号重载运算符给copy(String类型)赋值
*/
    copy=s;
    delete []s;
}
//返回target中第一次出现子串text的下标
int strstr(const String &text,const String &target)
{
    int ans;
    const char *content_s=text.c_str();
    char *ptr=strstr(content_s,target.c_str());
    
    if(ptr==NULL)
        ans=-1;
    else
        ans=p-content_s;
    return ans;
}


bool operator==(const String&first, const String &second)
{
    return strcmp(first.c_str(),second.c_str())==0;
}

bool operator>(const String&first, const String &second)
{
    return strcmp(first.c_str(),second.c_str())>0;
}
bool operator<(const String&first, const String &second)
{
    return strcmp(first.c_str(),second.c_str())<0;
}
bool operator>=(const String&first, const String &second)
{
    return strcmp(first.c_str(),second.c_str())>=0;
}
bool operator<=(const String&first, const String &second)
{
    return strcmp(first.c_str(),second.c_str())<=0;
}
bool operator!=(const String&first, const String &second)
{
    return strcmp(first.c_str(),second.c_str())!=0;
}

    
int main()
{
    cout << "Hello world!" << endl;
    return 0;
}

int strstr(const String &text,const String &target)
{
    int ans;
    const char *content_s=text.c_str();
    char *ptr=strstr((char *)content_s,target.c_str());
//(char *)
    if(ptr==NULL)
        ans=-1;
    else
        ans=ptr-content_s;
    return ans;
}
