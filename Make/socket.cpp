
// --------------------------------------------------------------------------
// cpp.
//
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include "socket.h"	
#include <MSWSock.h>


namespace YS_NETWORK
{
	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	bool WINDOWS_SOCKET::StartSockets()
	{
		WSADATA wsd;
		int result = WSAStartup(MAKEWORD(2,2), &wsd);
		if (result != 0)
		{
			printf("WSAStartup = %d , 소켓 에러 = %d", result, GetLastError());
			return false;
		}
		return true;
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	bool WINDOWS_SOCKET::ShutdownSockets()
	{
		int result = WSACleanup();
		if (result != 0)
		{
			printf("ShutdownSockets 소켓 에러 = %d", GetLastError());
			return false;
		}
		return true;
			
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	bool WINDOWS_SOCKET::CreateSocket(int domain,int nType)
	{
		m_socket = socket(domain, nType, 0);
		if (m_socket == SOCKET_ERROR)
		{
			printf("CreateSocket 소켓 에러 = %d", GetLastError());
			return false;
		}
		m_socketready = true;
		return true;
	}

	// --------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------
	bool WINDOWS_SOCKET::CloseSocket()
	{
		if (m_socket != INVALID_SOCKET)
		{
			CancelIo((HANDLE)m_socket);
			closesocket(m_socket);
			if (m_socket == SOCKET_ERROR)
			{
				printf("CloseSocket 소켓 에러 = %d", GetLastError());
				return false;
			}
			m_socket = INVALID_SOCKET;
		}
		return true;
	}

	// --------------------------------------------------------------------------
	// 어태치 소켓
	// --------------------------------------------------------------------------
	void WINDOWS_SOCKET::AttachSocket(socket_t s, bool own)
	{
		if (m_socketready)
			CloseSocket();

		m_socket = s;
		m_socketready = own;
	}

	// --------------------------------------------------------------------------
	// 바인드엔 리쓴
	// AcceptEx를 사용할려면 setsockopt를 성정해야하기떄문에 값을 추가하였습니다.
	// --------------------------------------------------------------------------
	bool WINDOWS_SOCKET::bindandlistenSocket(unsigned int port, const char *addr,bool used_acceptEx)
	{
		sockaddr_in sa;
		memset(&sa, 0, sizeof(sa));
		sa.sin_family = AF_INET;

		if (addr == NULL)
			sa.sin_addr.s_addr = htonl(INADDR_ANY);
		else
		{
			unsigned long result = inet_addr(addr);
			if (result == -1)	// Solaris doesn't have INADDR_NONE
			{
				WSASetLastError(WSAEINVAL);
				printf("inet_addr 소켓 에러 = %d", GetLastError());
				return false;
			}
			sa.sin_addr.s_addr = result;
		}
		sa.sin_port = htons((u_short)port);

		if (SOCKET_ERROR == bind(m_socket, (const sockaddr*)&sa, sizeof(sa)))
		{
			printf("bind 소켓 에러 = %d", GetLastError());
			return false;
		}
		bool on = true;

		if (used_acceptEx == true)
		{
			if (SOCKET_ERROR == setsockopt(m_socket, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char*)&on, sizeof(on)))
			{
				printf("listen 소켓 에러 = %d", GetLastError());
				return false;
			}

		}

		if (SOCKET_ERROR == listen(m_socket, SOMAXCONN))
		{
			printf("listen 소켓 에러 = %d", GetLastError());
			return false;
		}
		return true;
	}


	// --------------------------------------------------------------------------
	// Accept어쉡~!
	// --------------------------------------------------------------------------
	bool WINDOWS_SOCKET::AcceptSocket(WINDOWS_SOCKET& target, sockaddr *psa, int *psaLen)
	{
		if (m_socket == SOCKET_ERROR)
		{
			return false;
		}

		SOCKET s = accept(m_socket, psa, psaLen);
		if (s == INVALID_SOCKET && GetLastError() == WSAEWOULDBLOCK)
		{
			printf("accept 소켓 에러 = %d", GetLastError());
			return false;
		}
		target.AttachSocket(s, true);
		return true;
	}

	// --------------------------------------------------------------------------
	// AcceptEx~!
	// IOCP에 연결. ( IOCP가 accept를 관장하게 됩니다.)
	// Proactor방식을 사용하자~~
	// OVERLAP구조체를 먼저 만들고 만들자... 2014-07-04
	// --------------------------------------------------------------------------
	/*
	bool WINDOWS_SOCKET::AcceptExSocket(WINDOWS_SOCKET& target, sockaddr *psa, int *psaLen)
	{
		SOCKET s;
		if (AcceptEx(m_socket, target.get_socket(),

	}
	*/


	// --------------------------------------------------------------------------
	// 연결~~
	// --------------------------------------------------------------------------
	bool WINDOWS_SOCKET::Connect(const char *addr, unsigned int port)
	{
		sockaddr_in sa;
		memset(&sa, 0, sizeof(sa));
		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = inet_addr(addr);

		if (sa.sin_addr.s_addr == -1)	// Solaris doesn't have INADDR_NONE
		{
			hostent *lphost = gethostbyname(addr);
			if (lphost == NULL)
			{
				WSASetLastError(WSAEINVAL);
				printf("Connect 소켓 에러 = %d", GetLastError());
			}

			sa.sin_addr.s_addr = ((in_addr *)lphost->h_addr)->s_addr;
		}

		sa.sin_port = htons((u_short)port);

		return Connect((const sockaddr *)&sa, sizeof(sa));
	}

	// --------------------------------------------------------------------------
	// 연결
	// --------------------------------------------------------------------------
	bool WINDOWS_SOCKET::Connect(const sockaddr* psa, int saLen)
	{
		if (m_socket == INVALID_SOCKET)
		{
			int result = connect(m_socket, const_cast<sockaddr*>(psa), saLen);
			int error = GetLastError();
			if (result == SOCKET_ERROR && error == WSAEWOULDBLOCK)
				return false;
			if (result < 0)
			{
				if (WSAEWOULDBLOCK == error || WSAEISCONN == error)
				{
					return true;
				}
				return false;
			}
		}
		else
		{
			printf("소켓이 이미 준비되어있습니다.");
		}
		
		return true;
	}



	// --------------------------------------------------------------------------
	// socket정보를 가져옴당.
	// --------------------------------------------------------------------------
	void WINDOWS_SOCKET::GetSockName(sockaddr *psa, int *psaLen)
	{
		if (m_socket != INVALID_SOCKET)
		{
			getsockname(m_socket, psa, psaLen);
		}
		
	}

	// --------------------------------------------------------------------------
	// 
	// --------------------------------------------------------------------------
	void WINDOWS_SOCKET::GetPeerName(sockaddr *psa, int *psaLen)
	{
		if(m_socket != INVALID_SOCKET)
		{
			getpeername(m_socket, psa, psaLen);
		}
	}


	// --------------------------------------------------------------------------
	// 간단한 send
	// --------------------------------------------------------------------------
	unsigned int WINDOWS_SOCKET::Send(const byte* buf, size_t bufLen, int flags)
	{
		int result = 0;
		if (m_socket != INVALID_SOCKET)
		{
			int result = send(m_socket, (const char *)buf, bufLen, flags);
		}

		if (result == SOCKET_ERROR)
		{
			printf("Send socket에러");
			int error = WSAGetLastError();
			if (WSAEWOULDBLOCK == error)
			{
				return result;
			}
			else
			{
				CloseSocket();
				return -1;
			}
		}
		
		return result;
	}

	// --------------------------------------------------------------------------
	// 간단한 Receive
	// --------------------------------------------------------------------------
	DWORD WINDOWS_SOCKET::Receive(byte* buf, size_t bufLen, int flags)
	{
		DWORD result = 0;
		if (m_socket != INVALID_SOCKET)
		{
			result = recv(m_socket, (char *)buf, bufLen, flags);
		}

		if (result == SOCKET_ERROR)
		{
			
			int error = WSAGetLastError();
			printf("recv socket에러 = %d", error);
			if (error == WSAEWOULDBLOCK)
			{
				return 0;
			}
			else
			{
				CloseSocket();
				return -1;
			}

		}
		if (result == 0)
		{
			CloseSocket();
			return -1;
		}
		

		return result;
	}

	// --------------------------------------------------------------------------
	// 간단한 Receive
	// --------------------------------------------------------------------------
	DWORD WINDOWS_SOCKET::Receive(WSABUF* buffer, LPOVERLAPPED& overlapped)
	{
		DWORD dwreceive_bytes = 0;
		DWORD flags = 0;
		int result = WSARecv(m_socket, buffer, 1, &dwreceive_bytes, &flags, overlapped, NULL);
		if (0 != result)
		{
			if (SOCKET_ERROR == result)
			{
				int error = WSAGetLastError();
				printf("recv socket에러 = %d", error);
				if (WSA_IO_PENDING != error)
				{
					CloseSocket();
					return -1;
				}
			}

		}

		return dwreceive_bytes;
		
	}

	// --------------------------------------------------------------------------
	// IO를 제어할떄 사용합니다.`~~
	// cmd 실행한 IO명령어
	// argp 명령에 대한 변수 포인터.
	// --------------------------------------------------------------------------
	void WINDOWS_SOCKET::IOCtl(long cmd, unsigned long *argp)
	{
		if (m_socket != INVALID_SOCKET)
		{
			int result = ioctlsocket(m_socket, cmd, argp);
			if (result == SOCKET_ERROR)
			{
				printf("IOCtl socket에러");
			}
		}
	}

	// --------------------------------------------------------------------------
	//	IO를 제어할떄 사용합니다.`~~
	//	dwIoControlCode
	//	[입력] 함수가 수행하는 연산의 제어코드
	//	lpvInBuffer
	//	[입력] 입력 버퍼에 대한 포인터
	//	cbInBuffer
	//	[입력] 입력 버퍼의 크기를 알리는 수치
	//	lpvOutBuffer
	//	[출력] 출력 버퍼에 대한 포인터
	//	cbOutBuffer
	//	[입력] 출력 버퍼의 크기를 알리는 수치
	//	lpcbBytesReturned
	//	[출력] 출력시 실제로 받아내는 바이트 수치를 포인트 합니다.
	//	lpOverlapped
	//	[입력] WSAOVERLAPPED 구조체의 포인터(비동기 소켓에서는 무시됩니다)
	//	lpCompletionRoutine
	//	[입력] 연산이 완료 되었을 때 호출될 완료루틴(completion routine)의 포인터(넌 - 오버랩 소켓에서는 무시됩니다.)
	// --------------------------------------------------------------------------
	void WINDOWS_SOCKET::IOWSAloctl(DWORD dwControlCode,LPVOID& lpvInBuffer,DWORD dwcbInBuffer,LPVOID& lpvOutBuffer,DWORD dwcbOutBuffer,LPDWORD& lpcbBytesReturnd,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
		if (m_socket != INVALID_SOCKET)
		{
			int result = WSAIoctl(m_socket, dwControlCode, lpvInBuffer, dwcbInBuffer, lpvOutBuffer, dwcbOutBuffer, lpcbBytesReturnd, lpOverlapped, lpCompletionRoutine);
			if (result == SOCKET_ERROR)
			{
				printf("IOWSAloctl socket에러");
			}
		}
	}


	
}
