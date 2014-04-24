#ifndef MKS70_TONE_H
#define MKS70_TONE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>

class mks70_tone
{
private:
	std::string name;

	unsigned short dco_range[2]; // 4 values
	unsigned short dco_wave[2]; // 4 values
	unsigned short dco_tune[2];
	unsigned short dco_lfo[2];
	unsigned short dco_env[2];
	unsigned short dco2_xmod; // 4 values
	unsigned short dco2_ftune;
	unsigned short dco_dynamics; // 4 values

	unsigned short mix_dco[2];
	unsigned short mix_env;
	unsigned short mix_dynamics; // 4 values
	unsigned short mix_env_mode; // 4 values

	unsigned short hpf_cutoff_freq; // 4 values
	unsigned short vcf_cutoff_freq;
	unsigned short vcf_resonance;
	unsigned short vcf_lfo_mod_depth;
	unsigned short vcf_env_mod_depth;
	unsigned short vcf_key_follow; // 4 values
	unsigned short vcf_dynamics; // 4 values

	unsigned short chorus; // 3 values

	unsigned short lfo_waveform; // 3 values
	unsigned short lfo_delay_time;
	unsigned short lfo_rate;

	unsigned short env_attack_time[2];
	unsigned short env_decay_time[2];
	unsigned short env_sustain_level[2];
	unsigned short env_release_time[2];
	unsigned short env_key_follow[2]; // 4 values

	unsigned short vca_env_mode; // Only 2 values

public:
	mks70_tone();
	virtual ~mks70_tone();
	bool apr_send(unsigned short midi_channel, unsigned short tone);

	inline std::string get_name() {return name;};
	void set_name(std::string newname);

	void set_dco_range(unsigned short dco, unsigned short range, bool send = false);
	void set_dco_wave(unsigned short dco, unsigned short wave, bool send = false);

#ifdef HAVE_DEBUG
	void dump_tone();
#endif
};

#endif
