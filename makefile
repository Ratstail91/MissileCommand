#Windows 7:
#RM=del /y

#Windows 8.1, Windows 10:
RM=del /S
CP=copy

BINDIR=bin
OUTDIR=out

all: $(OUTDIR)
	$(MAKE) -C src
	$(CP) $(BINDIR) $(OUTDIR)

debug: export CXXFLAGS+=-g
debug: clean all

$(OUTDIR):
	mkdir $(OUTDIR)

clean:
ifeq ($(OS),Windows_NT)
	del /S /Q *.o *.a *.exe $(OUTDIR)\*.dll
#	rmdir /S /Q $(OUTDIR)
else ifeq ($(shell uname), Linux)
	find . -type f -name '*.o' -exec rm -f -r -v {} \;
	find . -type f -name '*.a' -exec rm -f -r -v {} \;
#	rm $(OUTDIR)/* -f
	find . -empty -type d -delete
endif

rebuild: clean all
