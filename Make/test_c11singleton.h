// --------------------------------------------------------------------------
// 2014-06-23�� ����. youn-sung.
// c++11�� ����մϴ�.
// --------------------------------------------------------------------------
#ifndef _TEST_C11SINGLETON
#define _TEST_C11SINGLETON
#pragma once
// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include "./Singleton/C11_Singleton.h"

class TEST_C11SINGLETON : public YS_LIB::C11_SINGLETON < TEST_C11SINGLETON >
{
public:
	TEST_C11SINGLETON() : m_nCount(0){}
	~TEST_C11SINGLETON() {}

	TEST_C11SINGLETON(const TEST_C11SINGLETON&) = delete;
	TEST_C11SINGLETON& operator=(const TEST_C11SINGLETON&) = delete;

	void test_function(void)
	{
		m_nCount++;
		printf("���������Ű���??  _ %d \n", m_nCount);
	}

private :
	int m_nCount;

};

#endif //_TEST_C11SINGLETON