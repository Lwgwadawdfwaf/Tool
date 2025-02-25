#ifndef _TOOL_QUEUE_
#define _TOOL_QUEUE_

#define _HASH_NODE_SIZE_ 2//初始化节点数量
#define _NODE_DATA_SIZE_ 256//初始化节点数据数量
#define _MAX_NODE_DATA_SIZE_ 4096//最大节点数据数量

#include<iostream>
#include<cassert>

//类名和自定义类型名:AbCd
//一般变量名:ab_cd
//类的变量名:m_ab_cd
//函数名:abCd

namespace lwg {//命名空间

//队列
	template<typename T>
	class Queue {
	private:
		//哈希桶的节点
		template<typename T>
		class BucketNode {
		public:
			//构造函数,初始化作用
			BucketNode(BucketNode* next_addr, int capacity)
				: m_next_addr(next_addr), m_capacity(capacity) {
				//m_array_head_addr = new T[capacity];会因为T没有默认构造函数报错
				while (NULL == (m_array_head_addr = (T*)malloc(m_capacity * sizeof(T))));
				m_data_size = 0;
				m_add_offset = -1;
				m_delete_offset = 0;
			}
			//析构函数
			~BucketNode() {
				//delete[] m_array_head_addr;
				free(m_array_head_addr);
			}
			//拷贝构造函数,复制队列时使用
			explicit BucketNode(BucketNode* copy_node)
				: m_next_addr(nullptr), m_capacity(copy_node->m_capacity) {
				while (NULL == (m_array_head_addr = (T*)malloc(m_capacity * sizeof(T))));
				m_data_size = copy_node->m_data_size;
				m_add_offset = copy_node->m_add_offset;
				m_delete_offset = copy_node->m_delete_offset;
				for (int i = m_delete_offset; i < m_add_offset + 1; i++) {
					m_array_head_addr[i] = copy_node->m_array_head_addr[i];
				}
			}
			//拷贝函数,不需要使用
			BucketNode<T>& operator=(BucketNode<T>& hash_bucket) = delete;
			//增加数据
			bool pushBack(T data) {
				if (m_add_offset == m_capacity - 1) {
					m_add_offset = -1;
					return true;
				}
				m_array_head_addr[++m_add_offset] = data;
				m_data_size++;
				return false;
			}
			//删除数据
			bool popFront() {
				m_delete_offset++;
				m_data_size--;
				if (m_delete_offset == m_capacity) {
					m_delete_offset = 0;
					return true;
				}
				return false;
			}
			//返回起始插入数据
			T front() {
				return m_array_head_addr[m_delete_offset];
			}
			//返回最后插入数据
			T back() {
				return m_array_head_addr[m_add_offset];
			}
			//清理节点空间
			void clean() {
				int new_capacity = m_capacity;
				while (new_capacity > _NODE_DATA_SIZE_) {
					if (new_capacity >> 1 < m_data_size)
						break;
					new_capacity = new_capacity >> 1;
				}
				T* new_arrdy_addr = new T[new_capacity];
				for (int i = 0; i < m_data_size; i++) {
					new_arrdy_addr[i] = m_array_head_addr[m_delete_offset++];
				}
				delete[] m_array_head_addr;
				m_array_head_addr = new_arrdy_addr;
				m_delete_offset = 0;
				m_add_offset = m_data_size - 1;
			}

		private:
			T* m_array_head_addr;//数组头地址
			int m_data_size;//数组元素个数
			int m_add_offset;//增加元素偏移量
			int m_delete_offset;//删除元素偏移量

