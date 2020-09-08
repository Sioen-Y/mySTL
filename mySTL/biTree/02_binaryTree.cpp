#include"stdaxf.h"
#include<iomanip>
//数据节点BinaryNode，并没有父节点，所以这里的操作实现会与《数据导论》上的伪代码不一致
//没有定义父节点的话，前驱元素和后驱元素不好找，这里就不定义该方法了
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
//因为树的方法用到了很多递归， 所以这里我们需要申明如下的私有成员函数
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
        return (BinaryNode<T>*)NULL; //如果一开始就为空树?别忘了
    while(x->right)
        x=x->right;
    return x;
}
template <class T>
BinaryNode<T>* CBinarySearchTree<T>::find_min( BinaryNode<T>* &t) const
{
    BinaryNode<T>* x=t;
    if(x==NULL)
        return (BinaryNode<T>*)NULL; //如果一开始就为空树?别忘了
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
    t=NULL; //每一次删除，别忘了重置
}
template <class T>
void CBinarySearchTree<T>::remove(const T& x)
{
    remove(x,m_pRoot);
}

template <class T>
void CBinarySearchTree<T>::remove(const T& x, BinaryNode<T>* &t) // 这里是BinaryNode<T>*的引用，并不会生成临时变量，同时该函数也不会修改t的值，t是头结点
{	//递归方法
    if (t == NULL)
        return;
    if (x < t->value) // 当
        remove(x, t->left);
    else if (x > t->element)
        remove (x, t->right);
    else // now == 需要删除头结点，此时需要寻找左子树中最大的值作为新的头结点，或者寻找右子树中最小的值为最为新的头节点
		 // 但是左子树最大的值深度浅，不太可能是叶子节点，而右子树最小值必是叶子节点，因此使用右子树最小值作为新头结点
    {
        if (t->left != NULL &&
            t->right != NULL)//two child，头结点拥有左子树和右子树
        {
            t->value = find_min(t->right)->value;
            remove(t->value, t->right);
        }
        else//只有左子树或者只有右子树
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
  二叉树的遍历有三种：前序，中序，后序。

        前序遍历的规律是：输出根结点，输出左子树，输出右子树；
        中序遍历的规律是：输出左子树，输出根结点，输出右子树；
        后序遍历的规律是：输出左子树，输出右子树，输出根结点；
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