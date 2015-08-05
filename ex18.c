/*
 * =====================================================================================
 *
 *       Filename:  ex18.c
 *
 *    Description:  ex18.c
 *
 *        Version:  1.0
 *        Created:  2015年08月05日 22时18分29秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  shiyan (), shiyan233@hotmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  die
 *  Description:  
 * =====================================================================================
 */
	void
die ( const char *message ){
	if(errno)
		perror(message);
	else {
		printf("ERROR: %s\n", message);
	}
	exit(1);
}	/* -----  end of function die  ----- */
 
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  (*compare)
 *  Description:  
 * =====================================================================================
 */
typedef	int(*compare)( int a, int b );

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  bubblesort
 *  Description:  
 * =====================================================================================
 */
	int *
bubblesort ( int *number, int count, compare cmp )
{
	int tmp=0;
	int i=0;
	int j=0;
	int *target = malloc(count * sizeof(int));
	memcpy(target, number, count*sizeof(int));
	for(i=0;i<count;i++)
	{
		for(j=0;j<count-1; j++)
		{
			if(cmp(target[j], target[j+1])>0){
				tmp = target[j+1];
				target[j+1] = target[j];
				target[j] = tmp;
				
			}
		}
	}
	return target;
}		/* -----  end of function bubblesort  ----- */

int sortorder(int a, int b){
	return a-b;
}
int sortorder2(int a, int b){
	return b-a;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  test
 *  Description:  
 * =====================================================================================
 */
	void
test ( int *number, int count, compare cmp )
{
	int i=0;
	int *sorted = bubblesort(number, count, cmp);
	
	if(!sorted) die("Fail to sort.");
	for(i=0; i<count;i++)
		printf("%d ", sorted[i]);
	putchar('\n');
	free(sorted);
}		/* -----  end of function test  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	if(argc < 2)die("USAGE:ex18 1 4 -1 3");	
	int count = argc-1;
	int i=0;
	char **input = argv+1;
	int *number = malloc(count * sizeof(int));
	if(!number) die("memory ERROR");
	for(i=0;i<count;i++){
		number[i] = atoi(input[i]);
	//	printf("%d ", number[i]);
	}
	test(number, count, sortorder);
	free(number);
	
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
