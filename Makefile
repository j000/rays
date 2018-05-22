SRC := rays.cpp
SRC += $(filter-out $(SRC),$(wildcard *.cpp))
########################################
EXE := $(basename $(firstword $(SRC)))
SRCDIR ?= .
TMPDIR ?= .tmp
PGODIR ?= .pgo
########################################
SHELL := /bin/sh
MKDIR ?= mkdir
RMDIR ?= $(RM) -d
########################################
# enable a lot of warnings and then some more
WARNINGS := -Wall -Wextra
# be more strict
ifeq ($(shell expr $(shell $(CC) -dumpversion) "<" "4.8"),1)
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
# the new-style casts are less vulnerable to unintended effects
WARNINGS += -Wold-style-cast

########################################
CXXFLAGS := $(CXXFLAGS) $(WARNINGS) -std=c++17 -O2 -pipe
# no runtime type information
CXXFLAGS += -fno-rtti
# LTO
CXXFLAGS += -flto
# probably best option
CXXFLAGS += -mtune=native
# use SSE2
CXXFLAGS += -mfpmath=sse -msse2 -ffast-math
# preserve debug info
CXXFLAGS += -g -ggdb
########################################
LDFLAGS := $(LDFLAGS)
# LDFLAGS += -static -static-libgcc
LDFLAGS += -static-libgcc -Wl,-R.
LOADLIBES := -Wl,--as-needed
LOADLIBES += -l:libm.a
LOADLIBES += -l:libstdc++.a
########################################
# generate better data for perf
# this will make program run slower
# CXXFLAGS += -p
# LDFLAGS += -p
########################################
ifndef NOCOLOR
  COLOR := [1;34m
  RESET := [0m
else
  COLOR :=
  RESET :=
endif
########################################
OBJ := $(patsubst %.cpp,$(TMPDIR)/%.o,$(SRC))
PGOOBJ := $(patsubst %.cpp,$(PGODIR)/%.o,$(SRC))
########################################
DEPFLAGS := -MMD -MP
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
debug: CXXFLAGS := $(filter-out -O2,$(CXXFLAGS)) -D_DEBUG -Og
debug: $(EXE) ## build with debug enabled

.PHONY: debugrun
debugrun: debug run ## run debug version

$(EXE): private CXXFLAGS += -fprofile-dir=$(PGODIR) -fprofile-use -fprofile-correction
$(EXE): $(OBJ)
	@echo "$(COLOR)Link $^ -> $@$(RESET)"
# default rule
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)

$(TMPDIR)/%.o: private CXXFLAGS += -fprofile-dir=$(PGODIR) -fprofile-use -fprofile-correction
$(TMPDIR)/%.o: $(SRCDIR)/%.cpp Makefile
	@echo "$(COLOR)Compile $(SRCDIR)/$*.cpp -> $(TMPDIR)/$*.o$(RESET)"
	$(COMPILE.cpp) $(DEPFLAGS) $(OUTPUT_OPTION) $<

.PHONY: pgo pgo-generate pgo-run
pgo: clean pgo-generate all
pgo-run: pgo-generate run

PGO-GENERATE-FLAGS := -fprofile-generate

pgo-generate: $(PGODIR)/$(EXE)
	@echo "$(COLOR)Generating profile...$(RESET)"
	-@$(PGODIR)/$(EXE)

$(PGODIR)/$(EXE): private CXXFLAGS += $(PGO-GENERATE-FLAGS)
$(PGODIR)/$(EXE): $(PGOOBJ)
	@echo "$(COLOR)Link $^ -> $@$(RESET)"
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)

$(PGODIR)/%.o: private CXXFLAGS += $(PGO-GENERATE-FLAGS)
$(PGODIR)/%.o: $(SRCDIR)/%.cpp Makefile
	@echo "$(COLOR)Compile $(SRCDIR)/$*.cpp -> $(PGODIR)/$*.o$(RESET)"
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

# include dependencies
-include $(wildcard $(OBJ:.o=.d))

# depend on directory
$(OBJ): | $(TMPDIR)/.keepme
$(PGOOBJ): | $(PGODIR)/.keepme
$(PGOOBJ:.o=.gcda): | $(PGODIR)/.keepme

# create directory
$(TMPDIR)/.keepme:
	-$(MKDIR) $(TMPDIR)
	touch $@

$(PGODIR)/.keepme:
	-$(MKDIR) $(PGODIR)
	touch $@

# delete stuff
.PHONY: clean
clean: mostlyclean ## delete everything this Makefile created
	-$(RM) $(EXE)

.PHONY: mostlyclean
mostlyclean: ## delete everything created, leave executable
	@echo "$(COLOR)Cleaning$(RESET)"
	-$(RM) $(TMPDIR)/.keepme $(OBJ) $(OBJ:.o=.d)
	-$(RM) $(PGODIR)/.keepme $(PGOOBJ) $(PGOOBJ:.o=.d) $(PGOOBJ:.o=.gcda) $(PGODIR)/$(EXE)
	-$(RMDIR) $(TMPDIR) $(PGODIR)

.PHONY: forceclean
forceclean: ## force delete all created temporary folders
	@echo "$(COLOR)Force cleaning$(RESET)"
	-$(RM) -r $(TMPDIR)

.PHONY: help
help: ## show this help
	@awk -F':.*##' '/: [^#]*##/{ printf("%12s: %s\n", $$1, $$2)}' $(MAKEFILE_LIST)

