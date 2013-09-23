# $Id: GNUmakefile,v 1.1 1999-01-07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := clinac
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

# enable ROOT output option
#CPPFLAGS += -D__OUTPUT_ROOTFILE__

CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS = $(shell $(ROOTSYS)/bin/root-config --glibs)
EXTRALIBS += $(ROOTLIBS)


include $(G4INSTALL)/config/binmake.gmk
