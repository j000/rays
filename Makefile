SRC := rays.cpp
SRC += $(filter-out $(SRC),$(wildcard *.cpp))
########################################
EXE := $(basename $(firstword $(SRC)))
SRCDIR ?= .
TMPDIR ?= .tmp
PGODIR ?= .pgo
PRECOMPILE_HEADER ?= 1
VERBOSE ?= 0
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
CXXFLAGS := $(CXXFLAGS) $(WARNINGS) -std=c++17 -O3 -pipe
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
# OpenMP
CXXFLAGS += -fopenmp
########################################
LDFLAGS := $(LDFLAGS)
LDFLAGS += -static-libgcc
ifdef STATIC
  LDFLAGS += -static
  # needed (why?) for openmp + lto
  LDFLAGS += -Wl,--allow-multiple-definition
endif
LDFLAGS += -Wl,-R.
LOADLIBES := -Wl,--as-needed
# LOADLIBES += -l:libm.a
# LOADLIBES += -lm
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
DEPFLAGS := -MD -MP
CXXFLAGS += -I$(SRCDIR)

LD := ld.gold
CXXFLAGS += -fuse-ld=gold

# be silent unless VERBOSE
ifeq ($(VERBOSE),0)
  .SILENT: ;
endif

# default target
.PHONY: all
all: $(EXE) ## build executable

.PHONY: run
run: $(EXE) ## run program
	@./$(EXE)

########################################

.PHONY: debug
debug: CXXFLAGS += -D_DEBUG -Og
debug: $(EXE) ## build with debug enabled

.PHONY: debugrun
debugrun: debug run ## run debug version

########################################

$(EXE): $(OBJ)
	@echo "$(COLOR)Link $^ -> $@$(RESET)"
# default rule
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)

$(TMPDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "$(COLOR)Compile $(SRCDIR)/$*.cpp -> $(TMPDIR)/$*.o$(RESET)"
	$(COMPILE.cpp) $(DEPFLAGS) $(OUTPUT_OPTION) $<

########################################
.PHONY: pgo pgo-generate pgo-run
pgo: pgo-generate all
pgo-run: pgo-generate run

PGO-GENERATE-FLAGS := -fprofile-generate
$(EXE) $(OBJ): private CXXFLAGS += -fprofile-dir=$(PGODIR) -fprofile-use -fprofile-correction

ifneq ($(wildcard $(PGODIR)/*.gcda),)
  $(OBJ): $(PGOOBJ:.o=.gcda)
endif

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

########################################
# precompiled header
HEADER := $(TMPDIR)/precompiled.hpp

ifeq ($(PRECOMPILE_HEADER),1)
$(OBJ) $(PGOOBJ): private CXXFLAGS += -Winvalid-pch -DPRECOMPILED=1 -idirafter $(TMPDIR) -include $(HEADER)
$(HEADER): | $(TMPDIR)
$(OBJ) $(PGOOBJ): $(HEADER).gch

$(HEADER): $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.hpp)
	@echo "$(COLOR)Prepare header $@$(RESET)"
	sed -n -e '/^#ifndef PRECOMPILED$$/,/^#endif$$/ {/^#ifndef PRECOMPILED$$/d; /^#endif$$/d; p}' $^ > $@

$(HEADER).gch: $(HEADER)
	@echo "$(COLOR)Precompile header $< -> $@$(RESET)"
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
endif

########################################
# include dependencies
-include $(wildcard $(OBJ:.o=.d))

# depend on directory
$(OBJ): Makefile | $(TMPDIR)
$(PGOOBJ): | $(PGODIR)
$(PGOOBJ:.o=.gcda):

# create directory
$(TMPDIR):
	-$(MKDIR) $(TMPDIR)

$(PGODIR):
	-$(MKDIR) $(PGODIR)

########################################
# delete stuff
.PHONY: clean
clean: mostlyclean ## delete everything this Makefile created
	-$(RM) $(EXE)

.PHONY: mostlyclean
mostlyclean: ## delete everything created, leave executable
	@echo "$(COLOR)Cleaning$(RESET)"
	-$(RM) $(OBJ) $(OBJ:.o=.d) $(HEADER).gch $(HEADER)
	-$(RM) $(PGOOBJ) $(PGOOBJ:.o=.d) $(PGOOBJ:.o=.gcda) $(PGODIR)/$(EXE)
	-$(RMDIR) $(TMPDIR) $(PGODIR)

.PHONY: forceclean
forceclean: ## force delete all created temporary folders
	@echo "$(COLOR)Force cleaning$(RESET)"
	-$(RM) -r $(TMPDIR)
	-$(RM) -r $(PGODIR)

########################################

.PHONY: help
help: ## show this help
	@awk -F':.*##' '/: [^#]*##/{ printf("%12s: %s\n", $$1, $$2)}' $(MAKEFILE_LIST)

