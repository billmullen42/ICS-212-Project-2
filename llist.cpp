/*****************************************************************
//
//  NAME: 		William Mullen
//
//  HOMEWORK:   	Project 2
//
//  CLASS:      	ICS 212
//
//  INSTRUCTOR: 	Ravi Narayan
//
//  DATE:       	July 25th, 2016               
//
//  FILE:        	llist.cpp
//
//  DESCRIPTION:     	This is a class file designed to manage a
//		database that contains account numbers, names,
//		addresses, and birth years. It has the following
//		specifications:
//
//		class llist
//		{
//		private:
//			record *	start;
//			char		filename[16];
//			int 		readfile();
//			int		writefile();
//			record *	reverse(record *);
//			void		cleanup();
//
//		public:
//			llist();
//			llist(char[]);
//			~llist();
//			int addRecord (int, char [], char[], int);
//			int printRecord (int);
//			int modifyRecord (int, char []);
//			void printAll ();
//			int deleteRecord (int);
//			void reverse();
//		};
//			
//****************************************************************/

#include<iostream>
#include<fstream>
#include<cstring>
#include"record.h"
#include"llist.h"

using namespace std;

/*****************************************************************
//
//  Function name:  llist::llist
//
//  DESCRIPTION:    This is the default constructor, which is hard-
//	coded to a database file.
//
//  Parameters:	   None
//
//  Return value:  None
//
//****************************************************************/

llist::llist ()
{
	
	#ifdef DEBUG
		cout << "\nDBM: Default constructor 'llist::llist'  called, no args passed:\n";
	#endif

	start = NULL;
	strcpy (filename, "database.txt");
	readfile();
}

/*****************************************************************
//
//  Function name:  llist::llist
//
//  DESCRIPTION:    This is an alternate constructor that can
//	receive a filename from the main function.	
//
//  Parameters:	   char pfilename[]: the name of the database file.
//
//  Return value:  None
//
//  NOTE: Main function should have error handling to prevent
//	invalid filenames being passed. Error handling within this
//	constructor is only a backup measure, to provide error 
//	output messages.
//
//****************************************************************/

llist::llist (char pfilename[])
{
	int plen;

	#ifdef DEBUG
		cout << "\nDBM: Alternate constructor 'llist::llist'  called, passed arg (filename):\n";
		cout << "\n";
	#endif

	plen = strlen(pfilename);
	if (plen > 16)
	{
		cout << "\n\nSEVERE ERROR: Invalid filename.\nVariables have not been initialized.\nDatabase not opened.\n\n";
	}
	else
	{
		start = NULL;
		strcpy (filename, pfilename);
		readfile();
		#ifdef DEBUG
			cout << "filename";
		#endif
	}
}
/*****************************************************************
//
//  Function name:  llist::operator=
//
//  DESCRIPTION:    This assignment operator transfers records from
//	a source llist object to a target llist object, without
//	changing the values of the target's 'filename' variables,
//	and using different allocated memory for 'start's target
//	address and all record addresses.
//
//  Parameters:	   llist& listB: the source list
//
//  Return value:  The target list.
//
//****************************************************************/

llist &llist::operator=(const llist& listB)
{
	struct record* activerec;
	struct record* nextrec;
	int pcount;

	int racc;
	char rname[25];
	char raddr[80];
	int ryob;


	activerec = NULL;
	nextrec = NULL;

	pcount = 0;	
	racc = 0;
	rname[0] = '\0';
	raddr[0] = '\0';
	ryob = 0;

	#ifdef DEBUG
		cout << "\nDBM: Alternate constructor 'llist::llist'  called, passed arg (filename):\n";
		cout << "\n";
	#endif

	/*Out with the Old*/
	if (start != NULL)
	{
		activerec = start;
		nextrec = (*activerec).next;
		
		while (activerec != NULL)
		{
			{
				pcount++;
				delete activerec;
			}											
			
			activerec = nextrec;
			if (activerec != NULL)
				nextrec = (*activerec).next;
		}
	}	
	
	cout << "\nDumped " << pcount << " old record(s)\n\n";
	pcount = 0;
	activerec = NULL;
	nextrec = NULL;

	/*In with the New*/
	if (listB.start != NULL)
	{
		activerec = listB.start;
		while (activerec != NULL)
		{

			racc = (*activerec).accountno;
			strcpy(rname, (*activerec).name);
			strcpy(raddr, (*activerec).address);
			ryob = (*activerec).yearofbirth;
				
			addRecord(racc, rname, raddr, ryob);
			activerec = (*activerec).next;
			pcount++;
			/*No way to return pcount, main function must update numCount after assignment operator is used)*/
		}
	}
}

