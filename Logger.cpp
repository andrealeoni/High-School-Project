/*
---------------LOGGER---------------
Andrea Leoni & Alessia Colombo, 5D1
*/

#include "stdafx.h"
#include <iostream>
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"
#include <fstream>
#include "UDP.h"

#pragma comment(lib, "Ws2_32.lib")	// !! libreria aggiornata gestione socket

#define  MAXCHSTR 20
#define  PORT_NUM 21103	// Numero porta comunicazione socket
#define  NSEC 10	// Secondi Timeout
#define  TIMEOUT NSEC * CLOCKS_PER_SEC


using namespace std;

fstream file_var;

// Converte Indirizzo IPv4 in numero binario a 32 bit
unsigned long IP_to_bin(char ip_address[]);
char f_receive();
void f_salva_file();

void main()
{
	// inizializzazione variabili
	unsigned char buffer[1024];	// Buffer di ricezione
	unsigned long ip_address;
	unsigned short port_number = PORT_NUM;
	unsigned int *num = (unsigned int*)buffer;	// Numero generato restituito
	int n;
	char str_ip_address[MAXCHSTR];
	UDP socket(PORT_NUM); // Inizializzazione socket UDP con numero porta fittizio
	char spaziatura1[4] = { ' ', ' ', '|', ' ' };
	char spaziatura2[5] = { ' ', ' ', '|', ' ', ' ' };
	char esitoNegativo[8] = { 'N', 'E', 'G', 'A', 'T', 'I', 'V', 'O' };

	// creazione del file nel caso esista viene solo aperto e chiuso
	file_var.open("FileDiLog.txt", ios::app);
	file_var.close();

	// per ora poi lo si mette fisso
	// Richiesta IP ADDRESS server (IP v4)
	cout << "IP ADDRESS Server (es: 127.0.0.1): ";
	cin >> str_ip_address;
	// Conversione IP ADDRESS in numero binario 32 bit
	ip_address = IP_to_bin(str_ip_address);
	cout << "ip_address = " << ip_address << endl;	// Display controllo
	


	while (true)
	{
		// PARTE PER LA RICEZIONE E STAMPA DEL MESSAGGIO
		if ((n = socket.receive(&ip_address, &port_number, buffer, sizeof(buffer))) > 0)
		{
			// apertura de file
			file_var.open("FileDiLog.txt", ios::app);

			cout << "Messaggio originale:  " << buffer << endl;

			cout << "messaggio modificato: ";
			for (int i = 0; i < 10; i++)
			{
				cout << buffer[i];
				file_var << buffer[i];
			}
			cout << spaziatura1;
			file_var << spaziatura1;
			for (int i = 10; i < 19; i++)
			{
				cout << buffer[i];
				file_var << buffer[i];
			}
			cout << spaziatura2;
			file_var << spaziatura2;
			if (buffer[19] == '\0') // se non c'è scritto niente nel messaggio l'esito è negativo
			{
				cout << esitoNegativo;
				file_var << esitoNegativo;
			}
			else
			{
				for (int i = 19; i < n; i++)
				{
					cout << buffer[i];
					file_var << buffer[i];
				}
			}
			cout << endl;
			file_var << endl;

			file_var.close(); //chiusura file
		}
	}

	system("pause");
}

///////////////////////////////////////////////////////////////////
//     1970 - 01 - 01 01:00 : 00CIAO
///////////////////////////////////////////////////////////////////



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

char f_receive()
{
	return 0;
}

void f_salva_file()
{
}
