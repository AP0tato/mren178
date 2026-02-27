// Program to create files required for ELEC278 Lab 3 part 2.
// IDENTS - file containing many lines of numeric ids and alphanumeric strings
// DELETES - file containing smaller amount of numbers - numbers from the same
//           set as IDENTS
// LOOKUPS - file containing smaller amount of numbers - to be looked up in
//			 BST.
//


#define	FL_IDENTS		"IDENTS.TXT"
#define FL_DELETES		"DELETES.TXT"
#define FL_LOOKUPS		"LOOKUPS.TXT"
#define	COUNT_IDENT		150000
#define	COUNT_DELS		1000
#define COUNT_LUKUPS	1000
#define	FIXED
//#define	TEST


#include	<stdio.h>
#include	<stdlib.h>
#include	<time.h>



char *random_string (int count)
// Routine generates random 8-character string.  Note that rand() is also
// being used to generate random integers, so if the seed for the generator
// is the same, the integers and the strings will also be the same.
{
	static char			buff [32];
	static unsigned int	range = '}' - '!';	// Range of visible ascii chars
	int					i;
	unsigned int		ch;

	// protect against mistakes
	if (count < 0 || count > 30)	return NULL;

	for (i=0; i<count; i++)	{
		ch = (rand() % (range+1)) + '!';
		buff [i] = (char) ch;
		}
	// Finished generating characters - add string terminator
	buff [i] = '\0';
	return buff;
} //random_string()


int main (int argc, char **argv)
{
	int				i;
	char			*pwd;
	unsigned char	dup = 0;
	unsigned int	seed = 27901;
	unsigned int	id;
	unsigned int	lower = 100000, upper = 19999999;
	FILE			*fp_idnt;
	FILE			*fp_dels;
	FILE			*fp_luks;

	// Seed random number generator. Note that seed is current time so sequence
	// of numbers will be different each run.
	printf ("FYI: Maximum value from rand() is: %d\n", RAND_MAX);
#ifdef FIXED
	srand (seed);
#else
	srand (time(0));
#endif

	// Create files. Generate all 3 at same time so we can ensure overlaps for
	// delete and lookup
	fp_idnt = fopen (FL_IDENTS, "w");
	if (fp_idnt == NULL)	{
		fprintf (stderr, "ERROR: Could not open %s\n", FL_IDENTS);
		return -1;
		}
	fp_dels = fopen (FL_DELETES, "w");
	if (fp_dels == NULL)	{
		fprintf (stderr, "ERROR: Could not open %s\n", FL_DELETES);
		fclose (fp_idnt);
		return -1;
		}
	fp_luks = fopen (FL_LOOKUPS, "w");
	if (fp_luks == NULL)	{
		fprintf (stderr, "ERROR: Could not open %s\n", FL_LOOKUPS);
		fclose (fp_idnt);
		fclose (fp_dels);
		return -1;
		}

	for (i=0; i<COUNT_IDENT; i++)	{
		// Create random 12 character string
		pwd = random_string (12);
		// create random id - guaranteeing some duplicates
		if (i % (COUNT_IDENT/20))	{
			dup = 0;
			id = (rand() << 15) + rand();
		} else	{
			dup = 1;	
			}
		// Print IDENT file
		if (id == 0) continue;
		fprintf (fp_idnt, "%010d    %s\n", id, random_string (12));
		// See if we should also add to deletes
		if (dup == 0)	{
			if ((i % 2000) == 0)	{
				fprintf (fp_dels, "%010d\n", id);
				}
			if ((i % 8192) == 0)	{
				fprintf (fp_luks, "%010d\n", id);
				}
			}//endif not duplicate
		}//endfor
	fclose (fp_luks);
	fclose (fp_dels);
	fclose (fp_idnt);
	return 0;
} //main()
