CFLAGS = -Wall -O2 -g
LDFLAGS += -pthread

HDRS = error.h container.h address.h fd.h transport_private.h
OBJS = smc_msg.o socket_common.o print.o transport.o uds_common.o uds.o

SERVER = server
CLIENT = client

all: $(SERVER) $(CLIENT)

$(SERVER): $(HDRS) $(OBJS) $(SERVER).o
	$(CC) -o $@ $^ $(LDFLAGS)

$(CLIENT): $(HDRS) $(OBJS) $(CLIENT).o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY: clean
clean:
	$(RM) *.o *~ $(CLIENT) $(SERVER)