/*****************************************************************
//
//  Function name:  llist::~llist
//
//  DESCRIPTION:    This is the destructor.
//
//  Parameters:	   None
//
//  Return value:  None
//
//****************************************************************/

llist::~llist()
{
	/* Declarations */
	struct record *activerec;			
	struct record *nextrec;
	int pcount;

	/* Initializations */
	activerec = NULL;
	nextrec = NULL;
	pcount = 0;

	#ifdef DEBUG
		cout << "\nDBM: Destructor 'llist::~llist' called (no parameters).\n";
	#endif	

	/*Actions*/
	writefile();

	if (start != NULL)
	{
		activerec = start;
		nextrec = (*activerec).next;
		
		while (activerec != NULL)
		{
			{
				pcount++;
				delete activerec;
			}											
			
			activerec = nextrec;
			if (activerec != NULL)
				nextrec = (*activerec).next;
		}
	}	
	
	cout << "\nSuccessfully closed "  << pcount << " active record(s).";
	cout << "\n\n";
}

/*****************************************************************
//
//  Function name:  llist::addRecord
//
//  DESCRIPTION:    This function adds a record to the database.
//		In the case of duplicate account numbers, addRecord will insert the new record before all matching duplicates that already exist.
//		Update: Previous version (pseudocode HW4) would have added duplicates after existing. This version (and above comment) adjusted to insert the newest record first before existing duplicates, in keeping with client's expected result as described in HW6 traces requirement.                 
//
//  Parameters:	int addaccount: The user's account number
//		char addname[]: The address of the array containing the user's name
//		char addaddr[]: The address of the array containing the user's address
//		int addyearofbirth: The user's year of birth
//     
//  Return value:  Number of records added
//
//****************************************************************/

int llist::addRecord (int addaccountno, char addname[], char addaddr[],int addyearofbirth)
{		
	int success;
	struct record *nextrec;
	struct record *prevrec;
	struct record *newrec;

	newrec = new struct record;
	success = 0;
	nextrec = NULL;
	prevrec = NULL;
	
	#ifdef DEBUG
		printf("\nDBM: Function named 'addRecord' called, passed args:\n");
		printf("\t'addaccountno':\t%d\n", addaccountno);
		printf("\t'addname':\t%s\n", addname);
		printf("\t'addaddr':\t%s\n", addaddr);
		printf("\t'addyearofbirth':\t%d\n", addyearofbirth);
	#endif	

	if (start != NULL)
	{
		nextrec = start;	

		while ((*nextrec).accountno < addaccountno)
		{
			prevrec = nextrec;
			if ((*prevrec).next == NULL)
			{
				nextrec = NULL;
				break;
			}
			else
			{
				nextrec = (*prevrec).next;
			}
		}
	}
	
	if (prevrec == NULL)
	{
		start = newrec;
	}
	else	
	{
		(*prevrec).next = newrec;
	}

	(*newrec).accountno = addaccountno;
	strcpy((*newrec).name, addname);
	strcpy((*newrec).address, addaddr);
	(*newrec).yearofbirth = addyearofbirth;
	(*newrec).next = nextrec;
	
	success = 1;
	
	return success;
}

/*****************************************************************
//
//  Function name:  llist::deleteRecord
//
//  DESCRIPTION:    This function deletes all records with a given account number.
//                 
//  Parameters:	   int delaccountno: the account number to be deleted.
//
//  Return value:  The number of records deleted.
//
//  Function call with given variables:
//	<object>.deleteRecord(uaccountno);
//
//****************************************************************/

int llist::deleteRecord(int delaccountno)
{
	struct record *activerec;
	struct record *nextrec;
	struct record *prevrec;
	int delcount;

	activerec = NULL;	
	nextrec = NULL;
	prevrec = NULL;
	delcount = 0;
	
	#ifdef DEBUG
		cout << "\nDBM: Function named 'deleteRecord' called, passed args:";
		cout << "\n\t'delaccountno':\t" << delaccountno;
		cout << "\n";
	#endif

	if (start != NULL)
	{
		activerec = start;					
		nextrec = (*activerec).next;
		
		while (activerec != NULL)
		{
			if (delaccountno == (*activerec).accountno)
			{
				if (prevrec == NULL)
					start = nextrec;		
				else
					(*prevrec).next = nextrec;
				delete activerec;
				delcount++;
			}	
			else													
			{													
				prevrec = activerec;
			}													
			
			activerec = nextrec;
			if (activerec != NULL)
				nextrec = (*activerec).next;
		}
	}	
	
	return delcount;
}

