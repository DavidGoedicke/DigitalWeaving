#!/bin/bash
source env/bin/activate
python3 Scantron_Reader.py &  echo $! >> py.pid