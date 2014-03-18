include param.mk
OBJS=task.o tasksystem.o filter.o sha2.o filters.o
PROG=yatm

all : $(LIB)

prog : $(OBJS) $(PROG)

$(LIB) : $(OBJS)
	$(CC) $(CXXFLAGS)    -o $@ $^ $(LDFLAGS) $(LIBFLAGS)

%.o : %.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

% : %.cpp $(OBJS)
	$(CC) $(CXXFLAGS)    -o $@.prog $^ $(LDFLAGS)

clear :
	@touch $(LIB) $(OBJS)
	@rm    $(LIB) $(OBJS)

rec : clean all

.PHONY: all prog clean rec


