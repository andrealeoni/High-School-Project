//Progetto sviluppato da Colombo Gloria e Grandesso Davide
#include "stdafx.h"
#include "iostream"
#include "stdio.h"
#include "string.h"
#include "UDP.h"

#pragma comment(lib, "Ws2_32.lib")	// !! libreria aggiornata gestione socket
#define  DIM 1024 //Dimensione stringa messaggio
#define  MAXCHSTR 20
#define  PORT_NUM 21101	// Numero porta comunicazione socket
using namespace std;

void main()
{
	unsigned char buffer[DIM];	// Buffer Ricezione/Trasmissione
	unsigned char ACK[2]; //	Protocollo scelto da gruppo di lavoro per la conferma di Messaggio ricevuto
	unsigned long ip_address;//	Indirizzo IP del REQUEST ottenuto alla prima ricezione del messaggio
	unsigned short port_number = PORT_NUM;//	Numero porta di comunicazione
	char ricevo[DIM];//	Variabile usata per il salvataggio in locale del messaggio ricevuto
	UDP socket(PORT_NUM);//	Inizializzazione socket UDP con numero porta fittizio
	ACK[0] = 06; ACK[1] = '\0';//	Assegnazione valore ACK
	while (true)
	{
		socket.receive(&ip_address, &port_number, buffer, sizeof(buffer));//	Ricezione del messaggio da parte del REQUEST
		strcpy_s(ricevo, (char*)buffer);//	Copia del messaggio in una variabile locale;	Utilizzo di strcpy_s al posto di strcpy perchè deprecato
		ricevo[strlen((char*)buffer)] = '\0';//	Forzatura del terminatore di stringa
		if (strcmp(ricevo, "exit") == 0)
		{
			system("exit");
		}
		cout << ricevo << endl;//	Stampa del messaggio ricevuto
		socket.send(ip_address, port_number, ACK, sizeof(ACK));//	Invio dell'ACK al REQUEST
	}
	

}  // End main