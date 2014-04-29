#include <string>
#include <iostream>
#include "window.h"
#include "preferences.h"

mks70_window::mks70_window()
	: m_Application_Box(Gtk::ORIENTATION_VERTICAL),
	m_Editor_Box(Gtk::ORIENTATION_HORIZONTAL),
	crossmod_label("Crossmod")
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
#endif
		throw error;
	}

	get_midi_port_names();
	if (midi_port < number_of_ports) midiout->openPort(midi_port);

	set_default_size(640, 400);
	set_title(mks70_tone::init_tone_name + " - " + window_title);
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
		dco_grid[i].set_border_width(4);

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

		m_Range_Label[i].set_label("Range");
		dco_grid[i].attach(m_Range_Label[i], 0, 0, 1, 1);
		dco_grid[i].attach(m_rb_dco_range16[i], 0, 1, 1, 1);
		dco_grid[i].attach(m_rb_dco_range8[i], 0, 2, 1, 1);
		dco_grid[i].attach(m_rb_dco_range4[i], 0, 3, 1, 1);
		dco_grid[i].attach(m_rb_dco_range2[i], 0, 4, 1, 1);

		m_Wave_Label[i].set_label("Waveform");
		dco_grid[i].attach(m_Wave_Label[i], 1, 0, 1 ,1);
		dco_grid[i].attach(m_rb_dco_wave_noise[i], 1, 1, 1,1);
		dco_grid[i].attach(m_rb_dco_wave_saw[i], 1, 2, 1 ,1);
		dco_grid[i].attach(m_rb_dco_wave_pulse[i], 1, 3, 1 ,1);
		dco_grid[i].attach(m_rb_dco_wave_square[i], 1, 4, 1 ,1);

		m_DCO_Frame[i].add(dco_grid[i]);
		m_Editor_Box.pack_start(m_DCO_Frame[i], Gtk::PACK_SHRINK);
	}

	// DCO2 Cross mod
	dco_grid[1].attach(crossmod_label, 2, 0, 1, 1);
	group = rb_crossmod[0].get_group();
	rb_crossmod[0].set_label("Off");
	rb_crossmod[1].set_label("Sync1");
	rb_crossmod[2].set_label("Sync2");
	rb_crossmod[3].set_label("Xmod");
	rb_crossmod[0].set_active();
	rb_crossmod[0].signal_clicked().connect(sigc::mem_fun(*this,
		&mks70_window::on_dco2_crossmod_button_clicked));
	dco_grid[1].attach(rb_crossmod[0], 2, 1, 1, 1);
	for (i = 1; i < 4; i++) {
		rb_crossmod[i].set_group(group);
		rb_crossmod[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco2_crossmod_button_clicked));
		dco_grid[1].attach(rb_crossmod[i], 2, i + 1, 1, 1);
	}

	// DCO tune
	for (i = 0; i < 2; i++) {
		dco_tune_label[i].set_label("Tune");
		adj_dco_tune[i] = Gtk::Adjustment::create(64.0, 0.0, 127.0, 1.0, 1.0, 0.0);
		sc_dco_tune[i] = new Gtk::Scale(adj_dco_tune[i], Gtk::ORIENTATION_VERTICAL);
		sc_dco_tune[i]->set_digits(0);
		sc_dco_tune[i]->set_value_pos(Gtk::POS_BOTTOM);
		sc_dco_tune[i]->set_draw_value();
		sc_dco_tune[i]->set_inverted(); // highest value at top
		sc_dco_tune[i]->set_size_request(-1, 100);

		adj_dco_tune[i]->signal_value_changed().connect(sigc::mem_fun(*this,
			&mks70_window::on_adj_dco_tune_value_changed));

		dco_grid[i].attach(dco_tune_label[i], 0, 5, 1, 1);
		dco_grid[i].attach(*sc_dco_tune[i], 0, 6, 1, 1);
	}

	show_all_children();
}

mks70_window::~mks70_window()
{
	unsigned short i;

	for (i = 0; i < 2; i++) {
		delete sc_dco_tune[i];
	}
	
	delete tone; delete midiout;
}

void mks70_window::on_dco_range_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 2; i++) {
		if (m_rb_dco_range16[i].get_active())
			tone->set_dco_range(i, 0, midi_channel, midiout, true);
		if (m_rb_dco_range8[i].get_active())
			tone->set_dco_range(i, 1, midi_channel, midiout, true);
		if (m_rb_dco_range4[i].get_active())
			tone->set_dco_range(i, 2, midi_channel, midiout, true);
		if (m_rb_dco_range2[i].get_active())
			tone->set_dco_range(i, 3, midi_channel, midiout, true);
	}
}

void mks70_window::on_dco_wave_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 2; i++) {
		if (m_rb_dco_wave_noise[i].get_active())
			tone->set_dco_wave(i, 0, midi_channel, midiout, true);
		if (m_rb_dco_wave_saw[i].get_active())
			tone->set_dco_wave(i, 1, midi_channel, midiout, true);
		if (m_rb_dco_wave_pulse[i].get_active())
			tone->set_dco_wave(i, 2, midi_channel, midiout, true);
		if (m_rb_dco_wave_square[i].get_active())
			tone->set_dco_wave(i, 3, midi_channel, midiout, true);
	}
}

void mks70_window::on_action_file_preferences()
{
	int result;
	unsigned int old_midi_port = midi_port;

	preferences* pref = new preferences(midi_port_name, midi_port, midi_channel);
	result = pref->run();
	if (result == Gtk::ResponseType::RESPONSE_OK) {
		midi_port = pref->get_midi_port_number ();
		midi_channel = pref->get_midi_channel();
	}
	delete pref;

	if (midi_port != old_midi_port) {
		midiout->closePort();
		if (midi_port < number_of_ports) midiout->openPort(midi_port);
	}
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
		midi_port_name.push_back (midiout->getPortName(i));

}

void mks70_window::on_dco2_crossmod_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 4; i++)
		if (rb_crossmod[i].get_active()) {
			tone->set_dco2_crossmod(i, midi_channel, midiout, true);
			break;
		}
}

void mks70_window::on_adj_dco_tune_value_changed()
{
	unsigned short i;

	for (i = 0; i < 2; i++)
		tone->set_dco_tune(i, adj_dco_tune[i]->get_value(), midi_channel, midiout, true);
}

const std::string mks70_window::window_title = "Roland MKS-70 Super JX Tone Editor";