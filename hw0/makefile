#################
#   variables   #
#################

# files
EXECUTABLE  = wc
SOURCES  = wc.c
OBJECTS  = $(SOURCES:.c=.o)

# compilation and linking
CC      = gcc
CFLAGS  = -std=c99 -ggdb3 -c
LDFLAGS = 
WARN    = -Wall -Wextra -pedantic
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(WARN)
LINK.c    = $(CC)

#################
#     targets   #
#################

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(LINK.c) $^ $(LDFLAGS) -o $@

$(OBJECTS): %.o: %.c
	$(COMPILE.c) $< -o $@

# phony targets
.PHONY: clean

# remove object files
clean:
	rm -f *.o *~

# print-VAR prints the value of the variable VAR
print-%  : ; @echo $* = $($*)

