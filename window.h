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
		static const unsigned int range_height = 120;

		Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
		Glib::RefPtr<Gtk::UIManager> m_refUIManager;

		Gtk::Box m_Application_Box;
		Gtk::Box m_Editor_Box;
		Gtk::Frame m_DCO_Frame[2];
		Gtk::Grid dco_grid[2];

		Gtk::Label m_Range_Label[2];
		Gtk::RadioButton m_rb_dco_range16[2], m_rb_dco_range8[2],
			m_rb_dco_range4[2], m_rb_dco_range2[2];
		Gtk::Label m_Wave_Label[2];
		Gtk::RadioButton m_rb_dco_wave_noise[2], m_rb_dco_wave_saw[2],
			m_rb_dco_wave_pulse[2], m_rb_dco_wave_square[2];
		Gtk::Separator m_separator[2];

		Gtk::Separator sep_crossmod;
		Gtk::Label crossmod_label;
		Gtk::RadioButton rb_crossmod[4];

		Gtk::Label dco_tune_label[2];
		Gtk::Scale* sc_dco_tune[2];
		Glib::RefPtr<Gtk::Adjustment> adj_dco_tune[2];

		Gtk::Label dco2_ftune_label;
		Gtk::Scale sc_dco2_ftune;
		Glib::RefPtr<Gtk::Adjustment> adj_dco2_ftune;

		// Signal handlers
		void on_action_file_open() {};
		void on_action_file_new() {};
		void on_action_file_save() {};
		void on_action_file_send() {};
		void on_action_file_preferences();
		void inline on_action_file_quit() {hide();};
		void on_dco_range_button_clicked();
		void on_dco_wave_button_clicked();
		void on_dco2_crossmod_button_clicked();
		void on_dco_tune_value_changed();
		void on_dco2_ftune_value_changed();

		void get_midi_port_names();

		mks70_tone* tone = 0;
		RtMidiOut* midiout = 0;
		unsigned int number_of_ports = 0;
		unsigned int midi_port = 0;
		unsigned short midi_channel = 0;
		std::vector<std::string> midi_port_name;
};

#endif
