#
# Copyright (c) 2014 PolyFloyd
#

NAME = procsweeper

IDIR = src
ODIR = bin
MAIN = $(ODIR)/$(NAME)

CC = clang
LD = clang

LIBS = \
	ncurses \
	procps

CFLAGS = \
	-O0 \
	-g3 \
	-std=c11

LDFLAGS = \
	$(addprefix -l, $(LIBS))

IFILES = $(wildcard $(IDIR)/*.c)
OFILES = $(IFILES:$(IDIR)/%.c=$(ODIR)/%.o)

.PHONY: all test clean

all: $(MAIN)

test: $(MAIN)
	@./$(MAIN) --test

clean:
	@rm -rf $(ODIR)

$(ODIR)/%.o: $(IDIR)/%.c
	@echo "[$(CC)] $@"
	@mkdir -p `dirname $@`
	@$(CC) -c -o $@ $< $(CFLAGS)

$(MAIN): $(OFILES)
	@echo "[$(LD)] $@"
	@$(LD) -o $@ $^ $(LDFLAGS)
