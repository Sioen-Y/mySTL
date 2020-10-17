// https://www.jianshu.com/p/6b526aa481b1

// max_heap每个节点都大于或等于其左子节点和右子节点
// 堆可以看成是数组，也可以看成是完全二叉树，堆就是利用完全二叉树结构来维护
// 的一维数组。

/*
	最大堆的根节点总是最大值，最小堆的根节点总是最小值
	堆常被当做优先队列使用，因为可以快速访问到“最重要”的元素

	大顶堆的最大元素是确定的，但其他节点的排序是未知的，最小元素未必是最后一个，
	只保证最小元素是叶子节点

	大顶堆和小顶堆不一定是有序数组，需要堆排序swap()

	heap.remove()删除的是根节点，将最后一个元素移动到根节点，重新进行最大堆build操作
	heap.insert()在堆的尾部插入数据，进行最大堆build操作
*/

/*
    1.满二叉树：层数为k,节点总数为2^k-1
    2.完全二叉树：二叉树深度为k，除第k层外，其他各层的节点都达到最大个数，第K层的所有节点都连续集中在最左边
    3.平衡二叉树：左子树和右子树深度之差的绝对值不超过1，且其左子树和右子树都是一颗平衡二叉树
*/
 
 // 11~15
 // 16~20已看
 // deque已看
 // stack,heap

// 创建大顶堆，假设heap[0]没有任何含义，只是帮助操作数组元素
// 以层排序的方式解析heap，假设int heap[7]={em,3,7,16,10,21,23}; 最后一个非叶子节点是heap[len/2+1],len表示堆的元素个数
// 节点heap[i]的左孩子和右孩子分别为heap[2*i],heap[2*i+1]
// vector<int> vct(heap,heap+sizeof(heap)/sizeof(int))
template<typename T>
void BuildMaxHeap(vector<T> heap, int len)
{
	bool flag = true;

	while (flag) {
		flag = false;
		for (int i = (len + 1) / 2; i >= 1; --i) {
			if ((2 * i <= len) && (heap[i] < heap[2 * i])) {
				flag = true;
				int tmp = heap[i];
				heap[i] = heap[2 * i];
				heap[2 * i] = tmp;
			}
			if (((2 * i + 1) <= len) && (heap[i] < heap[2 * i + 1]))
			{
				flag = true;
				int tmp = heap[i];
				heap[i] = heap[2 * i + 1];
				heap[2 * i + 1] = tmp;
			}
		}

	}
}


template<typename T>
void swap(vector<T>& heap,int len){
    T temp;
    temp = heap[1];
    heap[1] = heap[len];
    heap[len]=heap[1];
}

template<typename T>
void heaplist_create(vector<T> heap, int len){

    for(int i=len,i>=1;--i){
        BuildMaxHeap<T>(heap,len);
        swap<T>(heap,len); 
    }

}