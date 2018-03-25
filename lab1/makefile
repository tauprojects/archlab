CC = gcc

OBJS2 = iss.o 

# The executabel filename of the iss app
EXEC2 = iss

C_COMP_FLAG = -Wall -g  -DNDEBUG 

#C_COMP_FLAG = -std=c99 -Wall -Wextra \
#-Werror -pedantic-errors -DNDEBUG

all: $(EXEC1) $(EXEC2)


iss.o:	iss.h   
		$(CC) $(C_COMP_FLAG)  -c $*.c

		
$(EXEC2): $(OBJS2)
		$(CC) $(OBJS2) -o $@

# a rule for building a issple c souorce file


clean:
		rm -f $(OBJS2) $(EXEC2)

