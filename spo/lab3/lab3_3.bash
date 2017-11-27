#! /bin/bash
#exec 2>./log2
USERS=$(getent passwd)
#USERS=$(getent passwd | awk -F ":" '{print $1}')
#echo $USERS
if [ $# -lt 1 ]; then 
	echo "Missed filename!"
elif [ 1 -lt $# ]; then
	echo "Too many arguments!"
else
	INFO="$(ls -bn -- $1)"
	U=${INFO:3:1}
	G=${INFO:6:1}
	O=${INFO:9:1}
#	echo $U $G $O
	FUID=$(echo $INFO | awk '{ print $3 }')
	GUID=$(echo $INFO | awk '{ print $4 }')
	echo $FUID $GUID
	if [[ $U == "x" ]]; then
		USER=$(echo "$USERS" | awk -v FUID=$FUID -F ":"  '{ if (FUID==$3) print $1;}')
		RESULT="$RESULT$USER 
		"
	fi
		for P in ${USERS[@]}
		do
			UNAME=$(echo $P | awk -F ":" '{print  $1}')
			ID=$(id -u $UNAME 2>/dev/null) 
        		GR=$(id -G $UNAME 2>/dev/null)
			GR=`echo //${GR[@]/ //}/`
			if [[ ($GR  =~ "/$GUID/") && ($FUID != $ID) && ($G == "x") ]]; then
  				RESULT="$RESULT$UNAME 
				"
			fi
			if [[ ($O == "x") && ($FUID != $ID) && !($GR  =~ "/$GUID/") ]]; then
				RESULT="$RESULT$UNAME
				"
			fi
		done
echo $RESULT
fi
