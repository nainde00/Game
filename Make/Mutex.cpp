#include "Mutex.h"
CMuetx::CMuetx()
{
	 m_hMutex = CreateMutex(NULL,FALSE,NULL);

}


CMuetx::~CMuetx()
{
	CloseHandle(m_hMutex);


}

void CMuetx::Enter()
{
	
  if(WaitForSingleObject(m_hMutex,INFINITE) == WAIT_FAILED) printf("WaitForSingleObject Error\n");

	

}

void CMuetx::Leave()
{
	if( ReleaseMutex(m_hMutex) == FALSE ) printf("ReleaseMutex Error\n");

}

CMuetx::Owner::Owner(CMuetx &mutex) : m_mutex(mutex)
{
	m_mutex.Enter();
}

CMuetx::Owner::~Owner()
{
	m_mutex.Leave();
}