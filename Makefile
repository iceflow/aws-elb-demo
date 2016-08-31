CC=gcc

all:
	${CC} -o elb_tcp_server elb_tcp_server.c

clean:
	[ -f elb_tcp_server ] && rm -v elb_tcp_server
