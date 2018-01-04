#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <iostream>
#include "UDP.h"
#include "string.h"
#include "stdio.h"
#include "windows.h"
#include <ctime>

#pragma comment(lib, "Ws2_32.lib")	// !! libreria aggiornata gestione socket
#define DIM 1024					//Dimensione stringa messaggio
#define MAXCHSTR 20
#define PORTAREQ 21100	// Numero porta comunicazione socket con Requester
#define PORTAEXE 21102	// Numero porta comunicazione socket con Executer
#define PORTALOG 21103	// Numero porta comunicazione socket con Logger
using namespace std;

// Converte Indirizzo IPv4 in numero binario 32 bit
unsigned long IP_to_bin(char ip_address[]);


unsigned char buffer[DIM];	// Buffer Ricezione/Trasmissione
unsigned char ACK[4];
unsigned long ip_address_requester;
unsigned long ip_address_executer;
unsigned long ip_address_logger;
unsigned char MESSAGGIO_PER_EXECUTER[DIM];
unsigned char MESSAGGIO_PER_REQUESTER[DIM];
unsigned short port_number_req;
unsigned short port_number_exe;
unsigned short port_number_log;
time_t oraRicezioneReq;
time_t oraRicezioneExe;
UDP socketReq(PORTAREQ);
UDP socketExe(PORTAEXE);
UDP socketLog(PORTALOG);

//-------------------------------------------------------------------------------//
unsigned long IP_to_bin(char ip_add[]);
void SendtoLogger(unsigned char MESSAGGIO_PER_LOGGER[], time_t ora);
void ReceiveFromRequester();
void SendtoExecuter();
void ReceivefromExecuter();
void SendtoRequester();
void RichestaIpLogger();
void RichestaIpExecuter();


void main()
{
	ACK[0] = 'A';
	ACK[1] = 'C';
	ACK[2] = 'K';
	ACK[3] = '\0';
	RichestaIpLogger();
	RichestaIpExecuter();
	ReceiveFromRequester();
	SendtoExecuter();
	ReceivefromExecuter();
	SendtoRequester();
	system("pause");
	
}

void RichestaIpLogger() {
	char str_ip_address[MAXCHSTR];
	// Richiesta IP ADDRESS Executer
	cout << "IP ADDRESS Logger: ";
	strcpy(str_ip_address, "172.16.7.107");
	cout << str_ip_address;
	cout << endl;

	// Conversione IP ADDRESS in numero binario 32 bit
	ip_address_logger = IP_to_bin(str_ip_address);
}

void RichestaIpExecuter() {
	char str_ip_address[MAXCHSTR];
	// Richiesta IP ADDRESS Executer
	cout << "IP ADDRESS Executer: ";
	strcpy(str_ip_address, "172.16.7.112");
	cout << str_ip_address;
	cout << endl;

	// Conversione IP ADDRESS in numero binario 32 bit
	ip_address_executer = IP_to_bin(str_ip_address);
}
void SendtoLogger(unsigned char MESSAGGIO_PER_LOGGER[], time_t o) {
	port_number_log = PORTALOG;
	char ora[1024];
	strftime(ora, 30, "%Y-%m-%d %H:%M:%S", localtime(&o));
	strcat(ora, (char*)MESSAGGIO_PER_LOGGER );
	socketLog.send(ip_address_logger, port_number_log, (unsigned char*)ora, sizeof(ora));

}


void ReceiveFromRequester() {
	port_number_req = PORTAREQ;
	socketReq.receive(&ip_address_requester, &port_number_req, buffer, sizeof(buffer));
	time(&oraRicezioneReq);
	socketReq.send(ip_address_requester, port_number_req, ACK, sizeof(ACK));
	for (int i = 0; i < strlen((char*)(buffer)); i++)
	{
		MESSAGGIO_PER_EXECUTER[i] = buffer[i];
	}
		MESSAGGIO_PER_EXECUTER[strlen((char*)(buffer))] = '\0';
}

void SendtoExecuter() {
	port_number_exe = PORTAEXE;
	socketExe.send(ip_address_executer, port_number_exe, MESSAGGIO_PER_EXECUTER, sizeof(MESSAGGIO_PER_EXECUTER));
	socketExe.receive(&ip_address_executer, &port_number_exe, buffer, sizeof(buffer));
	SendtoLogger(MESSAGGIO_PER_EXECUTER, oraRicezioneReq);
	cout << "Conferma ricezione pacchetto ricevuta da ex: " << buffer << endl;
}
void ReceivefromExecuter() {
	port_number_exe = PORTAEXE;
	socketExe.receive(&ip_address_executer, &port_number_exe, buffer, sizeof(buffer));
	time(&oraRicezioneExe);
	socketExe.send(ip_address_executer, port_number_exe, ACK, sizeof(ACK)); // ack che manderò dopo aver consegnato ogni singola stringa al requester
	for (int i = 0; i < strlen((char*)(buffer)); i++)
	{
		MESSAGGIO_PER_REQUESTER[i] = buffer[i];
	}
	MESSAGGIO_PER_REQUESTER[strlen((char*)(buffer))] = '\0';
}

void SendtoRequester() {
	port_number_req = PORTAREQ;
	socketReq.send(ip_address_requester, port_number_req, MESSAGGIO_PER_REQUESTER, sizeof(MESSAGGIO_PER_REQUESTER));
	socketReq.receive(&ip_address_requester, &port_number_req, buffer, sizeof(buffer));
	SendtoLogger(MESSAGGIO_PER_REQUESTER, oraRicezioneReq);
	cout << "Conferma ricezione pacchetto da Requester: " << buffer << endl;
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


