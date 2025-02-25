#ifndef _TOOL_PAIR_
#define _TOOL_PAIR_

#include<iostream>
#include<system_error>

//数据组
template<typename K, typename V>
class Pair {
public:
	Pair(K m_key, V m_data){
		this->m_key = m_key;
		this->m_data = m_data;
	}
	//键值
	K m_key;
	//数据
	V m_data;
};

#endif//_TOOL_PAIR_
