#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char* argv[])
{
    printf(1,"semaphore test!\n");
    printf(1,"start: share_var = %d\n",share_var_read());
    
    
    
    int pid=fork();
    int i=0;
    while(i<10000){
    	share_var_write();
    	i++;
    }
    if (pid>0){
    	wait();
    }
    
    printf(1,"end: share_var = %d\n",share_var_read());
    exit();
}
