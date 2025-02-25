#include"bTree.hpp"
#include<stdio.h>

#include"stack.hpp"





void print(Node<int, int>* abc, int i, void* xxx){
	printf("%d ", abc->m_items[i]->m_data);
}


void xxxabc(int a,void* null)
{
	printf("%d\n ", a);
	printf("%d\n ", a);
	printf("%d\n ", a);
	printf("%d\n ", a);
	printf("%d\n ", a);
	xxxabc(a, null);
}

void xxx(void(*xxxxxx)(int,void*), int a,void* c) {
	xxxxxx(a,NULL);
}


void test2() {
	xxx(&xxxabc, 100,NULL);
}
//void test(){


//	bTree<int, int> b1(4);
//	Pair<int, int>* p;
//
//	//for (int i = 0; i < 10000; i++){
//	//	p = new Pair<int, int>(i, i);
//	//	b1.b_push(p);
//	//}
//	for (int i = 0; i < 1100000; i++) {
//		p = new Pair<int, int>(i, i);
//		//printf("\n");
//	}
//	p= b1.bTreeSeek(199687);
//	b1.traversalTree(&print);
//}
//
//
//void test3() {
//	Queue<Pair<int, int>*> s1;
//	Pair<int, int>* p1;
//	for (int i = 0; i < 1024; i++) {
//		p1 = new Pair<int, int>(i, i);
//		s1.s_push(p1);
//	}
//	for (int i = 0; i < 102; i++) {
//		printf("%d ", s1.s_pop()->m_data);
//	}
//	for (int i = 0; i < 1024; i++) {
//		p1 = new Pair<int, int>(i, i);
//		s1.s_push(p1);
//	}
//	for (int i = 0; i < 1024; i++) {
//		printf("%d ", s1.s_pop()->m_data);
//		if (NULL == s1.reHead())
//			printf("xxxx");
//		if (NULL == s1.reTail())
//			printf("xxxx");
//	}
//	for (int i = 0; i < 1024; i++) {
//		p1 = new Pair<int, int>(i, i);
//		s1.s_push(p1);
//	}
//	s1.initCapacity(100);
//	for (int i = 0; i < 100; i++) {
//		if (NULL == s1.reHead())
//			printf("xxxx");
//		if (NULL == s1.reTail())
//		{
//			printf("xxxx");
//
//		}
//		printf("%d ", s1.s_pop()->m_data);
//	}
//
//}
//
//
//
//
//void test4() {
//	Stack<Pair<int, int>*> s1;
//	Pair<int, int>* p1;
//	for (int i = 0; i < 1024; i++) {
//		p1 = new Pair<int, int>(i, i);
//		s1.push(p1);
//	}
//	s1.initCapacity(128);
//	for (int i = 0; i < 128; i++)
//		printf("%d, ", s1.pop()->m_data);
//}
//
//
//void test5() {
//	bTree<int, int> b1(4);
//	Pair<int, int>* p1;
//	printf("插入打印、\n");
//	for (int i = 0; i < 100; i++) {
//		p1 = new Pair<int, int>(i, i);
//		b1.bTreePush(p1);
//	}
//	b1.traversalTree(&print);
//	for (int i = 1000; i > 500; i--) {
//		p1 = new Pair<int, int>(i, i);
//		b1.bTreePush(p1);
//	}
//	b1.traversalTree(&print);
//	printf("\n删除打印 \n");
//	for (int i = 0; i < 100; i++) {
//		p1 = new Pair<int, int>(i, i);
//		b1.bTreePop(p1->m_key);
//	}
//	b1.traversalTree(&print);
//	for (int i = 1000; i > 500; i--) {
//		p1 = new Pair<int, int>(i, i);
//		b1.bTreePop(p1->m_key);
//	}
//	b1.traversalTree(&print);
//	printf("插入打印、\n");
//	for (int i = 0; i < 18; i++) {
//		p1 = new Pair<int, int>(i, i);
//		b1.bTreePush(p1);
//	}
//	b1.traversalTree(&print);
//
//	printf("\n删除打印 \n");
//	for (int i = 0; i < 18; i++) {
//		p1 = new Pair<int, int>(i, i);
//		b1.bTreePop(p1->m_key);
//		printf("\n第%d行\n", i);
//		b1.traversalTree(&print);
//
//	}
//	b1.traversalTree(&print);
//
//
//}
#include"queue.hpp"
void test5() {
	lwg::Queue<int> q1;
	q1.cleanSpace();
	int a, b;
	q1.push(1);
	q1.push(2);
	q1.push(3);
	a = q1.front(); b = q1.back();
	q1.pop();	
	a = q1.front(); b = q1.back();
	q1.pop();	
	a = q1.front(); b = q1.back();
	q1.pop();
	for (int i = 0; i < 257; i++) {
		q1.push(i);
	}

}

#include<queue>
int main() {
	std::queue<int> q2;
	//std::queue<int>::iterator it;
	std::vector<int>::iterator it;
	test5();
	return 0;
}