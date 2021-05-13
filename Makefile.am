# Written by Salonia Matteo on 13/05/2021

# Which compiler to use
CC = gcc

# Linker flags used for Dynamic linking
AM_LDFLAGS =
AM_LDFLAGS += -ltinfo
AM_LDFLAGS += -lncurses

# LINKER: choose a linker to use; can be bfd, gold, lld
# comment to use the default linker, uncomment to use a custom linker
#LINKER = -fuse-ld=gold

# CSTD: which C revision to use
CSTD = -std=c99

# Optimizations. Can be -O0, -O1, -O2, -O3, -Os, -Ofast
OPTS = -Ofast

# CPU-Specific Optimizations
CPUOPTS = -march=native -mtune=native

# CFLAGS: additional compiler flags
AM_CFLAGS = -Wall $(CSTD) $(LINKER) $(OPTS) $(CPUOPTS)

# Executable name
bin_PROGRAMS = csnake

# Source code location
csnake_SOURCES = src/csnake.c

# Documentation installation directory
docdir = @docdir@

# Various Documentation & Data files 
doc_DATA = csnake-doc.rst README.md

# Clean the project directory
cleandir:
	make mostlyclean
	make clean
	make distclean
	rm -rf autom4te.cache aclocal.m4 *.tar.gz

# Alias "uninstall-docDATA" to "uninstall-doc"
uninstall-doc: uninstall-docDATA
