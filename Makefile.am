ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = pg800
pg800_SOURCES = main.cc window.cc window.h tone.cc tone.h preferences.cc preferences.h about.cc about.h

pg800_CPPFLAGS=$(gtkmm_CFLAGS) $(rtmidi_CFLAGS) $(libxml_CFLAGS)
pg800_CXXFLAGS=-Wall
pg800_LDADD=$(gtkmm_LIBS) $(rtmidi_LIBS) $(libxml_LIBS) $(BOOST_SYSTEM_LIBS) $(BOOST_FILESYSTEM_LIBS)

icondir = $(datadir)/icons
dist_icon_DATA = icons/pg800.png
