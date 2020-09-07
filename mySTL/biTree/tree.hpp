#include <iostream>
// https://www.cnblogs.com/msymm/p/9752153.html
// 二叉查找树：
// 若其左子树存在，则其左子树中每个节点的值都不大于该节点值；
// 若其右子树存在，则其右子树中每个节点的值都不小于该节点值；

template<class T>
class BSTNode{
    public:
    T key;              //关键字（键值）
    BSTNode *parent;    //父节点
    BSTNode *left;      //左孩子
    BSTNode *right;     //右孩子
   
    
    //构造函数
    BSTNode(T value,BSTNode *p, BSTNode *l, BSTNode *r):key(value),parent(p),;left(l),right(r){}
};

template<class T>
class BSTree{//查找树

    private:
        BSTNode<T> *mRoot;    //根节点

    public:
        BSTree();
        ~BSTree();

        //前序遍历二叉树
        void preOrder() {preOrder(this->mRoot)};
        //中序遍历二叉树
        void midOrder() {midOrder(this->mRoot)};
        //后序遍历二叉树
        void postOrder() {postOrder(this->mRoot)};


        //查找二叉树中键值为key的节点(递归实现)
        BSTNode<T>* search_Recursive(T key){search_Recursive(mRoot,key)};

        //查找二叉树中键值为key的节点(非递归实现)
        BSTNode<T>* search_Iterative(T key){search_Iterative(mRoot,key)};

        // 查找最小节点
        T minimum();
        // 查找最大节点
        T maximum();

        // 找节点(x)的后继节点，即，查找二叉树中数据值大于该节点的最小节点
        BSTNode<T>* successor(BSTNode<T>* x);
        // 找节点(x)的前驱节点，即，查找二叉树中数据值小于该节点的最大节点

        // 将节点(key为节点键值)插入到二叉树中
        void insert(T key);

        //删除节点(key为节点键值)
        void remove(T key);

        // 销毁二叉树
        void destroy();

        // 打印二叉树
        void print();


private:
        // 前序遍历二叉树
        void preOrder(BSTNode<T>* tree) const;  
        // 中序遍历二叉树
        void midOrder(BSTNode<T>* tree) const;
        // 后序遍历二叉树
        void postOrder(BSTNode<T>* tree) const;

        //查找二叉树中键值为key的节点(递归实现)
        BSTNode<T>* search_Recursive(BSTNode<T>* x, T key) const;

        //查找二叉树中键值为key的节点(非递归实现)
        BSTNode<T>* search_Iterative(BSTNode<T>* x, T key) const;

        // 查找最小节点
        BSTNode<T>* minimum(BSTNode<T>* tree);
        // 查找最大节点
        BSTNode<T>* maximum(BSTNode<T>* tree);

        // 将节点(z)插入到二叉树中
        void insert(BSTNode<T> * tree, BSTNode<T> *z);

        // 删除二叉树(tree)中的节点(z)，并返回被删除的节点
        BSTNode<T>* remove(BSTNode<T> *tree, BSTNode<T> *z);

        // 销毁二叉树
        void destroy(BSTNode<T>* &tree);

        // 打印二叉树
        void print(BSTNode<T> *tree, T key, int direction);

};


        template<class T>
        void BSTree<T>::preOrder(BSTNode<T>* tree) const{// tree是根节点
            if(tree!=NULL)
            {
            cout<<tree->key<<" "
            preOrder(tree->left);
            preOrder(tree->right);
            }
        }

        template<class T>
        void BSTree<T>::midOrder(BSTNode<T>* tree) const{
            if(tree!=NULL){
                preOrder(tree->left);
                cout<<tree->key<<" " 
                preOrder(tree->right);
            }
        }

        template<class T>
        void BSTree<T>::postOrder(BSTNode<T>* tree) const{
            if(tree!=NULL){
                preOrder(tree->left);
                preOrder(tree->right);
                cout<<tree->key<<" " 
            }
        }


        //查找二叉树中键值为key的节点(递归实现)
        template<class T>
        BSTNode<T>* BSTree<T>::search_Recursive(BSTNode<T>* x, T key) const{
            if(x==NULL || x->key==key)
               return x;

            if(key < x->key)
                return search_Recursive(x->left,key);
            else
                return search_Recursive(x->right,key);
        }

        //查找二叉树中键值为key的节点(非递归实现)
        template<class T>
        BSTNode<T>* BSTree<T>::search_Iterative(BSTNode<T>* x, T key) const{
            // 该函数会创建一个临时变量用于接收x,当传入mRoot时，创建了一个临时变量 mRoot_x=mRoot，并不会修改成员变量
                while((key!=x->key) && (NULL!=x))
                {
                    if(key < x->key) 
                    x=x->left;
                else
                    x=x->right;
                }
            return x;
        }

        // 查找最小节点
        template<class T>
        BSTNode<T>* BSTree<T>::minimum(BSTNode<T>* tree)
        {//最小节点一定是左子树左节点

            while((tree-left)!=NULL){
                tree = tree->left;
            }
                return tree->key;
        }