ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = pg800
pg800_SOURCES = main.cc window.cc window.h tone.cc tone.h preferences.cc preferences.h about.cc about.h

pg800_CPPFLAGS=$(gtkmm_CFLAGS) $(librtmidi_CFLAGS) $(rtmidi_CFLAGS) $(libxml_CFLAGS)
pg800_CXXFLAGS=-Wall
pg800_LDADD=$(gtkmm_LIBS) $(librtmidi_LIBS) $(rtmidi_LIBS) $(libxml_LIBS) $(BOOST_SYSTEM_LIBS) $(BOOST_FILESYSTEM_LIBS)

icon48dir = $(datadir)/icons/hicolor/48x48/apps
dist_icon48_DATA = icons/48/pg800.png

icon128dir = $(datadir)/icons/hicolor/128x128/apps
dist_icon128_DATA = icons/128/pg800.png

desktopdir = $(datadir)/applications
dist_desktop_DATA = pg800.desktop
