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
#include <SDL.h>
#include <msclr\marshal.h>
#include "Server.h"
#include "Client.h"

#define CNT_SERVER 0
#define CNT_CLIENT 1

public ref class Application
{
private:
	Socket^ m_socket;
	SDL_Surface *m_screen;
	Uint32 m_colour;
	int m_size = 1;
	unsigned short m_port;
	System::String^ m_ipAddress;
	bool m_initialised;
	System::ComponentModel::BackgroundWorker^ backgroundWorker = gcnew System::ComponentModel::BackgroundWorker();

	void OnDoWork(Object^ sender, System::ComponentModel::DoWorkEventArgs^ e)
	{
		if(m_socket->ConnectionEstablished)
		{
			ReadSocket();
		}
	}

	void ClearScreen(bool send)
	{
		SDL_FillRect(m_screen, nullptr, SDL_MapRGB(m_screen->format, 0, 0, 0));

		if(send && m_socket->ConnectionEstablished)
		{
			WriteSocket("clear");
		}
	}

	void Draw(int x, int y, int size, int colour, bool send)
	{
		switch(size)
		{
			case 1:
				DrawPixel(x, y, colour);
				break;
			default:
				SDL_Rect dest;
				dest.x = x;
				dest.y = y;
				dest.w = size;
				dest.h = size;

				SDL_FillRect(m_screen, &dest, colour);
				break;
		}

		if(send && m_socket->ConnectionEstablished)
		{
			WriteSocket("{0},{1},{2},{3}", x, y, size, colour);
			/*char data[32];
			memset(data, NULL, 32);
			sprintf(data, "%d,%d,%d,%d", x, y, size, colour);
			if(!m_socket->SendPacket(data))
			{
				if(m_socket->GetType() == Server::typeid)
				{
					System::Console::WriteLine("Client disconnected.");
				}
				else
				{
					System::Console::WriteLine("Server droppped.");
				}
			}*/
		}
	}

	void DrawPixel(int x, int y, int colour)
	{
		Uint8 *p = (Uint8 *)m_screen->pixels + y * m_screen->pitch + x * m_screen->format->BytesPerPixel;
		switch(m_screen->format->BytesPerPixel)
		{
			case 1:
				*p = colour;
				break;
			case 2:
				*(Uint16 *)p = colour;
				break;
			case 3:
				if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				{
					p[0] = (colour >> 16) & 0xFF;
					p[1] = (colour >> 8) & 0xFF;
					p[2] = colour & 0xFF;
				}
				else
				{
					p[0] = colour & 0xFF;
					p[1] = (colour >> 8) & 0xFF;
					p[2] = (colour >> 16) & 0xFF;
				}
				break;
			case 4:
				*(Uint32 *)p = colour;
				break;
		}
	}

	void ReadSocket()
	{
		System::String^ data = m_socket->ReceivePacket();
		if(nullptr != data)
		{
			if("clear" == data->ToLower())
			{
				ClearScreen(false);
			}
			else if(data->Contains(","))
			{
				array<System::String^, 1>^ splitData = data->Split(',');
				if(4 == splitData->Length)
				{
					int x = System::Convert::ToInt32(splitData[0]);
					int y = System::Convert::ToInt32(splitData[1]);
					int size = System::Convert::ToInt32(splitData[2]);
					int colour = System::Convert::ToInt32(splitData[3]);

					Draw(x, y, size, colour, false);
				}
			}
			else if("bye" == data->ToLower())
			{
				System::Console::WriteLine("Remote connection terminated.");
				m_socket->Disconnect();
			}
		}
	}

	void WriteSocket(System::String^ data, ...array<Object^, 1>^ args)
	{
		msclr::interop::marshal_context^ marshalContext = gcnew msclr::interop::marshal_context();
		System::String^ formattedStr = System::String::Format(data, args);

		const char* cdata = marshalContext->marshal_as<const char*>(formattedStr);

		if(!m_socket->SendPacket(const_cast<char*>(cdata)))
		{
			if(m_socket->GetType() == Server::typeid)
			{
				System::Console::WriteLine("Client disconnected.");
			}
			else
			{
				System::Console::WriteLine("Server droppped.");
			}
		}

		delete marshalContext;
	}

	void EstablishConnection()
	{
		if(m_socket->GetType() == Server::typeid)
		{
			System::Console::WriteLine("Listening on port {0}", m_port);
			if((safe_cast<Server^>(m_socket))->AwaitConnection(m_port))
			{
				System::Console::WriteLine("Connection established with {0}", (safe_cast<Server^>(m_socket))->ClientIPAddress);
			}
		}
		else
		{
			System::Console::WriteLine("Attempting to connect to {0}:{1}", m_ipAddress, m_port);
			if((safe_cast<Client^>(m_socket))->Connect(m_ipAddress, m_port))
			{
				System::Console::WriteLine("Connection established with server.");
			}
		}
	}
public:
	Application()
	{
		backgroundWorker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Application::OnDoWork);
		backgroundWorker->WorkerSupportsCancellation = true;
	}

	int Initialise(array<System::String^, 1>^ args)
	{
		if(args->Length < 2)
		{
			System::Console::WriteLine("No arguments supplied.");
			System::Console::WriteLine("To run as a client: Paint+.exe <ip address> <port>");
			System::Console::WriteLine("To run as a server: Paint+.exe <port>");

			return -1;
		}

		int result = SDL_Init(SDL_INIT_VIDEO);
		if(result < 0)
		{
			throw gcnew System::Exception(System::String::Format("SDL_Init failed: {0}", gcnew System::String(SDL_GetError())));
		}

		m_initialised = true;

		m_screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
		if(!m_screen)
		{
			throw gcnew System::Exception(System::String::Format("SDL_SetVideoMode failed: {0}", gcnew System::String(SDL_GetError())));
		}

		m_colour = SDL_MapRGB(m_screen->format, 255, 0, 0);

		result = SDLNet_Init();
		if(result < 0)
		{
			throw gcnew System::Exception(System::String::Format("SDLNet_Init failed: {0}", gcnew System::String(SDL_GetError())));
		}

		if(args->Length < 3)
		{
			m_socket = gcnew Server();
			m_port = System::Convert::ToInt32(args[1]);
			SDL_WM_SetCaption("Paint+ Server", nullptr);
		}
		else
		{
			m_socket = gcnew Client();
			m_ipAddress = args[1];
			m_port = System::Convert::ToInt32(args[2]);
			SDL_WM_SetCaption("Paint+ Client", nullptr);
		}

		return 0;
	}

	void Run()
	{
		bool quit;
		bool draw;
		SDL_Event event;

		while(!quit)
		{
			SDL_PollEvent(&event);

			if(!m_socket->ConnectionEstablished)
			{
				EstablishConnection();
			}

			switch(event.type)
			{
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_1:
							m_size = 1;
							break;
						case SDLK_2:
							m_size = 2;
							break;
						case SDLK_3:
							m_size = 3;
							break;
						case SDLK_4:
							m_size = 4;
							break;
						case SDLK_5:
							m_size = 5;
							break;
						case SDLK_6:
							m_size = 6;
							break;
						case SDLK_7:
							m_size = 7;
							break;
						case SDLK_8:
							m_size = 8;
							break;
						case SDLK_9:
							m_size = 9;
							break;
						case SDLK_r:
							m_colour = SDL_MapRGB(m_screen->format, 255, 0, 0);
							break;
						case SDLK_g:
							m_colour = SDL_MapRGB(m_screen->format, 0, 255, 0);
							break;
						case SDLK_b:
							m_colour = SDL_MapRGB(m_screen->format, 0, 0, 255);
							break;
						case SDLK_ESCAPE:
							quit = true;
							break;
						case SDLK_SPACE:
							ClearScreen(true);
							break;
						default:
							break;
					}
					break;
				case SDL_MOUSEMOTION:
					if(draw)
					{
						Draw(event.motion.x, event.motion.y, m_size, m_colour, true);
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					draw = true;
					break;
				case SDL_MOUSEBUTTONUP:
					draw = false;
					break;
				case SDL_QUIT:
					quit = true;
					break;
			}

			/*if(m_socket->ConnectionEstablished)
			{
				ReadSocket();
			}*/
			if(!backgroundWorker->IsBusy)
			{
				backgroundWorker->RunWorkerAsync();
			}
			
			SDL_Delay(15);
			SDL_Flip(m_screen);
		}

		if(m_socket->ConnectionEstablished)
		{
			WriteSocket("bye");
		}

		backgroundWorker->CancelAsync();
	}

	void Shutdown()
	{
		if(m_initialised)
		{

			if(m_screen)
			{
				SDL_FreeSurface(m_screen);
			}

			if(m_socket)
			{
				m_socket->Disconnect();
			}

			SDLNet_Quit();
			SDL_Quit();
		}
	}

	property bool Initialised
	{
		bool get() 
		{
			return m_initialised;
		}
	}
};