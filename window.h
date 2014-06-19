/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * window.h
 * Copyright (C) 2014 Guido Aulisi <guido.aulisi@gmail.com>
 *
 * pg800 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pg800 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MKS70_WINDOW_H
#define MKS70_WINDOW_H

#include <gtkmm.h>

#include "RtMidi.h"
#include "tone.h"

class mks70_window : public Gtk::ApplicationWindow
{
	public:
		mks70_window();
		virtual ~mks70_window();
		static const std::string window_title;

	private:
		static const unsigned int range_height = 108;
		Glib::RefPtr<Gtk::IconTheme> icon_theme;

		Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
		Glib::RefPtr<Gtk::UIManager> m_refUIManager;

		Gtk::Box m_Application_Box;
		Gtk::Statusbar status_bar;
		Gtk::Box m_Editor_Box;
		Gtk::Frame m_DCO_Frame[2];
		Gtk::Grid dco_grid[2];

		Gtk::Label m_Range_Label[2];
/*
	Range buttons are:
	[0] -> 16
	[1] -> 8
	[2] -> 4
	[3] -> 2
*/
		Gtk::RadioButton rb_dco_range[4][2];

		Gtk::Label m_Wave_Label[2];
/*
	Waveform buttons are (wrong docs):
	[0] -> Noise
	[1] -> Square
	[2] -> Pulse
	[3] -> Saw
*/
		Gtk::RadioButton rb_dco_waveform[4][2];

		Gtk::Label crossmod_label;
		Gtk::RadioButton rb_crossmod[4];

		Gtk::Label dco_tune_label[2];
		Gtk::Scale* sc_dco_tune[2];
		Glib::RefPtr<Gtk::Adjustment> adj_dco_tune[2];

		Gtk::Label dco2_ftune_label;
		Gtk::Scale sc_dco2_ftune;
		Glib::RefPtr<Gtk::Adjustment> adj_dco2_ftune;

		Gtk::Label dco_lfo_label[2];
		Gtk::Scale* sc_dco_lfo[2];
		Glib::RefPtr<Gtk::Adjustment> adj_dco_lfo[2];
		Gtk::Label dco_envelope_label[2];
		Gtk::Scale* sc_dco_envelope[2];
		Glib::RefPtr<Gtk::Adjustment> adj_dco_envelope[2];

		Gtk::Label dco_dyna_label;
		Gtk::Box dco_dyna_box;
		Gtk::RadioButton rb_dco_dyna[4];
		Gtk::Label dco_mode_label;
		Gtk::Box dco_mode_box;
		Gtk::RadioButton rb_dco_mode[4];

		// Mixer frame
		Gtk::Frame mixer_frame;
		Gtk::Grid mixer_grid;
		Gtk::Label mixer_dco_label[2];
		Gtk::Scale* sc_mixer_dco[2];
		Glib::RefPtr<Gtk::Adjustment> adj_mixer_dco[2];
		Gtk::Label mixer_envelope_label;
		Gtk::Scale sc_mixer_envelope;
		Glib::RefPtr<Gtk::Adjustment> adj_mixer_envelope;
		Gtk::Label mixer_dyna_label;
		Gtk::RadioButton rb_mixer_dyna[4];
		Gtk::Label mixer_mode_label;
		Gtk::RadioButton rb_mixer_mode[4];

		// VCF frame
		Gtk::Frame vcf_frame;
		Gtk::Grid vcf_grid;
		Gtk::Label vcf_hpf_label;
		Gtk::RadioButton rb_vcf_hpf[4];
		Gtk::Label vcf_cutoff_label;
		Gtk::Scale sc_vcf_cutoff;
		Glib::RefPtr<Gtk::Adjustment> adj_vcf_cutoff;
		Gtk::Label vcf_resonance_label;
		Gtk::Scale sc_vcf_resonance;
		Glib::RefPtr<Gtk::Adjustment> adj_vcf_resonance;
		Gtk::Label vcf_lfo_label;
		Gtk::Scale sc_vcf_lfo;
		Glib::RefPtr<Gtk::Adjustment> adj_vcf_lfo;
		Gtk::Label vcf_env_label;
		Gtk::Scale sc_vcf_env;
		Glib::RefPtr<Gtk::Adjustment> adj_vcf_env;
		Gtk::Label vcf_key_label;
		Gtk::Scale sc_vcf_key;
		Glib::RefPtr<Gtk::Adjustment> adj_vcf_key;
		Gtk::Label vcf_dyna_label;
		Gtk::RadioButton rb_vcf_dyna[4];
		Gtk::Label vcf_env_mode_label;
		Gtk::RadioButton rb_vcf_env_mode[4];

