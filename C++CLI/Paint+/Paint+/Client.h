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
#include <msclr\marshal.h>
#include "Socket.h"

// Networking functionality that's specific to when the program is in client mode.
public ref class Client : Socket
{
public:
	// Connects the local machine to the remote server located at the specified IPV4 address and port.
	// throws System::Exception on error
	bool Connect(System::String^ ipAddress, unsigned short port)
	{
		IPaddress ip;
		msclr::interop::marshal_context^ marshalContext = gcnew msclr::interop::marshal_context();

		const char* ipaddress = marshalContext->marshal_as<const char*>(ipAddress);
		int result = SDLNet_ResolveHost(&ip, ipaddress, port);
		if(result < 0)
		{
			throw gcnew System::Exception(System::String::Format("SDLNet_ResolveHost failed: {0}", gcnew System::String(SDL_GetError())));
		}

		m_remoteSocket = SDLNet_TCP_Open(&ip);
		if(!m_remoteSocket)
		{
			//throw gcnew System::Exception(System::String::Format("SDLNet_TCP_Open failed: {0}", gcnew System::String(SDL_GetError())));
			return false;
		}

		delete marshalContext;
		return m_connectionEstablished = true;
	}
};