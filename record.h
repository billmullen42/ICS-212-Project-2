#ifndef RECORD_H
#define RECORD_H

struct record
{
	int 		accountno;
	char		name[25];
	char		address[80];
	int		yearofbirth;
	struct record*	next;
};

#endif
