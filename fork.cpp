#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;


void usage() {
	fprintf(stderr, "2 argument required: Metric and param.\n"); exit(1);
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

bool end(string id) {
	// check whether id job is already ended
	bool ended = true;
        string out = exec("qstat");
        istringstream bufo(out);
        int i = 0;
        for (string token; getline(bufo, token, '\n'); ++i) {
                string job_aux;
                istringstream aux(token);
                getline(aux, job_aux, ' ');

		if (job_aux == id) ended = false;
                /*if (atoi(job_aux.c_str()) > 0)
                        cout << "job_ID: " << atoi(job_aux.c_str()) << endl;*/
        }
	return ended;
}

string getJobID(string cmd) {
	// get the id job from qsub's return value
        boost::algorithm::trim(cmd);
        //printf("return qsub: |%s|\n", job.c_str());
        vector<string> v;
        istringstream buf(cmd);
        for (string token; getline(buf, token, ' '); ) v.push_back(token);
        /*for (int j = 0; j < v.size(); ++j) cout << "v[" << j << "]: " << v[j] << "\t";
        cout << endl;*/
	return v[2];
}

void counter_file(const char* filename) {
    int c = 0;
    string line;
    ifstream file(filename);
    if (file) {
        while(getline(file, line)) {
            //int cl = 0;
            //cout << "line [" << l++ << "]: " << line;
            istringstream bufer(line);
            for (string token; getline(bufer, token, ' '); ) c++;
            //cout << "\twords_line: " << cl << "\twords_total: " << c << endl;
        }
        file.close();
    } else { fprintf(stderr, "couldn't open file: %s\n", filename); exit(1); }
    printf("--> %s file has %d words\n", filename, c);
}

int main(int argc, char *argv[]) {
	if (argc < 3) usage();

	boost::filesystem::path c_path(boost::filesystem::current_path());
        string metric = c_path.string() + "/" + argv[1];

	//printf("[PATH]: %s\n", c_path.c_str());
	printf("metric: %s\n", argv[1]);
	printf("param: %s\n", argv[2]);
	printf("qsub metric: %s\n", metric.c_str());

	char qsub[] = "/usr/local/sge/bin/linux-x64/qsub -b y ";
	char qsub_d[] = "/usr/local/sge/bin/linux-x64/qsub -b y -hold_jid ";
	string cmd = string(qsub) + metric  + " " + argv[2];
	printf("[EXECUTE]: %s\n", cmd.c_str());

	printf("[FATHER]: My ID: %d\n", (int)getpid());
	printf("[FATHER]: jobID: %s\n", exec("echo $JOB_ID").c_str());

	string cmd_r = exec(cmd.c_str());
	string jobid_r = getJobID(cmd_r);
	printf("[QUEUED] /jobID_r: %s\n", jobid_r.c_str());

	string arg2 = string(argv[2]) + "_2";
	string cmd2 = string(qsub_d) + jobid_r + " " + metric + " " + arg2;
	string cmd2_r = exec(cmd2.c_str());
	string jobid2_r = getJobID(cmd2_r);
	printf("WAITING FOR /jobID2: %s (-hold_jid <%s>)\n", jobid2_r.c_str(), jobid_r.c_str());

	while(!end(jobid2_r)) {}	// Wait/

	printf("[DONE]\n");
}

