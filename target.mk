# http://make.mad-scientist.net/papers/multi-architecture-builds/
# used in conjuction with Makefile
.SUFFIXES:

OBJDIR := _test

MAKETARGET = $(MAKE) --no-print-directory -C $@ -f $(CURDIR)/Makefile \
                 SRCDIR=$(CURDIR) $(MAKECMDGOALS)

.PHONY: $(OBJDIR)
$(OBJDIR):
	+@[ -d $@ ] || mkdir -p $@
	+@$(MAKETARGET)

Makefile : ;
%.mk :: ;

% :: $(OBJDIR) ; :

.PHONY: clean realclean
clean:
	rm -f $(OBJDIR)/* && pwd
realclean:
	rm -f $(OBJDIR)/* && rm -f *.h.gch && pwd

# .PHONY: clean
# clean:
# 	rm -rf $(OBJDIR)
# 	rm -rf *.exe.stackdump