/*****************************************************************
//
//  Function name:  llist::modifyRecord
//
//  DESCRIPTION:    This function changes the address of all records 
//		with a given account number.
//                 
//  Parameters: int paccountno: 	The account number to modify.
//     		char addr[];		The new address.
//
//  Return value:  The number of records modified.
//
//  Function call with given variables:
//	<object>.modifyRecord(uaccountno, uaddress);
//
//****************************************************************/

int llist::modifyRecord(int paccountno, char addr[])
{
	struct record *activerec;
	struct record *nextrec;
	int pcount;

	activerec = NULL;
	nextrec = NULL;
	pcount = 0;

	#ifdef DEBUG
		printf("\nDBM: Function named 'modifyRecord' called, passed args:\n");
		printf("\t'paccountno':\t%d\n", paccountno);
		printf("\t'addr':\n%s\n", addr);
	#endif
	
	if (start != NULL)
	{
		activerec = start;					
		nextrec = (*activerec).next;
		
		while (activerec != NULL)
		{
			if(paccountno == (*activerec).accountno)
			{
				pcount++;
				strcpy((*activerec).address, addr);
			}											
			
			activerec = nextrec;
			if (activerec != NULL)
				nextrec = (*activerec).next;
		}
	}	
	
	return pcount;
}

/*****************************************************************
//
//  Function name:  llist::printRecord
//
//  DESCRIPTION:    This function prints all records with a given account number.
//
//  Parameters:	   int paccountno: the account number to print.
//
//  Return value:  The number of records printed.
//
//  Function call with given variables:
//	<object>.printRecord(uaccountno);
//
//****************************************************************/

int llist::printRecord(int paccountno)
{
	struct record *activerec;
	struct record *nextrec;
	int pcount;

	nextrec = NULL;
	activerec = NULL;
	pcount = 0;

	#ifdef DEBUG
		cout << "\nDBM: Function named 'llist::printRecord' called, passed args:\n";
		cout << "\t'paccountno':\t" <<  paccountno << "\n";
	#endif
	
	if (start != NULL)
	{
		activerec = start;					
		nextrec = (*activerec).next;
		
		while (activerec != NULL)
		{
			if(paccountno == (*activerec).accountno)
			{
				pcount++;
				cout << "\n__Record " << pcount << "__";
				cout << "\nAccount number: " << (*activerec).accountno;
				cout << "\nName: " << (*activerec).name;
				cout << "\nAddress: \n" << (*activerec).address;
				cout << "\nYear of Birth: " << (*activerec).yearofbirth;
				cout << "\n";
			}											
			
			activerec = nextrec;
			if (activerec != NULL)
				nextrec = (*activerec).next;
		}
	}	
	
	return pcount;
}

/*****************************************************************
//
//  Function name:  llist::printAll
//
//  DESCRIPTION:    This function prints all current records.
//                 
//  Parameters: none
//
//  Return value:  none
//
//****************************************************************/

void llist::printAll()
{
	/* Declarations */
	struct record *activerec;			
	struct record *nextrec;
	int pcount;

	/* Initializations */
	activerec = NULL;
	nextrec = NULL;
	pcount = 0;

	#ifdef DEBUG
		cout << "\nDBM: Function named 'printAllRecords' called (no parameters).\n";
	#endif	

	if (start != NULL)
	{
		activerec = start;
		nextrec = (*activerec).next;
		
		while (activerec != NULL)
		{
			{
				pcount++;
				cout << "\n__Record " << pcount << "__\n";
				cout << "Account number: " << (*activerec).accountno << "\n";
				cout << "Name: " << (*activerec).name << "\n";
				cout << "Address: \n" << (*activerec).address << "\n";
				cout << "Year of Birth: " << (*activerec).yearofbirth << "\n";

			}											
			
			activerec = nextrec;
			if (activerec != NULL)
				nextrec = (*activerec).next;
		}
	}	
	
	cout << "\nThere are currently " << pcount << " record(s).\n\n";
}

/*****************************************************************************
//
// FUNCTION NAME:	llist::readfile
//
// DESCRIPTION:		Loads records from a database file into struct records.
//
// PARAMETERS:		None.
//
// RETURN VALUES:	The number of records read.
//
//***************************************************************************/

