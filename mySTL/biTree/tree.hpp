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
        T minimum(){minimum(mRoot)};
        // 查找最大节点
        T maximum(){maximum(mRoot)};

        // 找节点(x)的后继节点，即，查找二叉树中数据值大于该节点的最小节点
        BSTNode<T>* successor(BSTNode<T>* x);
        // 找节点(x)的前驱节点，即，查找二叉树中数据值小于该节点的最大节点
        BSTNode<T>* predecessor(BSTNode<T> *x);
        // 将节点(key为节点键值)插入到二叉树中
        void insert(T key)
        {
            BSTNode<T> *newnode = new BSTNode(key,NULL,NULL,NULL);
            if(NULL = newnode) //创建失败
            return;
            insert(mRoot,newnode); // mRoot不会被修改
        };

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


        // 找节点(x)的后继节点，即，查找二叉树中数据值大于该节点的最小节点
        template<class T>
         BSTNode<T>* BSTree<T>::successor(BSTNode<T>* x)
         {
            // 如果x存在右节点，则x的后继节点为“以其右孩子为根的子树的最小节点”
            if(NULL != x->right)
             return minimum(x->right);
             
             // 若x没有右孩子，则x有两种可能
             // 1. x是一个左孩子，则x的后继节点为父节点
             // 2. x是一个右孩子，则查找x的最低的父节点，并且该父节点要有左孩子
             BSTNode<T> *y = x->parent;
             while((y!=NULL) && (x==y->right))
             {
                 x = y;
                 y = y->parent;
             }
             return y;
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

         // 查找最大节点
        template<class T>
        BSTNode<T>* BSTree<T>::maximum(BSTNode<T>* tree)
        {
            //最大节点一定在右子树右节点
            while(tree != NULL){
                tree = tree->right;
            }
            return tree->key;
        }

        // 将节点(z)插入到二叉树中
        // 二叉树的某个节点，它的所有右子树的值都比该节点大，它的所有左子树的值都比该节点小
        // 搜索二叉树中不存在相同数值的节点，被插入的节点其值不等于搜索树的任何节点的值
        // 函数形参不是使用 BSTNode<T>&*,内部会生成一个临时变量，并不会修改传入的值
        // 若使用引用：由于函数传递的形参是mRoot，而外部函数最终会通过mRoot取值 (*mRoot), mRoot->parent,即将mRoot当成地址看待，因此该值不能被修改
        // 但借由传递的地址进行地址元素操作形成的影响则是真实存在的
        template<class T>
        void BSTree<T>::insert(BSTNode<T> * tree, BSTNode<T> *z)
        {
            BSTNode<T> *y = NULL;
            BSTNode<T> *x = tree;

            //查找z的位置
            while(x != NULL){
                y = x;
                if(z->key < x->key)
                x = x->left;
                else
                x = x->right;
            }     

            z->parent = y;
            if(y == NULL)
            tree = z;
            else if(z->key < y->key)
            {
                z->left = y->left;
                y->left = z;
            }
            else
            {
                z->right = y->right;
                y->right = z;
            }
            
        }


        // 删除二叉树(tree)中的节点(z)，并返回被删除的节点
        // 有三种情况
        // 1.需要删除的子节点，它没有任何子节点 -> 直接删除
        // 2.需要删除的子节点，只有左子树或右子树 -> 直接删除 + 用其子节点占据删除掉的位置
        // 3.需要删除的子节点，同时拥有两个子节点 -> 在需要被删除节点的右子树中找到最小值节点
        // 然后使用该最小值替换需要删除节点的值，然后在右子树中删除最小值节点
        template<class T>
        BSTNode<T>* BSTree<T>::remove(BSTNode<T> *tree, BSTNode<T> *z){ 
          if(z==NULL)
          return;
          if(z->left==NULL && z->right==NULL)// 没有任何子节点
          delete(z);
          if(z->left==NULL || z->right!=NULL) //只有右子树或只有左子树
          z->parent->left = (z->left)?z->left:z->right;
        }

        // 销毁二叉树
        template<class T>
        void BSTree<T>::destroy(BSTNode<T>* &tree)
        {
            if(NULL == tree)
            return;

            delete(tree); //对应new

            if(tree->left!=NULL)
            return destroy(tree->left);
            
            if(tree->right!=NULL)
            return destroy(tree->right);

        }