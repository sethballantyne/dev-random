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
#include "Socket.h"

// TCP functionality required by the application when in server mode.
public ref class Server : Socket
{
private:
	// the local server socket
	TCPsocket m_serverSocket;

	// string representation of the clients IPV4 address. 
	// obviously, this is only instantiated when the client has connected to the server.
	System::String^ m_formattedClientIPAddress;

	// true if the server has been successfully initialised and is listening on a local port,
	// false otherwise. 
	bool m_initialised;

	// Initialises the server, creating the local server socket on the specified port. 
	// throws System::Exception on error.
	void Initialise(unsigned short port)
	{
		IPaddress ip;
		int result = SDLNet_ResolveHost(&ip, nullptr, port);
		if(result < 0)
		{
			throw gcnew System::Exception(System::String::Format("SDLNet_ResolveHost failed: {0}", gcnew System::String(SDL_GetError())));
		}

		m_serverSocket = SDLNet_TCP_Open(&ip);
		if(!m_serverSocket)
		{
			throw gcnew System::Exception(System::String::Format("SDLNet_TCP_Open failed: {0}", gcnew System::String(SDL_GetError())));
		}

		m_initialised = true;
	}

public:
	// puts the server in listen mode, where it awaits an incoming connection from a client.
	// returns true if a connection was successfully established with a client, otherwise false.
	bool AwaitConnection(unsigned int port)
	{
		if(!m_initialised)
		{
			Initialise(port);
		}

		m_remoteSocket = SDLNet_TCP_Accept(m_serverSocket);
		if(m_remoteSocket)
		{
			IPaddress* clientIPAddress = SDLNet_TCP_GetPeerAddress(m_remoteSocket);
			if(!clientIPAddress)
			{
				System::Console::WriteLine("SDLNet_TCP_GetPeerAddress failed: {0}", gcnew System::String(SDL_GetError()));
				Disconnect();

				return false;
			}

			m_connectionEstablished = true;
			Uint32 ipAddress = SDL_SwapBE32(clientIPAddress->host);
			m_formattedClientIPAddress = System::String::Format("{0}.{1}.{2}.{3}", ipAddress >> 24, (ipAddress >> 16) & 0xFF, (ipAddress >> 8) & 0xFF, ipAddress & 0xFF);

			return true;
		}

		return false;
	}

	// Gets the IPV4 address of the connected client as a String object.
	property System::String^ ClientIPAddress
	{
		System::String^ get()
		{
			return m_formattedClientIPAddress;
		}
	}
};