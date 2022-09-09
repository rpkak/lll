.SUFFIXES:

LDFLAGS += -lfl -lLLVM
BISON := bison
BFLAGS :=

HEADERS := lll.h lll.tab.h

.PHONY: all
all: lll


lll.tab.c lll.tab.h: lll.y
	$(BISON) -H $(BFLAGS) $^

lll.html: lll.y
	$(BISON) --html $(BFLAGS) $^

lex.yy.c: lll.l
	$(LEX) $(LFLAGS) $^

lll.tab.o: lll.tab.c $(HEADERS)
	$(CC) -c $(CFLAGS) -o $@ $<

lll.o: lll.c $(HEADERS)
	$(CC) -c $(CFLAGS) -o $@ $<

lex.yy.o: lex.yy.c $(HEADERS)
	$(CC) -c $(CFLAGS) -o $@ $<

lll: lll.tab.o lex.yy.o lll.o
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $^

.PHONY: clean
clean:
	rm -f lll.html lll.tab.c lll.tab.h lll.xml lex.yy.c lll.tab.o lex.yy.o lll lll.o


