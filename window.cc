#include <new>
#include <string>
#include <iostream>
#include "window.h"
#include "preferences.h"

mks70_window::mks70_window()
	: m_Application_Box(Gtk::ORIENTATION_VERTICAL),
	m_Editor_Box(Gtk::ORIENTATION_HORIZONTAL)
{
	unsigned short i;
	Gtk::RadioButton::Group group;

	tone = new mks70_tone();

	try {
		midiout = new RtMidiOut("pg800");
	}
	catch ( const RtError &error ) {
		// TODO handle error
#ifdef HAVE_DEBUG
		error.printMessage();
		throw error;
#endif
	}

	get_midi_port_names();
	if (midi_port < number_of_ports) midiout->openPort(midi_port);

	set_default_size(640, 400);
	set_title("Roland MKS-70 Super JX Tone Editor");
	set_border_width(6);

	add(m_Application_Box);

	m_refActionGroup = Gtk::ActionGroup::create();

	m_refActionGroup->add( Gtk::Action::create("MenuFile", "_File") );
	m_refActionGroup->add( Gtk::Action::create("New", "_New"),
		sigc::mem_fun(*this, &mks70_window::on_action_file_new) );
	m_refActionGroup->add( Gtk::Action::create("Open", "_Open"),
		sigc::mem_fun(*this, &mks70_window::on_action_file_open) );
	m_refActionGroup->add( Gtk::Action::create("Saveas", "_Save as"),
		sigc::mem_fun(*this, &mks70_window::on_action_file_save) );
	m_refActionGroup->add( Gtk::Action::create("Send", "S_end patch"),
		sigc::mem_fun(*this, &mks70_window::on_action_file_send) );
	m_refActionGroup->add( Gtk::Action::create("Preferences", "_Preferences"),
		sigc::mem_fun(*this, &mks70_window::on_action_file_preferences) );
	m_refActionGroup->add( Gtk::Action::create("Quit", "_Quit"),
		sigc::mem_fun(*this, &mks70_window::on_action_file_quit) );

	m_refUIManager = Gtk::UIManager::create();
	m_refUIManager->insert_action_group(m_refActionGroup);
	add_accel_group(m_refUIManager->get_accel_group());

	Glib::ustring ui_info =
		"<ui>"
		"  <menubar name='MenuBar'>"
		"    <menu action='MenuFile'>"
		"      <menuitem action='New'/>"
		"      <menuitem action='Open'/>"
		"      <menuitem action='Saveas'/>"
		"      <menuitem action='Send'/>"
		"      <separator/>"
		"      <menuitem action='Preferences'/>"
		"      <menuitem action='Quit'/>"
		"    </menu>"
		"  </menubar>"
		"  <toolbar  name='ToolBar'>"
		"    <toolitem action='Open'/>"
		"    <toolitem action='Quit'/>"
		"  </toolbar>"
		"</ui>";

	m_refUIManager->add_ui_from_string(ui_info);

	Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
	m_Application_Box.pack_start(*pMenubar, Gtk::PACK_SHRINK);

	m_Application_Box.pack_start(m_Editor_Box);

	// DCO range and waveform radio buttons
	for (i = 0; i < 2; i++) {
		m_DCO_Frame[i].set_label("DCO" + std::to_string(i + 1));
		m_DCO_Frame[i].set_size_request(40, 40);
		m_DCO_Frame[i].set_shadow_type(Gtk::SHADOW_ETCHED_OUT);

		group = m_rb_dco_range16[i].get_group();
		m_rb_dco_range8[i].set_group(group);
		m_rb_dco_range4[i].set_group(group);
		m_rb_dco_range2[i].set_group(group);
		m_rb_dco_range8[i].set_active();
		m_rb_dco_range16[i].set_label("16'");
		m_rb_dco_range8[i].set_label("8'");
		m_rb_dco_range4[i].set_label("4'");
		m_rb_dco_range2[i].set_label("2'");
		m_rb_dco_range16[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco_range_button_clicked));
		m_rb_dco_range8[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco_range_button_clicked));
		m_rb_dco_range4[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco_range_button_clicked));
		m_rb_dco_range2[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco_range_button_clicked));

		group = m_rb_dco_wave_noise[i].get_group();
		m_rb_dco_wave_saw[i].set_group(group);
		m_rb_dco_wave_pulse[i].set_group(group);
		m_rb_dco_wave_square[i].set_group(group);
		m_rb_dco_wave_saw[i].set_active();
		m_rb_dco_wave_noise[i].set_label("Noise");
		m_rb_dco_wave_saw[i].set_label("Saw");
		m_rb_dco_wave_pulse[i].set_label("Pulse");
		m_rb_dco_wave_square[i].set_label("Square");
		m_rb_dco_wave_noise[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco_wave_button_clicked));
		m_rb_dco_wave_saw[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco_wave_button_clicked));
		m_rb_dco_wave_pulse[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco_wave_button_clicked));
		m_rb_dco_wave_square[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco_wave_button_clicked));

		m_DCO_Box[i].set_orientation(Gtk::ORIENTATION_VERTICAL);
		m_Range_Label[i].set_label("Range");
		m_DCO_Box[i].pack_start(m_Range_Label[i], Gtk::PACK_SHRINK);
		m_DCO_Box[i].pack_start(m_rb_dco_range16[i], Gtk::PACK_SHRINK);
		m_DCO_Box[i].pack_start(m_rb_dco_range8[i], Gtk::PACK_SHRINK);
		m_DCO_Box[i].pack_start(m_rb_dco_range4[i], Gtk::PACK_SHRINK);
		m_DCO_Box[i].pack_start(m_rb_dco_range2[i], Gtk::PACK_SHRINK);

		m_DCO_Box[i].pack_start(m_separator[i], Gtk::PACK_SHRINK);

		m_Wave_Label[i].set_label("Waveform");
		m_DCO_Box[i].pack_start(m_Wave_Label[i], Gtk::PACK_SHRINK);
		m_DCO_Box[i].pack_start(m_rb_dco_wave_noise[i], Gtk::PACK_SHRINK);
		m_DCO_Box[i].pack_start(m_rb_dco_wave_saw[i], Gtk::PACK_SHRINK);
		m_DCO_Box[i].pack_start(m_rb_dco_wave_pulse[i], Gtk::PACK_SHRINK);
		m_DCO_Box[i].pack_start(m_rb_dco_wave_square[i], Gtk::PACK_SHRINK);

		m_DCO_Frame[i].add(m_DCO_Box[i]);
		m_Editor_Box.pack_start(m_DCO_Frame[i], Gtk::PACK_SHRINK);
	}

	show_all_children();
}

