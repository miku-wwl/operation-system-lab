#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char* argv[])
{
    printf(1,"semaphore test!\n");
    printf(1,"start: share_var = %d\n",share_var_read());
    
    
    
    int id = create_sem(1);
    
    
    
    int pid=fork();

    
    int i=0;
    while(i<10000){
    	sem_p(id);	
    	
    	share_var_write();
    	sem_v(id);
        i++;
    
    }
    
    if (pid>0){
    	wait();
    	free_sem(id);
    	printf(1,"end: share_var = %d\n",share_var_read());
    }
    
 
    
    exit();
}
