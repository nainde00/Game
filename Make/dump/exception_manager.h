// --------------------------------------------------------------------------
// 2014-06-12¿œ ¡¶¿€. youn-sung.
// 
// --------------------------------------------------------------------------
#pragma once

#ifndef EXCEPTION_MANAGER_H
#define EXCEPTION_MANAGER_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include "../Mutex.h"
#include "../Singleton/C11_Singleton.h"

// --------------------------------------------------------------------------

namespace YS_DUMP
{
	// --------------------------------------------------------------------------
	// Required Classes
	// --------------------------------------------------------------------------
	class EXCEPTION_HANDLER;

	class EXCEPTION_MANAGER : public YS_LIB::C11_SINGLETON < EXCEPTION_MANAGER >
	{
	public:
		EXCEPTION_MANAGER();
		~EXCEPTION_MANAGER();

		EXCEPTION_HANDLER* get_exception_handler() { return exception_handler; }
		void set_exception_handler(EXCEPTION_HANDLER* _exception_handler);
		void running_make_mini_dump(EXCEPTION_POINTERS* exception_data);
		
	private:
		EXCEPTION_HANDLER* exception_handler;
		CMuetx m_MutexLock;
	};

}
#define g_EXCEPTION_MANAGER EXCEPTION_MANAGER::get_Instance()



#endif // EXCEPTION_MANAGER_H

