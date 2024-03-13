#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NUMBERS 100
// Number of maximum p that can one enter into the system
#define MAX 100


typedef struct
{
    int pid;
    int arrival_time;
    int burst_time;
    int cpu_burst;
    int io_burst;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} process;


process p[MAX]; // array of process
int n, time = 0;
#define quantum 5


void input(int a[]){
    int trav_in_arr = 0; // its a counter

    for (int i = 0; i < n; i++)
    {
        // taking arrival time
        p[i].arrival_time = a[trav_in_arr];
        trav_in_arr++;

        // taking  Burst time
        p[i].burst_time = a[trav_in_arr];
        trav_in_arr++;

        // taking CPU Burst time
        p[i].cpu_burst = a[trav_in_arr];
        trav_in_arr++;

        // taking IO Burst time
        p[i].io_burst = a[trav_in_arr];
        trav_in_arr++;

        // Remaining CPU Burst time
        p[i].remaining_time = p[i].burst_time;

        // assign pid
        p[i].pid = i;
    }
}


//Virtual Round Robin Scheduling without IO Burst
void virtualRoundRobin(process *processes, int n, int time_quantum){
    int current_time = 0;
    int completed_processes = 0;
    int i;

    while (completed_processes < n){
        for (i = 0; i < n; i++){
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0){
               
                processes[i].remaining_time -= time_quantum;
                current_time += time_quantum;

                if (processes[i].remaining_time <= 0){
                    completed_processes++;
                    processes[i].completion_time = current_time;
                    processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                    processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;

                    
                }
            }
        }
    }
}


// Print the chart and analysis
void output(){
    int total_tat = 0, total_wt = 0, throughput = 0;

    printf("The overall chart of the processes Completion time, Turn-around time and Waiting time is as follows:\n\n");
    printf("AT: Arrival time, BT: Burst time, CT: Completion time, TAT: Turn-around time, WT: Waiting time\n");
    printf("---------------------------------------------------");
    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);
        total_tat += p[i].turnaround_time;
        total_wt += p[i].waiting_time;
    }
    printf("---------------------------------------------------\n");
    printf("\nAverage Turnaround Time: %0.2f\n", (float)total_tat / n);
    printf("Average Waiting Time: %0.2f\n", (float)total_wt / n);
    throughput = 100;
    printf("The System throughput is: %d percent", throughput);
}



int main()
{
    FILE *fp;
    char line[100];
    int numbers[MAX_NUMBERS], num, i = 0, j, lines = 0;

    // Open the file for reading
    fp = fopen("Data.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    // Read each line of the file
    while (fgets(line, 100, fp) != NULL)
    {
        lines++;
        char *token = strtok(line, ";"); // Split the line into tokens based on comma delimiter
        while (token != NULL)
        {
            num = atoi(token);
            numbers[i++] = num;
            token = strtok(NULL, ";"); // Move to the next token
        }
    }
    fclose(fp);

    // lines means number of processes
    n = lines;

    input(numbers);
    virtualRoundRobin(p, n, quantum);
    output();

    return 0;
}
