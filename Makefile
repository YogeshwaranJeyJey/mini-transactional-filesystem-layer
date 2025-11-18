CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = run.exe

SRCS = main.c \
       menuPage.c \
       common.c \
       bankServices.c \
       logHandler.c \
       displaySystemOps.c \
       crashSimulationHandler.c \
       crashRecovery.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
