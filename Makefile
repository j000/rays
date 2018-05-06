SRC ?= rays.cpp $(filter-out rays.cpp,$(wildcard *.cpp))

####################

# c++
# LINK.o := $(LINK.cc)

SHELL := /bin/sh
MKDIR ?= mkdir
RMDIR ?= rmdir

# enable a lot of warnings and then some more
WARNINGS := -Wall -Wextra
# be more strict
ifeq ($(shell $(CC) -dumpversion),4.7)
        # welcome to 2012
        # -Wpedantic is available since gcc 4.8
        WARNINGS += -pedantic
else
        WARNINGS += -Wpedantic
endif

# shadowing variables, are you sure?
WARNINGS += -Wshadow
# sizeof(void)
WARNINGS += -Wpointer-arith
# unsafe pointer cast qualifiers: `const char*` is cast to `char*`
WARNINGS += -Wcast-qual
# most of the time you don't want this
WARNINGS += -Werror=implicit-function-declaration
# why warning for comments inside comments?
WARNINGS += -Wno-comment

# warn when struct can be better
WARNINGS += -Wpadded

# implicit conversions that may alter a value
WARNINGS += -Wconversion
# "float" is implicitly promoted to "double"
WARNINGS += -Wdouble-promotion

# C only
CWARNINGS := $(WARNINGS)
CWARNINGS += -Wstrict-prototypes

# the new-style casts are less vulnerable to unintended effects
WARNINGS += -Wold-style-cast

CFLAGS := $(CFLAGS) $(CWARNINGS) -std=c11 -O2 -fPIC -g -ggdb
CXXFLAGS := $(CXXFLAGS) $(WARNINGS) -std=c++17 -O2 -fPIC -g -ggdb
LDFLAGS := -static -static-libgcc $(LDFLAGS)
# LDFLAGS := $(LDFLAGS)
LOADLIBES := -Wl,--as-needed -lm -l:libstdc++.a

# ncurses
# NCURSES := ncurses
# CFLAGS += -DNCURSES_WIDECHAR=1 $(shell pkg-config --cflags-only-I $(NCURSES))
# CXXFLAGS += -DNCURSES_WIDECHAR=1 $(shell pkg-config --cflags-only-I $(NCURSES))
# LDLIBS += $(shell pkg-config --libs-only-l $(NCURSES))
# LDFLAGS += $(shell pkg-config --libs-only-L $(NCURSES))

# COLOR := echo -en '\033[1;34m'
# RESET := echo -en '\033[0m'
COLOR := [1;34m
RESET := [0m

####################

EXE := $(basename $(firstword $(SRC)))
SRCDIR ?= .
TMPDIR ?= .tmp

OBJ := $(addprefix $(TMPDIR)/, \
	$(addsuffix .o, $(SRC)) \
)

DEP := $(addprefix $(TMPDIR)/, \
	$(addsuffix .d, $(SRC)) \
)

CFLAGS += -I$(SRCDIR)
CXXFLAGS += -I$(SRCDIR)

# be silent unless VERBOSE
ifndef VERBOSE
.SILENT: ;
endif

# default target
.PHONY: all
all: $(EXE) ## build executable

.PHONY: run
run: $(EXE) ## run program
	@./$(EXE)

.PHONY: debug
debug: CFLAGS := $(filter-out -O2,$(CFLAGS)) -D_DEBUG -Og
debug: CXXFLAGS := $(filter-out -O2,$(CXXFLAGS)) -D_DEBUG -Og
debug: $(EXE) ## build with debug enabled

.PHONY: debugrun
debugrun: debug run ## run debug version

$(EXE): $(OBJ)
	@echo "$(COLOR)Link $^ -> $@$(RESET)"
# almost default rule
	$(LINK.o) $(OUTPUT_OPTION) $^ $(LOADLIBES) $(LDLIBS)

$(TMPDIR)/%.c.o: $(TMPDIR)/%.c.d
	@echo "$(COLOR)Compile $(SRCDIR)/$*.c -> $(TMPDIR)/$*.c.o$(RESET)"
	$(COMPILE.c) $(OUTPUT_OPTION) $(SRCDIR)/$*.c

$(TMPDIR)/%.cpp.o: $(TMPDIR)/%.cpp.d
	@echo "$(COLOR)Compile $(SRCDIR)/$*.cpp -> $(TMPDIR)/$*.cpp.o$(RESET)"
	$(COMPILE.cc) $(OUTPUT_OPTION) $(SRCDIR)/$*.cpp

$(TMPDIR)/%.c.d: $(SRCDIR)/%.c
	@echo "$(COLOR)Dependencies $(SRCDIR)/$*.c -> $(TMPDIR)/$*.c.d$(RESET)"
	$(CC) $(DEPFLAGS) -MM -MT '$$(TMPDIR)/$*.c.o' -MF $@ $<
	sed -i 's,^\([^:]\+.o\):,\1 $$(TMPDIR)/$*.c.d:,' $@

$(TMPDIR)/%.cpp.d: $(SRCDIR)/%.cpp
	@echo "$(COLOR)Dependencies $(SRCDIR)/$*.cpp -> $(TMPDIR)/$*.cpp.d$(RESET)"
	$(CXX) $(DEPFLAGS) -MM -MT '$$(TMPDIR)/$*.cpp.o' -MF $@ $<
	sed -i 's,^\([^:]\+.o\):,\1 $$(TMPDIR)/$*.c.d:,' $@

# include dependencies
-include $(wildcard $(DEP))

# depend on directory
$(OBJ): | $(TMPDIR)/.keepme
$(DEP): | $(TMPDIR)/.keepme

# create directory
$(TMPDIR)/.keepme:
	-$(MKDIR) $(TMPDIR)
	touch $@

# delete stuff
.PHONY: clean
clean: mostlyclean ## delete everything this Makefile created
	-$(RM) $(EXE)

.PHONY: mostlyclean
mostlyclean: ## delete everything created, leave executable
	@echo "$(COLOR)Cleaning$(RESET)"
ifneq ($(wildcard $(TMPDIR)),)
	-$(RM) $(OBJ)
	-$(RM) $(DEP)
	-$(RM) $(TMPDIR)/.keepme
	-$(RMDIR) $(TMPDIR)
endif

.PHONY: forceclean
forceclean: ## force delete all created temporary folders
	@echo "$(COLOR)Force cleaning$(RESET)"
ifneq ($(wildcard $(TMPDIR)),)
	-$(RM) -r $(TMPDIR)
endif

.PHONY: help
help: ## show this help
	@awk -F':.*##' '/: [^#]*##/{ printf("%12s: %s\n", $$1, $$2)}' $(MAKEFILE_LIST)

