#! /bin/bash
LOGFILE=log8
exec 2>./$LOGFILE

if [ "$#" -gt 1 ]; then	 
	echo "Too many arguments!"
else
	if [ "$#" -eq 0 ]; then
		UNAME=$USER
	else 
		UNAME=$1
	fi
	ID=`id -u $UNAME || echo -1`
	GR=`id -G $UNAME || echo -1`
	#echo $ID  $GR
	if [[ "$ID" -eq -1 || "$GR" -eq -1 ]]; then
		echo "Incorrect name"
	else
		ls -abn | awk -v ID=$ID -v GR=$GR ' { 	
		if (($1 ~ "?r.*" && $3==ID) || 	 
		    ($1 ~ "????r.*" && $4==GR) ||	
		    ($1 ~ "???????r.*" && $3!=ID && $4!=GR)) print $NF}'
	fi
fi
