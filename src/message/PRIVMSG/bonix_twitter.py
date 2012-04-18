#!/usr/bin/python
# -*- coding: iso-8859-15 -*-

import sys
from bonix import message

if __name__ == "__main__":
	msg = message(sys.argv)
	if  msg.is_public and msg.content[0:6] == "!learn":
		slist1 = msg.content[7:].split(" ", 1)
		if slist1[0] == "-f":
			slist2 = slist1[1].split(" ", 1)
			key = slist2[0]
			value = slist2[1]
		else:
			key = slist1[0]
			value = slist1[1]

		key = key.replace("ñ", "n");
		value = value.replace("ñ", "n");

		learn = open("../learn", "w")
		learn.write(key + "\n")
		learn.write(value + "\n")
		learn.close()
