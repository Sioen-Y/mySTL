// RBtree 红黑树
// 红黑树是一个平衡二叉树
// 每个节点不是黑色就是红色
// 根节点是黑色
// 每个叶子节点是黑色
// 每个红色节点的两个子节点一定都是黑色，如果该红色节点是叶子节点则无所谓该规则
// 任意一个结点到每个叶子节点的路径都包含数量相同的黑节点


// 红黑树不是严格的AVL树，AVL树中任何节点的两个子树的高度最大差别为1
// 而红黑树的1~6性质保证了其最长路径不多于最短路径长度的两倍


// https://segmentfault.com/a/1190000012728513
// https://developer.51cto.com/art/201908/601688.htm
// https://www.cnblogs.com/skywang12345/p/3624291.html


enum RBTColor{RED,BLACK};

template<class T>
class RBTNode
{
public:
    RBTNode color; //颜色
    T key; //关键字
    RBTNode *left;
    RBTNode *right;
    RBTNode *parent;

    RBTNode(T value, RBTColor c, RBTNode *l, RBTNode *r, RBTNode* p):
    key(value),color(c),left(l),right(r),parent(p){}
};

template<class T>
class RBTree{
    private:
    RBTree<T> *mRoot;
    public:
    RBTree();
    ~RBTree();

    //前序遍历“红黑树”
    void preOrder();
    //中序遍历“红黑树”
    void midOrder();
    //后序遍历“红黑树”
    void postOrder();

    //递归实现查找“红黑树”中键值为key的节点
    RBTNode<T>* search_recursive(T key);
    //非递归实现
    RBTNode<T>* search_iterative(T key);

    //查找最小节点：返回最小结点的键值
    T minimum();
    //查找最大节点：返回最大节点的键值
    T maximum();

    // 找节点x的后继节点，即查找红黑树中数据值大于该节点的最小结点
    RBTNode<T> *successor(RBTNode<T> *x);
    // 找节点x的前驱节点，即查找红黑树中数据值小于该节点的最大节点
    RBTNode<T> *predecessor(RBTNode<T> *x);

    //将结点(key为结点键值)插入到红黑树中
    void insert(T key);

    //删除结点
    void delete(T key);

    //销毁红黑树
    void destroy();
    private:
    // 前序遍历"红黑树"
        void preOrder(RBTNode<T>* tree) const;
        // 中序遍历"红黑树"
        void inOrder(RBTNode<T>* tree) const;
        // 后序遍历"红黑树"
        void postOrder(RBTNode<T>* tree) const;

        // (递归实现)查找"红黑树x"中键值为key的节点
        RBTNode<T>* search(RBTNode<T>* x, T key) const;
        // (非递归实现)查找"红黑树x"中键值为key的节点
        RBTNode<T>* iterativeSearch(RBTNode<T>* x, T key) const;

        // 查找最小结点：返回tree为根结点的红黑树的最小结点。
        RBTNode<T>* minimum(RBTNode<T>* tree);
        // 查找最大结点：返回tree为根结点的红黑树的最大结点。
        RBTNode<T>* maximum(RBTNode<T>* tree);

        // 左旋
        void leftRotate(RBTNode<T>* &root, RBTNode<T>* x);
        // 右旋
        void rightRotate(RBTNode<T>* &root, RBTNode<T>* y);
        // 插入函数
        void insert(RBTNode<T>* &root, RBTNode<T>* node);
        // 插入修正函数
        void insertFixUp(RBTNode<T>* &root, RBTNode<T>* node);
        // 删除函数
        void remove(RBTNode<T>* &root, RBTNode<T> *node);
        // 删除修正函数
        void removeFixUp(RBTNode<T>* &root, RBTNode<T> *node, RBTNode<T> *parent);

        // 销毁红黑树
        void destroy(RBTNode<T>* &tree);

        // 打印红黑树
        void print(RBTNode<T>* tree, T key, int direction);

};

/* 
 * 对红黑树的节点(x)进行左旋转
 *
 * 左旋示意图(对节点x进行左旋)：
 *      px                              px
 *     /                               /
 *    x                               y                
 *   /  \      --(左旋)-->           / \                #
 *  lx   y                          x  ry     
 *     /   \                       /  \
 *    ly   ry                     lx  ly  
 *
 *
 */

// 每个节点拥有left,right和parent
// 左旋牵扯到的元素有px,x,y,ly
// 需要修改的有 px->left,px->right(两者选其一), x->parent,x->right,  y->parent,y->left, ly->parent