		// VCA frame
		Gtk::Frame vca_frame;
		Gtk::Box vca_box;
		Gtk::Label vca_level_label;
		Gtk::Scale sc_vca_level;
		Glib::RefPtr<Gtk::Adjustment> adj_vca_level;
		Gtk::Label vca_env_mode_label;
		Gtk::RadioButton rb_vca_env_mode[2];
		Gtk::Label vca_dyna_label;
		Gtk::RadioButton rb_vca_dyna[4];
		Gtk::Separator vca_separator;

		Gtk::Box editor_box2;

		// LFO Frame
		Gtk::Frame lfo_frame;
		Gtk::Grid lfo_grid;
		Gtk::Label lfo_waveform_label;
		Gtk::RadioButton rb_lfo_waveform[3];
		Gtk::Label lfo_delay_time_label;
		Gtk::Scale sc_lfo_delay_time;
		Glib::RefPtr<Gtk::Adjustment> adj_lfo_delay_time;
		Gtk::Label lfo_rate_label;
		Gtk::Scale sc_lfo_rate;
		Glib::RefPtr<Gtk::Adjustment> adj_lfo_rate;

		// Envelope Frames
		Gtk::Frame envelope_frame[2];
		Gtk::Grid envelope_grid[2];
		Gtk::Label envelope_attack_label[2];
		Gtk::Label envelope_decay_label[2];
		Gtk::Label envelope_sustain_label[2];
		Gtk::Label envelope_release_label[2];
		Gtk::Scale* sc_envelope_attack[2];
		Glib::RefPtr<Gtk::Adjustment> adj_envelope_attack[2];
		Gtk::Scale* sc_envelope_decay[2];
		Glib::RefPtr<Gtk::Adjustment> adj_envelope_decay[2];
		Gtk::Scale* sc_envelope_sustain[2];
		Glib::RefPtr<Gtk::Adjustment> adj_envelope_sustain[2];
		Gtk::Scale* sc_envelope_release[2];
		Glib::RefPtr<Gtk::Adjustment> adj_envelope_release[2];
		Gtk::Label envelope_key_follow_label[2];
		Gtk::RadioButton rb_envelope_key_follow[4][2];

		// Chorus Frame
		Gtk::Frame chorus_frame;
		Gtk::Label chorus_label;
		Gtk::Box chorus_box;
		Gtk::RadioButton rb_chorus[3];

		// Signal handlers
		void on_action_file_open();
		void on_action_file_new();
		void on_action_file_save_as();
		void on_action_file_send();
		void on_action_file_preferences();
		void inline on_action_file_quit() {hide();};
		void on_action_help_about();
		void on_dco_range_button_clicked(unsigned short dco);
		void on_dco_waveform_button_clicked(unsigned short dco);
		void on_dco2_crossmod_button_clicked();
		void on_dco_tune_value_changed(unsigned short dco);
		void on_dco2_ftune_value_changed();
		void on_dco_lfo_value_changed(unsigned short dco);
		void on_dco_envelope_value_changed(unsigned short dco);
		void on_dco_dyna_button_clicked();
		void on_dco_mode_button_clicked();
		void on_mixer_dco_value_changed(unsigned short dco);
		void on_mixer_envelope_value_changed();
		void on_mixer_dyna_button_clicked();
		void on_mixer_mode_button_clicked();
		void on_vcf_hpf_button_clicked();
		void on_vcf_cutoff_value_changed();
		void on_vcf_resonance_value_changed();
		void on_vcf_lfo_value_changed();
		void on_vcf_env_value_changed();
		void on_vcf_key_value_changed();
		void on_vcf_dyna_button_clicked();
		void on_vcf_env_mode_button_clicked();
		void on_vca_level_value_changed();
		void on_vca_env_mode_button_clicked();
		void on_vca_dyna_button_clicked();
		void on_lfo_waveform_button_clicked();
		void on_lfo_delay_time_value_changed();
		void on_lfo_rate_value_changed();
		void on_envelope_attack_value_changed(unsigned short envelope);
		void on_envelope_decay_value_changed(unsigned short envelope);
		void on_envelope_sustain_value_changed(unsigned short envelope);
		void on_envelope_release_value_changed(unsigned short envelope);
		void on_envelope_key_follow_button_clicked(unsigned short envelope);
		void on_chorus_button_clicked();

		void get_midi_port_names();
		void reset_controllers();

		mks70_tone* tone;
		RtMidiOut* midiout;
		unsigned int number_of_ports;
		unsigned int midi_port;
		unsigned short midi_channel;
		std::vector<std::string> midi_port_name;

		Glib::RefPtr<Gtk::FileFilter> filter;
		Glib::RefPtr<Gtk::FileFilter> filter_any;
};

#endif
