
#!/bin/bash

while read line
do
  echo -e "$line\n"
done < keylogger.log > keylog_final.log
