
// --------------------------------------------------------------------------
// cpp.
//
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include "atomic_windows.h"
#include <intrin.h>

namespace YS_THREAD
{
	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------

	long ATOMIC_WINDOWS::increment(long& value)
	{
		return _InterlockedIncrement(&value);
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------

	long ATOMIC_WINDOWS::decrement(long& value)
	{
		return _InterlockedDecrement(&value);
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------

	long ATOMIC_WINDOWS::add(long& value, long increment)
	{
		return _InterlockedExchangeAdd(&value, increment) + increment;
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------

	long ATOMIC_WINDOWS::bit_and(long& value, long mask)
	{
		return _InterlockedAnd(reinterpret_cast<long*>(&value), mask) & mask;
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------

	long ATOMIC_WINDOWS::bit_or(long& value, long mask)
	{
		return _InterlockedOr(&value, mask) | mask;
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------

	long ATOMIC_WINDOWS::bit_xor(long& value, long mask)
	{
		return _InterlockedXor(&value, mask) ^ mask;
	}


	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------

	void ATOMIC_WINDOWS::exchange(long& value, long new_value)
	{
		_InterlockedExchange(&value, new_value);
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------

	bool ATOMIC_WINDOWS::compare_and_exchange(long& destination, long new_value, long compare_to)
	{
		return _InterlockedCompareExchange(&destination, new_value, compare_to) == compare_to;
	}


}

