/*
 *hashtable实现
 *
 *hashtable做为hash_*系列容器的底层容器
 */

#ifndef _STL_HASHTABLE_H_
#define _STL_HASHTABLE_H_

#include "1stl_alloctor.h"
#include "2stl_algobase.h"	//select1st less
#include "4stl_vector.h"
#include <utility>			//pair


namespace lfp {
namespace detail {

/* hashtable 节点结构 */
	template<class Value>
	struct __hashtable_node {
		Value val;				//键值和实值的合体叫做value
		__hashtable_node* next;
	};


/* hashtable 结构声明 */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc = alloc>
	struct hashtable;


/* hashtable 迭代器定义 */
	//注意：hashtable 的迭代器没有后退操作，也没有reverse iterator
	template<class Value, class Key, class HashFunc,
			 class ExtractKey, class EqualKey, class Alloc = alloc>
	struct __hashtable_iterator {
	public:
		typedef forward_iterator_tag		iterator_category;
		typedef Value						value_type;
		typedef Value*						pointer;
		typedef Value&						reference;
		typedef size_t						size_type;
		typedef ptrdiff_t					difference_type;
		typedef __hashtable_node<Value>		node_type;

		typedef __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>	self;
		typedef __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>	iterator;
		typedef __hashtable_iterator<const Value, const Key, HashFunc, ExtractKey, EqualKey, Alloc>	const_iterator;
		typedef hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>	HashTable;
		
		node_type* cur;		//迭代器目前所指结点
		HashTable* htb;		//指向hashtable，保持与容器之间的连结关系

	public:
		__hashtable_iterator() : cur(nullptr), htb(nullptr) { }
		__hashtable_iterator(node_type* _cur, HashTable* _htb) : cur(_cur), htb(_htb) { }
		__hashtable_iterator(const self& rhs) : cur(rhs.cur), htb(rhs.htb) { }

		// 这里的const指this指向的东西是const，这函数无法改动成员数据
		reference operator*() const { return cur->val; } // __hashtable_iterator example; *example;
		pointer operator->() const { return &(operator*()); }// __hashtable_iterator *example; example->...;

		iterator& operator++() {	//前置++
		/* operator++() 或者 iterator++寻找的是节点(即链表中某个节点)的下一个节点，使用链表方式访问cur=cur->next，当
		链表中下一个节点存在时跳转到下一个节点，若此当前节点已经是链表的尾结点，则跳转到下一个buckets，即下一个链表的头结点，
		若当前节点已经是hashtable的最后一个链表的尾结点，则operator()++不会进行任何操作*/
			const node_type* old = cur;
			cur = cur->next;		//如果存在就是他，否则进入以下 if 流程

			if (cur == nullptr)
			{
				/*
				size_type bucket_num(const value_type& obj) const {return bucket_num_key(get_key(obj));}	 
				size_type bucket_num_key(const key_type& key, size_t n) const {return hasher(key) % n;}				
				*/ 
				size_type bucket = htb->bucket_num(old->val);	//找出当前元素所在的桶

				/* 寻找当前桶之后第一个存在元素的桶，其第一个元素即为所求 */
				// 建议下面添加判断bucket是否已经超出buckets.size()，如果是则不进行++bucket操作且直接退出
				while (++bucket < htb->buckets.size()) { // buckets是hashtable的成员变量，且是vector类型
					if (htb->buckets[bucket] != nullptr) {
						cur = htb->buckets[bucket];
						break;
					}
				}
			}

			return *this;
		}
		
		iterator operator++(int) {	//后置++
			iterator new_node = *this;// this是个指针，指向当前object，static没有指针，它不隶属于某个实例化对象,*this这里指iterator
			++*this;		//调用operator++
			return new_node;
		}

		bool operator==(const iterator& rhs) { return (cur == rhs.cur); }// node_type* cur;		//迭代器目前所指结点,cur是地址
		bool operator!=(const iterator& rhs) { return (cur != rhs.cur); }
	};

	


