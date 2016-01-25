CFLAGS += -Wall -g -O2 
XMLLIB += /usr/local/lib/libmxml.a 
LDFLAGS += -lstdc++ -lpthread -lrt  -lcrypto -L/usr/lib/mysql -lmysqlclient 
LIBXML2 = /usr/lib/libxml2.a -lz -lm
XMLINC = -I/usr/include/libxml2

OBJS=buff_list.o myconfig.o db.o parse_thread.o  parse_xml.o select_tcp.o scan_log_switch.o trace.o userv.o

PROG_NAME = userv

.PHONY: all clean distclean install

all: $(PROG_NAME)

$(PROG_NAME): $(OBJS)
	$(CC) -o $@ $^ $(LIBXML2) $(LDFLAGS)

buff_list.o:buff_list.c clist.h
	$(CC) -c buff_list.c

myconfig.o:myconfig.c
	$(CC) -c myconfig.c

db.o:db.c
	$(CC) -c db.c 

parse_thread.o:parse_thread.c buff_list.h parse_xml.h 
	$(CC) -c parse_thread.c 

parse_xml.o:parse_xml.c 
	$(CC) -c parse_xml.c $(XMLINC)

select_tcp.o:select_tcp.c buff_list.h
	$(CC) -c select_tcp.c

scan_log_switch.o:scan_log_switch.c
	$(CC) -c scan_log_switch.c

trace.o:trace.c
	$(CC) -c trace.c

userv.o:userv.c parse_thread.h scan_log_switch.h
	$(CC) -c userv.c

clean distclean:
	rm -f *.o 
	rm -rf $(PROG_NAME)

