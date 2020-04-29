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

	pid_t pidServer, pidClient ;

	int status ;
	char dropStr[10] ;
	double drop = 0 ;

	while (drop < 1)
	{
		gettimeofday (&start, NULL) ;

	    if (!(pidServer = fork()))
	    {
	    	memset (dropStr, 0, 10) ;
	    	sprintf (dropStr , "%f", drop) ;
	    	char *argsServer[]={"./server", dropStr , "FALSE" , NULL}; 
	    	execvp(argsServer[0],argsServer); 
	    }

	    if (!(pidClient = fork()))
	    {
	    	char *argsClient[]={"./client" , "FALSE" , NULL}; 
	    	execvp(argsClient[0],argsClient); 
	    }

	    waitpid (pidClient, &status, 0) ;
	    waitpid (pidServer, &status, 0) ;

	    gettimeofday (&end, NULL) ;

	    time = 1000*(end.tv_sec - start.tv_sec) ;
	    time += (end.tv_usec - start.tv_usec)/1000 ;

	    printf ("Time taken = %fms\n", time) ;

	    drop += 1 ;
	}
      
    return 0; 
    
} 