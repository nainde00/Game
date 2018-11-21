// --------------------------------------------------------------------------
// 2014-06-23일 제작. youn-sung.
// c++11을 사용합니다.
// http://msdn.microsoft.com/ko-kr/library/vstudio/hh921437(v=vs.110).aspx
// --------------------------------------------------------------------------
#ifndef _SINGLETON_H
#define _SINGLETON_H
#pragma once
#include <memory>
//Project Platform toolset을 2013버전으로 변경하세요~~
#include <mutex>


namespace YS_LIB
{
	template< typename TYPE >
	class C11_SINGLETON
	{
	protected:
		C11_SINGLETON() = default;
		C11_SINGLETON(const C11_SINGLETON<TYPE>& type) = delete;
		C11_SINGLETON<TYPE>& operator=(const C11_SINGLETON<TYPE>& type) = delete;

	public:

		//thread를 생성합니다.
		static TYPE& get_Instance(void)
		{
			//다른 쓰레드에서도 안전하게~.
			//실행 중 정확히 한 번만 지정한 호출 가능 개체를 호출 하는 메커니즘을 제공 합니다. 
			std::call_once(m_onceflag, []
			{
				m_instance.reset(new TYPE);
			});
			return *m_instance.get();
		}

		//메모리 해젱
		static void destroyInstance()
		{
			m_instance.reset();
		}

	private:
		static std::unique_ptr<TYPE> m_instance;
		static std::once_flag m_onceflag;

#ifdef _UNITTEST
	public:
		static void reset() { m_instance.reset(new DeviceManager); }
#endif
	};

	
	template <typename TYPE>
	std::unique_ptr<TYPE> C11_SINGLETON<TYPE>::m_instance;

	template <typename TYPE>
	std::once_flag C11_SINGLETON<TYPE>::m_onceflag;
	



}

#endif //_SINGLETON_H



