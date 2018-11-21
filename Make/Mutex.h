#ifndef _CPOSIXMUTEX_H
#define _CPOSIXMUTEX_H

#include <process.h>
#include <Windows.h>
#include <stdio.h>

class CMuetx
{
public:
	class Owner
	{
	public:
		explicit Owner(CMuetx &mutex_);

		~Owner();
	private: 
		CMuetx &m_mutex;

		Owner(const Owner &rhs);
		Owner &operator=(const Owner &rhs);
	};

	CMuetx();
	~CMuetx();

	void Enter();
	void Leave();
private:
	HANDLE m_hMutex;

//	pthread_mutex_t m_mutex;

	CMuetx(const CMuetx& rhs);
	CMuetx &operator=(const CMuetx &rhs);
};

#endif //_CPOSIXMUTEX_H