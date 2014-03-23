include param.mk
OBJS=task.o tasksystem.o filter.o sha2.o filters.o
PROG=sptm

all : $(LIB)

prog : $(OBJS) $(PROG)

static : $(STATIC)

$(STATIC) : $(OBJS)
	ar rcs $@ $^

$(LIB) : $(OBJS)
	$(CC) $(CXXFLAGS)    -o $@ $^ $(LDFLAGS) $(LIBFLAGS)

%.o : %.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

% : %.cpp $(STATIC)
	$(CC) $(CXXFLAGS)    -o $@.prog $^ $(LDFLAGS)

clear :
	@touch $(LIB) $(OBJS)
	@rm    $(LIB) $(OBJS)

rec : clean all

.PHONY: all prog clean rec


