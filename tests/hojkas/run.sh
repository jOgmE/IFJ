#!/bin/bash

make p

if [ $# -eq 0 ]
	then
		./p
	else
		valgrind ./p
fi

rm p