	//用于维护hashtanle桶子个数的质数数组（两相邻数之间大致成两倍关系）
	static const int __stl_num_primes = 28;
	static const unsigned long __stl_prime_list[__stl_num_primes] = {
		53,         97,           193,         389,       769,
		1543,       3079,         6151,        12289,     24593,
		49157,      98317,        196613,      393241,    786433,
		1572869,    3145739,      6291469,     12582917,  25165843,
		50331653,   100663319,    201326611,   402653189, 805306457,
		1610612741, 3221225473ul, 4294967291ul
	};

	// unsigned int: 32 bit
	// unsigned long: 32 bits
	// unsigned long long: 64 bit
	// size_t: 64 bit

	//以下函数找出以上数组中不小于n的最小的质数，二分法查找
	inline unsigned long __stl_next_prime(unsigned long n) {
		const unsigned long* first = __stl_prime_list;// 64位系统地址由32位，unsigned long长为32位
		const unsigned long* last = __stl_prime_list + __stl_num_primes;
		const unsigned long* pos = lower_bound(first, last, n);

		return pos == last ? *(last - 1) : *pos;
	}



/* hashtable 结构定义 */
	/*	Value:		键和值的合体叫做Value
	 *	Key:		节点键的类别
	 *	HashFcn:	hash函数
	 *	ExtractKey:	如何从Value里面提取出Key(函数或者仿函数)
	 *	EqualKey:	如何比较key(函数或者仿函数)
	 *	Alloc:		内存分配
	 */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	struct hashtable {
	public:
		typedef typename __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::iterator			iterator;
		typedef typename __hashtable_iterator<const Value, const Key, HashFunc, ExtractKey, EqualKey, Alloc>::const_iterator	const_iterator;
		typedef Value			value_type;
		typedef Key				key_type;
		typedef size_t			size_type;
		typedef Value*			pointer;
		typedef const Value*	const_pointer;
		typedef Value&			reference;
		typedef const Value&	const_reference;
		typedef ptrdiff_t		difference_type;
		
		// 友元类，__hashtable_iterator中的所有成员函数都可以访问hashtable对象的私有成员
		friend class __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>;	 //访问buckets

	private:
		typedef __hashtable_node<Value>	 node_type;
		typedef simple_alloc<node_type, Alloc>	node_allocater;
		typedef hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc> self;

		//bucket 聚合体，以vector实现便于扩展
		vector<node_type*, Alloc> buckets;
		size_type	num_elements;			//元素个数

		HashFunc	hasher;			//hash函数将键值映射为一个数值
		EqualKey	key_equals;		//键值判等方法
		ExtractKey	get_key;		//从元素中提取出键值

	protected:
		/* 配置一个新节点 */
		node_type* create_node(const value_type& x) {
			node_type* node = node_allocater::allocate();
			node->next = nullptr;
			try {
				construct(&node->val, x);
				return node;
			}
			catch (...) {
				node_allocater::deallocate(node);
				throw;
			}
		}
		/* 销毁一个节点 */
		void delete_node(node_type* node) {
			destroy(&node->val);// destroy(&(node->val));
			node_allocater::deallocate(node);
		}

		/* 判断是否需要重建表格，如果不需要就立刻返回，否则重建一个更大的表格 */
		void resize(size_type num_elements_hint);

		/* 在不重建表格的情况下插入节点。键值不允许重复 */
		// bool用于表征是否有相同键值的元素插入，有则为false，否则为true
		std::pair<iterator, bool> insert_unique_noresize(const value_type& x);

		/* 在不重建表格的情况下插入节点。键值允许重复 */
		iterator insert_equal_noresize(const value_type& x);

		/* 实际执行删除元素的操作 */
		void __erase(const value_type& x);

		/* nest_size 返回最接近 n 且不小于 n 的质数 */
		size_type next_size(size_type n) const { return __stl_next_prime(n); }

	public:
		/* 计算元素 x 落脚点的函数 */

		//版本1：接受实值（value）和 buckets 个数
		size_type bucket_num(const value_type& obj, size_t n) const {
			return bucket_num_key(get_key(obj), n);			//调用版本4
		}
		//版本2：只接受实值
		size_type bucket_num(const value_type& obj) const {
			return bucket_num_key(get_key(obj));			//调用版本3
		}
		//版本3：只接受键值
		size_type bucket_num_key(const key_type& key) const {
			return bucket_num_key(key, buckets.size());		//调用版本4
		}
		//版本4：接受键值和 buckets 个数
		size_type bucket_num_key(const key_type& key, size_t n) const {
			return hasher(key) % n;// hasher是函数指针
		}

		//桶子个数即vector大小
		size_type bucket_count() const { return buckets.size(); }
		
		/* 计算 n 号桶的元素个数 */
		// 每个桶子存放的是每个链表的头结点，n号桶元素个数指的是n号桶对应的链表其长度
		int elemts_in_buckets(size_type n) {
			int count = 0;
			node_type* first = buckets[n];// n号桶头结点

			while (first != nullptr) {
				++count;
				first = first->next;
			}
			return count;
		}

		HashFunc hash() const {
			return hasher;// hasher是函数指针
		}

		EqualKey equals() const {
			return key_equals;
		}

		size_type max_bucket_count() const {
			return __stl_prime_list[__stl_num_primes - 1];	 //4294967291ul
		}

	public:
		// HashFunc	hasher;			    //hash函数将键值映射为一个数值
		// EqualKey	key_equals;		    //键值判等方法
		// ExtractKey	get_key;		//从元素中提取出键值
		hashtable(size_type n, const HashFunc& hf, const EqualKey& eql)
		  : buckets(next_size(n), nullptr), hasher(hf), key_equals(eql), get_key(ExtractKey())// vector buckets使用(next_size(n),nullptr初始化)，类似于vector v1(next_size(n), nullptr);
		{
			num_elements = 0;
		}

		iterator begin() {//找到buckets中第一个不为空的元素，该元素是个hashNode指针
			for (int i = 0; i < buckets.size(); ++i) {
				if (buckets[i] != nullptr) {
					return iterator(buckets[i], this);// 调用hash表迭代器构造函数 __hashtable_iterator(node_type* _cur, HashTable* _htb) : cur(_cur), htb(_htb) { }
				}
			}
			return end();
		}

		// node_type* cur;		//迭代器目前所指结点
		// HashTable* htb;		//指向hashtable，保持与容器之间的连结关系
		const_iterator begin() const {// buckets的每个元素是各个链表的头结点
			for (int i = 0; i < buckets.size(); ++i) {// vector<node_type*, Alloc> buckets;
				if (buckets[i] != nullptr) {
					// typedef typename __hashtable_iterator<const Value, const Key, HashFunc, ExtractKey, EqualKey, Alloc>::const_iterator	const_iterator;
					return const_iterator(buckets[i], this);// __hashtable_iterator(const self& rhs) : cur(rhs.cur), htb(rhs.htb) { }使用(buckets[i], this)构造__hashtable_iterator
				}
			}
			return end();
		}


		iterator end() { return iterator(nullptr, this); }
		const_iterator end() const { return const_iterator(nullptr, this); }

		//size返回元素个数而不是桶子个数
		size_type size() const { return num_elements; }
		bool empty() { return num_elements == 0; }

		/* 查找键值为 key 的元素 */
		iterator find(const key_type& key) {
			size_type n = bucket_num_key(key);		//找到 key 落在哪个桶内， size_t n = hasher(key)%buckets.size()

			node_type* cur = buckets[n]; // buckets是一个vector类型变量，其中每个元素为一个链表的头结点
			while (cur) {// cur!=NULL
			//链表中存放的元素其key值也可以不是原元素key值，也可以是经过转译后的值，这里需要经过翻译
			// get_key(cur->val)是翻译链表中的元素其key值
				if (key_equals(get_key(cur->val), key))// key_equals是个函数 typedef int (*fun)(int)
					return iterator(cur, this);//构造一个iterator，通过*iterator访问到它的值
				cur = cur->next;
			}
			return end();// 应该是 return iterator(cur,this)
		}
		const_iterator find(const key_type& key) const {
			size_type n = bucket_num_key(key);		//找到 key 落在哪个桶内

			node_type* cur = buckets[n];
			while (cur) {
				if (key_equals(get_key(cur->val), key))
					return const_iterator(cur, this);
				cur = cur->next;
			}
			return end();//应该是 return const_iterator(cur,this)
		}

		//查找当前键值为key的元素所在的链表，其剩余元素（即此元素到对应链表的尾结点）中也符合键值为key的元素有多少个
		size_type count(const key_type& key) {
			int result = 0;
			iterator it = find(key);

			if (it != end())// iterator end() { return iterator(nullptr, this); }
			{
				node_type* cur = it.cur;
				while (cur) {
					if (key_equals(get_key(cur->val), key))
						++result;
					cur = cur->next;
				}
			}
			return result;
		}

		/* 插入元素，不允许重复 */
		std::pair<iterator, bool> insert_unique(const value_type& x) {
			resize(num_elements + 1);		//判断是否需要重建表格
			return insert_unique_noresize(x);
		}

		//将iterator first到last的头结点元素插入到旧表中
		template<class InputIterator>
		void insert_unique(InputIterator first, InputIterator last) {
			for (; first != last; ++first)
				insert_unique(*first);
		}

		/* 插入元素，允许重复 */
		iterator insert_equal(const value_type& x) {
			resize(num_elements + 1);		//判断是否需要重建表格
			return insert_equal_noresize(x);
		}
		template<class InputIterator>
		void insert_equal(InputIterator first, InputIterator last) {
			for (; first != last; ++first)
				insert_equal(*first);
		}

		/* 删除元素 */
		void erase(iterator it) { __erase(*it); }
		void erase(node_type* node) { __erase(node->val); }
		void erase(const value_type& x) { __erase(x); }
		template<class InputIterator>
		void erase(InputIterator first, InputIterator last) {
			while (first != last) {
				InputIterator tmp = first++;
				__erase(*tmp);		//注意不能直接使用 first 进行删除，这会使 first 失效
			}
		}

		/* 清空 hasher table */
		void clear();

		/* 复制 rhs 到本hashtable */
		void copy_from(const hashtable& rhs);

		void swap(self& rhs) {
			buckets.swap(rhs.buckets);		//聚合体（vector）只需交换内部的指针即可
			swap(num_elements, rhs.num_elements);
		}
	};



