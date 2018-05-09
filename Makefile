CXXFLAGS := -Wall -g -O2

PREFIX := /usr
BINDIR := $(PREFIX)/bin
DATADIR := $(PREFIX)/share
DESTDIR :=

CROSS :=

CONVERT := convert
CXX := $(CROSS)g++
INSTALL := install
MSGFMT := msgfmt
MSGMERGE := msgmerge
PKG_CONFIG := $(CROSS)pkg-config
POVRAY := povray
STRIP := $(CROSS)strip
XGETTEXT := xgettext

PKG_LUA := $(shell $(PKG_CONFIG) --exists lua-5.2 && echo lua-5.2 || echo lua)

MSGID_BUGS_ADDRESS := roever@users.sf.net

.DELETE_ON_ERROR:

.PHONY: default
default: all

VERSION := $(shell cat src/version)
FILES_DIST += src/version

PKGS += $(PKG_LUA)
PKGS += SDL_mixer
PKGS += SDL_ttf
PKGS += fribidi
PKGS += libpng
PKGS += sdl
PKGS += zlib

LIBS += -lboost_filesystem
LIBS += -lboost_system

DEFS += -DVERSION='"$(VERSION)"'
DEFS += -DDATADIR='"$(DATADIR)"'

FILES_H := $(wildcard src/pushover/*.h src/pushover/linebreak/*.h src/pushover/sha1/*.hpp)
FILES_DIST += $(FILES_H)
FILES_CPP := $(wildcard src/pushover/*.cpp src/pushover/linebreak/*.c src/pushover/sha1/*.cpp)
FILES_DIST += $(FILES_CPP)
FILES_O := $(patsubst src/pushover/%,_tmp/pushover/%.o,$(FILES_CPP))

.SECONDARY: $(FILES_O)
_tmp/pushover/%.o: src/pushover/% src/version $(FILES_H)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) `$(PKG_CONFIG) --cflags $(PKGS)` $(DEFS) -c -o $@ $<

FILES_DEBUG += pushover.debug

pushover.debug: $(FILES_O)
	$(CXX) $(CXXFLAGS) `$(PKG_CONFIG) --libs $(PKGS)` $(LIBS) -o $@ $(FILES_O)

FILES_BINDIR += pushover

pushover: pushover.debug
	cp $< $@
	$(STRIP) -s $@

ASSEMBLER_PKGS += SDL_image
ASSEMBLER_PKGS += libpng
ASSEMBLER_PKGS += sdl

.SECONDARY: _tmp/dominoes/assembler
_tmp/dominoes/assembler: src/dominoes/assembler.cpp src/dominoes/pngsaver.h
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) `$(PKG_CONFIG) --cflags $(ASSEMBLER_PKGS)` `$(PKG_CONFIG) --libs $(ASSEMBLER_PKGS)` -o $@ $<

.SECONDARY: _tmp/dominoes/done
_tmp/dominoes/done: src/dominoes/domino.ini src/dominoes/*.pov
	mkdir -p $(dir $@)
	$(POVRAY) $<
	touch $@

FILES_DIST += generated/dominoes.png
FILES_DATADIR += pushover_data/pushover/data/dominoes.png

generated/dominoes.png: _tmp/dominoes/assembler _tmp/dominoes/done
	mkdir -p $(dir $@)
	_tmp/dominoes/assembler $@ 58 2 200 src/dominoes/dominoes.lst

pushover_data/pushover/data/dominoes.png: generated/dominoes.png
	mkdir -p $(dir $@)
	cp $< $@

FILES_DATA_SRC := $(wildcard data/*.ogg data/*.png)
FILES_DIST += $(FILES_DATA_SRC)
FILES_DATADIR += $(patsubst data/%,pushover_data/pushover/data/%,$(FILES_DATA_SRC))

pushover_data/pushover/data/%: data/%
	mkdir -p $(dir $@)
	cp $< $@

FILES_THEMES_SRC := $(wildcard src/themes/*)
FILES_DIST += $(FILES_THEMES_SRC)
FILES_DATADIR += $(patsubst src/themes/%,pushover_data/pushover/themes/%,$(FILES_THEMES_SRC))

pushover_data/pushover/themes/%: src/themes/%
	mkdir -p $(dir $@)
	cp $< $@

FILES_PO := $(wildcard src/po/*.po)
FILES_DIST += $(FILES_PO)
FILES_DATADIR += $(patsubst src/po/%.po,pushover_data/locale/%/LC_MESSAGES/pushover.mo,$(FILES_PO))

pushover_data/locale/%/LC_MESSAGES/pushover.mo: src/po/%.po
	mkdir -p $(dir $@)
	$(MSGFMT) -c -o $@ $<

FILES_DIST += src/description/pushover.desktop
FILES_DATADIR += pushover_data/applications/pushover.desktop

pushover_data/applications/pushover.desktop: src/description/pushover.desktop
	mkdir -p $(dir $@)
	cp $< $@

FILES_DIST += src/description/pushover.ico
FILES_DATADIR += pushover_data/icons/hicolor/16x16/apps/pushover.png
FILES_DATADIR += pushover_data/icons/hicolor/32x32/apps/pushover.png
FILES_DATADIR += pushover_data/icons/hicolor/48x48/apps/pushover.png
FILES_DATADIR += pushover_data/icons/hicolor/64x64/apps/pushover.png

pushover_data/icons/hicolor/%/apps/pushover.png: src/description/pushover.ico
	mkdir -p $(dir $@)
	$(CONVERT) $<[$(shell expr substr $* 1 2 / 16 - 1)] $@

FILES_DIST += src/description/pushover.6
FILES_DATADIR += pushover_data/man/man6/pushover.6.gz

pushover_data/man/man6/pushover.6.gz: src/description/pushover.6
	mkdir -p $(dir $@)
	gzip -9n <$< >$@

FILES_DIST += $(wildcard src/levels/*/*)
FILES_LEVELS_SRCDIRS := $(wildcard src/levels/*)
FILES_DATADIR += $(patsubst src/levels/%,pushover_data/pushover/levels/%.gz,$(FILES_LEVELS_SRCDIRS))

pushover_data/pushover/levels/%.gz: src/levels/%/*.level
	mkdir -p $(dir $@)
	cat src/levels/$*/*.level | gzip -9 >$@

_tmp/po/leveltexts.cpp: src/levels/*/*.level
	mkdir -p $(dir $@)
	sed -n '/^\(Description\|Hint\|Name\|Tutorial\)$$/,/^[^|]/ s,^| \(.*\)$$,_("\1"),p' src/levels/*/*.level | LC_ALL=C sort -u >$@

.PHONY: all
all: $(FILES_BINDIR) $(FILES_DATADIR) $(FILES_DEBUG)

DIST := pushover-$(VERSION).tgz

.PHONY: dist
dist: $(DIST)

FILES_DIST += AUTHORS
FILES_DIST += COPYING
FILES_DIST += Makefile
FILES_DIST += NEWS
FILES_DIST += README

$(DIST): $(FILES_DIST)
	rm -rf _tmp/$(basename $@)
	mkdir -p _tmp/$(basename $@)
	tar c $^ | tar x -C _tmp/$(basename $@)
	tar c -C _tmp $(basename $@) | gzip -9n >$@

.PHONY: check
check: all
	./pushover -c src/recordings/finish/*
	./pushover -y src/recordings/fail
	./pushover -x src/recordings/crash
	@echo OK

FILES_INSTALL += $(patsubst %,$(DESTDIR)$(BINDIR)/%,$(FILES_BINDIR))

$(DESTDIR)$(BINDIR)/%: %
	$(INSTALL) -m 755 -d $(dir $@)
	$(INSTALL) -m 755 $< $@

FILES_INSTALL += $(patsubst pushover_data/%,$(DESTDIR)$(DATADIR)/%,$(FILES_DATADIR))

$(DESTDIR)$(DATADIR)/%: pushover_data/%
	$(INSTALL) -m 755 -d $(dir $@)
	$(INSTALL) -m 644 $< $@

.PHONY: install
install: $(FILES_INSTALL)

.PHONY: clean
clean:
	rm -f $(DIST) $(FILES_BINDIR) $(FILES_DEBUG)
	rm -rf _tmp/ pushover_data/
	@echo Not removing generated/

.PHONY: update-po
update-po: _tmp/po/messages.pot
	for PO_FILE in src/po/*.po; do $(MSGMERGE) -Uq --backup=none $$PO_FILE $<; done

_tmp/po/messages.pot: _tmp/po/leveltexts.cpp src/pushover/*.cpp
	mkdir -p $(dir $@)
	$(XGETTEXT) --msgid-bugs-address=$(MSGID_BUGS_ADDRESS) -cTRANSLATORS: -k_ -kN_ -o $@ $^
