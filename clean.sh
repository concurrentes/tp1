#!/bin/bash

(

IFS='
'

for line in `ipcs | grep 644`; do
  ipcrm shm `echo $line | cut -f2 -d' '`
done

)
