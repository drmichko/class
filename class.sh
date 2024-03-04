VAL=$1
DEG=$2
DIM=$3

PARMS="-d /tmp -n baby -m $DIM -t $DEG"
val=$DEG
	
/usr/bin/time --format="cpu=%U"  ./initial.exe $PARMS  -w

while [ $val -ge $VAL ] ; do 
	/usr/bin/time --format="cpu=%U"  ./class.exe $PARMS  -l -w 
	((val--))
done


