#!/bin/bash

# run -3 4 12 13 100000000 &> acp.-341213.100000000.txt;
acp_dev.py 0 0 1 1 10000000 &> acp.0011.10000000.py.txt;
echo "acp python finished 0 0 1 1" | mail -s "acp finished 0 0 1 1" -a acp.0011.10000000.py.txt msm697@gmail.com;
acp_dev.py -6 11 14 15 10000000 &> acp.-6111415.10000000.py.txt;
echo "acp python finished -6 11 14 15" | mail -s "acp finished -6 11 14 15" -a acp.-6111415.10000000.py.txt msm697@gmail.com;
acp_dev.py -2 3 6 7 10000000 &> acp.-2367.10000000.py.txt;
echo "acp python finished -2 3 6 7" | mail -s "acp finished -2 3 6 7" -a acp.-2367.10000000.py.txt msm697@gmail.com;
