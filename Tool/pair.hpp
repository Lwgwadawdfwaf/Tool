#ifndef _TOOL_PAIR_
#define _TOOL_PAIR_

#include<iostream>
#include<system_error>

//������
template<typename K, typename V>
class Pair {
public:
	Pair(K m_key, V m_data){
		this->m_key = m_key;
		this->m_data = m_data;
	}
	//��ֵ
	K m_key;
	//����
	V m_data;
};

#endif//_TOOL_PAIR_