		public:
			int m_capacity;//数组容积
			BucketNode* m_next_addr;//下一个节点地址
		};
		//哈希桶
		template<typename T>
		class HashBucket {
		public:
			//默认构造函数,初始化作用
			HashBucket() {
				//初始化节点数
				m_node_size = _HASH_NODE_SIZE_;
				//初始化有数据节点数
				m_have_data_node_size = 1;
				//初始化数据数
				m_data_size = 0;
				//初始化节点
				BucketNode<T>* next_addr = NULL;
				BucketNode<T>* head_node_addr = new BucketNode<T>(next_addr, _NODE_DATA_SIZE_);
				next_addr = head_node_addr;
				m_tail_node_addr = head_node_addr;
				for (int i = 1; i < _HASH_NODE_SIZE_; i++) {
					head_node_addr = new BucketNode<T>(next_addr, _NODE_DATA_SIZE_);
					next_addr = head_node_addr;
				}
				m_tail_node_addr->m_next_addr = head_node_addr;
				m_push_node_addr = head_node_addr;
				m_pop_node_addr = head_node_addr;
			}
			//析构函数
			~HashBucket() {
				BucketNode<T>* head_node_addr = m_tail_node_addr;
				m_tail_node_addr = m_tail_node_addr->m_next_addr;
				BucketNode<T>* node_copy = m_tail_node_addr->m_next_addr;
				while (head_node_addr != m_tail_node_addr) {
					delete m_tail_node_addr;
					m_tail_node_addr = node_copy;
					node_copy = node_copy->m_next_addr;
				}
				delete head_node_addr;
			}
			//拷贝构造函数,复制队列时使用
			explicit HashBucket(HashBucket<T>* hash_bucket) {
				m_node_size = hash_bucket->m_node_size;
				m_have_data_node_size = hash_bucket->m_have_data_node_size;
				m_data_size = hash_bucket->m_data_size;
				//拷贝节点
				m_pop_node_addr = new BucketNode<T>(hash_bucket->m_pop_node_addr);
				BucketNode<T>* next_node = hash_bucket->m_pop_node_addr;
				BucketNode<T>* new_next_node;
				BucketNode<T>* node_copy = m_pop_node_addr;
				while (next_node != hash_bucket->m_push_node_addr) {
					new_next_node = new BucketNode<T>(next_node->m_next_addr);
					next_node = next_node->m_next_addr;
					node_copy->m_next_addr = new_next_node;
					node_copy = node_copy->m_next_addr;
				}
				m_push_node_addr = node_copy;
				for (int i = 0; i < hash_bucket->m_node_size - hash_bucket->m_have_data_node_size; i++) {
					new_next_node = new BucketNode<T>(next_node->m_next_addr);
					next_node = next_node->m_next_addr;
					if (next_node == hash_bucket->m_tail_node_addr) {
						m_tail_node_addr = new_next_node;
					}
					node_copy->m_next_addr = new_next_node;
					node_copy = node_copy->m_next_addr;
				}
				node_copy->m_next_addr = m_pop_node_addr;
			}
			//拷贝函数,不需要使用
			HashBucket<T>& operator=(HashBucket<T>& hash_bucket) = delete;
			BucketNode<T>* m_tail_node_addr;//尾节点地址
			BucketNode<T>* m_push_node_addr;//插入数据节点地址
			BucketNode<T>* m_pop_node_addr;//删除数据节点地址

			int m_node_size;//节点数
			int m_have_data_node_size;//有数据节点数
			int m_data_size;//数据数

			//增加一个尾节点
			void addNode() {
				int capacity = m_tail_node_addr->m_capacity;
				if (capacity >= _MAX_NODE_DATA_SIZE_);
				else
					capacity = capacity << 1;
				m_tail_node_addr->m_next_addr = new BucketNode<T>(m_tail_node_addr->m_next_addr, capacity);
			}
		};

