#ifndef _UTIL_H_
#define _UTIL_H_
#include <cstring>

namespace util {

    template<class A, class B>
	static const inline B min(const A a, const B b){ return a < b ? a : b; }
	template<class A, class B>
	static const inline B max(const A a, const B b){ return a > b ? a : b; }
	
	template<class InputIterator, class Function, class Arg>
	void forEach(InputIterator first, InputIterator last, Function f, Arg& a) {
		for ( ; first!=last; ++first ) ((*first)->*f)(a);
	}
	
	template<class InputIterator, class Function, class Arg>
	void forEach2(InputIterator first, InputIterator last, Function f, Arg& a) {
		for ( ; first!=last; ++first ) ((*first).*f)(a);
	}

	template <typename T, size_t N>
	char (&ArraySizeHelper(T (&array)[N]))[N];
	#define arraysize(array) (sizeof(ArraySizeHelper(array)))
	
	struct strLess {
		bool operator() (const char* a, const char* b){
			return strcmp(a, b) < 0 ? true : false;
		}
	};
	
	struct deleter {
		template<typename T> 
		void operator()(T* t){
			delete t;
			t = NULL;
		}
	};
}

#endif
