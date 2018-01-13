// fileserver.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "iostream"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"
#include "UDP.h"
#include "fstream"
#include "fileserver.h"
#pragma comment(lib, "Ws2_32.lib")
#define  MAXCHSTR 20
#define  PORT_NUM 23365
#define  DIM 50
#define MAX_LINES 1024
#define MAX_LINE_LEN 1024

using namespace std;

void intToString(int num, char str[]);
// Converte Indirizzo IPv4 in numero binario 32 bit
unsigned long IP_to_bin(char ip_address[]);

int main()
{
	unsigned char buffer[1024];
	unsigned long ip_address;
	unsigned short port_number= PORT_NUM;
	fstream nome_file;
	int j = 0;
	int n;
	int contarighe = 0;
	char str_ip_address[MAXCHSTR];
	unsigned char string[DIM];
	char lunghezza;
	

	UDP socket(PORT_NUM);

	cout << "Servizio attivo..." << "\r\n";


	char ciao[1024];
	


	do
	{
			//Dichiaro l'array di stringhe in cui andranno i dati del file
			
			//Apro il file

			if (((n = socket.receive(&ip_address, &port_number, buffer, sizeof(buffer))) > 0))
			{
				buffer[n] = '\0';
				if ((strcmp(((char*)buffer), "T2E3R")) != 0)
				{
					cout << endl;
					cout << "\n" << "Ricezione Datagram" << "\r\n";
					buffer[n] = '\0';	// Terminatore di stringa
					nome_file.open("ciao.txt", ios::in);

					//Grazio (Merlo)
					do
					{
						nome_file.getline(ciao, 1023);
						contarighe++;
					} while ((!nome_file.eof()));

					//Merlo

					intToString(contarighe, (char*)buffer);
					socket.send(ip_address, port_number, buffer, strlen((char*)buffer)); //invio del numero di righe 
					nome_file.close();
					nome_file.open("ciao.txt", ios::in);
					socket.receive(&ip_address, &port_number, buffer, sizeof(buffer)); //ricezione dell'ACK
					buffer[3] = '\0';
					cout << buffer;
					cout << endl;

					//Merlo

					if (strcmp((char*)buffer, "ACK") == 0)
					{
						do
						{

							//Ciclo che recupera i dati del file riga per riga e li mette in strings
								nome_file.getline(((char*)buffer), MAX_LINE_LEN);
								cout << buffer << endl;
								lunghezza = strlen((char*)buffer);
								buffer[lunghezza] = '\0';
								socket.send(ip_address, port_number, buffer, strlen((char*)buffer));
							/*nome_file >> ciao;
							strcpy(((char*)buffer), ciao);
							cout << buffer << endl;*/
							//socket.send(ip_address, port_number, buffer, strlen((char*)buffer));
							socket.receive(&ip_address, &port_number, buffer, sizeof(buffer));
						} while ((!nome_file.eof()));
					}
					cout << "Il file e' stato inviato correttamente " << endl;
				}
			}
	} while ((strcmp(((char*)buffer), "T2E3R")) != 0);
	nome_file.close();
	system("pause");
	return 0;
}


//Funzione Int to String Merlo Grandesso

void intToString(int num, char str[]) //Trasformazione da un numero ad una stringa
{
	int num1, num2 = 0, i = 0, div2, div1 = 10;
	num1 = num%div1;
	str[i] = (char)(num1 + 48);
	i++;
	while (num != num1)
	{
		num2 = num%div1;
		div2 = div1;
		div1 = div1 * 10;
		num1 = num%div1;
		for (int j = i; j > 0; j--)
		{
			str[j] = str[j - 1];
		}
		str[0] = (char)(((num1 - num2) / div2) + 48);
		i++;
	}
	str[i] = '\0';
}


//Merlo, Grazio e Giganti 

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


