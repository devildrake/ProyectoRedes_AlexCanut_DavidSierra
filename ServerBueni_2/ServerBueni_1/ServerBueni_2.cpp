#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

using namespace std;

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

struct jugadoresT {
	string name;
	int score;
};

void selection_sort(jugadoresT arr[], int length) {
	int minPos;
	jugadoresT tmp;
	for (int i = 0; i < length; i++) {
		minPos = i;

		for (int j = i + 1; j < length; j++) {
			if (arr[i].score < arr[j].score) {
				minPos = j;
			}
		}
		tmp = arr[i];
		arr[i] = arr[minPos];
		arr[minPos] = tmp;
	}
}

void selection_sortInteger(int arr[], int length) {
	int minPos;
	int tmp;
	for (int i = 0; i < length; i++) {
		minPos = i;

		for (int j = i + 1; j < length; j++) {
			if (arr[i] < arr[j]) {
				minPos = j;
			}
		}
		tmp = arr[i];
		arr[i] = arr[minPos];
		arr[minPos] = tmp;
	}
}

void CreateTop10() {
	jugadoresT arrayVacio[10];
	for (int i = 0; i < 10; i++) {
		arrayVacio[i].name = "---";
		arrayVacio[i].score = 0;
	}
	ofstream archivoSalidas;
	archivoSalidas.open("Top10.txt");
	archivoSalidas << "top10\nplayer\n" << arrayVacio[0].name << "\n" << to_string(arrayVacio[0].score) << "\nplayer\n" << arrayVacio[1].name << "\n" << to_string(arrayVacio[1].score) << "\nplayer\n" << arrayVacio[2].name << "\n" << to_string(arrayVacio[2].score) << "\nplayer\n" << arrayVacio[3].name << "\n" << to_string(arrayVacio[3].score) << "\nplayer\n" << arrayVacio[4].name << "\n" << to_string(arrayVacio[4].score) << "\nplayer\n" << arrayVacio[5].name << "\n" << to_string(arrayVacio[5].score) << "\nplayer\n" << arrayVacio[6].name << "\n" << to_string(arrayVacio[6].score) << "\nplayer\n" << arrayVacio[7].name << "\n" << to_string(arrayVacio[7].score) << "\nplayer\n" << arrayVacio[8].name << "\n" << to_string(arrayVacio[8].score) << "\nplayer\n" << arrayVacio[9].name << "\n" << to_string(arrayVacio[9].score) << "\n";
	archivoSalidas.close();
}

inline bool exist(const string &name) {
	ifstream file(name);
	if (!file) {
		return false;
	}
	else {
		return true;
	}
}


void CheckInsert(jugadoresT arr[], int length, jugadoresT toInsert, bool* shouldWriteBack) {
	jugadoresT tmp;
	jugadoresT tmp2;
	bool found = false;

	for (int i = 0; i < length; i++) {
		if (arr[i].score<toInsert.score && !found)
		{
			*shouldWriteBack = true;
			tmp = arr[i];
			arr[i] = toInsert;
			found = true;
			i++;
		}
		else if (found) {
			tmp2 = arr[i];
			arr[i] = tmp;
			tmp = tmp2;
		}
	}
	selection_sort(arr, length);
}


int __cdecl main(void)
{
	//Cosas sobre archivos 

	int i = 0;
	bool mustWriteBackTop10 = false;
	string line;
	if (!exist("Top10.txt")) {
		CreateTop10();
	}
	ifstream myfile("Top10.txt");
	ofstream outPutFile;
	jugadoresT losPlayers[10];
	int newScore;
	bool mustRead = false;
	bool hasReadName = false;
	string newName;

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (hasReadName) {
				losPlayers[i].score = stoi(line);
				i++;
				mustRead = false;
				hasReadName = false;
			}
			if (mustRead) {
				losPlayers[i].name = line;
				hasReadName = true;
			}
			if (line == "player") {
				mustRead = true;
			}
		}

		myfile.close();
	}

	bool finished = false;

	WSADATA wsaData;
	int iResult;
	int terminateOrder = 0;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int msgL;
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	char* sendbuf;
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);
	
	while (!finished) {

		iResult = listen(ListenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		// Accept a client socket
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		// No longer need server socket

		//closesocket(ListenSocket);

		// Receive until the peer shuts down the connection
		bool once = false;
		do {

			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

			//Esto se hace una vez
			if (!once) {
				msgL = iResult;
				once = true;
			}

			//Esto mientras iResult sea mayor a 0
			if (iResult > 0) {

				msgL = iResult;

				printf("Bytes received: %d\n", iResult);

				// Echo the buffer back to the sender
				string superString = losPlayers[0].name + "?" + to_string(losPlayers[0].score) + "?" + losPlayers[1].name + "?" + to_string(losPlayers[1].score) + "?" + losPlayers[2].name + "?" + to_string(losPlayers[2].score) + "?" + losPlayers[3].name + "?" + to_string(losPlayers[3].score) + "?" + losPlayers[4].name + "?" + to_string(losPlayers[4].score) + "?" + losPlayers[5].name + "?" + to_string(losPlayers[5].score) + "?" + losPlayers[6].name + "?" + to_string(losPlayers[6].score) + "?" + losPlayers[7].name + "?" + to_string(losPlayers[7].score) + "?" + losPlayers[8].name + "?" + to_string(losPlayers[8].score) + "?" + losPlayers[9].name + "?" + to_string(losPlayers[9].score) + "!";

				char * S = new char[superString.length() + 1];
				if (recvbuf[0] == '1') {

					cout << "TryingTodoShit" << endl;

					//Si recibe un uno tiene que hacer un send de los datos del Top 10 leídos desde el archivo
	

					std::strcpy(S, superString.c_str());


					cout << S << endl;

				}

				//Emision original
				iSendResult = send(ClientSocket, S, superString.length(), 0);

				//iSendResult = send(ClientSocket, recvbuf, iResult, 0);
				if (iSendResult == SOCKET_ERROR) {
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(ClientSocket);
					WSACleanup();
					return 1;
				}
				printf("Bytes sent: %d\n", iSendResult);
			}
			else if (iResult == 0)
				printf("Connection closing...\n");
			else {
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}


		} while (iResult > 0);



		// shutdown the connection since we're done
		iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

		// cleanup

		//cout << "Wanna terminate connecion?" << endl;
		//cout << "1-- Yes" << endl;
		//cout << "0-- No" << endl;

		//! es fin de lectura


		for (int i = 0; i < msgL; i++) {
		
			if (recvbuf[1] == '1') {

			}

			if (recvbuf[i] == '!') {
				break;
			}

			else {
				cout <<recvbuf[i];
			}
			
		}
		cout << endl;
		once = false;
		//cin >> terminateOrder;

		if (terminateOrder == 1) {
			finished = true;
		}

		if (finished) {

			closesocket(ListenSocket);
			closesocket(ClientSocket);
			WSACleanup();
		}
	}
}