	public:
		Queue() {
			this->m_hash_bucket = new HashBucket<T>;
		}
		~Queue() {
			delete m_hash_bucket;
		}
		//拷贝构造函数,传递参数时会自动调用
		Queue(Queue<T>& queue) {
			this->m_hash_bucket = new HashBucket<T>(queue.m_hash_bucket);
		}
		//移动构造函数,explicit为了防止返回参数时调用此函数
		explicit Queue(Queue<T>&& queue) {
			this->m_hash_bucket = queue.m_hash_bucket;
		}
		//不允许赋值,想要复制就用拷贝构造
		void operator=(Queue<T>& queue) = delete;
		//插入数据
		void push(T data);
		//删除数据
		void pop();
		//返回首数据
		T front();
		//返回尾数据
		T back();
		//返回元素数量
		int size();
		//判断是否为空
		bool empty();
		//清理空间
		void cleanSpace();


	private:
		HashBucket<T>* m_hash_bucket;//创造一个hash桶作为队列
	};
	//插入数据
	template<typename T>
	void Queue<T>::push(T data) {
		//正常插入
		bool full = m_hash_bucket->m_push_node_addr->pushBack(data);
		m_hash_bucket->m_data_size++;
		if (!full) {//无事发生
		}
		else if (m_hash_bucket->m_push_node_addr == m_hash_bucket->m_tail_node_addr) {//到尾了
			//扩容并移动尾节点再插入
			m_hash_bucket->addNode();
			m_hash_bucket->m_tail_node_addr = m_hash_bucket->m_tail_node_addr->m_next_addr;
			m_hash_bucket->m_push_node_addr = m_hash_bucket->m_tail_node_addr;
			m_hash_bucket->m_have_data_node_size++;
			m_hash_bucket->m_node_size++;
			m_hash_bucket->m_push_node_addr->pushBack(data);
		}
		else {//没到尾
			m_hash_bucket->m_push_node_addr = m_hash_bucket->m_push_node_addr->m_next_addr;
			m_hash_bucket->m_have_data_node_size++;
			m_hash_bucket->m_push_node_addr->pushBack(data);
		}
	}
	//删除起始插入数据
	template<typename T>
	void Queue<T>::pop() {
		if (0 == m_hash_bucket->m_data_size)
			return;
		bool empty = m_hash_bucket->m_pop_node_addr->popFront();
		m_hash_bucket->m_data_size--;
		if (empty) {
			m_hash_bucket->m_have_data_node_size--;
			m_hash_bucket->m_pop_node_addr = m_hash_bucket->m_pop_node_addr->m_next_addr;
		}
	}
	//返回起始插入数据
	template<typename T>
	T Queue<T>::front() {
/*		if (0 == m_hash_bucket->m_data_size);
			return (T)0;报错	*/				
		assert(0 != m_hash_bucket->m_data_size);
		return m_hash_bucket->m_pop_node_addr->front();
	}
	//返回最后插入数据
	template<typename T>
	T Queue<T>::back() {
		/*if (0 == m_hash_bucket->m_data_size)
			return (T)0;*/
		assert(0 != m_hash_bucket->m_data_size);
		return m_hash_bucket->m_push_node_addr->back();
	}
	//返回数据数量
	template<typename T>
	int Queue<T>::size() {
		return m_hash_bucket->m_data_size;
	}
	//判断是否为空
	template<typename T>
	bool Queue<T>::empty() {
		if (0 == m_hash_bucket->m_data_size)
			return true;
		return false;
	}
	//清理空间
	template<typename T>
	void Queue<T>::cleanSpace() {
		BucketNode<T>* node_copy;
		for (int i = 0; i < m_hash_bucket->m_node_size - m_hash_bucket->m_have_data_node_size; i++) {
			node_copy = m_hash_bucket->m_push_node_addr->m_next_addr->m_next_addr;
			delete m_hash_bucket->m_push_node_addr->m_next_addr;
			m_hash_bucket->m_push_node_addr->m_next_addr = node_copy;
		}
		m_hash_bucket->m_node_size = m_hash_bucket->m_have_data_node_size;
		if (1 == m_hash_bucket->m_node_size) {
			m_hash_bucket->m_tail_node_addr->clean();
		}
	}

}//命名空间结束

#endif//_TOOL_QUEUE_