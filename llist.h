#ifndef LLIST_H
#define LLIST_H

#include<iostream>

using std::ostream;

class llist
{
private:
	record *	start;
	char		filename[16];
	int 		readfile();
	int		writefile();
	record *	reverse(record *);
	void		cleanup();

public:
	llist();
	llist(char[]);
	llist& operator=(const llist& listB);
	~llist();
	int addRecord (int, char [], char[], int);
	int printRecord (int);
	int modifyRecord (int, char []);
	void printAll ();
	int deleteRecord (int);
	void reverse();
	friend ostream& operator<<(ostream& ostreamA, const llist& listB);
};

#endif
