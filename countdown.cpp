#include <stdio.h>

#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void usage() {
	fprintf(stderr, "Input param. required\n"); exit(1);
}

string exec(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
    }
    pclose(pipe);
    return result;
}

int main(int argc, char *argv[]) {
	if (argc < 2) usage();

	//cout << "argc: " << argc << endl;
	printf("My ID: %d\n", (int)getpid());
        printf("My jobID: %s\n", exec("echo $JOB_ID").c_str());
	for(int i = 0; i < argc; ++i) {
		printf("argv[%d]: %s\t", i, argv[i]);
	}
	printf("\n\t[FORK 1] says HI!!!\n");
	for (int i = 10; i > 0; --i) {
		printf ("%d\n", i);
		this_thread::sleep_for(chrono::seconds(1));
	}
	printf("\t[FORK 1] says BYE!!!\n");
}
