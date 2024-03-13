#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_NUMBERS 100
//Number of maximum processes that can one enter into the system
#define MAX 100

typedef struct {
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
} process;


process p[MAX];     // array of processes
int b[MAX],ideal_time,time,c[MAX],n;


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


//Shortest process calculation 
int find_shortest_process(int time) {
    int min = 9999, shortest_process = -1;
    for(int i = 0; i < n; i++) {
        if(i!=b[i]){
            if(p[i].at <= time && p[i].done == 0 && p[i].rt <= min){
                min = p[i].rt;
                shortest_process = i;
            }
        }
        b[i]=-1;
    }
    return shortest_process;
}


//Shortest Job First (Non-Premptive) with IO Burst
void SJF(){
    int completed=0;    //Check for how many processes are completed

    for(int i=0;i<n;i++){
        b[i]=-1;
    }
    while(completed<n){
        int shortest_process = find_shortest_process(time);
        if(shortest_process == -1) {
            ideal_time=ideal_time+p[c[0]].io;
            time=time+p[c[0]].io;
            continue;
        }
        if(p[shortest_process].rt==p[shortest_process].bt){
            p[shortest_process].rpt=time;
        }
        while(p[shortest_process].cfit!=0 && p[shortest_process].rt!=0){
            p[shortest_process].cfit--;
            p[shortest_process].rt--;
            time++;
        }

        p[shortest_process].cfit=p[shortest_process].cpu_for_io_transfer;

        if(completed==n-1){
            c[0]=shortest_process;
        }

        b[shortest_process]=shortest_process;

        if(p[shortest_process].rt==0){
            p[shortest_process].done = 1;
            completed++;
            p[shortest_process].ct = time;
            p[shortest_process].tat = p[shortest_process].ct - p[shortest_process].at;
            p[shortest_process].wt = p[shortest_process].tat - p[shortest_process].bt;
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

    //System throughput calculation
    throughput=100-((ideal_time*100)/time);
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
            // Convert each token to a number and store in the array
            num = atoi(token);
            numbers[i++] = num;
            token = strtok(NULL, ";");      // Move to the next token
        }
    }
    fclose(fp);
    n=lines;    //lines means number of processes

    input(numbers);
    SJF();     //call srtf function
    output();
    
    return 0;
}