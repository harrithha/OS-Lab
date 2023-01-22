
/*******************************************************************************
 *  The BYTE UNIX Benchmarks - Release 3
 *          Module: arith.c   SID: 3.3 5/15/91 19:30:19
 *
 *******************************************************************************
 * Bug reports, patches, comments, suggestions should be sent to:
 *
 *	Ben Smith, Rick Grehan or Tom Yager
 *	ben@bytepb.byte.com   rick_g@bytepb.byte.com   tyager@bytepb.byte.com
 *
 *******************************************************************************
 *  Modification Log:
 *  May 12, 1989 - modified empty loops to avoid nullifying by optimizing
 *                 compilers
 *  August 28, 1990 - changed timing relationship--now returns total number
 *	                  of iterations (ty)
 *  November 9, 1990 - made changes suggested by Keith Cantrell
 *                        (digi!kcantrel) to defeat optimization
 *                        to non-existence
 *  October 22, 1997 - code cleanup to remove ANSI C compiler warnings
 *                     Andy Kahn <kahn@zk3.dec.com>
 *
 ******************************************************************************/

char SCCSid[] = "@(#) @(#)arith.c:3.3 -- 5/15/91 19:30:19";
/*
 *  arithmetic test
 *
 */

#include <stdio.h>
#include <stdlib.h>


volatile unsigned long iter;

/* this function is called when the alarm expires */
void report()
{
	fprintf(stderr,"COUNT|%ld|1|lps\n", iter);
	exit(0);
}

int main(argc, argv)
int	argc;
char	*argv[];
{
	while(1)
    {
    }

return(0);
}