	/* 清空 hashtable */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::clear() {
		size_type sz = buckets.size();

		for (size_type i = 0; i < sz; ++i) {		//对 buckets 的每一个元素进行处理
			node_type* cur = buckets[i];
			while (cur) {
				node_type* next = cur->next;
				delete_node(cur);
				cur = next;
			}
			buckets[i] = nullptr;
		}
		num_elements = 0;//如果delete_node中间出现崩溃，无法知道还有多少num_element需要删除，建议放在delete_node之后， --num_elements
		//注意：clear操作并未释放buckets vector 的空间
	}

	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable& rhs) {
		size_type rhs_size = rhs.buckets.size();

		//先将自己的 buckets 清空
		clear();
		buckets.reserve(rhs_size);		//在buckets中预留rhs_size个空间
		buckets.insert(buckets.end(), rhs_size, nullptr);	//这时候因为buckets已清空，实际就是在头部插入

		try {
			for (size_type i = 0; i < rhs_size; ++i) {
				//复制每一个存在的串行
				node_type* cur = rhs.buckets[i];
				if (cur != nullptr)
				{
					node_type* copy = create_node(cur->val);
					buckets[i] = copy;

					/* 复制当前串行的每一个节点 */
					for (cur = cur->next; cur != nullptr; cur = cur->next) {
						copy->next = create_node(cur->val);
						copy = copy->next;
					}
				}
			}
			num_elements = rhs.num_elements;
		}
		catch (...) {
			clear();
		}
	}


