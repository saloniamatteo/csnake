# CC: can be gcc, clang, or a compiler of your choice

# Cross-compiler
#CC = aarch64-linux-gnu-gcc

# Normal compiler
CC = gcc

# Linker flags used for Dynamic linking
LDFLAGS = -ltinfo -lncurses

# Optimizations. Can be -O0, -O1, -O2, -O3, -Os, -Ofast
OPTS = -Ofast

# Architecture, Tune. Set to 1 to disable.
NOARCHTUNE = 0

# Install directory
INSTALLDIR = ${DESTDIR}${PREFIX}/usr/bin

# Documentation directory
DOCDIR = ${DESTDIR}${PREFIX}/usr/share/doc/csnake

# Disable ARCH & TUNE if using cross-compiler,
# or if compiling on ARM.
ifeq ($(CC), aarch64-linux-gnu-gcc)
NOARCHTUNE = 1
else ifeq ($(shell uname -m), "aarch64")
NOARCHTUNE = 1
endif

# Check if NOARCHTUNE is 0 (enable ARCH & TUNE)
ifeq (NOARCHTUNE, 0)
	ARCH = -march=native
	TUNE = -mtune=native
endif

# CFLAGS: additional compiler flags
CFLAGS = -Wall
# LINKER: choose a linker to use; can be bfd, gold, lld
# comment to use the default linker, uncomment to use a custom linker
#LINKER = -fuse-ld=gold
# CSTD: which C revision to use
CSTD = -std=c99

# MAJVER: Major Version
MAJVER = $(shell grep "__CSNAKE_MAJ_VER" csnake.c -m1 | awk '{print $$3}')
# MINVER: Minor Version
MINVER = $(shell grep "__CSNAKE_MIN_VER" csnake.c -m1 | awk '{print $$3}')
# FULLVER: Full Version
FULLVER = "$(MAJVER).$(MINVER)"

csnake: csnake.o
	@$(CC) $^ -o $@ $(CFLAGS) $(OPTS) $(LINKER) $(ARCH) $(TUNE) $(CSTD) $(LDFLAGS)
	@echo "CC $<"

csnake.o: csnake.c
	@$(CC) -c $< -o $@ $(CFLAGS) $(OPTS) $(LINKER) $(ARCH) $(TUNE)
	@echo "CC $<"

# Generate tags with cscope and ctags
gentags:
	@echo "Generating tags..."
	$(shell cscope -bkq)
	$(shell find . -type f -name "*.[chsS]" -print > cscope.files)
	$(shell ctags -L cscope.files)
	@echo "Done! "

install: csnake
	mkdir -pv ${INSTALLDIR}
	cp -fv csnake ${INSTALLDIR}
	chmod -v 755 ${INSTALLDIR}/csnake
	mkdir -pv ${DOCDIR}
	cp -fv csnake-doc.rst README.md ${DOCDIR}
	chmod -v 644 ${DOCDIR}/csnake-doc.rst ${DOCDIR}/README.md

uninstall: clean
	rm -fv ${INSTALLDIR}/csnake ${DOCDIR}/csnake-doc.rst

version:
	@echo $(FULLVER)

clean:
	rm -f *.o csnake cscope* tags

.PHONY = clean csnake gentags install uninstall version
