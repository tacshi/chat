VPATH = common:client:server
LIBS = -lpthread
OBJS_C = MyClient.o MyClientService.o MyTCP.o MyTools.o
OBJS_S = MyServer.o MyService.o MyTCP.o MyTools.o

both: clnt serv
.PHONY: both

clnt: $(OBJS_C)
	cc -o clnt $(OBJS_C) $(LIBS)

serv: $(OBJS_S)
	cc -o serv $(OBJS_S) $(LIBS)

MyClient.o: MyClientService.h
MyClientService.o: MyTCP.h MyTools.h MyClientService.h
MyServer.o: MyService.h
MyService.o: MyTCP.h MyTools.h MyService.h
MyTCP.o: MyTCP.h
MyTools.o: MyTools.h

.PHONY: clean
clean:
	rm -rf *.o clnt serv
	clear
