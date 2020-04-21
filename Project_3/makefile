CC      := g++
CCFLAGS := -O3 -lm -fopenmp
LDFLAGS :=

TARGETS:= Project_3
MAINS  := $(addsuffix .o, $(TARGETS) )
OBJ    := Project_3_functions.o $(MAINS)
DEPS   := Project_3_functions.hpp

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS) $(OBJ)

$(OBJ): %.o : %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CCFLAGS)

$(TARGETS): % : $(filter-out $(MAINS), $(OBJ)) %.o
	$(CC) -o $@ $(LIBS) $^ $(CCFLAGS) $(LDFLAGS)