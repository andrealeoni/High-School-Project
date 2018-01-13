#include <iostream>
using namespace std;
extern "C"
{
#include "WinSock2.h"
}

class UDP
{
private:

	SOCKET socket_id; // Identificatore del socket


public:

	int coderr;		// Codice errore per display controllo

					// Costruttore
					// Parametro: numero di porta locale (Default= numero da definire)
	UDP(unsigned short port_number)
	{
		WSADATA wsaData;
		struct sockaddr_in add;	// Struttura per indirizzo
		unsigned long arg = 1;	// Parametro per funzione ioctlsocket

								// Inizializzazione WinSock (versione 2.02)
		if (WSAStartup(0x0202, &wsaData) != 0)
			//if (WSAStartup(0x0232, &wsaData) != 0)
		{
			socket_id = INVALID_SOCKET;	// ERRORE Inizializzazione Socket UDP
			coderr = -1;
			cout << coderr;
			return;
		}

		// Apertura socket UDP
		if ((socket_id = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
		{
			WSACleanup();	// ERRORE Apertura Socket UDP
			coderr = -2;
			cout << coderr;
			return;
		}

		// Costruzione struttura indirizzo
		memset(&add, 0, sizeof(add));	// Azzeramento struttura
		add.sin_family = AF_INET;		// Dominio indirizzo IP
		add.sin_addr.s_addr = 0;		// Indirizzo IP locale
		add.sin_port = htons(port_number); // Numero porta assegnato
		if (bind(socket_id, (struct sockaddr*)&add, sizeof(add)) == SOCKET_ERROR)
		{
			closesocket(socket_id);
			WSACleanup();	// ERRORE Associazione Porta Socket UDP
			socket_id = INVALID_SOCKET;
			coderr = -3;
			cout << coderr;
			return;
		}

		/* Impostazione del socket come non bloccante
		if (ioctlsocket(socket_id, FIONBIO, &arg) == SOCKET_ERROR)
		{
			closesocket(socket_id);
			WSACleanup();	// ERRORE Socket non Bloccante UDP
			socket_id = INVALID_SOCKET;
			coderr = -4;
			return;
		}

		coderr = 0;
		return;	// Inizializzazione socket OK*/

	}


	// Distruttore
	~UDP(void)
	{
		if (socket_id != INVALID_SOCKET)
			closesocket(socket_id);	// Chiusura socket
									// Terminazione WinSock
		WSACleanup();
		return;
	}


	// Trasmissione dati al computer remoto.  Parametri:
	// ip_address:	Indirizzo IP Destinatario
	// port_number:	Numero porta UDP Destinatario
	// buffer:		Dati da trasmettere
	// byte:		Numero di byte da trasmettere
	// Valore Restituito: -1 se Errore 0 se OK
	int send(unsigned long ip_address, unsigned short port_number, unsigned char buffer[], int byte)
	{
		struct sockaddr_in add;	// Struttura per indirizzo Destinatario
		int n;

		if (socket_id == INVALID_SOCKET)
		{
			coderr = -5;
			cout << coderr;
			return -1;
		}

		// Costruzione struttura indirizzo
		memset(&add, 0, sizeof(add));	// Azzeramento struttura
		add.sin_family = AF_INET;		// Dominio indirizzo IP
		add.sin_port = htons(port_number); // Numero porta UDP
		add.sin_addr.s_addr = htonl(ip_address);// Indirizzo IP

												// Trasmissione Datagram
		if ((n = sendto(socket_id, (char*)buffer, byte, 0, (struct sockaddr*)&add, sizeof(add))) < 0)
		{
			coderr = -6;
			cout << coderr;
			return -1;
		}

		return n;

	} // End send


	  // Ricezione dati dal computer remoto.  Parametri:
	  // ip_address:	Restituisce l'indirizzo IP del Mittente
	  // port_number:	Restituisce Numero porta UDP Mittente
	  // buffer:		Dati ricevuti
	  // size:			Dimensione del buffer
	  // Valore Restituito: Numero byte ricevuti, -1 se Errore
	int receive(unsigned long* ip_address, unsigned short* port_number, unsigned char* buffer, int size)
	{
		struct sockaddr_in add;	// Struttura per indirizzo Mittente
		int dim = sizeof(add);
		int n;

		if (socket_id == INVALID_SOCKET)
		{
			coderr = -7;
			cout << coderr;
			return -1;
		}

		// Ricezione dati (Non Bloccante)
		if ((n = recvfrom(socket_id, (char*)buffer, size, 0, (struct sockaddr*)&add, &dim)) <= 0)
		{
			coderr = -8;
			//cout << coderr;
			return -1;
		}
		// Estrazione indirizzo IP e numero porta UDP
		*ip_address = (unsigned long)(ntohl(add.sin_addr.s_addr));
		*port_number = (unsigned short)(ntohs(add.sin_port));

		return n;

	} // End receive

};	// End class UDP

#pragma once
