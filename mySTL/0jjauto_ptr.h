//简易的auto_ptr实现，为iterator的设计提供参考

#ifndef _JJAUTOPTR_H_
#define _JJAUTOPTR_H_


namespace JJ {

	template<class T>
	class auto_ptr { //智能指针,C++11已被废弃，使用unique_ptr
	public:
		explicit auto_ptr(T* p = nullptr) : pointee(p) { } // explicit(显式)构造函数，防止单参数构造函数隐式转换
		template<class U>
		auto_ptr(auto_ptr<U>& rhs) : pointee(rhs.release()) { } // 拿到了rhs对应的pointee所指向的内存单元的控制权
		~auto_ptr() { delete pointee; }

		template<class U>
		auto_ptr<T>& operator=(auto_ptr<U>& rhs) {
			if (this != &rhs)
				reset(rhs.release());
			return *this;
		}
		T& operator*() const { return *pointee; } // auto_ptr* a;  *a; 这里const不同于const T&, 指的是不能修改非静态数据成员的值
		T* operator->() const { return pointee; } // auto_ptr* a;  a->
		T* get() const { return pointee; }
		T* release() { // release()释放了对pointee指向的内存单元的控制，转而将控制权返回，pointee清空
			T* tmp = pointee;
			pointee = nullptr;
			return tmp;
		}

	private:
		T* pointee;
	};

}// end of namespace JJ

#endif // !_JJAUTOPTR_H_