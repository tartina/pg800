#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_DEBUG
#include <iostream>
#include <iomanip> 
#endif

#include <vector>
#include "tone.h"

mks70_tone::mks70_tone()
{
	unsigned short i;

	// Initialize the tone with default values
	name = "INIT TONE";

	for (i = 0; i < 2; i++) {
		dco_range[i] = 1; // dco range = 8'
		dco_wave[i] = 1; // dco wave = saw
	}

#ifdef HAVE_DEBUG
	dump_tone();
#endif
}

mks70_tone::~mks70_tone() {}

void mks70_tone::set_dco_range(unsigned short dco, unsigned short range,
                               unsigned short midi_channel, RtMidiOut* midi_out, bool send)
{
	if (dco > 1 || range > 3) return;

	if (range != dco_range[dco]) {
		dco_range[dco] = range;
		if (send) {
			// TODO send dco_range via MIDI
		}
#ifdef HAVE_DEBUG
		dump_tone();
#endif
	}
}

void mks70_tone::set_dco_wave(unsigned short dco, unsigned short wave,
                              unsigned short midi_channel, RtMidiOut* midi_out, bool send)
{
	std::vector<unsigned char> message;

	if (dco > 1 || wave > 3) return;

	if (wave != dco_wave[dco]) {
		dco_wave[dco] = wave;
		if (send) {
			message.push_back(0xF0);
			message.push_back(0x41);
			message.push_back(0x36);
			message.push_back(midi_channel);
			message.push_back(0x24);
			message.push_back(0x01);
			switch(dco) {
				case 0:
					message.push_back(12);
					break;
				case 1:
					message.push_back(17);
					break;
			}
			message.push_back(wave * 32);
			message.push_back(0xF7);
#ifdef HAVE_DEBUG
			std::cout << "MIDI DCO wave: ";
			for (std::vector<unsigned char>::iterator it = message.begin();
			     it < message.end(); ++it)
				std::cout << std::setbase(16) << (unsigned short)*it << std::endl;
#endif
			// TODO send dco_wave via MIDI
			midi_out->sendMessage(&message);
		}
#ifdef HAVE_DEBUG
		dump_tone();
#endif
	}
}

void mks70_tone::set_name(const std::string& name)
{
	this->name = name.substr(0 ,9);
}

#ifdef HAVE_DEBUG
void mks70_tone::dump_tone()
{
	unsigned short i;

	std::cout << "Name: " << name << "\n";

	for (i = 0; i < 2; i++) {
		std::cout << "DCO" << (i + 1) << " Range: " << dco_range[i] << "\n";
		std::cout << "DCO" << (i + 1) << " Waveform: " << dco_wave[i] << "\n";
	}
}
#endif
