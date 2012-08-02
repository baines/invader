#ifndef _RINGBUFF_H_
#define _RINGBUFF_H_
#include <cstdlib>
#include <cstring>
#include <cstdio>

template<typename T>
class Ringbuff;

template<typename T>
class Ringbuff<T *> {
	public:
		Ringbuff(){
			init(32);
		}
		Ringbuff(size_t n){
			init(n);
		}
		void push(T* item){
			if(elems == b_size){	
				resize(b_size * 2);
			}
			*b_tail = item;
			if(b_tail == (buffer + (b_size-1))){
				b_tail = buffer;
			} else {
				b_tail++;
			}
			elems++;
		}
		T* pop(){
			if(elems == 0) return (T*)0;
			T* item = *b_head;
			if(b_head == (buffer + (b_size-1))){
				b_head = buffer;
			} else {
				b_head++;
			}
			elems--;
			return item;
		}
		bool empty() const { return (elems == 0); }
		size_t size() const { return elems;	}
		T* front() const { return *b_head; }
		~Ringbuff(){ free(buffer); }
			
	private:
		void init(size_t num){
			buffer = (T**) malloc(num * sizeof(T*));
			memset(buffer, 0, num*sizeof(T*));
			b_head = b_tail = buffer;
			b_size = num;
			elems = 0;
		}
		void resize(size_t new_size){
			T** tmp = (T**) malloc(new_size * sizeof(T*));
			size_t amount = ((buffer + b_size) - b_head);
			memcpy(tmp, b_head, amount*sizeof(T*));
			memcpy(tmp+amount, buffer, (b_tail - buffer)*sizeof(T*));
			free(buffer);
			buffer = tmp;
			b_head = buffer;
			b_tail = buffer + b_size;
			b_size = new_size;
		}
		T **buffer, **b_head, **b_tail;
		size_t b_size, elems;
};
#endif
