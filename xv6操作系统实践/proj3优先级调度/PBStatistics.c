#include "types.h"
#include "stat.h"
#include "user.h"

long long calcunumber(long long n){
    long long i, j;
    long long result = 0;
    for(i=0;i < n;i++){
        result = 1;
        for(i = 0;i < n;i++){
            for(j = 0;j < n;j++){
                result += 1;
                result *= -1;
            }
        }
    }
    return result;
}

int child(){
    long long i, pid, a;
    for(i=0; i<3; i++){
		if((pid = fork()) == 0){
			a = calcunumber(15000);
            printf(0, "pid = %d, %d\n", pid, a);
			break;
		}
        else{
            a = calcunumber(5000);
            printf(0, "pid = %d, %d\n", pid, a);
        }
	}
    for(i=0; i<3; i++){
		wait();
	}
	exit();
}

int string2int(char *s){
    int i, len = 0;
    int res = 0;
    len = strlen(s);
    for(i = 0;i < len;i++){
        res = 10*res + (s[i] - 48);
    }
    return res;
}

int main(int argc, char *argv[])
{
    int n = 0, pid, a;
    int i;
    n = 4 * string2int(argv[1]);
    for(i = 0;i < n; i++){
        if((pid = fork()) == 0){
            child();
            break;
        }
        else{
            setPriority((pid % 4)+1, pid);
            a = calcunumber(1000);
            printf(0, "pid = %d, %d\n", pid, a);
        }
    }
    for(i=0; i < n; i++){
		waitSch();
	}
    exit();
}