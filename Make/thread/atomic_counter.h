// --------------------------------------------------------------------------
// 2014-06-17일 제작. youn-sung.
// thread에 안전한 숫자~~~~~
// --------------------------------------------------------------------------
#ifndef _ATOMIC_COUNTER_H
#define _ATOMIC_COUNTER_H
// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include "atomic_windows.h"

namespace YS_THREAD
{
	class ATOMIC_COUNTER
	{
	public:
		ATOMIC_COUNTER(long initial_value = 0);
		long		operator++(void);
		long		operator--(void);
		ATOMIC_COUNTER&		operator+=(long value_in);
		ATOMIC_COUNTER&		operator-=(long value_in);
		ATOMIC_COUNTER&		operator=(long value_in)	{ set_value(value_in); return *this; }
		operator			long() const	{ return value; }
				
		void				set_value(long new_value);
		long				get_value(void)	const	{ return value; }

	private:
		long					value;	//!< The value of the counter.
	};

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	inline ATOMIC_COUNTER::ATOMIC_COUNTER(long initial_value) : value(initial_value)
	{

	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	inline long ATOMIC_COUNTER::operator++(void)
	{
		return ATOMIC_WINDOWS::increment(value);
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------

	inline long ATOMIC_COUNTER::operator--(void)
	{
		return ATOMIC_WINDOWS::decrement(value);
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------

	inline ATOMIC_COUNTER& ATOMIC_COUNTER::operator+=(long value_in)
	{
		ATOMIC_WINDOWS::add(value, value_in);
		return *this;
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------

	inline ATOMIC_COUNTER& ATOMIC_COUNTER::operator-=(long value_in)
	{
		ATOMIC_WINDOWS::add(value, -value_in);
		return *this;
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------

	inline void ATOMIC_COUNTER::set_value(long value_in)
	{
		ATOMIC_WINDOWS::exchange(value, value_in);
	}


}

#endif //_ATOMIC_COUNTER_H