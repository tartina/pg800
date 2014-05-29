bin_PROGRAMS = pg800
pg800_SOURCES = main.cc window.cc window.h tone.cc tone.h preferences.cc preferences.h about.cc about.h

pg800_CPPFLAGS=$(gtkmm_CFLAGS) $(rtmidi_CFLAGS) $(libxml_CFLAGS)
pg800_CXXFLAGS=-std=c++11
pg800_LDADD=$(gtkmm_LIBS) $(rtmidi_LIBS) $(libxml_LIBS)
