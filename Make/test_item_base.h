// --------------------------------------------------------------------------
// 2014-06-17�� ����. youn-sung.
// 
// --------------------------------------------------------------------------
#ifndef _TEST_ITEM_BASE
#define _TEST_ITEM_BASE
#include "./iocp/item_base.h"

using namespace YS_THREAD;
class TEST_ITEM_BASE : public ITEM_BASE
{
public:
	//�°����� �ۼ��Ѵ�..
	TEST_ITEM_BASE();
	virtual ~TEST_ITEM_BASE();

	virtual void do_work();

private:
	int value;
};


#endif//_TEST_ITEM_BASE