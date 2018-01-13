// Fileclient.cpp 5D1 
// Ultimo aggiornamento 19/12/2017 10:30


#include "stdafx.h"
#include <iostream>
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"

#include "UDP.h"
#include "fstream"

#pragma comment(lib, "Ws2_32.lib")

#define  MAXCHSTR 20
#define  PORT_NUM 23365	// Numero porta comunicazione socket
using namespace std;

// Converte Indirizzo IPv4 in numero binario 32 bit
unsigned long IP_to_bin(char ip_address[]);

int main()
{
	unsigned char buffer[1024];				// Buffer Ricezione/Trasmissione
	unsigned char ACK[4];
	unsigned long ip_address;
	unsigned short port_number;
	char lunghezza;
	char nome_file[100];
	int num_pacchetti;
	bool rcv = FALSE;
	int n;



	//char rsp;
	char str_ip_address[MAXCHSTR];

	// Inizializzazione socket UDP con numero porta fittizio
	UDP socket(0);


	// Richiesta IP ADDRESS server (IP v4)
	cout << "IP ADDRESS Server (es: 127.0.0.1): ";
	cin >> str_ip_address;

	// Conversione IP ADDRESS in numero binario 32 bit
	ip_address = IP_to_bin(str_ip_address);
	cout << "ip_address = " << ip_address << "\r\n\n";	// Display controllo

	port_number = PORT_NUM;

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							// Giganti
	ACK[0] = 'A';
	ACK[1] = 'C';
	ACK[2] = 'K';
	ACK[3] = '\0';
	cout << "inserisci il nome del file: ";													//inserimento del nome del file
	cin.ignore();	
	cin.getline(nome_file, 100);
	for (int j = 0; j < 100; j++)															//ciclo che copia il nome del file nella stringa
	{
		buffer[j] = nome_file[j];
	}
	socket.send(ip_address, port_number, buffer, strlen((char*)buffer));					// invio buffer

	for (int k = 0; k < 1024; k++) {															//pulizia buffer
		buffer[k] = ' ';
	}

	
	//////////////////////////////////////	RICEZIONE NUMERO PACCHETTI	////////////////////////////////////////////////////////////////
						// Grazio con aiuto di Giganti
	
	if ((n = socket.receive(&ip_address, &port_number, buffer, sizeof(buffer))) > 0)		//ricevo num pacchetti
	{
		
		buffer[n + 1] = '\0';
		cout << "numero pacchetti:  " << buffer << endl;
		num_pacchetti = atoi((char*)buffer);
		socket.send(ip_address, port_number, ACK, strlen((char*)ACK));				//invio un ack di corretta ricezione	
		cout << endl;
	}
	else
	{
		cout << "Errore! " << endl;
	}

	//////////////////////////////////////// RICEZIONE RIGHE ////////////////////////////////////////////////////////////
						// Giganti						
	
	for (int i = 0; i < num_pacchetti; i++)													//riceve le righe e stampa a schermo
	{
		int zz=socket.receive(&ip_address, &port_number, buffer, sizeof(buffer));
		buffer[zz] = '\0';
		cout << "BUFFER:" << buffer << endl;
		cout << "num pacchetto: " << i << endl;
		socket.send(ip_address, port_number, ACK, strlen((char*)ACK));
		for (int k = 0; k < 1024; k++) {														//pulizia buffer
			buffer[k] = ' ';
		}
	}
	system("pause");
}

//Merlo Giganti Grazio
unsigned long IP_to_bin(char ip_add[])
// Converte Indirizzo IPv4 (es 127.0.0.1) in numero binario 32 bit
{
	unsigned long add;
	unsigned char byte;
	char *token;
	char *nextoken;

	if ((token = strtok_s(ip_add, ".", &nextoken)) == NULL)
		return 0x00000000;

	byte = (unsigned char)atoi(token);
	add = (unsigned long)byte * 16777216;

	if ((token = strtok_s(NULL, ".", &nextoken)) == NULL)
		return 0x00000000;

	byte = (unsigned char)atoi(token);
	add += (unsigned long)byte * 65536;

	if ((token = strtok_s(NULL, ".", &nextoken)) == NULL)
		return 0x00000000;

	byte = (unsigned char)atoi(token);
	add += (unsigned long)byte * 256;

	if ((token = strtok_s(NULL, ".", &nextoken)) == NULL)
		return 0x00000000;

	byte = (unsigned char)atoi(token);
	add += (unsigned long)byte * 1;

	return add;
} // End IP_to_bin