// --------------------------------------------------------------------------
// 2014-06-17일 제작. youn-sung.
// thread에 안전한 변수.~~
// --------------------------------------------------------------------------
#ifndef _ATOMIC_WINDOWS_H
#define _ATOMIC_WINDOWS_H
// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
namespace YS_THREAD
{
	class ATOMIC_WINDOWS
	{
	public:
		static long increment(long& value);
		static long decrement(long& value);

		static long add(long& value, long increment);
		static long bit_and(long& value, long mask);
		static long bit_or(long& value, long mask);
		static long bit_xor(long& value, long mask);

		static void exchange(long& value, long new_value);

		static bool compare_and_exchange(long& destination, long new_value, long compare_to);
	};
}

#endif //_ATOMIC_WINDOWS_H