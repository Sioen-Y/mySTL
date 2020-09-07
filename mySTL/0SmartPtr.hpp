#include <cstdio>
#include <cstring>

class SmartPtr{
	public:
	int count;
	int *p;
	SmartPtr():count(1),p(new int()){} //heap堆上申请一块int内存单元
	~SmartPtr(){
		printf("%d smart_ptr deleted\n", *p);
		delete p;
	}
}

class Ptr{
	public:
	SmartPtr *sp;
	Ptr():sp(new SmartPtr()) {} // 默认构造函数
	Ptr(const Ptr& other):sp(other.sp){
		sp->count++;
	}
	
	Ptr operator=(const Ptr& other){ // 赋值操作符将other和本Ptr的sp设置为同一个，即Ptr p1; Ptr p2; p1=p2; 此时p1和p2的SmartPtr是同一块内存单元
		sp = other.sp;				 // 又由于SmartPtr开在heap上，即使某个临时变量p1被收回它的SmarkPtr还在，
		sp->count++;
		return *this;
	}
	
	~Ptr(){
		if(--sp->count == 0)
			delete sp;
	}
	
}