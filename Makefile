MAIN = procsweeper

IDIR = src
ODIR = bin

CC = clang

LIB = \
	-lncurses \
	-lprocps

CFLAGS = \
	-O0 \
	-g3 \
	-std=c11

IFILES = $(wildcard $(IDIR)/*.c)
OFILES = $(IFILES:$(IDIR)/%.c=$(ODIR)/%.o)

.PHONY: all test clean

all: $(MAIN)

test: $(MAIN)
	@./$(MAIN)

clean:
	@rm -rf $(ODIR)

$(ODIR)/%.o: $(IDIR)/%.c
	@echo "[$(CC)] $@"
	@mkdir -p `dirname $@`
	@$(CC) -c -o $@ $< $(CFLAGS)

$(MAIN): $(OFILES)
	@echo "[$(CC)] $@"
	@$(CC) -o $@ $^ $(LIB) $(CFLAGS) $(LIBS)
