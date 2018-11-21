// --------------------------------------------------------------------------
// 2014-07-03일 제작. youn-sung.
// window socket를 사용합니다.
// 나주엥 리눅스용 socket도 작성할 예정입니다.
// 리눅스 소켓은 윈도우와 리눅스 code가 같아야합니다.
// --------------------------------------------------------------------------
#pragma once

#ifndef _SOCKET_H
#define _SOCKET_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include <map>
#include <tchar.h>
#include <WinSock2.h>
using namespace std;
typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;
// --------------------------------------------------------------------------
typedef ::SOCKET socket_t;

namespace YS_NETWORK
{
	// --------------------------------------------------------------------------
	// Required Classes
	// --------------------------------------------------------------------------
	class WINDOWS_SOCKET
	{

	public:
		WINDOWS_SOCKET() : m_socket(INVALID_SOCKET), m_socketready(false) {}
		virtual ~WINDOWS_SOCKET() = 0;

		bool CreateSocket(int domain, int nType);
		bool CloseSocket();
		void AttachSocket(socket_t s, bool own);
		bool bindandlistenSocket(unsigned int port, const char *addr, bool used_acceptEx=false);
		bool AcceptSocket(WINDOWS_SOCKET& target, sockaddr *psa, int* psaLen);
		bool AcceptExSocket(WINDOWS_SOCKET& target, sockaddr *psa, int *psaLen);


		bool Connect(const char *addr, unsigned int port);
		bool Connect(const sockaddr* psa, int saLen);


		void GetSockName(sockaddr *psa, int *psaLen);
		void GetPeerName(sockaddr *psa, int *psaLen);

		unsigned int Send(const byte* buf, size_t bufLen, int flags);
		DWORD Receive(byte* buf, size_t bufLen, int flags);
		DWORD Receive(WSABUF* buffer, LPOVERLAPPED& overlapped);


		void IOCtl(long cmd, unsigned long *argp);
		void IOWSAloctl(DWORD dwControlCode, LPVOID& lpvInBuffer, DWORD dwcbInBuffer, LPVOID& lpvOutBuffer, DWORD dwcbOutBuffer, LPDWORD& lpcbBytesReturnd, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

		void set_socket(SOCKET& socket)	{ m_socket = socket; };

		static bool StartSockets();
		static bool ShutdownSockets();

		SOCKET get_socket() { return m_socket; }

	private:
		SOCKET	m_socket;
		bool	m_socketready;
	};

	// --------------------------------------------------------------------------
	// other Classes
	// --------------------------------------------------------------------------
	class SocketsInitializer
	{
	public:
		SocketsInitializer() { WINDOWS_SOCKET::StartSockets(); }
		~SocketsInitializer() { try { WINDOWS_SOCKET::ShutdownSockets(); } catch (...) {} }
	};


}

#endif //_SOCKET_H