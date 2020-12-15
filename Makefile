CC= gcc -w -g
CFLAGS += -s -w -fPIC
SRCS = server.c util.c http.c 
LIBS = -L. -lpthread

OBJS = $(SRCS:.c=.o)
TARGET = server

all:$(TARGET)
$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

%.o : %.c %.h
	$(CC) -c $(CFLAGS) $< 
clean:
	rm -f *.o  $(TARGET) *~


