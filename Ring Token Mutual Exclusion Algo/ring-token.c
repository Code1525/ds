#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<time.h>


void main(){
    int  cs=0,pro=0;
    double run=5;
    char key='a';
    time_t t1,t2;
    printf("Press a key(except q) to enter a process into critical section.");
    printf(" \nPress q at any time to exit.");
    t1 = time(NULL) - 5;
    while(key!='q')
    {
        while(!kbhit())
        if(cs!=0)
        {
            t2 = time(NULL);
            if(t2-t1 > run)
            {
                printf("Process%d ",pro-1);
                printf(" exits critical section.\n");
                cs=0;
            }
        }
        key = getch();
        if(key!='q')
        {
            if(cs!=0)
            printf("Error: Another process is currently executing critical section Please wait till its execution is over.\n");
            else
            {
                printf("Process %d ",pro);
                printf(" entered critical section\n");
                cs=1;
                pro++;
                t1 = time(NULL);
            }
        }
    }
}
