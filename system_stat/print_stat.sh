#!/bin/sh
cd `dirname $0`/logs

NETWORK="network.log"
CPU="cpu.log"
LOAD="load.log"
IO="io.log"

echo "=======NETWORK IN/OUT(MB)========"
 inkB=`tail -1 $NETWORK | awk '{print $5}'`
 inmB=`echo "scale=2; $inkB / 1024" | bc`

outkB = `tail -1 $NETWORK | awk '{print $6}'`
outmB = `echo "scale=2; $outkB / 1024" | bc`
echo -e "$inmB\t$outmB"


echo -e "\n========CPU USAGE========"
idle=`tail -1 $CPU | awk '{print $8}'`
usage=`echo 100 - $idle | bc`
echo -e "$usage"


echo -e "\n=========LOAD=========="
echo -e `tail -1 $LOAD | awk '{print $4}'`


echo -e "\n=======IO========"
echo -e "tps\trtps\twtps\tbread/s\tbwrtn/s"
echo -e `tail -1 $IO | awk '{print $2"\t"$3"\t"$4"\t"$5"\t"$6}'`
