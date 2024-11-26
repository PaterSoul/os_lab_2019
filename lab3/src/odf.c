#include <stdio.h>
int main(){
    int id = fork();
    if(id == 0){
        execlp("/workspaces/os_lab_2019/lab3/src/sequential_min_max","sequential_min_max","4","400",NULL);
        printf("Error");
        return 1;
    } 
    wait(NULL);
    return 0;
}