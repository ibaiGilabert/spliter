#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sstream>
#include <iostream>
#include <string>

using namespace std;

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
	printf("[FATHER]: My ID: %d\n", (int)getpid());
	string job = "JOB=/usr/local/sge/bin/linux-x64/qsub spliter/run.sh a";
	printf("Let's invoque spliter\n");
	string job_i = exec(job.c_str());
	string job_id = exec("echo $JOB");
	/*stringstream st;
	st << system(job_id.c_str());
	string st_s = st.str();
	printf("job_id: %s\n", st_s.c_str())*/;
	//printf("OUTPUT: %s\n", job_i.c_str());
	printf("EXECUTED /jobid: %s\n", job_id.c_str());
}
