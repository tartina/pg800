bin_PROGRAMS = pg800
pg800_SOURCES = main.cc window.cc window.h tone.cc tone.h preferences.cc preferences.h about.cc about.h

pg800_CPPFLAGS=$(gtkmm_CFLAGS) $(rtmidi_CFLAGS) $(libxml_CFLAGS)
pg800_CXXFLAGS=-Wall -std=c++11
pg800_LDADD=$(gtkmm_LIBS) $(rtmidi_LIBS) $(libxml_LIBS)

icons/pg800.png:	icons/pg800-48x48.svg
	inkscape --export-png=icons/pg800.png icons/pg800-48x48.svg

pixmapdir = $(datadir)/pixmaps
pixmap_DATA = icons/pg800.png