	// 注意：扩大聚合体时不能简单的将桶子内容复制过去，必须要rehash
	
	/* 以下函数判断是否需要重建表格，由以下可以看出元素个数最多与桶子个数相同 */
	/*本程序的做法是当hashtable中元素个数大于buckets.size时就会扩容，另一种推荐的做法是当hashtable中
	元素个数超过buckets.size()*loadFactor时，就会进行扩容，默认值为0.75，也就是说一个大小为16的vector，当
	插入元素超过12个时就会扩容，如果能预知元素个数选取大小合适的map能够提高hashtable性能*/
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint)
	{
		const size_type old_size = buckets.size();//返回vector的size，有53,97,193,389,769等
		
		if (num_elements_hint > old_size)
		{
			const size_type new_size = next_size(num_elements_hint);	//找出下一个质数，即大于num_elements_hint的最小质数
			vector<node_type*, Alloc> tmp_buckets(new_size, nullptr);	//建立一个临时的 buckets 用于保存原始 rehash 的内容
			
			for (int i = 0; i < old_size; ++i) {
				node_type* first = buckets[i];

				//以下rehash每一条开链的每一个元素
				while (first != nullptr) {//直到链表结束
					//确定在就的buckets里的各个first节点在新buckets中应该处于什么位置
					size_type bucket = bucket_num(first->val, new_size);	//找到first所属桶子，return hasher(key) % n;// hasher是函数指针

					//注意以下实现技巧
					//逐个判断链表中元素应该放置在什么位置上
					buckets[i] = first->next;
					first->next = tmp_buckets[bucket];//链表头插法，适用于空表插入新元素
					tmp_buckets[bucket] = first;
					first = buckets[i];
				}
			}

			buckets.swap(tmp_buckets);//将tmp_buckets中的元素复制到buckets中
		}
	}

	/* 在不重建表格的情况下插入节点。键值不允许重复 */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	std::pair<typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::iterator, bool>
	hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::insert_unique_noresize(const value_type& obj) {
		const size_type n = bucket_num(obj);	//找到 obj 应该插入的桶子
		node_type* first = buckets[n];			//令 first 指向对应串行的头部

		/* 查看当前串行中是否存在和 obj 相同的元素 */
		for (node_type* cur = first; cur != nullptr; cur = cur->next) {
			if (key_equals(get_key(cur->val), get_key(obj)))
				return std::pair<iterator, bool>(iterator(cur, this), false);
			//如存在相同的元素则立即返回
		}

		/* 进行到此说明当前串行为空或无重复元素，可进行插入 */
		node_type* new_node = create_node(obj);
		new_node->next = first;//头插法
		buckets[n] = new_node;
		++num_elements;
		return std::pair<iterator, bool>(iterator(new_node, this), true);
	}

	/* 在不重建表格的情况下插入节点。键值允许重复 */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::iterator
	hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type& obj) {
		const size_type n = bucket_num(obj);	//找到 obj 应该插入的桶子
		node_type* first = buckets[n];			//令 first 指向对应串行的头部

		/* 查找串行中是否存在与 obj 相同的元素，如果有则立即插入 */
		for (node_type* cur = first; cur != nullptr; cur = cur->next) {
			if (key_equals(get_key(cur->val), get_key(obj))) {
				node_type* new_node = create_node(obj);
				new_node->next = cur->next;//将相同元素的obj放在一起
				cur->next = new_node;
				++num_elements;
				return iterator(new_node, this);
			}
		}

		/* 进行到此说明当前串行为空或无重复元素 */
		//没有重复的元素则适用头插法，如果有重复的元素则将重复的元素放在一起
		node_type* new_node = create_node(obj);
		new_node->next = first;
		buckets[n] = new_node;
		++num_elements;
		return iterator(new_node, this);
	}

	/* 实际执行删除元素的操作 */
	// 该删除操作会删除所有符合条件的元素
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::__erase(const value_type& x) {
		size_type n = bucket_num(x);
		node_type* cur = buckets[n];
		if (cur != nullptr)//此时的cur是链表中第一个符合条件的元素指针
		{
			if (key_equals(get_key(cur->val), get_key(x))) {
				buckets[n] = cur->next;//直接指向下一个节点
				delete_node(cur);
				--num_elements;
				return;
			}
	//查找链表中是否还有其他符合条件的元素，循环删除
			while (cur->next != nullptr)
			{
				if (key_equals(get_key(cur->next->val), get_key(x))) {
					node_type* tmp_node = cur->next;
					cur->next = tmp_node->next;
					delete_node(tmp_node);
					--num_elements;
					return;
				}
				cur = cur->next;
			}
		}
	}

} // end of namespace detail

} // end of namespace lfp

#endif	//! _STL_HASHTABLE_H_