CC	= gcc
CFLAGS	 = -Wall -Werror
OBJFILES = cmdline.o plist.o statparser.o 
TARGET = tups

all: $(TARGET)
$(TARGET):$(OBJFILES)
	$(CC) -o $(TARGET) $(OBJFILES) $(CFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~
