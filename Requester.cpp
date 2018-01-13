 // Requester.cpp : Defines the entry point for the console application. 
// 

#include "stdafx.h"
#include "stdlib.h"
#include "iostream"
#include "string.h"
#include "UDP.h"

#pragma comment(lib, "Ws2_32.lib")	// !! libreria aggiornata gestione socket
//Definizioni Costanti
#define lunghezza 100
#define  MAXCHSTR 20
#define  PORT_RMA_NUM 21100	// Numero porta comunicazione socket Request/Mask
#define  PORT_RMO_NUM 21101	// Numero porta comunicazione socket Request/Monitor
#define _CRT_SECURE_NO_WARNINGS

//Elenco Variabili 
unsigned char buffer_T[1024];	// Buffer Trasmissione
unsigned char buffer_R[1024];	// Buffer Ricezione
unsigned char ACK[4];
unsigned char T_Comando[1024];	//Comando Inserito
unsigned char T_Esito[1024];	//Esito Ricevuto
using namespace std;
char IP_MASK[20] = "172.16.7.122";
char IP_MONITOR[20] = "172.16.7.116";

// Converte Indirizzo IPv4 in numero binario 32 bit
unsigned long IP_to_bin(char ip_address[]);

//Elenco Funzioni
void Acquisizione_Comando();
bool Send_To_Mask();
bool Receive_From_Mask();
bool Send_To_Monitor();
bool Send_To_Monitor_Comando();
//Elenco Variabili
unsigned long ip_address;
unsigned long ip_address_Mask;
unsigned long ip_address_Monitor;
unsigned short port_number;
unsigned short port_number_To_Mask;
unsigned short port_number_To_Monitor;
char str_ip_address[MAXCHSTR];
int num_pacchetti = 0;
int n;

// Creazione oggetto di classe UDP (socket con porta n. 23365)
UDP socket_RMA(PORT_RMA_NUM);
UDP socket_RMO(PORT_RMO_NUM);
//cout<<"coderr= "<<socket.coderr<<"\r\n";	// Display controllo 




void main(void) //Scritta da Walter e Modificata da Custureri
{
	bool esito;
	ACK[0] = 06;
	ACK[1] = '\0';

	int n = 0;
	
	// Richiesta IP ADDRESS Mask (IPv4)
	strcpy(str_ip_address, IP_MASK);
	// Conversione IP ADDRESS in numero binario 32 bit
	ip_address_Mask = IP_to_bin(str_ip_address);
	cout << "ip_address_Mask = " << ip_address_Mask << "\r\n\n";	// Display controllo
	port_number_To_Mask = PORT_RMA_NUM;
	// Richiesta IP ADDRESS Monitor (IPv4)															
	strcpy(str_ip_address, IP_MONITOR);
	// Conversione IP ADDRESS in numero binario 32 bit
	ip_address_Monitor = IP_to_bin(str_ip_address);
	cout << "ip_address_Monitor = " << ip_address_Monitor << "\r\n\n";	// Display controllo
	port_number_To_Monitor = PORT_RMO_NUM;
	do //Ciclo per la richiesa di più comandi
	{
		Acquisizione_Comando();
		esito = Send_To_Mask();
		esito = Receive_From_Mask(); //Richiamo delle funzioni: Send_To_Monitor(),Send_To_Monitor_Comando()
		cout << esito << endl;
	} while (toupper((char)T_Comando) != toupper((char)("EXIT")));
	
	//system("EXIT");
	
}





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

void Acquisizione_Comando()	//Scritta da Walter
{
	char comando[150];
	int i = 0;
	cout << "Digita comando: ";
	//cin.ignore();
	do
	{
		i++;
		if (i>1) {		cout << "Digita comando: ";		}
		cin.getline(comando, 150);
	} while ((strlen(comando) == 0));
	i = 0;
	do
	{
		i++;
	} while (comando[i] != '\0');
	comando[i + 1] = '\0';
	for (int j = 0; j < i + 1; j++)
	{
		buffer_T[j] = comando[j];
		T_Comando[j] = comando[j];	
	}
	cout << "contenuto del buffer di Trasmissione: \n" << buffer_T<<endl;

}

bool Send_To_Mask()		//Scritta da Walter
{

	socket_RMA.send(ip_address_Mask, port_number_To_Mask, buffer_T, sizeof(buffer_T));
	socket_RMA.receive(&ip_address, &port_number, buffer_R, sizeof(buffer_R));
	if ((strcmp( ((char*)buffer_R), ((char*)ACK) )==0))
	{
		cout << "\n Da Mask " << buffer_R << endl;;

		return true;
	}
	else
	{
		return false;
	}
	

}

bool Receive_From_Mask()  //Scritta da Walter, implementazione della funzione scritta da Giganti, implementazione della funzione send_to_monitor di Custureri
{

	for (int k = 0; k < 1024; k++) {							//pulizia buffer 
		buffer_R[k] = ' ';
	}
	cout << "\n a \n";
	n = socket_RMA.receive(&ip_address, &port_number, buffer_R, sizeof(buffer_R)); //ricezione num pacchetti
	cout << "\n b \n";
	//buffer_R[n + 1] = '\0';
	cout << "numero pacchetti:  " << buffer_R << endl;
	num_pacchetti = atoi((char*)buffer_R);	
	Send_To_Monitor_Comando();	//invio del comando al Monitor di Custureri
	socket_RMA.send(ip_address_Mask, port_number_To_Mask, ACK, sizeof(ACK));  //invio un ack di corretta ricezione	
	cout << "ACK iviato a MASK";

	for (int i = 0; i < num_pacchetti; i++)								//riceve le righe e stampa a schermo
	{
		cout << "ok";
		int zz = socket_RMA.receive(&ip_address, &port_number, buffer_R, sizeof(buffer_R));
		cout << buffer_R;
		//buffer_R[zz] = '\0';
		socket_RMA.send(ip_address_Mask, port_number_To_Mask, ACK, strlen((char*)ACK));
		T_Esito[i] = buffer_R[i];
		Send_To_Monitor();	//invio della stringa Monitor di Custureri
		cout << "Mask: " << (char*)buffer_R << endl;
		for (int k = 0; k < 1024; k++) {							//pulizia buffer
			buffer_R[k] = ' ';
		}
	}

	return true;
	
}
bool Send_To_Monitor_Comando() //Scritta da Custureri e Modificata da Walter
{
	socket_RMO.send(ip_address_Monitor, port_number_To_Monitor, T_Comando, sizeof(buffer_T));
	socket_RMO.receive(&ip_address_Monitor, &port_number_To_Monitor, buffer_R, sizeof(buffer_R));
	if ((strcmp(((char*)buffer_R), ((char*)ACK)) == 0))
	{
		cout << "\n Da Monitor " << buffer_R << endl;
		return true;

	}
	else
	{
		return false;
	}
}
bool Send_To_Monitor() //Scritta da Custureri e Modificata da Walter
{
	socket_RMO.send(ip_address_Monitor, port_number_To_Monitor, T_Esito, sizeof(buffer_T));
	socket_RMO.receive(&ip_address_Monitor, &port_number_To_Monitor, buffer_R, sizeof(buffer_R));
	if ((strcmp(((char*)buffer_R), ((char*)ACK)) == 0))
	{
		cout << "\n Da Monitor " << buffer_R<<endl;
		return true;

	}
	else
	{
		return false;
	}


}

