https://www.cnblogs.com/skywang12345/p/3577360.html
https://blog.csdn.net/fxkcsdn/article/details/81674653#%E5%AE%9A%E4%B9%89%E5%8F%8A%E5%8E%9F%E7%90%86%C2%A0
http://www.cppblog.com/cxiaojia/archive/2012/08/20/187776.html
https://blog.csdn.net/qq_37934101/article/details/81160254
// AVL树是平衡树，AVL树中任何节点的两个子树的高度最大差别为1
template<typename T>
struct AVLTreeNode
{
    T key;// 键值
    int height;
    AVLTreeNode<T>* left;
    AVLTreeNode<T>* right;
    AVLTreeNode(T val,int h=0):key(val),height(h),left(NULL),right(NULL){}
};


template<class T>
class AVLTree{
 private:
    AVLTreeNode<T> *mRoot; //根节点

 public:


    AVLTree();
    ~AVLTree();

    //获取树的高度   树的高度为最大层次，空节点高度为0，只有一个根节点则高度为1
    int height();
    //比较大小
    int max(int a,int b);
    // 前序遍历AVL树
    void preOrder();
    // 中序遍历AVL树
    void midOrder();
    // 后序遍历AVL树
    void postOrder();

    //(递归实现)查找AVL树中键值为key的节点
    AVLTreeNode<T>* search(T key);
    //(非递归实现)查找AVL树中键值为Key的节点
    AVLTreeNode<T>* iterativeSearch(T key);

    //查找最小结点：返回最小结点的值
    T minimum();
    //查找最大结点：返回最大结点的值
    T maximum();

    //将结点（key为结点键值）插入到AVL树中
    void insert(T key);

    //删除结点(key为结点键值)
    void remove(T key);

    //销毁AVL树
    void destroy();

    private:

    // 获取树的高度
        int height(AVLTreeNode<T>* tree) ;

        // 前序遍历"AVL树"
        void preOrder(AVLTreeNode<T>* tree) const;
        // 中序遍历"AVL树"
        void inOrder(AVLTreeNode<T>* tree) const;
        // 后序遍历"AVL树"
        void postOrder(AVLTreeNode<T>* tree) const;

        // (递归实现)查找"AVL树x"中键值为key的节点
        AVLTreeNode<T>* search(AVLTreeNode<T>* x, T key) const;
        // (非递归实现)查找"AVL树x"中键值为key的节点
        AVLTreeNode<T>* iterativeSearch(AVLTreeNode<T>* x, T key) const;

        // 查找最小结点：返回tree为根结点的AVL树的最小结点。
        AVLTreeNode<T>* minimum(AVLTreeNode<T>* tree);
        // 查找最大结点：返回tree为根结点的AVL树的最大结点。
        AVLTreeNode<T>* maximum(AVLTreeNode<T>* tree);

        // LL：左左对应的情况(左单旋转)。
        AVLTreeNode<T>* leftLeftRotation(AVLTreeNode<T>* k2);

        // RR：右右对应的情况(右单旋转)。
        AVLTreeNode<T>* rightRightRotation(AVLTreeNode<T>* k1);

        // LR：左右对应的情况(左双旋转)。
        AVLTreeNode<T>* leftRightRotation(AVLTreeNode<T>* k3);

        // RL：右左对应的情况(右双旋转)。
        AVLTreeNode<T>* rightLeftRotation(AVLTreeNode<T>* k1);

        // 将结点(z)插入到AVL树(tree)中
        AVLTreeNode<T>* insert(AVLTreeNode<T>* &tree, T key);

        // 删除AVL树(tree)中的结点(z)，并返回被删除的结点，没有必要返回节点，因为形参使用了引用
        AVLTreeNode<T>* remove(AVLTreeNode<T>* &tree, AVLTreeNode<T>* z);

        // 销毁AVL树
        void destroy(AVLTreeNode<T>* &tree);

        // 打印AVL树
        void print(AVLTreeNode<T>* tree, T key, int direction);

};

// 获取树的高度
template<class T>
int AVLTree<T>::height(AVLTreeNode<T>* tree){// tree为根节点
    if(tree != NULL)
    return tree->height;
    return 0;
}

template<class T>
int AVLTree<T>::height(){
    return height(mRoot);
}

//比较大小
template<class T>
int AVLTree<T>::max(int a, int b)
{
    return (a>b)?a:b;
}

// 失去平衡的状态有LL(左左)，LR(左右)，RR(右右)，RL(右左)

// 平衡二叉树 - 左单旋
// 返回值：旋转后的根节点
template<class T>
AVLTreeNode<T>* AVLTree<T>::leftLeftRotation(AVLTreeNode<T>* k2)
{
    AVLTreeNode<T>* k1 = k2->left;
    k1->right = k2;
    k2->left = k1->right;

     k2->height = max( height(k2->left), height(k2->right)) + 1;
     k1->height = max( height(k1->left), k2->height) + 1;

     return k1;
}

