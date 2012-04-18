#!/usr/bin/python

import sys
from bonix import message

event_response_list = [ [ ["hice", "termine"], "?? aqxli"], [ ["JOA"], "JOAJOAJOAJOA"],
[ ["sali a", "salimos", "anduve en", "fui a", "me gusta", "me gustaria", "me ha gustado"], "?? cuentame_mas"], [["taya", "zoes", "desinibir"], "%n, no se escribe %e BESTIA!"],
[ ["yo tambien", "me hicieron", "me trajeron"], "%ntedezante"], [["invesil"], "%n es imbecil, BESTIA!"] ]

def complete_reply(event, response, msg):
	partial = response
	if "%n" in response:
		partial = partial.replace("%n", msg.msg_from["nick"])
	if "%e" in response:
		partial = partial.replace("%e", event)

	return partial

if __name__ == "__main__":
	msg = message(sys.argv)
	# Por cada par (events, response), verifico que al menos un event de events este en msg.content para enviar response
	for ev_res in event_response_list:
		for event in ev_res[0]:
			if event in msg.content:
				reply = complete_reply(event, ev_res[1], msg)
				msg.reply(True, reply)
				sys.exit(0)

