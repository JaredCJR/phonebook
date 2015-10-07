CC ?= gcc
CFLAGS_common ?= -O0 -Wall -std=gnu99 -g3

EXEC = phonebook_orig phonebook_opt
all: $(EXEC)

SRCS_common = main.c

phonebook_orig: $(SRCS_common) phonebook_orig.c phonebook_orig.h
	$(CC) $(CFLAGS_common) -DIMPL="\"$@.h\"" -DORIG -o $@ \
		$(SRCS_common) $@.c

phonebook_opt: $(SRCS_common) phonebook_opt.c phonebook_opt.h
	$(CC) $(CFLAGS_common) -DIMPL="\"$@.h\"" -DmurmurHash -o $@ \
		$(SRCS_common) $@.c

run: $(EXEC)
	./phonebook_orig
	#./phonebook_opt

clean:
	$(RM) $(EXEC) *.o perf.*