int llist::readfile ()
{
	struct record *crec;
	fstream ifile;

	int racc;
	char rname[250];
	char raddr[250];
	int ryob;

	char temp[250];
	char trash[250];

	int fieldlen;
	int templen;

	int exit; /*used for a small loop, recognizing end of address*/
	int n; /*Loop count - actual accounts read*/

	int finished;
	
	racc = 0;
	rname[0] = '\0';
	raddr[0] = '\0';
	ryob = 0;
	temp[0] = '\0';
	trash[0] = '\0';

	fieldlen = 0;
	templen = 0;

	exit = 0;
	n = 0;
	finished = 0;

	ifile.open(filename, ios::in);

	#ifdef DEBUG
		cout << "\nDBM: Function named 'llist::readfile' called (no args):\n";
	#endif

	cout << "\nLoading prexisting records...\n";

	if (ifile != NULL)
	{

		while (finished == 0)
		{
			if (ifile.eof())
			{
				if (n == 0)
					cout << "\nDatabase empty";
				break;
			}			

			ifile >> racc;
			if (!ifile)
			{
				if (n == 0 && ifile.eof())
				{
					cout << "\nDatabase empty";
					break;
				}
				else
				{
					cout << "\nRead Error 2, account " << n+1 << ": Account number is not int. Data discarded.";
					break;
				}
			}
			ifile.getline(trash, 250, '\n');
			/*Read Error 2? non-integer check*/
			if (ifile.eof())
			{
				cout << "\nRead Error 1, account " << n+1 << ": may contain incomplete account data, incomplete account discarded";
				break;
			}

			ifile.getline(rname, 250, '\n');
			fieldlen = strlen(rname);
			if (fieldlen > 25)
			{
				cout << "\nRead Error 3, account " << n+1 << ": name too long. Data discarded.";
				break;
			}
			if (ifile.eof())
			{
				cout << "\nRead Error 1, account " << n+1 << ": may contain incomplete account data, incomplete account discarded";
				break;
			}

			ifile.getline(temp, 250, '\n');
			templen = strlen(temp);
			if (temp[templen - 1] != '!')
			{
				strcpy(raddr, temp);
				while (exit == 0)
				{
					temp[0] = '\0';
					ifile.getline(temp, 250, '\n');
					templen = strlen(temp);
					if (temp[templen - 1] == '!')
					{
						temp[templen - 1] = '\0';
						exit = 1;
					}
					strcat (raddr, "\n");
					strcat (raddr, temp);
					fieldlen = strlen(raddr);
					if (fieldlen > 80)
					{
						cout << "\nRead Error 4, account " << n+1 << ": address too long. Data discarded.";
						break;
					}
				}
			}
			else
			{
				temp[templen - 1] = '\0';
				strcpy(raddr, temp);
			}
			fieldlen = strlen(raddr);
			if (fieldlen > 80)
			{
				cout << "\nRead Error 4, account " << n+1 << ": address too long. Data discarded.";
				break;
			}
			if (ifile.eof())
			{
				cout << "\nRead Error 1, account " << n+1 << ": may contain incomplete account data, incomplete account discarded";
				break;
			}

			ifile >> ryob;
			if (!ifile)
			{
				cout << "\nRead Error 5, account " << n+1 << ": Year of birth is not int. Data discarded.";
				break;
			}
			ifile.getline(trash, 250, '\n');
			/*Read Error 5? non-integer check*/

			n++;
			addRecord(racc, rname, raddr, ryob);
			
			cout << "\n__Account " << n << "__";
			cout << "\nAccount number: " << racc;
			cout << "\nName: " << rname;
			cout << "\nAddress: \n" << raddr;
			cout << "\nYear of Birth: " << ryob;
			cout << "\n";

			racc = 0;
			rname[0] = '\0';
			raddr[0] = '\0';
			ryob = 0;
			temp[0] = '\0';
			templen = 0;
			exit = 0;

			if (ifile.eof())
				break;
		}
		ifile.close();
		
		cout <<"\nNumber of accounts read: " << n;
		return n;
	}
	else
		cout << "\nNo database file found, file will be created!";

}/*End readfile*/


/*****************************************************************************
//
// FUNCTION NAME:	llist::writefile
//
// DESCRIPTION:		Writes records from an array into a database file.
//
// PARAMETERS:		None.
//
// RETURN VALUES:	The number of records written to file.
//
//***************************************************************************/

int llist::writefile()
{
	struct record *crec;
	ofstream ofile;

	int exit;
	int n; /*number of accounts written*/
	int clear;
	
	crec = start;
	exit = 0;

	n = 0;
	clear = 0;

	#ifdef DEBUG
		cout << "\nDBM: Function named 'llist::writefile' called (no args):\n";
	#endif

	ofile.open(filename, ios::out);
	
	while (exit == 0)
	{
		if (crec == NULL)
		{
			break;
		}

		ofile << (*crec).accountno << "\n" << (*crec).name << "\n" << (*crec).address << "!\n" << (*crec).yearofbirth;
		
		if ((*crec).next == NULL)
		{
			exit = 1;
		}
		else
		{
			crec = (*crec).next;
			ofile << "\n";
		}
		n++;
	}
	
	ofile.close();
	cout << "\nNumber of accounts saved: " << n;
}/*End writefile*/