mks70_window::~mks70_window() {delete tone; delete midiout;}

void mks70_window::on_dco_range_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 2; i++) {
		if (m_rb_dco_range16[i].get_active())
			tone->set_dco_range(i, 0);
		if (m_rb_dco_range8[i].get_active())
			tone->set_dco_range(i, 1);
		if (m_rb_dco_range4[i].get_active())
			tone->set_dco_range(i, 2);
		if (m_rb_dco_range2[i].get_active())
			tone->set_dco_range(i, 3);
	}
}

void mks70_window::on_dco_wave_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 2; i++) {
		if (m_rb_dco_wave_noise[i].get_active())
			tone->set_dco_wave(i, 0);
		if (m_rb_dco_wave_saw[i].get_active())
			tone->set_dco_wave(i, 1);
		if (m_rb_dco_wave_pulse[i].get_active())
			tone->set_dco_wave(i, 2);
		if (m_rb_dco_wave_square[i].get_active())
			tone->set_dco_wave(i, 3);
	}
}

void mks70_window::on_action_file_preferences()
{
	int result;

	preferences* pref = new preferences(midi_port_name);
	result = pref->run();
	delete pref;
}

void mks70_window::get_midi_port_names()
{
	unsigned i;

	number_of_ports = midiout->getPortCount();
#ifdef HAVE_DEBUG
	std::cout << "Number of MIDI ports: " << number_of_ports << "\n";
#endif

	midi_port_name.clear();
	for (i = 0; i < number_of_ports; i++)
		midi_port_name.push_back (midiout->getPortName());

}
