//简易的空间配置器实现

#ifndef _JJALLOC_H_
#define _JJALLOC_H_

#include <iostream>		//for std::cerr
#include <new>			//for placement new
#include <climits>		//for UINT_MAX
#include <cstddef>		//for ptrdiff_t, size_t
#include <cstdlib>		//for exit()

namespace JJ {			//第一级namespace是提供给外部用户使用的（后同）
namespace detail {		//第二级namespace是stl内部使用的实现细节（后同）

	//申请一块内存但不构造对象
	template<class T>
	inline T* _allocate(ptrdiff_t size, T*) {
		std::set_new_handler(0);	//设置new失败时的处理函数为默认行为（抛出bad_alloc异常）
		T*  tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
		if (tmp == nullptr) {
			std::cerr << "out of memory" << std::endl;
			exit(1);
		}

		return tmp;
	}

	//释放ptr指向的内存
	template<class T>
	inline void _deallocate(T* ptr) {
		::operator delete(ptr);
	}

	//在ptr所指的内存上构造一个T2对象
	template<class T1, class T2>
	inline void _construct(T1* ptr, const T2& value) {
		new(ptr) T1(value);// placement new
	}

	//析构ptr所指内存上的对象
	template<class T>
	inline void _destroy(T* ptr) {
		ptr->~T();
	}

}// end of namespace detail

	//简易的空间配置器
	template<class T>
	class allocator {
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

		//rebind allocator of type U
		template<class U>
		struct rebind {
			typedef allocator<U> other;
		};

		pointer allocate(size_type n, const void* hint = 0) { // allocator::allocate
			return detail::_allocate((difference_type)n, (pointer)0);
		}

		void deallocate(pointer p, size_type n) {// deallocate的参数n没有起到作用
			detail::_deallocate(p);
		}

		void construct(pointer p, const T& value) {
			detail::_construct(p, value);
		}

		void destroy(pointer p) {
			detail::_destroy(p);
		}

		pointer address(reference x) { return (pointer)&x; }

		const_pointer const_address(const_reference x) { return (const_pointer)&x; }

		size_type max_size() const { return size_type(UINT_MAX / sizeof(T)); } // UINT_MAX=0xffffffff 长度为四个字节，对应32/64位系统最大寻址范围，32/64位系统内存单元地址以4个字节表示，内存最大空间：int(0xffffffff)*8 (bit)
	};

}// end of namespace JJ


#endif // !_JJALLOC_H_

