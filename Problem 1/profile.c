#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 

int main () 
{ 
	struct timeval start, end ;
	double time ;

	int status ;
	char dropStr[10] ;
	double drop = 0.95 ;

	while (drop < 1)
	{
		gettimeofday (&start, NULL) ;

	    if (!fork())
	    {
	    	memset (dropStr, 0, 10) ;
	    	sprintf (dropStr , "%f", drop) ;
	    	char *argsServer[]={"./server", dropStr , "FALSE" , NULL}; 
	    	execvp(argsServer[0],argsServer); 
	    }

	    if (!fork())
	    {
	    	char *argsClient[]={"./client" , "FALSE" , NULL}; 
	    	execvp(argsClient[0],argsClient); 
	    }

	    wait (&status) ;
	    gettimeofday (&end, NULL) ;

	    time = 1000*(end.tv_sec - start.tv_sec) ;
	    time += (end.tv_usec - start.tv_usec)/1000 ;

	    printf ("Time taken = %fms\n", time) ;

	    drop += 0.05 ;
	}
      
    return 0; 
    
} 