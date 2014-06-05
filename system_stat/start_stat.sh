#!/bin/sh
cd `dirname $0`

if [ $# -ne 2 ]; then
   echo "Usage: $0 INTERVAL COUNT"
   exit -1
fi

INTERVAL=$1
COUNT=$2

# monitor system info ($INTERVAL * $COUNT) seconds
mkdir -p logs
sar -n DEV $INTERVAL $COUNT > logs/network.log &
sar -q $INTERVAL $COUNT > logs/load.log &
sar -u $INTERVAL $COUNT > logs/cpu.log &
sar -b $INTERVAL $COUNT > logs/io.log &
