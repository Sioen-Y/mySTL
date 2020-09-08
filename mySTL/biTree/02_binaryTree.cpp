#include"stdaxf.h"
#include<iomanip>
//���ݽڵ�BinaryNode����û�и��ڵ㣬��������Ĳ���ʵ�ֻ��롶���ݵ��ۡ��ϵ�α���벻һ��
//û�ж��常�ڵ�Ļ���ǰ��Ԫ�غͺ���Ԫ�ز����ң�����Ͳ�����÷�����
template <class T>
struct BinaryNode
{
    T value;
    BinaryNode* left;
    BinaryNode* right;
    BinaryNode(T val,BinaryNode* lc,BinaryNode* rl):value(val),
        left(lc),right(rl)
    {
    }
};
enum PrintMode{PRE=0,MID,AFT};
template <class T>
class CBinarySearchTree
{
public:
    CBinarySearchTree();
    ~CBinarySearchTree();
private:
    BinaryNode<T>* m_pRoot;
public:
    void makeEmpty();
    void InsertNode(T &val);
    void remove(const T& x);
    void printTree(PrintMode mode);
    const T& findmax() ;
    const T& findmin() ;
    BinaryNode<T>*& searchNode(const T&x) const;
//��Ϊ���ķ����õ��˺ܶ�ݹ飬 ��������������Ҫ�������µ�˽�г�Ա����
private:
    void Insert(const T &val,BinaryNode<T>* &t);
    void make_empty(BinaryNode<T>* &t);
    void remove(const T& x, BinaryNode<T>* &t);
    void printTreeInPrev(BinaryNode<T>* t) const;
    void printTreeInMid(BinaryNode<T>* t)const;
    void printTreeInPost(BinaryNode<T>* t)const;

    BinaryNode<T>* find_max( BinaryNode<T>* &t) const;
    BinaryNode<T>* find_min( BinaryNode<T>* &t) const;
    BinaryNode<T>*& search_node(const T&x,BinaryNode<T>* &t) const;
};
template <class T>
CBinarySearchTree<T>::CBinarySearchTree<T>()
{
    m_pRoot=NULL;
}
template <class T>
CBinarySearchTree<T>::~CBinarySearchTree<T>()
{
    makeEmpty();
}
template <class T>
void CBinarySearchTree<T>::InsertNode(T &val)
{
    Insert(val,m_pRoot);
}
template <class T>
void CBinarySearchTree<T>::Insert(const T &val,BinaryNode<T>* &t)
{
    if(t==NULL)
     t=new BinaryNode<T>(val,NULL,NULL);
    else if(val>t->value)
      Insert(val,t->right);
    else
     Insert(val,t->left);    
}
template <class T>
BinaryNode<T>* CBinarySearchTree<T>::find_max(BinaryNode<T>* &t) const
{
    BinaryNode<T>* x=t;
    if(x==NULL)
        return (BinaryNode<T>*)NULL; //���һ��ʼ��Ϊ����?������
    while(x->right)
        x=x->right;
    return x;
}
template <class T>
BinaryNode<T>* CBinarySearchTree<T>::find_min( BinaryNode<T>* &t) const
{
    BinaryNode<T>* x=t;
    if(x==NULL)
        return (BinaryNode<T>*)NULL; //���һ��ʼ��Ϊ����?������
    while(x->left)
        x=x->left;
    return x;
}
template <class T>
void CBinarySearchTree<T>::makeEmpty()
{
    make_empty(m_pRoot);
}
template <class T>
void CBinarySearchTree<T>::make_empty(BinaryNode<T>* &t)
{
    if(t)
    {
    if(t->right)
        make_empty(t->right);
    if(t->left)
        make_empty(t->left);
    if(t!=NULL)
     delete t;

    }
    t=NULL; //ÿһ��ɾ��������������
}
template <class T>
void CBinarySearchTree<T>::remove(const T& x)
{
    remove(x,m_pRoot);
}

template <class T>
void CBinarySearchTree<T>::remove(const T& x, BinaryNode<T>* &t) // ������BinaryNode<T>*�����ã�������������ʱ������ͬʱ�ú���Ҳ�����޸�t��ֵ��t��ͷ���
{	//�ݹ鷽��
    if (t == NULL)
        return;
    if (x < t->value) // ��
        remove(x, t->left);
    else if (x > t->element)
        remove (x, t->right);
    else // now == ��Ҫɾ��ͷ��㣬��ʱ��ҪѰ��������������ֵ��Ϊ�µ�ͷ��㣬����Ѱ������������С��ֵΪ��Ϊ�µ�ͷ�ڵ�
		 // ��������������ֵ���ǳ����̫������Ҷ�ӽڵ㣬����������Сֵ����Ҷ�ӽڵ㣬���ʹ����������Сֵ��Ϊ��ͷ���
    {
        if (t->left != NULL &&
            t->right != NULL)//two child��ͷ���ӵ����������������
        {
            t->value = find_min(t->right)->value;
            remove(t->value, t->right);
        }
        else//ֻ������������ֻ��������
        {
            BinaryNode<T> *oldNode = t;
            t = (t->left != NULL) ? t->left : t->right;
            delete oldNode;
        }
    }
}
template <class T>
BinaryNode<T>*& CBinarySearchTree<T>::search_node(const T&x,BinaryNode<T>* &t) const
{
    if(x==NULL||x==t->value)
        return t;
    if(x>t->value)
        return search_node(x,t->right);
    else
        return search_node(x,t->left);
}
/*
  �������ı��������֣�ǰ�����򣬺���

        ǰ������Ĺ����ǣ��������㣬����������������������
        ��������Ĺ����ǣ�������������������㣬�����������
        ��������Ĺ����ǣ������������������������������㣻
*/
template <class T>
 void CBinarySearchTree<T>::printTreeInPrev(BinaryNode<T>* t) const
{
    if (t)
    {
        cout<<t->value<<' ';
        printTreeInPrev(t->left);
        printTreeInPrev(t->right);
    }
}
template <class T>
 void CBinarySearchTree<T>::printTreeInMid(BinaryNode<T>* t) const
{
    if (t)
    {
        printTreeInMid(t->left);
        cout <<t->value<<' ';
        printTreeInMid(t->right);
    }
}
template <class T>
 void CBinarySearchTree<T>::printTreeInPost(BinaryNode<T>* t) const
{
    if (t)
    {
        printTreeInPost(t->left);
        printTreeInPost(t->right);
        cout<<t->value<<' ';
    }
}
template <class T>
void CBinarySearchTree<T>::printTree(PrintMode mode)
{
    switch(mode)
    {
    case PRE:
        printTreeInPrev(m_pRoot);
        break;
    case MID:
        printTreeInMid(m_pRoot);
        break;
    case AFT:
        printTreeInPost(m_pRoot);
        break;
    default:
        break;
    }
}
template <class T>
const T& CBinarySearchTree<T>::findmax()
{
    BinaryNode<T>* p=find_max(m_pRoot);
    return p->value;
}
template <class T>
const T& CBinarySearchTree<T>::findmin()
{
    BinaryNode<T>* p=find_min(m_pRoot);
    return p->value;
}
template <class T>
BinaryNode<T>*& CBinarySearchTree<T>::searchNode(const T&x) const
{
    search_node(x,m_pRoot);
}