#include <stdio.h>

enum process_state {
    run,
    wait,
    finish
};

struct Process
{
	float start_time;
	float finish_time;
	float remain_time;
};

int main() {
    int n = 0;
    printf("%s", "Enter the number of processes: ");
    scanf("%d", &n);

    srand((int)time(NULL));
    vector<Process> ready;
    for (int i = 1; i <= n; ++i) {
        Process process = Process(PCB(string("p").append(std::to_string(i)), wait, rand()%15 + 1, rand()%20 + 1));
        process.remain_time = process.pcb.run_time;
        ready.push_back(process);
    }
    //FCFS(ready);
    Priority(ready);
    return 0;
}
