#$ -S /bin/bash
#$ -V
#$ -cwd
#$ -m eas
#$ -M gilabert@cs.upc.edu
#$ -l h_vmem=4G

bash

metric=$1
param=$2

echo "fork $metric $param > $metric.report"
./fork $metric $param
