#! /bin/bash
LOGFILE=log8
#exec 2>./$LOGFILE

if [ "$#" -gt 1 ]; then	 
	echo "Too many arguments!"
else
	if [ "$#" -eq 0 ]; then
		UNAME=$USER
	else 
		UNAME=$1
	fi
	ID=$(id -u $UNAME)
	GR=$(id -G $UNAME)
	GR=`echo //${GR[@]/ //}/`
#	echo $ID $GR
	if [[ -z "$ID" || -z "$GR" ]]; then
		echo "Incorrect name"
	else
		ls -Rabn | awk -v ID=$ID -v GR=$GR ' { P="\/" $4 "\/"; 	
		if ((NF>2) && ((($1 ~ /^.r.*$/) && ($3==ID)) || (($1 ~ /^....r.*$/) && (index(GR,"\/" $4 "\/") >0 )) || (($1 ~ /^.......r.*$/) && ( ($3!=ID) && index(GR,"\/" $4 "\/") == 0 ))) && ($NF !~ /^\.\.?$/)) print $NF}'
	fi
# || (($1 ~ /^.......r.*$/) && ( ($3!=ID) && index(GR,"\/" $4 "\/") == 0 ))
fi
