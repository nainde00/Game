// --------------------------------------------------------------------------
// 2014-06-23�� ����. youn-sung.
// c++11�� ����մϴ�.
// http://msdn.microsoft.com/ko-kr/library/vstudio/hh921437(v=vs.110).aspx
// --------------------------------------------------------------------------
#ifndef _SINGLETON_H
#define _SINGLETON_H
#pragma once
#include <memory>
//Project Platform toolset�� 2013�������� �����ϼ���~~
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

		//thread�� �����մϴ�.
		static TYPE& get_Instance(void)
		{
			//�ٸ� �����忡���� �����ϰ�~.
			//���� �� ��Ȯ�� �� ���� ������ ȣ�� ���� ��ü�� ȣ�� �ϴ� ��Ŀ������ ���� �մϴ�. 
			std::call_once(m_onceflag, []
			{
				m_instance.reset(new TYPE);
			});
			return *m_instance.get();
		}

		//�޸� ����
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



