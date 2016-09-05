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
// FILE:	main.cpp
//
//  DESCRIPTION:
//		This file contains the main menu functionality for a 
//	program that would allow the user to interact with database 
//	records. Database functions are found in llist.cpp, and input-
//	gathering functions are found in grabm.cpp.
//
//***************************************************************************/

#include<iostream>
#include<string>
#include"record.h"
#include"llist.h"
#include"grabm.h"

using namespace std;

/*****************************************************************************
//
// FUNCTION NAME:	main
//
// DESCRIPTION:		A userinterface function. This function accepts 
//			user input and calls other functions.
//
// PARAMETERS:		int argc:	the number of command-line arguments
			char *argv[]:	the argument vector
//
// RETURN VALUES:	None
//
//***************************************************************************/

main(int argc, char *argv[])
{
	/* Declarations */
	llist::llist *database;
	llist::llist *tempList;

	int found;       /*Number of accounts found by a given function*/

	int entry;
	int exit;
	int menu;
	char trash[250];	
	char customfilename[16];
	char customfilename2[16];
	
	int uaccountno;
	char uname[25];
	char uaddress[80];
	int uyob;

	int namelen;
	int addrlen;
	int nlen;
	int cfnlen; /*max length of custom filename */
	int cfnactual;

	/* Initializations */
	found = 0;
	entry = 0;
	exit = 0;
	menu = 0;
	uaccountno = 0;
	namelen = (sizeof(uname)/sizeof(char));
	addrlen = (sizeof(uaddress)/sizeof(char));
	cfnlen = (sizeof(customfilename)/sizeof(char));
	cfnactual = 0;
	nlen = 0;

	/*Command line handling*/
	if (argc != 1)
	{
	
		cout << "\nError, invalid number of arguments\nDo not use command-line arguments with this program.\n";
		exit = 1;
		entry = -1;
	}
	
	/* Program Launch Greeting/Actions */
	if (exit == 0)
	{
		cout << "\nWelcome to your database!\n\n";
	}
	
	/* Database file management menu */
	while (entry == 0)
	{
		cout << "Please select from the following menu:\n";
		cout << "\t1. Load records from the default database\n";
		cout << "\t2. Load records from an alternate database file\n";
		cout << "\t3. Copy records to a new file\n";
		cout << "\t4. Quit the program\n";

		menu = grabmi();
		
		if (menu <= 0)
			cout << "\nError: Please enter a valid input\n\n";
		if (menu > 4)
			cout << "\nError: Please enter a valid input\n\n";
		if (menu == 1) /*Default Constructor*/
		{
			database = new llist();		
			entry = 1;
		}
		if (menu == 2)
		{
			cout << "\nPlease enter a filename to load or create:"; 
			cout << "\n(max " << (cfnlen - 4) << " characters, do not include file extension (.txt) in your input)";
			grabms(customfilename, (cfnlen - 4));
			strcat(customfilename, ".txt");
			cfnactual = strlen(customfilename);
			if (cfnactual > cfnlen)
			{
				cout << "\nFilename too long.\n";
			}
			else
			{
				database = new llist (customfilename);
				entry = 1;
			}	
		}
		if (menu == 3)
		{
			cout << "\nThis menu option is currently unavailable\n";
/*			cout << "\nPlease enter a destination filename:"; 
			cout << "\n(max " << (cfnlen - 4) << " characters, do not include file extension (.txt) in your input.)";
			cout << "\n(Type 'database' to overwrite the default database.)";
			grabms(customfilename, (cfnlen - 4));
			strcat(customfilename, ".txt");
			cfnactual = strlen(customfilename);
			if (cfnactual > cfnlen)
			{
				cout << "\nFilename too long.\n";
			}
			else
			{
				database = new llist (customfilename);

				cfnactual = 0;

				cout << "\nPlease enter a source filename:"; 
				cout << "\n(max " << (cfnlen - 4) << " characters, do not include file extension (.txt) in your input.)";
				cout << "\n(Type 'database' to use the default database as your source.)";
				grabms(customfilename2, (cfnlen - 4));
				strcat(customfilename2, ".txt");
				cfnactual = strlen(customfilename2);
				if (cfnactual > cfnlen)
				{
					cout << "\nFilename too long.\n";
				}
				else
				{
					tempList = new llist (customfilename2);
					
					cout << "\n\nAre you sure you want to replace the records (if any) in " << customfilename;
					cout << " with the records in " << customfilename2 << "?";
					cout << "\nType '1' for yes, any other number to return to the file management menu.";
					menu = 0;
					menu = grabmi();
					if (menu == 1)
					{
						database = tempList;
						delete tempList;
						cout << " (From " << customfilename2 << ")";
						cout << "\nOn exit, records will be save to " << customfilename << ".\n";
						entry = 1;
					}
				}
			}
*/		}			
		if (menu == 4)
		{
			exit = 1;
			entry = 1;
			cout << "\nBut we were just getting to know each other!\nGoodbye.\n\n";
		}	
		menu = 0;	
	}

	/* Main Menu */
	cout << "\n\n";
	menu = 0;
	while (exit == 0)
	{	
		cout << "Please select from the following menu:\n";
		cout << "\t1. Add a new record in the database\n";
		cout << "\t2. Modify a record in the database using accountno as the key\n";
		cout << "\t3. Print information about a record using accountno as the key\n";
		cout << "\t4. Print all information in the database\n";
		cout << "\t5. Delete an existing record from the database using the accountno as a key\n";
		cout << "\t6. Temporarily reverse all records (For Novelty Purposes Only)\n";
		cout << "\t7. Quit the program\n";

		menu = grabmi();
		
		if (menu <= 0)
			cout << "\nError: Please enter a valid input\n\n";
		if (menu > 7)
			cout << "\nError: Please enter a valid input\n\n";
		if (menu == 1) /*add*/
		{
			cout << "\nPlease enter account number:\n";
			uaccountno = grabmi();

			cout << "\nPlease enter name of account holder:\n";
			grabms(uname, namelen);

			cout << "\nPlease enter address (type ! when you are done):\n";
			grabma(uaddress, addrlen);

			cout << "\nPlease enter year of birth:\n";
			uyob = grabmi();
			
			found = (*database).addRecord(uaccountno, uname, uaddress, uyob);
			cout << "\n" << found << " record(s) have been successfully added.\n\n";
		}

		if (menu == 2) /*modify*/
		{
			cout << "\nPlease enter account number:\n";
			uaccountno = grabmi();

			/*New functionality checks for valid account number before asking for address.*/

			cout << "\nThe following record(s) will be modified:";
			found = (*database).printRecord(uaccountno);
			if (found == 0)
			{
				cout << "\nNo matching records found.\n\n";
			}
			else
			{			
				cout << "\nPlease enter new address (type ! when you are done):\n";
				grabma(uaddress, addrlen);
			
				found = (*database).modifyRecord(uaccountno, uaddress);
				cout << "\n" << found << " record(s) have been updated.\n\n";
			}
		}
		if (menu == 3) /*print*/
		{
			cout << "\nPlease enter account number:\n";
			uaccountno = grabmi();

			found = (*database).printRecord(uaccountno);
			cout << "\n" << found << " record(s) have been found with that account number.\n\n";
		}
		if (menu == 4) /*printall*/
			cout << (*database);
		if (menu == 5) /*delete*/
		{
			cout << "\nPlease enter account number:\n";
			uaccountno = grabmi();

			found = (*database).deleteRecord(uaccountno);
			cout << "\n" << found << " record(s) have been deleted.\n\n";
		}
		if (menu == 6) /*reverse*/
		{
			(*database).reverse();
		}
		if (menu == 7) /*quit*/
		{
			exit = 1;
			delete database;
			/* Destructor called, which writes to file, cleans up, and provides confirmation output msgs*/
			cout << "Goodbye\n\n";
		}
		
		/*reset variables in case next loop has an invalid input*/
		found = 0;
		uaccountno = 0;
		uname[0] = '\0';
		uaddress[0] = '\0';
		uyob = 0;
		menu = 0;
	} /* end of while loop for menu */
} /* end of main */
