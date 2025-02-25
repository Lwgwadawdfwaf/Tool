#ifndef _TOOL_STACK_
#define _TOOL_STACK_

#include"pair.hpp"

template<typename T>
class Stack {
public:
	Stack() {
		while (NULL == (m_posArr = (T*)malloc(sizeof(T*) * 1024)));
		m_size = 0;
		m_capacity = 1024;
	}
	Stack(int capacity) {
		while (NULL == (m_posArr = (T*)malloc(sizeof(T*) * capacity)));
		m_size = 0;
		m_capacity = capacity;
	}
	~Stack() {
		free(m_posArr);
	}

	//��ջ
	bool push(T pos) {
		if (m_size == m_capacity)
			return false;
		m_posArr[m_size++] = pos;
		return true;
	}
	//��ջ
	T pop() {
		return m_posArr[--m_size];
	}
	//����ջ��Ԫ��
	T top() {
		return m_posArr[m_size];
	}
    //��ʼ���ݻ�
	void initCapacity(int capacity) {
		T* posArr;
		while (NULL == (posArr = (T*)malloc(sizeof(T*) * capacity)));
		if (capacity > m_size) {
			for (int i = 0; i < m_size; i++) {
				posArr[i] = m_posArr[i];
			}
		}
		else {
			for (int i = 0; i < capacity; i++) {
				posArr[i] = m_posArr[i];
			}
			m_size = capacity;
		}
		free(m_posArr);
		m_posArr = posArr;
		m_capacity = capacity;
	}

private:
    //��ջ�׵�ַ
	T* m_posArr;
	//��������
	int m_size;
	//��ջ�ݻ�
	int m_capacity;

};

#endif//_TOOL_STACK_
