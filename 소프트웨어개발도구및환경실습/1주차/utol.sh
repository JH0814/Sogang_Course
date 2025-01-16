#!/bin/bash

echo "working directory:"
read dir
if [ -n "$dir" ]
then
	if [ ! -d $dir ]
	then
		echo "Input Correct directory name!"
		exit 0
	else
		cd $dir
	fi
fi
for i in *
do
	temp=$(echo $i | tr "[a-z] [A-Z]" "[A-Z] [a-z]")
	mv $i $temp
done