/*****************************************************************************
//
// FUNCTION NAME:	llist::reverse
//
// DESCRIPTION:		This is the public function, which will call on the
//	private reverse function to reverse the entire list, printAll, then
//	reverse the list back.
//
// PARAMETERS:		None.
//
// RETURN VALUES:	None.
//
// WARNING:	For Novelty Purposes Only.
//
// NOTE:	List will become disordered if "addRecord" is called when
//	the list has been reversed. Quitting and re-launching the program
//	will preserve added records, and re-sort the list in ascending 
//	order.
//
//	Functions deleteRecord, printRecord, and modifyRecord appear to
//	perform normally on a reverse-ordered list, but further testing may 
//	be required if the reverse function is intended to be used in
//	combination with these functions.
//
//***************************************************************************/

void llist::reverse()
{
	cout << "\nI will now reverse and print the entire list of records!";
	cout << "\nWARNING: For Novelty Purposes Only.";
	start = llist::reverse(start);
	cout << "\n(If you forget to switch back, exiting and re-launching will re-sort your records)\n\n";
}

/*****************************************************************************
//
// FUNCTION NAME:	llist::reverse
//
// DESCRIPTION:		This is the private function reverse.
//	It will reverse the entire list of records, and break most other 
//	functions if not immediately reversed back. Should only be accessed 
//	via the public function reverse.
//
// PARAMETERS:		record * start: The start of the list.
//
// RETURN VALUES:	record * start: The new start of the list.
//
// WARNING:	For Novelty Purposes Only.
//
// NOTE:	List will become disordered if "addRecord" is are called when
//	the list has been reversed. Quitting and re-launching the program
//	will preserve added records, and re-sort the list in ascending 
//	order.
//
//	Functions deleteRecord, printRecord, and modifyRecord appear to
//	perform normally on a reverse-ordered list, but further testing may 
//	be required if the reverse function is intended to be used in
//	combination with these functions.
//
//***************************************************************************/

record *llist::reverse(record *activerec)
{
	struct record *nextrec;
	struct record *ans;

	nextrec = (*activerec).next;

	#ifdef DEBUG
		cout << "\nDBM: Function named 'llist::reverse' called, passed arg (activerec):";
		cout << "\n";
	#endif

	if ((*nextrec).next != NULL)
	{
		ans = reverse((*activerec).next);			
		(*nextrec).next = activerec;
		if ((*activerec).next == (*start).next)
		{
			(*activerec).next = NULL;
		}
	}
	else
	{
		ans = (*activerec).next;	
		(*ans).next = activerec;
	}
	
	return ans;
}


/*****************************************************************************
//
// FUNCTION NAME:	ostream& operator<<
//
// DESCRIPTION:		This is an overloaded << operator for ostream, that
//		prints all active records in the llist object.
//
//		It is a friend function to the llist class.
//
// PARAMETERS:		ostream&: the target output stream
//			const llist&: the object that contains records to be printed.
//
// RETURN VALUES:	ostream&: printable output is returned to the ostream
//
//***************************************************************************/

ostream& operator<<(ostream& ostreamA, const llist& listB)
{
	/* Declarations */
	struct record *activerec;			
	struct record *nextrec;
	int pcount;

	/* Initializations */
	activerec = NULL;
	nextrec = NULL;
	pcount = 0;

	#ifdef DEBUG
		cout << "\nDBM: Special function called, overloaded << operator (parameters are ostream and database object).\n";
	#endif	

	if (listB.start != NULL)
	{
		activerec = listB.start;
		nextrec = (*activerec).next;
		
		while (activerec != NULL)
		{
			{
				pcount++;
				cout << "\n__Record " << pcount << "__\n";
				cout << "Account number: " << (*activerec).accountno << "\n";
				cout << "Name: " << (*activerec).name << "\n";
				cout << "Address: \n" << (*activerec).address << "\n";
				cout << "Year of Birth: " << (*activerec).yearofbirth << "\n";

			}											
			
			activerec = nextrec;
			if (activerec != NULL)
				nextrec = (*activerec).next;
		}
	}	
	
	cout << "\nThere are currently " << pcount << " record(s).\n\n";
}

