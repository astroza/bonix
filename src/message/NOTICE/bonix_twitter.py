#!/usr/bin/python

import sys
import twitter
from bonix import message

if __name__ == "__main__":
	msg = message(sys.argv)

	if  msg.msg_from['nick'] == "root" and msg.content[0:7] == "Learned":
		learn = open("../learn", "r")
		key1 = learn.readline()
		value = learn.readline()
		key1 = key1[0:len(key1)-1]

		key2 = msg.content[9:]
		key2 = key2[0:len(key2)-2]

		if key1 == key2:
			api = twitter.Api("user", "password")
			status = api.PostUpdate(key1 + ": " + value)
			id = status.GetId()
			msg.reply(True, key1 + " Twitted!: " + "http://twitter.com/ircpowerscl/status/" + str(id))

		learn.close()
