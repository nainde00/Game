// --------------------------------------------------------------------------
// 2014-06-17¿œ ¡¶¿€. youn-sung.
// 
// --------------------------------------------------------------------------
#ifndef _TEST_THREAD_BASE
#define _TEST_THREAD_BASE
#include "./thread/thread_base.h"

using namespace YS_THREAD;
class TEST_THREAD_BASE : public THREAD_BASE
{
public:
	TEST_THREAD_BASE(tstring thread_name);
	virtual ~TEST_THREAD_BASE();
private:
	virtual bool do_work();
};


#endif//_TEST_THREAD_BASE