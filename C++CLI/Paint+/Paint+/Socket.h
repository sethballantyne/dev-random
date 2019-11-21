// Copyright(c) 2019 Seth Ballantyne <seth.ballantyne@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files(the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
#pragma once
#include <SDL_net.h>

// Implements TCP functionality required by the application regardless 
// of whether it's in server or client mode. 
public ref class Socket
{
protected:
	// the socket created when a connection is established with a remote machine
	TCPsocket m_remoteSocket;

	// true if a connection has been established with a remote machine, false otherwise. 
	bool m_connectionEstablished = false;
public:
	// Kills the connection with the remote machine and resets the local state.
	void Disconnect()
	{
		SDLNet_TCP_Close(m_remoteSocket);
		m_remoteSocket = nullptr;
		m_connectionEstablished = false;
	}

	// Reads up to 32 bytes from the incoming TCP stream. 
	// Returns nullptr on failure, otherwise returns the received data as a String object.
	System::String^ ReceivePacket()
	{
		char data[32];
		int length = SDLNet_TCP_Recv(m_remoteSocket, data, 32);
		if(length > 0)
		{
			return gcnew System::String(data);
		}

		return nullptr;
	}

	// sends upto 32 bytes of data specified in 'data'.
	// the amount is an arbitrary amount chosen just because; change if you wish.
	// returns false if sending failed, otherwise false. If sending failed, it's most
	// likely because the connection was terminated.
	bool SendPacket(char* data)
	{
		int result = SDLNet_TCP_Send(m_remoteSocket, data, 32);
		if(result < 32)
		{
			// connection terminated or some other error.
			Disconnect();
			return false;
		}

		return true;
	}

	// Gets or sets the boolean value indicating a connection has been established
	// with a remote machine.
	property bool ConnectionEstablished
	{
		bool get()
		{
			return m_connectionEstablished;
		}
	}
};