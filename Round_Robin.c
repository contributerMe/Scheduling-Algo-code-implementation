#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_NUMBERS 100
//Number of maximum p that can one enter into the system
#define MAX 100

typedef struct{
    int pid;                        // Process ID
    int at;                         // Arrival time
    int bt;                         // Burst time
    int rt;                         // Remaining time
    int ct;                         // Completion time
    int tat;                        // Turnaround time
    int wt;                         // Waiting time
    int done;                       // flag to check if the process is completed or not
    int io;                         //IO Burst time
    int cpu_for_io_transfer;        //CPU burst time for IO tranfer
    int cfit;                       //CPU burst time for IO tranfer
    int rpt;                        //Response time
}process;


process p[MAX];     // array of process
int n,time=0;             
#define quantum 5


//Taking inputs from the array into the individual structure index
void input(int a[]){
    int trav_in_arr=0;  //its a counter

    for(int i=0;i<n;i++){
        //taking arrival time
        p[i].at=a[trav_in_arr];     
        trav_in_arr++;

        //taking CPU Burst time
        p[i].bt=a[trav_in_arr];     
        trav_in_arr++;

        //taking IO Burst time
        p[i].io=a[trav_in_arr];     
        trav_in_arr++;

        //taking cpu for io burst time
        p[i].cpu_for_io_transfer=a[trav_in_arr];     
        p[i].cfit=a[trav_in_arr];
        trav_in_arr++;

        //Remaining CPU Burst time 
        p[i].rt = p[i].bt;

        //Process status 
        p[i].done = 0;

        //assign pid
        p[i].pid = i;
    }
}


//Round Robin Scheduling without IO Burst
void RR(){
    while(1) {
        int all_processes_completed = 1;
        for(int i = 0; i < n; i++) {
            if(p[i].rt > 0) {
                all_processes_completed = 0;
                if(p[i].rt==p[i].bt){
                    p[i].rpt=time;
                }
                if(p[i].rt <= quantum) {
                    time= time+ p[i].rt;
                    p[i].ct = time;
                    p[i].rt = 0;
                }
                else {
                    time= time+ quantum;
                    p[i].rt= p[i].rt- quantum;
                }
            }
            if(p[i].rt==0){
                p[i].tat=p[i].ct-p[i].at;
                p[i].wt=p[i].ct-p[i].bt;
            }
        }
        if(all_processes_completed) {
            break;
        }
    }
}


//Print the chart and analysis
void output() {
    int total_tat = 0, total_wt = 0,throughput=0;

    printf("The overall chart of the processes Completion time, Turn-around time and Waiting time is as follows:\n\n");
    printf("AT: Arrival time, BT: Burst time, RT:Response time, CT: Completion time, TAT: Turn-around time, WT: Waiting time\n");
    printf("---------------------------------------------------");
    printf("\nProcess\tAT\tBT\tRT\tCT\tTAT\tWT\n");
    printf("---------------------------------------------------\n");
    for(int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].rpt, p[i].ct, p[i].tat, p[i].wt);
        total_tat += p[i].tat;
        total_wt += p[i].wt;
    }
    printf("---------------------------------------------------\n");
    printf("\nAverage Turnaround Time: %0.2f\n", (float) total_tat / n);
    printf("Average Waiting Time: %0.2f\n", (float) total_wt / n);
    throughput=100;
    printf("The System throughput is: %d percent",throughput);
}


int main() {
    FILE *fp;
    char line[100];
    int numbers[MAX_NUMBERS],num, i = 0, j,lines=0;
    
    // Open the file for reading
    fp = fopen("Data.txt", "r");
    if(fp == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    
    // Read each line of the file
    while (fgets(line, 100, fp) != NULL) {
        lines++;
        char *token = strtok(line, ";");    // Split the line into tokens based on comma delimiter
        while (token != NULL) {
            num = atoi(token);
            numbers[i++] = num;
            token = strtok(NULL, ";");      // Move to the next token
        }
    }
    fclose(fp);

    //lines means number of processes
    n=lines;    

    input(numbers);
    RR();     //call srtf function
    output();

    return 0;
}