// RR - 右单旋
// 返回旋转后的根节点
template<class T>
AVLTreeNode<T>* AVLTree<T>::rightRightRotation(AVLTreeNode<T>* k1)
{
    AVLTreeNode<T>* k2 = k1->right;
    k2->left = k1;
    k1->left = k2->left;

     k1->height = max( height(k1->left), height(k1->right)) + 1;
     k2->height = max( height(k2->right), k1->height) + 1;

     return k2;
}


// LR -左双旋
// 返回旋转后的根节点
// 原本k+2深度的树，进过变换后深度变为k+1
template <class T>
AVLTreeNode<T>* AVLTree<T>::leftRightRotation(AVLTreeNode<T>* k3)
{
    //先右单旋，再左单旋
    k3->left = rightRightRotation<T>(k3->left);
    
    return leftLeftRotation<T>(k3);
     
}

// RL - 右双旋
//返回旋转后的根节点
template <class T>
AVLTreeNode<T>* AVLTree<T>::rightLeftRotation(AVLTreeNode<T>* k1)
{
    k1->right = leftLeftRotation<T>(k1->right);
    return rightRightRotation<T>(k1);
}


/* 
 * 将结点插入到AVL树中，并返回根节点
 *
 * 参数说明：
 *     tree AVL树的根结点
 *     key 插入的结点的键值
 * 返回值：
 *     根节点
 */
template <class T>
AVLTreeNode<T>* AVLTree<T>::insert(AVLTreeNode<T>* &tree, T key)//注意这里使用了引用，避免了使用return
{
    if(tree == NULL)//节点为空，在此节点处加入key的信息
    {
        tree = new AVLTreeNode<T>(key);
        tree->key = key;
        return tree;
    }

    if(tree->key > key)//插入左子树
    {
    insert(tree->left,key);//这里调用的是AVL插入操作，经过该函数后认为左子树已经是一个AVL树
    if(2==(height(tree->left)-height(tree->right))){
        if(key<tree->left->data)// 新增加的键值插入到根节点左子树的左子树
        leftLeftRotation(tree);
        else// 新增加的键值插入到根节点左子树的右子树
        leftRightRotation(tree);
    }
    else if(tree->data<key)//插入右子树
    {
        insert(tree->right,key);
        if(2==(height(tree->right)-height(tree->left))){
        if(key > tree->right->key)
        rightRightRotation(tree);
        else
        rightLeftRotation(tree);
        }
    }
    else
    {
        // key == tree->key,不允许添加相同的节点
    }
    tree->height = max(height(tree->left),height(tree->right))+1; //这里加1是认为头结点也是一层高度
    }
    return tree;
}


template<class T>
void AVLTree<T>::insert(T,key)
{
    insert(mRoot,key);
}

/* 
 * 删除结点(z)，返回根节点
 *
 * 参数说明：
 *     tree AVL树的根结点
 *     z 待删除的结点
 * 返回值：
 *     根节点
 */
template <class T>
AVLTreeNode<T>* AVLTree<T>::remove(AVLTreeNode<T>* &tree, AVLTreeNode<T>* z)
{
     //根为空 或者没有必要删除节点，直接返回NULL
     if(tree == NULL || z==NULL)
     return NULL;

     if(z->key < tree->key) //需要删除左子树节点
     {
         AVLTree<T>::remove(tree->left,z);
         // 删除节点后，若AVL失去平衡，则进行相应调节
         if(2==(height(tree->right)-height(tree->left))//此时右子树还是AVL树
         {
               AVLTreeNode<T> *r =  tree->right;
               if(height(r->left) > height(r->right)) // 对应RL情况
                tree = rightLeftRotation(tree);
               else // 对应RR情况 ,当height(r->right)==height(r->left)时，使用RR
               tree = rightRightRotation(tree);     
         }
     }
     else if(z->key > tree->key)//删除右子树节点
     {
         tree->right = remove(tree->right,z);

         if(2==(height(tree->left)-height(tree->right)))
         {
             AVLTreeNode<T> *r = tree->left;
             if(height(r->left) >= height(r->right))
             tree = leftLeftRotation(tree);
             else
             tree = leftRightRotation(tree);
         }
     }
     else //需要删除根节点
     {

        if((tree->right!=NULL)&&(tree->left!=NULL))
        {
            if(height(tree->left) > height(tree->right))
            {
                //如果tree左子树比右子树高，则
                // 找出tree左子树中最大节点，将该节点key值赋值给tree,删除该节点
                // 该方式的好处是删除左子树最大节点后AVL仍是平衡的
                AVLTreeNode<T> *max = maximum(tree->left); //max节点一定是叶子节点
                tree->key = max->key;
                tree->left = remove(tree->left,max); //使用内置remove保证该子树是AVL树
            }

             else if(height(tree->left) < height(tree->right))
            {
                
                AVLTreeNode<T> *min = minimum(tree->right);   
                tree->key = min->key;
                tree->right = remove(tree->rights,min);  
            }
            else
            {
                 AVLTreeNode<T> *min = minimum(tree->right);   
                 tree->key = min->key;
                 
            }
 
        }
     }
     
}