#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

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

int main(int argc, char *argv[]) {
	if (argc < 2) usage();

	int c = 0;
	string line;
	ifstream file(argv[1]);
	if (file) {
		while(getline(file, line)) {
			istringstream bufer(line);
			for (string token; getline(bufer, token, ' '); ) c++;
		}
		file.close();
	} else { fprintf(stderr, "couldn't open file: %s\n", argv[1]); exit(1); }

	printf("--> %s file has %d words\n", argv[1], c);

/*	printf("[FATHER]: My ID: %d\n", (int)getpid());
	printf("[FATHER]: jobID: %s\n", exec("echo $JOB_ID").c_str());
        printf("Let's invoque spliter\n");

	string cmd1 = exec("/usr/local/sge/bin/linux-x64/qsub /home/usuaris/gilabert/spliter/countdown a");
	string jobid1 = getJobID(cmd1);
	printf("EXECUTED /jobID1: %s\n", jobid1.c_str());

        string cmd2 = exec(string("/usr/local/sge/bin/linux-x64/qsub -hold_jid "+ jobid1 +" /home/usuaris/gilabert/spliter/countdown a").c_str());
        string jobid2 = getJobID(cmd2);
        printf("EXECUTED /jobID2: %s (-hold_jid <%s>)\n", jobid2.c_str(), jobid1.c_str());

	while (!end(jobid2)) {}

	cout << "DONE" << endl;
*/}
