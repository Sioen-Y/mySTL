// https://www.jianshu.com/p/a3951ce7574d
#include <iostream>
#include <queue>
using namespace std;

class tree_node{
    //该tree不是二叉树，而是有三个子节点
    public:
    tree_node(int m):num(m),left(nullptr),mid(nullptr),right(nullptr){}
    tree_node():num(0),left(nullptr),mid(nullptr),right(nullptr){}
    int num;
    tree_node* left;
    tree_node* mid;
    tree_node* right;
    ~tree_node(){}
};

// 这里创建树,假设除头结点有8个{ 1,1,1,3,5,3,5,7 }，数组中的数字是对应的子节点的头结点

queue<tree_node *> ccl;//声明一个队列,用于存储树的节点
tree_node mytree[10];
int ar_tree[8] = { 1,1,1,3,5,3,5,7 };
tree_node * tree_ptr;

int main() {
	ios::sync_with_stdio(false);
	mytree[1] = tree_node(1);
	for (int i = 2; i <= 9; i++) {
		mytree[i] = tree_node(i);//重新构造
		tree_ptr= &mytree[ar_tree[i - 2]];
		if (tree_ptr->left == nullptr) {
			tree_ptr->left = &mytree[i];
		}
		else{
			if (tree_ptr->mid== nullptr) {
				tree_ptr->mid = &mytree[i];
			}
			else {
				tree_ptr->right= &mytree[i];
			}
		}
	}
	//把根节点放入队列中,队列是先进先出，保证最早被push的结点能够被先访问，而遍历时按照left,mid,right的顺序就会造成按层从左到右遍历
	ccl.push(&mytree[1]);
	while (!ccl.empty()) {//当队列不为空时，程序运行
		tree_ptr = ccl.front();//读取节点
		//cout << tree_ptr->num << endl;
		if (tree_ptr->left != nullptr) {
			ccl.push(tree_ptr->left);
		}
		if (tree_ptr->mid != nullptr) {
			ccl.push(tree_ptr->mid);
		}
		if (tree_ptr->right != nullptr) {
			ccl.push(tree_ptr->right);
		}
		cout << tree_ptr->num << " ";
		ccl.pop();
	}
	cout << endl;
	return 0;