template<class T>
void RBTree<T>::leftRotate(RBTNode<T>* &root, RBTNode<T>* x)// root是被左旋子树的父节点
{
     RBTNode<T> *y = x->right;
     x->right = y->left;
     if(y->left != NULL)
        y->left->parent = x;

     y->parent = x->parent;
     if(x->parent == NULL)
     //如果x的父节点是空节点，则将y设置为根节点
     root = y;
     else
     {
         if(x->parent->left ==x)//如果x是其父节点的左孩子，则将y设置为父节点的左孩子
            x->parent->left == y;
            else
            {
                x->parent->right==y;
            }
            y->left =x;
            x->parent = y;
     }
}

/* 
 * 对红黑树的节点(y)进行右旋转
 *
 * 右旋示意图(对节点y进行左旋)：
 *            py                               py
 *           /                                /
 *          y                                x                  
 *         /  \      --(右旋)-->            /  \                     #
 *        x   ry                           lx   y  
 *       / \                                   / \                   #
 *      lx  rx                                rx  ry
 * 
 */
template<class T>
void RBTree<T>::rightRotate(RBTNode<T>* &root, RBTNode<T> *y)// root使用引用，避免产生临时变量
{
    RBTNode<T> *x = y->left;
    y->left = x->right;
    x->right->parent = y;
    x->right = y;

    if(y->parent != NULL)
    root = x;
    else{
    if(y->parent->left == y)//左孩子
    y->parent->left =x;
    else//右孩子
    y->parent->right=x;
      }
    y->parent = x;
       
}


/* 
 * 将结点插入到红黑树中
 *
 * 参数说明：
 *     root 红黑树的根结点
 *     node 插入的结点        // 对应《算法导论》中的node
 */

/*
    1. 红黑树是一个平衡二叉树
    2. 每个节点不是黑色就是红色
    3. 根节点是黑色
    4. 每个叶子节点是黑色
    5. 每个红色节点的两个子节点一定都是黑色->红色子节点的父节点一定是黑色
    6. 任意一个结点到每个叶子节点的路径都包含数量相同的黑节点
    
    搜索树的插入操作永远是在叶子节点扩容


    将结点插入到红黑树中，首先将红黑树当做一个搜索二叉树进行插入操作，即此时只是插入操作，并不进行旋转和变色操作
    
    当插入后的结点N其父节点为黑色时，满足2,3,5,6

    当插入后的结点N其父节点P为红色时,P的父节点G一定是黑色(如果G为红色，则根据5，P应该为黑色)
    在上述情况下：当G的另一个子节点U，即N的叔结点为红色时，条件5不满足
    由于N结点是红色，而其父节点P也是红色，因此P原来的子树不需要做任何变化
    结点P因条件5必须变为黑色，由于新增了一个黑色结点，根据条件6，叔结点U(如果存在)需要变为黑色
    而结点G需要变为红色，若G为黑色则，G的父节点的左右子树其黑色结点数量不同，因此G需要变为红色，
    G的父节点需要变为黑色，这个P节点从红色变为黑色的情况相同，因此需要递归向上排列

    当插入的节点N其父节点P为红色，叔节点为黑色，N是P的左孩子，且结点P是G的左孩子，此时对G进行右旋，调整P
    和G的位置，并互换颜色，此时为一个红黑树

    当插入的结点N其父节点P为红色，叔结点为黑色，N是P的右孩子，且结点P是G的左孩子，此时以P结点为根节点进行左旋，
    此时就变为上面的一种情况

*/

template <class T>
void RBTree<T>::insert(RBTNode<T>* &root, RBTNode<T>* node)
{
    RBTNode<T> *y = NULL;
    RBTNode<T> *x = root;

    // 1.将红黑树当做一棵二叉查找树，将结点添加到二叉查找树中
    while(x != NULL)
    {
        y = x;
        if(node->key < x->key)
        x=x->left;
        else
        x=x->right;
    }

    node->parent = y;
    
    if(y!=NULL)
    {
        if(node->key < y->key)
        y->left = node;
        else
        y->right = node;
        
    }
    else // root是空节点
    {
        root = node;
    }

    // 2. 设置节点的颜色为红色
    node->color = RED;

    // 3. 将它重新修正为一棵二叉查找树
    insertFixUp(root,node);
    
}

template<class T>
void RBTree<T>::insert(T key)
{
    RBTNode<T> *z=NULL;
    //如果新建节点失败，则返回
    if(z = new RBTNode<T>(key,BLACK,NULL,NULL,NULL)==NULL)
    return ;
    insert(mRoot,z);
}

/*
 * 红黑树插入修正函数
 *
 * 在向红黑树中插入节点之后(失去平衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 *
 * 参数说明：
 *     root 红黑树的根
 *     node 插入的结点        // 对应《算法导论》中的z
 */

  // 1.将红黑树作为一棵二叉查找树，将节点插入
  // 2.将插入的节点变为红色



