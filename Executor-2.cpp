#include "stdafx.h"
#include "stdlib.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include "UDP.h"
#define dim 111
#define dimerr 20
#define dimbuffer 1024
unsigned char ack[2] = { 06, '\0' };
int n_commands=0;
char coma[2] = { ';', '\0' };
char space[2] = { ' ' , '\0' };
UDP sMask(21102);
unsigned long ip_address;
unsigned short port_number;
char stringend = '\0';
char str[1000] = { " > C:\\Users\\t.digiorgio\\Desktop\\Executor\\Executor\\CMDResults.txt" };
unsigned char cd[67] = { "cd > C:\\Users\\t.digiorgio\\Desktop\\Executor\\Executor\\CMDResults.txt" };
unsigned char n[dimbuffer];
unsigned char cmdhistory[100000];
unsigned char tmphistory[100000];
char execute[60] = { "C:\\Users\\t.digiorgio\\Desktop\\Executor\\Executor\\Commands.bat" };
char error[50] = { "Comando errato" };
#pragma comment(lib, "Ws2_32.lib")

using namespace std;


void intToString(int num, char str[]) //Merlo e Grandesso
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

void conta() //Merlo
{
	int contarighe = 0;
	char strTemp[1024];
	ifstream results;
	results.open("CMDResults.txt");
	do
	{
		results.getline(strTemp, 1023);
		contarighe++;
	} while ((!results.eof()));
	results.close();
	intToString(contarighe, (char*)n);
	sMask.send(ip_address, port_number, n, strlen((char*)n)); //invio del numero di righe
	cout << "SENT_N" << endl;
	cout << contarighe << endl;
	sMask.receive(&ip_address, &port_number, (unsigned char*)ack, sizeof(ack));
	cout << "ACK_N" << endl;
}

void fromFile() //Rivolta
{
	char strResult[1024];
	fstream result;
	result.open("CMDResults.txt", ios::out);
	if (result.eof())
	{
		result.close();
		result << error;
		result.close();
	}
	else
	{
		result.close();
		result.open("CMDResults.txt", ios::in);
		if (result.is_open())
		{
			while (!result.eof())
			{
				result.getline(strResult, 1023);
				//strResult[strlen(strResult) - 1] = '\0';
				sMask.send(ip_address, port_number, (unsigned char*)strResult, strlen((char*)strResult));
				cout << "SENT_LINE" << endl;
				cout << "$" << strResult << "$" << endl;
				sMask.receive(&ip_address, &port_number, (unsigned char*)ack, sizeof(ack));
				cout << "ACK_LINE" << endl;
			}
		}
		result.close();
	}
}

int main(void)	//Di Giorgio
{
	fstream exe;
	fstream White_List, Results;
	char DOScommand_list[dim];
	cout << "Servizio attivo..." << "\r\n";
	while (true)
	{
		exe.open("Commands.bat", ios::out);
		Results.open("CMDResults.txt", ios::out);
		Results << "";
		Results.close();
		int j = 0;
		int k = 0, w = 0;
		bool flag = false;
		sMask.receive(&ip_address, &port_number, n, sizeof(n));
		cout << "RECEIVED" << endl;
		sMask.send(ip_address, port_number, (unsigned char*)ack, sizeof(ack));
		cout << "ACK_SENT" << endl;
		/*n[0] = 'c';
		n[1] = 'd';
		n[2] = '.';
		n[3] = '.';
		n[4] = '\0';*/
		White_List.open("WhiteList.txt", ios::in);
		if (White_List.is_open())
		{
			while ((!White_List.eof()) && (!flag))
			{
				White_List >> DOScommand_list;
				k = 0;
				int cont = 0;
				flag = false;
				while (DOScommand_list[w] != '\0')
				{
					cont++;
					if ((n[w] >= 97) && (n[w] <= 122)) //97 = a 122 = z
					{
						n[w] = toupper(n[w]);
					}
					w++;
				}
				w = 0;
				for (int i = 0; i < cont; i++)
				{
					if (DOScommand_list[i] != ';')
					{
						if (DOScommand_list[i] == n[k])
						{
							w++;
						}
						k++;
					}
					else
					{
						if (k == w)
						{
							flag = true;
						}
					}
				}
			}
		}
		if (flag)
		{
			cout << "Comando corretto" << endl;
			if (n_commands == 0)
				strcpy((char*)cmdhistory, (char*)n);
			else
				strcat((char*)cmdhistory, (char*)n);
			strcpy((char*)tmphistory, (char*)cmdhistory);
			//Rivolta
			if (((n[0] == cd[0]) || (n[0] == (cd[0] - 32))) && ((n[1] == cd[1]) || (n[1] == (cd[1] - 32))))
			{
				//strcat((char*)tmphistory, space);
				exe << tmphistory << "\n\r";
				exe << cd << "\n\r";
				exe.close();
				system(execute);
				cout << "CD" << endl;
			}
			else
			{
				strcat((char*)tmphistory, str);
				exe << tmphistory << "\n\r";
				exe.close();
				system(execute);
			}
			n_commands++;
			conta();
			fromFile();
		}
		else  //Comando non contenuto nella white list
		{
			char errore[dimerr] = { "Comando sbagliato" };
			int val = 1;
			sMask.send(ip_address, port_number, (unsigned char*)val, sizeof(unsigned int));
			sMask.receive(&ip_address, &port_number, (unsigned char*)ack, sizeof(ack));
			sMask.send(ip_address, port_number, (unsigned char*)errore, sizeof(errore));
			sMask.receive(&ip_address, &port_number, (unsigned char*)ack, sizeof(ack));
			cout << "Comando errato" << endl;
		}
	}
}
