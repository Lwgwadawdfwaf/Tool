#ifndef _TOOL_QUEUE_
#define _TOOL_QUEUE_

#define _HASH_NODE_SIZE_ 2//��ʼ���ڵ�����
#define _NODE_DATA_SIZE_ 256//��ʼ���ڵ���������
#define _MAX_NODE_DATA_SIZE_ 4096//���ڵ���������

#include<iostream>
#include<cassert>

//�������Զ���������:AbCd
//һ�������:ab_cd
//��ı�����:m_ab_cd
//������:abCd

namespace lwg {//�����ռ�

//����
	template<typename T>
	class Queue {
	private:
		//��ϣͰ�Ľڵ�
		template<typename T>
		class BucketNode {
		public:
			//���캯��,��ʼ������
			BucketNode(BucketNode* next_addr, int capacity)
				: m_next_addr(next_addr), m_capacity(capacity) {
				//m_array_head_addr = new T[capacity];����ΪTû��Ĭ�Ϲ��캯������
				while (NULL == (m_array_head_addr = (T*)malloc(m_capacity * sizeof(T))));
				m_data_size = 0;
				m_add_offset = -1;
				m_delete_offset = 0;
			}
			//��������
			~BucketNode() {
				//delete[] m_array_head_addr;
				free(m_array_head_addr);
			}
			//�������캯��,���ƶ���ʱʹ��
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
			//��������,����Ҫʹ��
			BucketNode<T>& operator=(BucketNode<T>& hash_bucket) = delete;
			//��������
			bool pushBack(T data) {
				if (m_add_offset == m_capacity - 1) {
					m_add_offset = -1;
					return true;
				}
				m_array_head_addr[++m_add_offset] = data;
				m_data_size++;
				return false;
			}
			//ɾ������
			bool popFront() {
				m_delete_offset++;
				m_data_size--;
				if (m_delete_offset == m_capacity) {
					m_delete_offset = 0;
					return true;
				}
				return false;
			}
			//������ʼ��������
			T front() {
				return m_array_head_addr[m_delete_offset];
			}
			//��������������
			T back() {
				return m_array_head_addr[m_add_offset];
			}
			//����ڵ�ռ�
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
			T* m_array_head_addr;//����ͷ��ַ
			int m_data_size;//����Ԫ�ظ���
			int m_add_offset;//����Ԫ��ƫ����
			int m_delete_offset;//ɾ��Ԫ��ƫ����

		public:
			int m_capacity;//�����ݻ�
			BucketNode* m_next_addr;//��һ���ڵ��ַ
		};
		//��ϣͰ
		template<typename T>
		class HashBucket {
		public:
			//Ĭ�Ϲ��캯��,��ʼ������
			HashBucket() {
				//��ʼ���ڵ���
				m_node_size = _HASH_NODE_SIZE_;
				//��ʼ�������ݽڵ���
				m_have_data_node_size = 1;
				//��ʼ��������
				m_data_size = 0;
				//��ʼ���ڵ�
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
			//��������
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
			//�������캯��,���ƶ���ʱʹ��
			explicit HashBucket(HashBucket<T>* hash_bucket) {
				m_node_size = hash_bucket->m_node_size;
				m_have_data_node_size = hash_bucket->m_have_data_node_size;
				m_data_size = hash_bucket->m_data_size;
				//�����ڵ�
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
			//��������,����Ҫʹ��
			HashBucket<T>& operator=(HashBucket<T>& hash_bucket) = delete;
			BucketNode<T>* m_tail_node_addr;//β�ڵ��ַ
			BucketNode<T>* m_push_node_addr;//�������ݽڵ��ַ
			BucketNode<T>* m_pop_node_addr;//ɾ�����ݽڵ��ַ

			int m_node_size;//�ڵ���
			int m_have_data_node_size;//�����ݽڵ���
			int m_data_size;//������

			//����һ��β�ڵ�
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
		//�������캯��,���ݲ���ʱ���Զ�����
		Queue(Queue<T>& queue) {
			this->m_hash_bucket = new HashBucket<T>(queue.m_hash_bucket);
		}
		//�ƶ����캯��,explicitΪ�˷�ֹ���ز���ʱ���ô˺���
		explicit Queue(Queue<T>&& queue) {
			this->m_hash_bucket = queue.m_hash_bucket;
		}
		//������ֵ,��Ҫ���ƾ��ÿ�������
		void operator=(Queue<T>& queue) = delete;
		//��������
		void push(T data);
		//ɾ������
		void pop();
		//����������
		T front();
		//����β����
		T back();
		//����Ԫ������
		int size();
		//�ж��Ƿ�Ϊ��
		bool empty();
		//����ռ�
		void cleanSpace();


	private:
		HashBucket<T>* m_hash_bucket;//����һ��hashͰ��Ϊ����
	};
	//��������
	template<typename T>
	void Queue<T>::push(T data) {
		//��������
		bool full = m_hash_bucket->m_push_node_addr->pushBack(data);
		m_hash_bucket->m_data_size++;
		if (!full) {//���·���
		}
		else if (m_hash_bucket->m_push_node_addr == m_hash_bucket->m_tail_node_addr) {//��β��
			//���ݲ��ƶ�β�ڵ��ٲ���
			m_hash_bucket->addNode();
			m_hash_bucket->m_tail_node_addr = m_hash_bucket->m_tail_node_addr->m_next_addr;
			m_hash_bucket->m_push_node_addr = m_hash_bucket->m_tail_node_addr;
			m_hash_bucket->m_have_data_node_size++;
			m_hash_bucket->m_node_size++;
			m_hash_bucket->m_push_node_addr->pushBack(data);
		}
		else {//û��β
			m_hash_bucket->m_push_node_addr = m_hash_bucket->m_push_node_addr->m_next_addr;
			m_hash_bucket->m_have_data_node_size++;
			m_hash_bucket->m_push_node_addr->pushBack(data);
		}
	}
	//ɾ����ʼ��������
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
	//������ʼ��������
	template<typename T>
	T Queue<T>::front() {
/*		if (0 == m_hash_bucket->m_data_size);
			return (T)0;����	*/				
		assert(0 != m_hash_bucket->m_data_size);
		return m_hash_bucket->m_pop_node_addr->front();
	}
	//��������������
	template<typename T>
	T Queue<T>::back() {
		/*if (0 == m_hash_bucket->m_data_size)
			return (T)0;*/
		assert(0 != m_hash_bucket->m_data_size);
		return m_hash_bucket->m_push_node_addr->back();
	}
	//������������
	template<typename T>
	int Queue<T>::size() {
		return m_hash_bucket->m_data_size;
	}
	//�ж��Ƿ�Ϊ��
	template<typename T>
	bool Queue<T>::empty() {
		if (0 == m_hash_bucket->m_data_size)
			return true;
		return false;
	}
	//����ռ�
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

}//�����ռ����

#endif//_TOOL_QUEUE_