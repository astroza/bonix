class message:

	def __init__(self, arr):

		from_splitted = arr[1].split(" ", 3)

		self.msg_from = {}

		self.msg_from['full'] = arr[1];
		if len(from_splitted) > 2:
			self.msg_from['nick'] = from_splitted[0];
			self.msg_from['user'] = from_splitted[1];
			self.msg_from['host'] = from_splitted[2];

		self.msg_dest = arr[2];
		if ("#" in self.msg_dest) or ("&" in self.msg_dest):
			self.is_public = True
		else:
			self.is_public = False

		self.content = arr[3]

	def reply(self, publicMessage, message):

		if self.is_public and publicMessage:
			print "PRIVMSG %s :%s\r\n" % (self.msg_dest, message)
		else:
			print "PRIVMSG %s :%s\r\n" % (self.msg_from['nick'], message)
