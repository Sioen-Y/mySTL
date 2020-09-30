#include "hash_table.hpp"
struct MyKeyHash {
    unsigned long operator()(const int& k) const
    {
        return k % 10;
    }
};

HashMap<int, string, MyKeyHash> hmap;
hmap.put(1, "val1");
hmap.put(2, "val2");
hmap.put(3, "val3");

string value;
hmap.get(2, value);//2%TABLE_SIZE = 2，表明在hash表的第二个元素（该元素为链表头结点），在此链表中找到key为2的hashnode(key是hashnode的成员变量)
cout << value << endl;
bool res = hmap.get(3, value);
if (res)
    cout << value << endl;
hmap.remove(3);
res = hmap.get(3, value);
if (res)
    cout << value << endl;