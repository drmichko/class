for t in {2..7} ; do 
	for s in {1..7}; do
	        if [ $s -le $t ]; then	
		     ./check.exe -m7 -s$s -t$t; 
		fi
	done; 
done
