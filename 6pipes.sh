ls -l |head -5|tail -3|awk -F " " '{print $8,$9}'|grep e|sed 's/3/5/g'|tr 'a' 'e'
