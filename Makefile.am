ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = pg800
pg800_SOURCES = main.cc window.cc window.h tone.cc tone.h preferences.cc \
	preferences.h about.cc about.h bulkdump.cc bulkdump.h bulkdump_exception.h \
	tonechooser.cc tonechooser.h tonemodelcolumns.h

pg800_CPPFLAGS=$(gtkmm_CFLAGS) $(librtmidi_CFLAGS) $(rtmidi_CFLAGS) $(libxml_CFLAGS)
pg800_CXXFLAGS=-Wall
pg800_LDADD=$(gtkmm_LIBS) $(librtmidi_LIBS) $(rtmidi_LIBS) $(libxml_LIBS) $(BOOST_SYSTEM_LIBS) $(BOOST_FILESYSTEM_LIBS)

icon48dir = $(datadir)/icons/hicolor/48x48/apps
dist_icon48_DATA = icons/48/pg800.png

icon128dir = $(datadir)/icons/hicolor/128x128/apps
dist_icon128_DATA = icons/128/pg800.png

desktopdir = $(datadir)/applications
dist_desktop_DATA = pg800.desktop

icons/48:
	mkdir -p icons/48

icons/128:
	mkdir -p icons/128

$(dist_icon48_DATA): icons/pg800-48x48.svg | icons/48
	inkscape --export-png=$@ icons/pg800-48x48.svg

$(dist_icon128_DATA): icons/pg800-128x128.svg | icons/128
	inkscape --export-png=$@ icons/pg800-128x128.svg

clean-local:
	-rm -rf icons/48 icons/128

EXTRA_DIST = icons/pg800-48x48.svg icons/pg800-128x128.svg