template <class T>
void RBTree<T>::insertFixUp(RBTNode<T>* &root, RBTNode<T>* node)
{ 
    RBTNode<T> *parent, *gparent;

    // 若“父节点存在，并且父节点的颜色是红色”
    while ((parent = rb_parent(node)) && rb_is_red(parent))
    {
        gparent = rb_parent(parent);

        //若“父节点”是“祖父节点的左孩子”
        if (parent == gparent->left)
        {
            // Case 1条件：叔叔节点是红色
            {
                RBTNode<T> *uncle = gparent->right;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;//跳过下面的判断，进行上面的while循环判断
                }
            }

            // Case 2条件：叔叔是黑色，且当前节点是右孩子
            if (parent->right == node)
            {
                RBTNode<T> *tmp;
                leftRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3条件：叔叔是黑色，且当前节点是左孩子。
            rb_set_black(parent);
            rb_set_red(gparent);
            rightRotate(root, gparent);
        } 
        else//若“z的父节点”是“z的祖父节点的右孩子”
        {
            // Case 1条件：叔叔节点是红色
            {
                RBTNode<T> *uncle = gparent->left;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2条件：叔叔是黑色，且当前节点是左孩子
            if (parent->left == node)
            {
                RBTNode<T> *tmp;
                rightRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3条件：叔叔是黑色，且当前节点是右孩子。
            rb_set_black(parent);
            rb_set_red(gparent);
            leftRotate(root, gparent);
        }
    }

    // 将根节点设为黑色
    rb_set_black(root);
}

/* 
 * 删除结点(node)，并返回被删除的结点
 *
 * 参数说明：
 *     root 红黑树的根结点
 *     node 删除的结点
 */
template <class T>
void RBTree<T>::remove(RBTNode<T>* &root, RBTNode<T> *node)
{
    RBTNode<T> *child, *parent;
    RBTColor color;

    // 被删除节点的"左右孩子都不为空"的情况。
    if ( (node->left!=NULL) && (node->right!=NULL) ) 
    {
        // 被删节点的后继节点。(称为"取代节点")
        // 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
        RBTNode<T> *replace = node;

        // 获取后继节点
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        // "node节点"不是根节点(只有根节点不存在父节点)
        if (rb_parent(node))
        {
            if (rb_parent(node)->left == node)
                rb_parent(node)->left = replace;
            else
                rb_parent(node)->right = replace;
        } 
        else 
            // "node节点"是根节点，更新根节点。
            root = replace;

        // child是"取代节点"的右孩子，也是需要"调整的节点"。
        // "取代节点"肯定不存在左孩子！因为它是一个后继节点。
        child = replace->right;
        parent = rb_parent(replace);
        // 保存"取代节点"的颜色
        color = rb_color(replace);

        // "被删除节点"是"它的后继节点的父节点"
        if (parent == node)
        {
            parent = replace;
        } 
        else
        {
            // child不为空
            if (child)
                rb_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == BLACK)
            removeFixUp(root, child, parent);

        delete node;
        return ;
    }

    if (node->left !=NULL)
        child = node->left;
    else 
        child = node->right;

    parent = node->parent;
    // 保存"取代节点"的颜色
    color = node->color;

    if (child)
        child->parent = parent;

    // "node节点"不是根节点
    if (parent)
    {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }
    else
        root = child;

    if (color == BLACK)
        removeFixUp(root, child, parent);
    delete node;
}

/* 
 * 删除红黑树中键值为key的节点
 *
 * 参数说明：
 *     tree 红黑树的根结点
 */
template <class T>
void RBTree<T>::remove(T key)
{
    RBTNode<T> *node; 

    // 查找key对应的节点(node)，找到的话就删除该节点
    if ((node = search(mRoot, key)) != NULL)
        remove(mRoot, node);
}

/*
 * 红黑树删除修正函数
 *
 * 在从红黑树中删除插入节点之后(红黑树失去平衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 *
 * 参数说明：
 *     root 红黑树的根
 *     node 待修正的节点
 */
template <class T>
void RBTree<T>::removeFixUp(RBTNode<T>* &root, RBTNode<T> *node, RBTNode<T> *parent)
{
    RBTNode<T> *other;

    while ((!node || rb_is_black(node)) && node != root)
    {
        if (parent->left == node)
        {
            other = parent->right;
            if (rb_is_red(other))
            {
                // Case 1: x的兄弟w是红色的  
                rb_set_black(other);
                rb_set_red(parent);
                leftRotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->right || rb_is_black(other->right))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rightRotate(root, other);
                    other = parent->right;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                leftRotate(root, parent);
                node = root;
                break;
            }
        }
        else
        {
            other = parent->left;
            if (rb_is_red(other))
            {
                // Case 1: x的兄弟w是红色的  
                rb_set_black(other);
                rb_set_red(parent);
                rightRotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->left || rb_is_black(other->left))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
                    rb_set_black(other->right);
                    rb_set_red(other);
                    leftRotate(root, other);
                    other = parent->left;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rightRotate(root, parent);
                node = root;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}