#ifndef _TOOL_B_TREE_
#define _TOOL_B_TREE_

#include"pair.hpp"
#include"stack.hpp"

enum {
	LEFT,
	RIGHT
};


//节点
template<typename K, typename V>
class Node {
public:
	Node(int order, Node<K, V>* parent = NULL) {
		m_ite_size = 0;
		m_chi_size = 0;
		m_parent = parent;
		m_items = new Pair<K, V>*[order];
		m_children = new Node<K, V>*[order + 1];
		for (int i = 0; i < order; i++)
		{
			m_children[i] = NULL;
			m_items[i] = NULL;
		}
		m_children[order] = NULL;
	}

	//遍历节点
	void traversal(Node<K, V>* node, void(*callback)(Node<K, V>*, int, void*), void* parameter = NULL) {
		if (NULL == node)
			return;
		for (int i = 0; i < node->m_ite_size; i++)
		{
			traversal(node->m_children[i], callback, parameter);
			callback(node, i, parameter);
			if (i == node->m_ite_size - 1)
				traversal(node->m_children[i + 1], callback, parameter);
		}
	}
	//处理节点
	bool handleNode(Node<K, V>* node, int order)
	{
		if (order == node->m_ite_size) {
			//判断是否根节点
			if (NULL == node->m_parent) {
				if (node->m_children[0] == NULL) {
					node->m_children[0] = new Node<K, V>(order, node);
					for (int i = 0; i < order / 2; i++) {
						node->m_children[0]->m_items[node->m_children[0]->m_ite_size++] = node->m_items[i];
					}
					node->m_children[1] = new Node<K, V>(order, node);
					for (int i = order / 2 + 1; i < order; i++) {
						node->m_children[1]->m_items[node->m_children[1]->m_ite_size++] = node->m_items[i];
					}
					node->m_items[0] = node->m_items[order / 2];
					node->m_ite_size = 1;
					node->m_chi_size = 2;
					for (int i = 1; i < order; i++) {
						node->m_items[i] = NULL;
						node->m_children[i + 1] = NULL;
					}
					return true;
				}
				else {
					Node<K, V>* children1 = new Node<K, V>(order, node);
					Node<K, V>* children2 = new Node<K, V>(order, node);
					for (int i = 0; i < order / 2; i++) {
						children1->m_items[children1->m_ite_size++] = node->m_items[i];
						children1->m_children[children1->m_chi_size++] = node->m_children[i];
						node->m_children[i]->m_parent = children1;
					}
					children1->m_children[children1->m_chi_size++] = node->m_children[order / 2];
					node->m_children[order / 2]->m_parent = children1;
					for (int i = order / 2 + 1; i < order; i++) {
						children2->m_items[children2->m_ite_size++] = node->m_items[i];
						children2->m_children[children2->m_chi_size++] = node->m_children[i];
						node->m_children[i]->m_parent = children2;
					}
					children2->m_children[children2->m_chi_size++] = node->m_children[order];
					node->m_children[order]->m_parent = children2;
					node->m_items[0] = node->m_items[order / 2];
					node->m_children[0] = children1;
					node->m_children[1] = children2;
					for (int i = 1; i < order; i++) {
						node->m_items[i] = NULL;
						node->m_children[i + 1] = NULL;
					}
					node->m_ite_size = 1;
					node->m_chi_size = 2;
					return true;
				}
			}
			//分裂节点
			Node<K, V>* children1 = new Node<K, V>(order, node->m_parent);
			for (int i = order / 2 + 1; i < order; i++) {
				children1->m_items[children1->m_ite_size++] = node->m_items[i];
				node->m_items[i] = NULL;
				node->m_ite_size--;
				if (NULL != node->m_children[i]) {
					children1->m_children[children1->m_chi_size++] = node->m_children[i];
					node->m_children[i]->m_parent = children1;
					node->m_children[i] = NULL;
					node->m_chi_size--;
				}
			}
			if (NULL != node->m_children[order]) {
				children1->m_children[children1->m_chi_size++] = node->m_children[order];
				node->m_children[order]->m_parent = children1;
				node->m_children[order] = NULL;
				node->m_chi_size--;
			}
			//将分裂的节点插入父节点
			Pair<K, V>* p_cmp;
			Node<K, V>* n_cmp;
			for (int i = 0; i < node->m_parent->m_ite_size; i++) {
				if (node->m_items[order / 2]->m_key < node->m_parent->m_items[i]->m_key) {
					p_cmp = node->m_parent->m_items[i];
					node->m_parent->m_items[i] = node->m_items[order / 2];
					node->m_items[order / 2] = p_cmp;
					n_cmp = node->m_parent->m_children[i + 1];
					node->m_parent->m_children[i + 1] = children1;
					children1 = n_cmp;
				}
			}
			node->m_parent->m_items[node->m_parent->m_ite_size] = node->m_items[order / 2];
			node->m_parent->m_children[node->m_parent->m_ite_size + 1] = children1;
			node->m_parent->m_ite_size++;
			node->m_parent->m_chi_size++;

			node->m_items[order / 2] = NULL;
			node->m_ite_size--;
			return node->m_parent->handleNode(node->m_parent, order);
		}
		return false;
	}
	
