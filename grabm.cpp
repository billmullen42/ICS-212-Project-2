/*****************************************************************************
//
// NAME:	William (Bill) Mullen
//
// HOMEWORK:	Project 2
//
// CLASS:	ICS 212
//
// INSTRUCTOR:	Ravi Narayan
//
// DATE:	July 25th, 2016
//
// FILE:	grabm.cpp
//
//  DESCRIPTION:
//		Grabm = "grab Mullen"
//		This file contains customized input functions:
//		grabmi = "grab Mullen integer", accepts int but rejects float
//		grabms = "grab Mullen string", accepts 1-line input
//		grabma = "grab Mullen address", accepts multi-line input
//
//***************************************************************************/

#include<iostream>
#include<cstring>
#include<limits>
#include"grabm.h"

using namespace std;

/*****************************************************************************
//
// FUNCTION NAME:	grabmi
//
// DESCRIPTION:		A function to capture a positive integer from input, 
//		rejecting float values, negative numbers,  or other bad input, 
//		and prompting the user to re-enter.
//
// PARAMETERS:		None
//
// RETURN VALUES:	The int value captured
//
//***************************************************************************/

int grabmi()
{
	int exit;
	int i;
	char trash[250];

	exit = 0;
	i = 0; 
	trash[0] = '\0';

	#ifdef DEBUG
		cout << "\nDBM: Function 'grabmi' called (no parameters)";
	#endif	

	if (!cin)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	while (exit == 0)
	{
		cout << "Your input: ";	
		cin >> i;
		if (!cin)
		{
			cout << "\nError1: please enter a positive integer.\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			i = 0;
		}
		else if (i <= 0)
		{
			cout << "\nError2: please enter a positive integer.\n";
		}
		else /* This last check rejects floats and other weird input */
		{
			cin.get(trash, numeric_limits<streamsize>::max(), '\n');
			if (trash[0] != NULL)
			{
				i = 0;
				cout << "\nError3: please enter a positive integer.\n";
			}
		}

		if (i > 0)
			exit = 1;			
	}

	if (!cin)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	return i;
}

/*****************************************************************************
//
// FUNCTION NAME:	grabms
//
// DESCRIPTION:		A function to capture text input, rejecting input that
//		is too long, and prompting user to re-enter.
//
// PARAMETERS:		char field[]:	The target array to be entered
//			int size:	The maximum size of target array
//
// RETURN VALUES:	None
//
//***************************************************************************/

void grabms (char field[], int size)
{
	int exit;
	int fieldlen;

	exit = 0;
	field[0] = '\0';
	fieldlen = 0;

	#ifdef DEBUG
		cout << "\nDBM: Function named 'grabms' called, passed args:";
		cout << "\n\t'field':\t" << field;
		cout << "\n\t'size':\t" << size;
		cout << "\n";
	#endif	

	if (!cin)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	while (exit == 0)
	{
		cin.getline(field, 80);
		fieldlen = strlen(field);
		
		#ifdef DEBUG
			cout << "\nfield = >" << field << "<";
		#endif

		if ((fieldlen) >= size)
		{
			cout << "\nInput exceeds maximum length, maximum is " << size << " characters.\nPlease re-enter:\n";
			field[0] = '\0';
			if (!cin)
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
		else
		{
			exit = 1;
		}
	}

	if (!cin)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}


/*****************************************************************************
//
// FUNCTION NAME:	grabma
//
// DESCRIPTION:		A function to capture multi-line input, such as an address. '!' is used as an escape 
//			character when entered by the user.
//
// PARAMETERS:		char field[]:	The target array to be entered
			int size:	The maximum size of target array
//
// RETURN VALUES:	None
//
//***************************************************************************/

void grabma (char field[], int size)
{
	int exit;
	char temp[80];

	int templen;
	int fieldlen;

	exit = 0;
	temp[0] = '\0';
	field[0] = '\0';
	templen = 0;
	fieldlen = 0;

	#ifdef DEBUG
		cout << "\nDBM: Function named 'grabma' called, passed args:";
		cout << "\n\t'field':\t" << field;
		cout << "\n\t'size':\t" << size;
		cout << "\n";
	#endif	

	if (!cin)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	while (exit == 0)
	{
		fieldlen = strlen(field);
		cin.getline(temp, 80);
		templen = strlen(temp);
		
		#ifdef DEBUG
			cout << "\nfield = >" << field << "<";
			cout << "\ntemp = >" << temp << "<";
		#endif

		if ((templen + fieldlen) >= size)
		{
			cout << "\nAddress exceeds maximum length, maximum is " << size << " characters.\nPlease re-enter:\n";
			field[0] = '\0';
			temp[0] = '\0';
			if (!cin)
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
		else
		{
			if (temp[(templen - 1)] == '!' )
			{
				temp[(templen - 1)] = '\0';
				exit = 1;
			}
			else
			{
				#ifdef DEBUG
					cout << "\ntemp = >" << temp << "<";
					cout << "\ntemplen = >" << templen << "<";
				#endif

				if (templen < 80)
					temp[templen] = '\n';
				if (templen < 79)
					temp[templen + 1] = '\0';
				templen = strlen(temp);

				#ifdef DEBUG
					cout << "\ntemp = >" << temp << "<";
					cout << "\ntemplen = >" << templen << "<";
				#endif
			}
	
			strcat (field, temp);
		}/*end of else, maximum len exceeded will jump to here*/
	}
	
	fieldlen = strlen(field);

	/*the following loop removes terminal returns*/
	while (field[(fieldlen - 1)] == '\n')
	{
		field[(fieldlen - 1)] = '\0';
		fieldlen = strlen(field);
	}

	cout << "\nI recorded address as:\n>" << field << "<\n";
	if (!cin)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

