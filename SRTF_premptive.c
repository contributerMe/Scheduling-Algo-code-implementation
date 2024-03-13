#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_NUMBERS 100
//Number of maximum processes that can one enter into the system
#define MAX 100



//Structure for process so that each process can get every field inside the "struct process"
typedef struct{
    int pid;                        // Process ID
    int at;                         // Arrival time
    int bt;                         // Burst time
    int ct;                         // Completion time
    int tat;                        // Turnaround time
    int wt;                         // Waiting time
    int io;                         //IO Burst time
    int cpu_for_io_transfer;        //CPU burst time for IO tranfer
    int rpt;                        //Response time of the process
} process;


int n;              // number of processes
process p[MAX];     // array of processes


//Taking inputs from the array into the individual structure index
void input(int a[]) {
    int trav_in_arr=0;  //it is a counter

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

        //assign pid 
        p[i].pid = i;
    }
}


//Shortest Job First (Premptive) without IO Burst
void srtf(){
    int time = 0, complete = 0, shortest;
    int remaining[n];

    // Initialize remaining time of all processes
    for (int i = 0; i < n; i++){
        remaining[i] = p[i].bt;
    }
    while (complete != n) {
        shortest = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && remaining[i] > 0) {
                if (shortest == -1 || remaining[i] < remaining[shortest]) {
                    shortest = i;
                }
            }
        }
        
    if (shortest == -1) {
        time++;
        continue;
    }
    if(remaining[shortest]==p[shortest].bt){
        p[shortest].rpt=time;
    }
        
    // Execute the process for 1 unit of time
    remaining[shortest]--;
        
    // Update the shortest remaining time
    if (shortest != -1 && remaining[shortest] == 0) {
        complete++;
        p[shortest].ct = time + 1;
        p[shortest].wt= p[shortest].ct - p[shortest].at - p[shortest].bt;
        p[shortest].tat= p[shortest].ct - p[shortest].at;
        p[shortest].ct=p[shortest].ct;
    }
    //Increment the time   
    time++; 
    }  
}


//Print the chart and analysis
void output() {
    int total_tat = 0, total_wt = 0,throughput=100;
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
    srtf();     //call srtf function
    output();
    
    return 0;
}