	//节点数据数量
	int m_ite_size;
	//数据项数组
	Pair<K, V>** m_items;
	//节点子节点数量
	int m_chi_size;
	//子节点数组
	Node<K, V>** m_children;
	//父节点
	Node<K, V>* m_parent;
private:

};

//入栈元素
template<typename K, typename V>
class node_location_driection {
public:
	node_location_driection() {

	}
	node_location_driection
	(Node<K, V>* node, int location, int driection) {
		m_node = node;
		m_location = location;
		m_driection = driection;
	}
	node_location_driection
		operator=(node_location_driection copy) {
		m_node = copy.m_node;
		m_location = copy.m_location;
		m_driection = copy.m_driection;
		return *this;
	}

	//节点
	Node<K, V>* m_node;
	//节点元素位置
	int m_location;
	//儿子节点方向
	int m_driection;
};

//B树
template<typename K, typename V>
class bTree{
public:
	bTree(int order){
		this->m_root = new Node<K,V>(order);
		this->m_height = 0;
		this->m_order = order;
		this->m_number = 0;
	}
	~bTree(){

	}

	//查找节点
	Pair<K, V>* bTreeSeek(K m_key) {
		if (0 == m_root->m_ite_size) {
			return NULL;
		}
		//记录查找的节点
		Node<K, V>* node_copy = m_root;
		//循环二分查找
		int low, high, mid;
		while (1) {
			low = 0;
			high = node_copy->m_ite_size - 1;
			while (low <= high) {
				mid = (low + high) / 2;
				if (node_copy->m_items[mid]->m_key == m_key) {
					return node_copy->m_items[mid];
				}
				if (node_copy->m_items[mid]->m_key > m_key) {
					high = mid - 1;
				}
				else {
					low = mid + 1;
				}
			}
			if (node_copy->m_chi_size != 0) {
				node_copy = node_copy->m_children[low];
			}
			else {
				return NULL;
			}
		}
	}
	//插入元素
	void bTreePush(Pair<K, V>* push_Pair){
		m_number++;
		if (0 == m_root->m_ite_size) {
			m_root->m_items[m_root->m_ite_size++] = push_Pair;
			this->m_height++;
			return;
		}
		//记录查找的节点
		Node<K, V>* node_copy = m_root;
		//循环二分查找
		int low, high, mid;
		while (1) {
			low = 0;
			high = node_copy->m_ite_size - 1;
			while (low <= high) {
				mid = (low + high) / 2;
				if (node_copy->m_items[mid]->m_key == push_Pair->m_key) {
					Pair<K, V>* item_copy = node_copy->m_items[mid];
					node_copy->m_items[mid] = push_Pair;
					delete item_copy;
					return;
				}
				if (node_copy->m_items[mid]->m_key > push_Pair->m_key) {
					high = mid - 1;
				}
				else {
					low = mid + 1;
				}
			}
			if (node_copy->m_chi_size != 0) {
				node_copy = node_copy->m_children[low];
			}
			else {
				break;
			}
		}
		//查找完后key替换low
		Pair<K, V> * item_copy;
		while (low <= node_copy->m_ite_size) {
			item_copy = node_copy->m_items[low];
			node_copy->m_items[low++] = push_Pair;
			push_Pair = item_copy;
		}
		node_copy->m_ite_size++;
			//处理节点
		if (node_copy->handleNode(node_copy, m_order))
		{
			this->m_height++;
		}
	}
	//删除元素
	int bTreePop(K Pair_key) {
		//记录查找过程中经历的节点
		node_location_driection<K, V> stack_pos;
		Stack<node_location_driection<K, V>> stack;
		//查找元素所在位置
		if (0 == m_root->m_ite_size) {
			return 0;
		}
		//记录查找的节点
		Node<K, V>* node_copy = m_root;
		//循环二分查找
		int low, high, mid;
		while (1) {
			low = 0;
			high = node_copy->m_ite_size - 1;
			mid = low + high / 2;
			while (low <= high) {
				mid = (low + high) / 2;
				if (node_copy->m_items[mid]->m_key == Pair_key) {
					break;
				}
				if (node_copy->m_items[mid]->m_key > Pair_key) {
					high = mid - 1;
				}
				else {
					low = mid + 1;
				}
			}
			if (node_copy->m_items[mid]->m_key == Pair_key) {
				break;
			}
			if (node_copy->m_chi_size != 0) {
				//将经过节点入栈
				if (NULL == node_copy->m_items[low]) {
					stack_pos.m_node = node_copy;
					stack_pos.m_location = low - 1;
					stack_pos.m_driection = RIGHT;
					//将当前节点变为子节点
					node_copy = node_copy->m_children[low];
				}
				else if (node_copy->m_items[low]->m_key < Pair_key) {
					stack_pos.m_node = node_copy;
					stack_pos.m_location = low;
					stack_pos.m_driection = RIGHT;
					//将当前节点变为子节点
					node_copy = node_copy->m_children[low + 1];
				}
				else {
					stack_pos.m_node = node_copy;
					stack_pos.m_location = low;
					stack_pos.m_driection = LEFT;
					//将当前节点变为子节点
					node_copy = node_copy->m_children[low];
				}
				stack.push(stack_pos);
			}
			else {
				return 0;
			}
		}
		//删除找到的元素node_copy->n_items[mid]
		//记录要删除的元素
		Pair<K, V>* delete_item = node_copy->m_items[mid];
		//孩子节点
		Node<K, V>* children_copy;
		//处理内部节点
		if (node_copy->m_chi_size != 0) {
			//将现节点入栈
			stack_pos = node_location_driection<K, V>(node_copy, mid, LEFT);
			stack.push(stack_pos);
			children_copy = node_copy->m_children[mid];
			//将该节点与左子树的最右元素交换
			while (1) {
				if (0 == children_copy->m_chi_size) {
					//交换元素
					node_copy->m_items[mid] =
						children_copy->m_items[children_copy->m_ite_size - 1];
					children_copy->m_items[children_copy->m_ite_size - 1]
						= delete_item;
					mid = children_copy->m_ite_size - 1;
					node_copy = children_copy;
					break;
				}
				else {
					//将经过节点入栈
					stack_pos = node_location_driection<K, V>
						(children_copy, children_copy->m_ite_size - 1, RIGHT);
					stack.push(stack_pos);
					children_copy = children_copy->m_children[children_copy->m_chi_size - 1];
					mid = children_copy->m_ite_size - 1;
				}
			}
		}
		//删除叶节点元素
		delete node_copy->m_items[mid];
		//调整叶节点
		//创建父节点拷贝和兄弟节点拷贝		
		Node<K, V> *parent_copy, *brother_copy;
		//处理根节点或可直接删除的叶节点
		if (NULL == node_copy->m_parent ||
			m_order / 2 + m_order % 2 - 1 != node_copy->m_ite_size) {
			for (int i = mid; i < node_copy->m_ite_size; i++) {
				node_copy->m_items[mid] = node_copy->m_items[mid + 1];
			}
				node_copy->m_ite_size--;
			this->m_number--;
			return 1;
		}
		//处理不能直接删除的叶节点
		stack_pos = stack.pop();
		parent_copy = stack_pos.m_node;
		//向右兄弟节点借数据
		if (LEFT == stack_pos.m_driection &&
			m_order / 2 + m_order % 2 - 1 !=
			parent_copy->m_children[stack_pos.m_location + 1]->m_ite_size) {
			brother_copy = parent_copy->m_children[stack_pos.m_location + 1];
			for (int i = mid; i < node_copy->m_ite_size; i++) {
				node_copy->m_items[mid] = node_copy->m_items[mid + 1];
			}
			node_copy->m_items[node_copy->m_ite_size - 1] =
				parent_copy->m_items[stack_pos.m_location];
			parent_copy->m_items[stack_pos.m_location] = brother_copy->m_items[0];
			for (int i = 0; i < brother_copy->m_ite_size; i++) {
				brother_copy->m_items[i] = brother_copy->m_items[i + 1];
			}
			brother_copy->m_ite_size--;
			this->m_number--;
			return 1;
		}
		//向左兄弟节点借数据
		if (RIGHT == stack_pos.m_driection &&
			m_order / 2 + m_order % 2 - 1 !=
			parent_copy->m_children[stack_pos.m_location]->m_ite_size) {
			brother_copy = parent_copy->m_children[stack_pos.m_location];
			for (int i = mid; i > 0; i--) {
				node_copy->m_items[i] = node_copy->m_items[i - 1];
			}
			node_copy->m_items[0] = parent_copy->m_items[stack_pos.m_location];
			parent_copy->m_items[stack_pos.m_location] =
				brother_copy->m_items[brother_copy->m_ite_size - 1];
			brother_copy->m_items[brother_copy->m_ite_size - 1] = NULL;
			brother_copy->m_ite_size--;
			this->m_number--;
			return 1;
		}
		//向父节点借数据
		if (LEFT == stack_pos.m_driection &&
			m_order / 2 + m_order % 2 - 1 ==
			parent_copy->m_children[stack_pos.m_location + 1]->m_ite_size) {
			//合并兄弟节点
			brother_copy = parent_copy->m_children[stack_pos.m_location + 1];
			for (int i = mid; i < node_copy->m_ite_size; i++) {
				node_copy->m_items[mid] = node_copy->m_items[mid + 1];
			}
			node_copy->m_items[node_copy->m_ite_size - 1] =
				parent_copy->m_items[stack_pos.m_location];
			for (int i = 0; i < brother_copy->m_ite_size; i++) {
				node_copy->m_items[node_copy->m_ite_size + i] =
					brother_copy->m_items[i];
			}
			node_copy->m_ite_size += brother_copy->m_ite_size;
			delete brother_copy;
		}
		//向父节点借数据
		if (RIGHT == stack_pos.m_driection &&
			m_order / 2 + m_order % 2 - 1 ==
			parent_copy->m_children[stack_pos.m_location]->m_ite_size) {
			//合并兄弟节点
			brother_copy = parent_copy->m_children[stack_pos.m_location];
			brother_copy->m_items[brother_copy->m_ite_size] =
				parent_copy->m_items[stack_pos.m_location];
			for (int i = 0; i < mid; i++) {
				brother_copy->m_items[brother_copy->m_ite_size + i + 1] = node_copy->m_items[i];
			}
			for (int i = mid + 1; i < node_copy->m_ite_size - 1; i++) {
				brother_copy->m_items[brother_copy->m_ite_size + i] = node_copy->m_items[i];
			}
			brother_copy->m_ite_size += node_copy->m_ite_size;
			delete node_copy;
		}
		//处理内部节点
		while (1) {
			mid = stack_pos.m_location;
			node_copy = parent_copy;
			//处理被借空的根节点
			if (NULL == node_copy->m_parent && 1 == node_copy->m_ite_size) {
				this->m_root = node_copy->m_children[0];
				delete node_copy;
				m_root->m_parent = NULL;
				this->m_number--;
				this->m_height--;
				return 1;
			}
			//处理没被借空的根节点或可直接删除的叶节点
			if (NULL == node_copy->m_parent ||
				m_order / 2 + m_order % 2 - 1 != node_copy->m_ite_size) {
				//调整父节点和儿子节点
				for (int i = mid; i < node_copy->m_ite_size; i++) {
					node_copy->m_items[i] = node_copy->m_items[i + 1];
					node_copy->m_children[i + 1] = node_copy->m_children[i + 2];
				}
				node_copy->m_ite_size--;
				node_copy->m_chi_size--;
				return 1;
			}
			//处理内部节点
			stack_pos = stack.pop();
			parent_copy = stack_pos.m_node;
			//向右兄弟节点借数据
			if (LEFT == stack_pos.m_driection &&
				m_order / 2 + m_order % 2 - 1 !=
				parent_copy->m_children[stack_pos.m_location + 1]->m_ite_size) {
				brother_copy = parent_copy->m_children[stack_pos.m_location + 1];
				//调整父节点和儿子节点
				for (int i = mid; i < node_copy->m_ite_size; i++) {
					node_copy->m_items[i] = node_copy->m_items[i + 1];
					node_copy->m_children[i + 1] = node_copy->m_children[i + 2];
				}
				node_copy->m_items[node_copy->m_ite_size - 1] =
					parent_copy->m_items[stack_pos.m_location];
				node_copy->m_children[node_copy->m_chi_size - 1] =
					brother_copy->m_children[0];
				brother_copy->m_children[0]->m_parent = node_copy;
				parent_copy->m_items[stack_pos.m_location] = brother_copy->m_items[0];
				//调整兄弟节点
				for (int i = 0; i < brother_copy->m_ite_size; i++) {
					brother_copy->m_items[i] = brother_copy->m_items[i + 1];
					brother_copy->m_children[i] = brother_copy->m_children[i + 1];
				}
				brother_copy->m_children[brother_copy->m_chi_size - 1] =
					brother_copy->m_children[brother_copy->m_chi_size];
				brother_copy->m_ite_size--;
				brother_copy->m_chi_size--;
				return 1;
			}
			//向左兄弟节点借数据
			if (RIGHT == stack_pos.m_driection &&
				m_order / 2 + m_order % 2 - 1 !=
				parent_copy->m_children[stack_pos.m_location]->m_ite_size) {
				brother_copy = parent_copy->m_children[stack_pos.m_location];
				//调整父节点和儿子节点
				for (int i = mid; i > 0; i--) {
					node_copy->m_items[i] = node_copy->m_items[i - 1];
					node_copy->m_children[i + 1] = node_copy->m_children[i];
				}
				node_copy->m_children[1] = node_copy->m_children[0];
				node_copy->m_children[0]->m_parent = node_copy;
				node_copy->m_items[0] = parent_copy->m_items[stack_pos.m_location];
				node_copy->m_children[0] = brother_copy->m_children[brother_copy->m_chi_size - 1];
				parent_copy->m_items[stack_pos.m_location] =
					brother_copy->m_items[brother_copy->m_ite_size - 1];
				//调整兄弟节点
				brother_copy->m_items[brother_copy->m_ite_size - 1] = NULL;
				brother_copy->m_children[brother_copy->m_chi_size - 1] = NULL;
				brother_copy->m_ite_size--;
				brother_copy->m_chi_size--;
				return 0;
			}
			//向父兄弟节点借数据
			if (LEFT == stack_pos.m_driection &&
				m_order / 2 + m_order % 2 - 1 ==
				parent_copy->m_children[stack_pos.m_location + 1]->m_ite_size) {
				//合并兄弟节点
				brother_copy = parent_copy->m_children[stack_pos.m_location + 1];
				for (int i = mid; i < node_copy->m_ite_size; i++) {
					node_copy->m_items[mid] = node_copy->m_items[mid + 1];
					node_copy->m_children[mid + 1] = node_copy->m_children[mid + 2];
				}
				node_copy->m_items[node_copy->m_ite_size - 1] =
					parent_copy->m_items[stack_pos.m_location];
				node_copy->m_children[node_copy->m_chi_size - 1] =
					brother_copy->m_children[0];
				brother_copy->m_children[0]->m_parent = node_copy;
				for (int i = 0; i < brother_copy->m_ite_size; i++) {
					node_copy->m_items[node_copy->m_ite_size + i] =
						brother_copy->m_items[i];
					node_copy->m_children[node_copy->m_chi_size + i] =
						brother_copy->m_children[i + 1];
					brother_copy->m_children[i + 1]->m_parent = node_copy;
				}
				node_copy->m_ite_size += brother_copy->m_ite_size;
				node_copy->m_chi_size += brother_copy->m_chi_size - 1;
				delete brother_copy;
			}
			//向父兄弟节点借数据
			if (RIGHT == stack_pos.m_driection &&
				m_order / 2 + m_order % 2 - 1 ==
				parent_copy->m_children[stack_pos.m_location]->m_ite_size) {
				//合并兄弟节点
				brother_copy = parent_copy->m_children[stack_pos.m_location];
				brother_copy->m_items[brother_copy->m_ite_size] =
					parent_copy->m_items[stack_pos.m_location];
				brother_copy->m_children[brother_copy->m_chi_size] =
					node_copy->m_children[0];
				node_copy->m_children[0]->m_parent = brother_copy;
				for (int i = 0; i < mid; i++) {
					brother_copy->m_items[brother_copy->m_ite_size + i + 1] = node_copy->m_items[i];
					brother_copy->m_children[brother_copy->m_chi_size + i + 1] = node_copy->m_children[i + 1];
					node_copy->m_children[i + 1]->m_parent = brother_copy;
				}
				for (int i = mid + 1; i < node_copy->m_ite_size - 1; i++) {
					brother_copy->m_items[brother_copy->m_ite_size + i] = node_copy->m_items[i];
					brother_copy->m_children[brother_copy->m_chi_size + i] = node_copy->m_children[i + 1];
					node_copy->m_children[i + 1]->m_parent = brother_copy;
				}
				brother_copy->m_ite_size += node_copy->m_ite_size;
				brother_copy->m_chi_size += node_copy->m_chi_size - 1;
				delete node_copy;
			}
		}
	}
	//遍历树
	void traversalTree(void(*callback)(Node<K, V>*, int, void*), void* parameter = NULL) {
		m_root->traversal(m_root, callback, parameter);
	}
	//返回树高
	int reHeight() {
		return m_height;
	}
	//返回返回树的阶数
	int reOrder() {
		return m_order;
	}	
	//返回数据数量
	int reNumber() {
		return m_number;
	}
private:
	//根节点
	Node<K, V>* m_root;
	//高度
	int m_height;
	//阶数
	int m_order;
	//数据数量
	int m_number;

};




#endif//_TOOL_B_TREE_