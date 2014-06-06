/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * window.cc
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <iostream>
#include "window.h"
#include "preferences.h"
#include "about.h"

mks70_window::mks70_window()
	: m_Application_Box(Gtk::ORIENTATION_VERTICAL),
	m_Editor_Box(Gtk::ORIENTATION_HORIZONTAL),
	crossmod_label("Crossmod"),
	sc_dco2_ftune(Gtk::ORIENTATION_VERTICAL),
	dco_dyna_label("Dynamics"),
	dco_dyna_box(Gtk::ORIENTATION_VERTICAL),
	dco_mode_label("Env Mode"),
	dco_mode_box(Gtk::ORIENTATION_VERTICAL),
	mixer_frame("Mixer"),
	mixer_envelope_label("Envelope"),
	sc_mixer_envelope(Gtk::ORIENTATION_VERTICAL),
	mixer_dyna_label("Dynamics"),
	mixer_mode_label("Env Mode"),
	vcf_frame("VCF"),
	vcf_hpf_label("HPF"),
	vcf_cutoff_label("Cutoff"),
	sc_vcf_cutoff(Gtk::ORIENTATION_VERTICAL),
	vcf_resonance_label("Resonance"),
	sc_vcf_resonance(Gtk::ORIENTATION_VERTICAL),
	vcf_lfo_label("LFO"),
	sc_vcf_lfo(Gtk::ORIENTATION_VERTICAL),
	vcf_env_label("Envelope"),
	sc_vcf_env(Gtk::ORIENTATION_VERTICAL),
	vcf_key_label("Key"),
	sc_vcf_key(Gtk::ORIENTATION_VERTICAL),
	vcf_dyna_label("Dynamics"),
	vcf_env_mode_label("Env Mode"),
	vca_frame("VCA"),
	vca_box(Gtk::ORIENTATION_VERTICAL),
	vca_level_label("Level"),
	sc_vca_level(Gtk::ORIENTATION_VERTICAL),
	vca_env_mode_label("Env Mode"),
	vca_dyna_label("Dynamics"),
	vca_separator(Gtk::ORIENTATION_HORIZONTAL),
	editor_box2(Gtk::ORIENTATION_HORIZONTAL),
	lfo_frame("LFO"),
	lfo_waveform_label("Waveform"), lfo_delay_time_label("Delay Time"),
	sc_lfo_delay_time(Gtk::ORIENTATION_VERTICAL),
	lfo_rate_label("Rate"),
	sc_lfo_rate(Gtk::ORIENTATION_VERTICAL),
	chorus_frame("Chorus"), chorus_label("Mode"),
	chorus_box(Gtk::ORIENTATION_VERTICAL)
{
	unsigned short i, k;
	Gtk::RadioButton::Group group;
	Gtk::IconInfo icon_info;

	try {
		icon_theme = Gtk::IconTheme::get_default();
#ifdef HAVE_DEBUG
		std::vector<Glib::ustring> paths = icon_theme->get_search_path();
		for (std::vector<Glib::ustring>::const_iterator it = paths.begin();
		     it < paths.end(); ++it)
				std::cout << "Icon search path: " << *it << std::endl;
#endif
		icon_info = icon_theme->lookup_icon(PACKAGE_NAME, 48, Gtk::IconLookupFlags::ICON_LOOKUP_NO_SVG);
#ifdef HAVE_DEBUG
		if (icon_info) std::cout << "Icon path: " << icon_info.get_filename() << std::endl;
#endif
		if (icon_info) set_default_icon_from_file(icon_info.get_filename());
	}
	catch (const Gdk::PixbufError& pixbuf_error) {
		std::cerr << pixbuf_error.what() << std::endl;
	}
	catch (const Glib::FileError& file_error) {
		std::cerr << file_error.what() << std::endl;
	}

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
		sigc::mem_fun(*this, &mks70_window::on_action_file_save_as) );
	m_refActionGroup->add( Gtk::Action::create("Send", "S_end patch"),
		sigc::mem_fun(*this, &mks70_window::on_action_file_send) );
	m_refActionGroup->add( Gtk::Action::create("Preferences", "_Preferences"),
		sigc::mem_fun(*this, &mks70_window::on_action_file_preferences) );
	m_refActionGroup->add( Gtk::Action::create("Quit", "_Quit"),
		sigc::mem_fun(*this, &mks70_window::on_action_file_quit) );
	m_refActionGroup->add( Gtk::Action::create("MenuHelp", "_Help") );
	m_refActionGroup->add( Gtk::Action::create("About", "_About"),
		sigc::mem_fun(*this, &mks70_window::on_action_help_about) );

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
		"    <menu action='MenuHelp'>"
		"      <menuitem action='About'/>"
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
	m_Application_Box.pack_start(m_Editor_Box, Gtk::PACK_SHRINK);
	m_Application_Box.pack_start(editor_box2, Gtk::PACK_SHRINK);
	status_bar.push("Tone A");
	m_Application_Box.pack_end(status_bar, Gtk::PACK_SHRINK);

	// DCO range and waveform radio buttons
	for (i = 0; i < 2; i++) {
		dco_grid[i].set_border_width(2);
		dco_grid[i].set_column_spacing(1);

		m_DCO_Frame[i].set_label("DCO " + std::to_string(i + 1));
		m_DCO_Frame[i].set_border_width(1);
		m_DCO_Frame[i].set_shadow_type(Gtk::SHADOW_ETCHED_OUT);

		m_Range_Label[i].set_label("Range");
		dco_grid[i].attach(m_Range_Label[i], 0, 0, 1, 1);

		group = rb_dco_range[0][i].get_group();
		for (k = 1; k < 4; k++) rb_dco_range[k][i].set_group(group);
		rb_dco_range[0][i].set_label("16");
		rb_dco_range[1][i].set_label("8");
		rb_dco_range[2][i].set_label("4");
		rb_dco_range[3][i].set_label("2");
		rb_dco_range[1][i].set_active();
		for (k = 0; k < 4; k++) {
			rb_dco_range[k][i].signal_clicked().connect(sigc::bind<unsigned short>(sigc::mem_fun(*this,
				&mks70_window::on_dco_range_button_clicked), i));
			dco_grid[i].attach(rb_dco_range[k][i], 0, 1 + k, 1, 1);
		}

		m_Wave_Label[i].set_label("Waveform");
		dco_grid[i].attach(m_Wave_Label[i], 1, 0, 1 ,1);

		group = rb_dco_waveform[0][i].get_group();
		for (k = 1; k < 4; k++) rb_dco_waveform[k][i].set_group(group);
		rb_dco_waveform[0][i].set_label("Noise");
		rb_dco_waveform[1][i].set_label("Square");
		rb_dco_waveform[2][i].set_label("Pulse");
		rb_dco_waveform[3][i].set_label("Saw");
		rb_dco_waveform[3][i].set_active();
		for (k = 0; k < 4; k++) {
			rb_dco_waveform[k][i].signal_clicked().connect(sigc::bind<unsigned short>(sigc::mem_fun(*this,
				&mks70_window::on_dco_waveform_button_clicked), i));
			dco_grid[i].attach(rb_dco_waveform[k][i], 1, 1 + k, 1, 1);
		}

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
	rb_crossmod[0].signal_clicked().connect(sigc::mem_fun(*this,
		&mks70_window::on_dco2_crossmod_button_clicked));
	dco_grid[1].attach(rb_crossmod[0], 2, 1, 1, 1);
	for (i = 1; i < 4; i++) {
		rb_crossmod[i].set_group(group);
		rb_crossmod[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco2_crossmod_button_clicked));
		dco_grid[1].attach(rb_crossmod[i], 2, i + 1, 1, 1);
	}
	rb_crossmod[0].set_active();

	// DCO tune
	for (i = 0; i < 2; i++) {
		dco_tune_label[i].set_label("Tune");
		adj_dco_tune[i] = Gtk::Adjustment::create(64.0, 0.0, 127.0, 1.0, 10.0, 0.0);
		sc_dco_tune[i] = new Gtk::Scale(adj_dco_tune[i], Gtk::ORIENTATION_VERTICAL);
		sc_dco_tune[i]->set_digits(0);
		sc_dco_tune[i]->set_value_pos(Gtk::POS_BOTTOM);
		sc_dco_tune[i]->set_draw_value();
		sc_dco_tune[i]->set_inverted(); // highest value at top
		sc_dco_tune[i]->set_size_request(-1, range_height);
		sc_dco_tune[i]->signal_value_changed().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco_tune_value_changed));
		dco_grid[i].attach(dco_tune_label[i], 0, 5, 1, 1);
		dco_grid[i].attach(*sc_dco_tune[i], 0, 6, 1, 1);
	}

	// DCO2 fine tune
	dco2_ftune_label.set_label("Fine tune");
	adj_dco2_ftune = Gtk::Adjustment::create(64.0, 0.0, 127.0, 1.0, 10.0, 0.0);
	sc_dco2_ftune.set_adjustment(adj_dco2_ftune);
	sc_dco2_ftune.set_digits(0);
	sc_dco2_ftune.set_value_pos(Gtk::POS_BOTTOM);
	sc_dco2_ftune.set_draw_value();
	sc_dco2_ftune.set_inverted(); // highest value at top
	sc_dco2_ftune.set_size_request(-1, range_height);
	sc_dco2_ftune.signal_value_changed().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco2_ftune_value_changed));
	dco_grid[1].attach(dco2_ftune_label, 1, 5, 1, 1);
	dco_grid[1].attach(sc_dco2_ftune, 1, 6, 1, 1);

	// DCO lfo and envelope
	for (i = 0; i < 2; i++) {
		dco_lfo_label[i].set_label("LFO");
		adj_dco_lfo[i] = Gtk::Adjustment::create(0.0, 0.0, 127.0, 1.0, 10.0, 0.0);
		sc_dco_lfo[i] = new Gtk::Scale(adj_dco_lfo[i], Gtk::ORIENTATION_VERTICAL);
		sc_dco_lfo[i]->set_digits(0);
		sc_dco_lfo[i]->set_value_pos(Gtk::POS_BOTTOM);
		sc_dco_lfo[i]->set_draw_value();
		sc_dco_lfo[i]->set_inverted(); // highest value at top
		sc_dco_lfo[i]->set_size_request(-1, range_height);
		sc_dco_lfo[i]->signal_value_changed().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco_lfo_value_changed));
		dco_grid[i].attach(dco_lfo_label[i], 0, 7, 1, 1);
		dco_grid[i].attach(*sc_dco_lfo[i], 0, 8, 1, 1);

		dco_envelope_label[i].set_label("Envelope");
		adj_dco_envelope[i] = Gtk::Adjustment::create(0.0, 0.0, 127.0, 1.0, 10.0, 0.0);
		sc_dco_envelope[i] = new Gtk::Scale(adj_dco_envelope[i], Gtk::ORIENTATION_VERTICAL);
		sc_dco_envelope[i]->set_digits(0);
		sc_dco_envelope[i]->set_value_pos(Gtk::POS_BOTTOM);
		sc_dco_envelope[i]->set_draw_value();
		sc_dco_envelope[i]->set_inverted(); // highest value at top
		sc_dco_envelope[i]->set_size_request(-1, range_height);
		sc_dco_envelope[i]->signal_value_changed().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco_envelope_value_changed));
		dco_grid[i].attach(dco_envelope_label[i], 1, 7, 1, 1);
		dco_grid[i].attach(*sc_dco_envelope[i], 1, 8, 1, 1);
	}

	// DCO Dyna
	dco_grid[1].attach(dco_dyna_label, 2, 5, 1, 1);
	dco_grid[1].attach(dco_dyna_box, 2, 6, 1, 1);
	group = rb_dco_dyna[0].get_group();
	for (i = 1; i < 4; i++) rb_dco_dyna[i].set_group(group);
	rb_dco_dyna[0].set_label("Off");
	rb_dco_dyna[1].set_label("1");
	rb_dco_dyna[2].set_label("2");
	rb_dco_dyna[3].set_label("3");
	rb_dco_dyna[0].set_active();
	for (i = 0; i < 4; i++) {
		rb_dco_dyna[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco_dyna_button_clicked));
		dco_dyna_box.pack_start(rb_dco_dyna[i], Gtk::PACK_SHRINK);
	}

	// DCO Mode
	dco_grid[1].attach(dco_mode_label, 2, 7, 1, 1);
	group = rb_dco_mode[0].get_group();
	rb_dco_mode[0].set_label("\\/2");
	rb_dco_mode[1].set_label("/\\2");
	rb_dco_mode[2].set_label("\\/1");
	rb_dco_mode[3].set_label("/\\1");
	rb_dco_mode[0].signal_clicked().connect(sigc::mem_fun(*this,
		&mks70_window::on_dco_mode_button_clicked));
	dco_grid[1].attach(dco_mode_box, 2, 8, 1, 1);
	dco_mode_box.pack_start(rb_dco_mode[0], Gtk::PACK_SHRINK);
	for (i = 1; i < 4; i++) {
		rb_dco_mode[i].set_group(group);
		rb_dco_mode[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_dco_mode_button_clicked));
		dco_mode_box.pack_start(rb_dco_mode[i], Gtk::PACK_SHRINK);
	}
	rb_dco_mode[3].set_active(true);

	// Mixer frame
	mixer_frame.set_border_width(1);
	mixer_frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	m_Editor_Box.pack_start(mixer_frame, Gtk::PACK_SHRINK);
	mixer_grid.set_border_width(2);
	mixer_grid.set_column_spacing(1);
	mixer_frame.add(mixer_grid);

	for (i = 0; i < 2; i++) {
		mixer_dco_label[i].set_label("DCO " + std::to_string(i + 1));
		adj_mixer_dco[i] = Gtk::Adjustment::create(0.0, 0.0, 127.0, 1.0, 10.0, 0.0);
		sc_mixer_dco[i] = new Gtk::Scale(adj_mixer_dco[i], Gtk::ORIENTATION_VERTICAL);
		sc_mixer_dco[i]->set_digits(0);
		sc_mixer_dco[i]->set_value_pos(Gtk::POS_BOTTOM);
		sc_mixer_dco[i]->set_draw_value();
		sc_mixer_dco[i]->set_inverted(); // highest value at top
		sc_mixer_dco[i]->set_size_request(-1, range_height);
		sc_mixer_dco[i]->signal_value_changed().connect(sigc::mem_fun(*this,
			&mks70_window::on_mixer_dco_value_changed));
		mixer_grid.attach(mixer_dco_label[i], i, 0, 1, 1);
		mixer_grid.attach(*sc_mixer_dco[i], i, 1, 1, 1);
	}

	adj_mixer_envelope = Gtk::Adjustment::create(0.0, 0.0, 127.0, 1.0, 10.0, 0.0);
	sc_mixer_envelope.set_adjustment(adj_mixer_envelope);
	sc_mixer_envelope.set_digits(0);
	sc_mixer_envelope.set_value_pos(Gtk::POS_BOTTOM);
	sc_mixer_envelope.set_draw_value();
	sc_mixer_envelope.set_inverted(); // highest value at top
	sc_mixer_envelope.set_size_request(-1, range_height);
	sc_mixer_envelope.signal_value_changed().connect(sigc::mem_fun(*this,
			&mks70_window::on_mixer_envelope_value_changed));
	mixer_grid.attach(mixer_envelope_label, 0, 2, 1, 1);
	mixer_grid.attach(sc_mixer_envelope, 0, 3, 1, 1);

	// Mixer Dyna
	mixer_grid.attach(mixer_dyna_label, 0, 4, 1, 1);
	group = rb_mixer_dyna[0].get_group();
	for (i = 1; i < 4; i++) rb_mixer_dyna[i].set_group(group);
	rb_mixer_dyna[0].set_label("Off");
	rb_mixer_dyna[1].set_label("1");
	rb_mixer_dyna[2].set_label("2");
	rb_mixer_dyna[3].set_label("3");
	rb_mixer_dyna[0].set_active();
	for (i = 0; i < 4; i++) {
		rb_mixer_dyna[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_mixer_dyna_button_clicked));
		mixer_grid.attach(rb_mixer_dyna[i], 0, 5 + i, 1, 1);
	}

	// Mixer Mode
	mixer_grid.attach(mixer_mode_label, 1, 4, 1, 1);
	group = rb_mixer_mode[0].get_group();
	for (i = 1; i < 4; i++) rb_mixer_mode[i].set_group(group);
	rb_mixer_mode[0].set_label("\\/2");
	rb_mixer_mode[1].set_label("/\\2");
	rb_mixer_mode[2].set_label("\\/1");
	rb_mixer_mode[3].set_label("/\\1");
	rb_mixer_mode[3].set_active();
	for (i = 0; i < 4; i++) {
		rb_mixer_mode[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_mixer_mode_button_clicked));
		mixer_grid.attach(rb_mixer_mode[i], 1, 5 + i, 1, 1);
	}

	// VCF frame
	vcf_frame.set_border_width(1);
	vcf_frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	m_Editor_Box.pack_start(vcf_frame, Gtk::PACK_SHRINK);
	vcf_grid.set_border_width(2);
	vcf_grid.set_column_spacing(1);
	vcf_frame.add(vcf_grid);

	// VCF HPF
	vcf_grid.attach(vcf_hpf_label, 0, 0, 1, 1);
	group = rb_vcf_hpf[0].get_group();
	for (i = 1; i < 4; i++) rb_vcf_hpf[i].set_group(group);
	rb_vcf_hpf[0].set_active();
	for (i = 0; i < 4; i++) {
		rb_vcf_hpf[i].set_label(std::to_string(i));
		rb_vcf_hpf[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_vcf_hpf_button_clicked));
		vcf_grid.attach(rb_vcf_hpf[i], 0, 1 + i, 1, 1);
	}

	// VCF Cutoff Freq
	vcf_grid.attach(vcf_cutoff_label, 1, 0, 1, 1);
	adj_vcf_cutoff = Gtk::Adjustment::create(127.0, 0.0, 127.0, 1.0, 10.0, 0.0);
	sc_vcf_cutoff.set_adjustment(adj_vcf_cutoff);
	sc_vcf_cutoff.set_digits(0);
	sc_vcf_cutoff.set_value_pos(Gtk::POS_BOTTOM);
	sc_vcf_cutoff.set_draw_value();
	sc_vcf_cutoff.set_inverted(); // highest value at top
	sc_vcf_cutoff.set_size_request(-1, range_height);
	sc_vcf_cutoff.signal_value_changed().connect(sigc::mem_fun(*this,
		&mks70_window::on_vcf_cutoff_value_changed));
	vcf_grid.attach(sc_vcf_cutoff, 1, 1, 1, 4);

	// VCF Resonance
	vcf_grid.attach(vcf_resonance_label, 2, 0, 1, 1);
	adj_vcf_resonance = Gtk::Adjustment::create(0.0, 0.0, 127.0, 1.0, 10.0, 0.0);
	sc_vcf_resonance.set_adjustment(adj_vcf_resonance);
	sc_vcf_resonance.set_digits(0);
	sc_vcf_resonance.set_value_pos(Gtk::POS_BOTTOM);
	sc_vcf_resonance.set_draw_value();
	sc_vcf_resonance.set_inverted(); // highest value at top
	sc_vcf_resonance.set_size_request(-1, range_height);
	sc_vcf_resonance.signal_value_changed().connect(sigc::mem_fun(*this,
		&mks70_window::on_vcf_resonance_value_changed));
	vcf_grid.attach(sc_vcf_resonance, 2, 1, 1, 4);

	// VCF LFO
	vcf_grid.attach(vcf_lfo_label, 0, 5, 1, 1);
	adj_vcf_lfo = Gtk::Adjustment::create(0.0, 0.0, 127.0, 1.0, 10.0, 0.0);
	sc_vcf_lfo.set_adjustment(adj_vcf_lfo);
	sc_vcf_lfo.set_digits(0);
	sc_vcf_lfo.set_value_pos(Gtk::POS_BOTTOM);
	sc_vcf_lfo.set_draw_value();
	sc_vcf_lfo.set_inverted(); // highest value at top
	sc_vcf_lfo.set_size_request(-1, range_height);
	sc_vcf_lfo.signal_value_changed().connect(sigc::mem_fun(*this,
		&mks70_window::on_vcf_lfo_value_changed));
	vcf_grid.attach(sc_vcf_lfo, 0, 6, 1, 1);

	// VCF Envelope
	vcf_grid.attach(vcf_env_label, 1, 5, 1, 1);
	adj_vcf_env = Gtk::Adjustment::create(0.0, 0.0, 127.0, 1.0, 10.0, 0.0);
	sc_vcf_env.set_adjustment(adj_vcf_env);
	sc_vcf_env.set_digits(0);
	sc_vcf_env.set_value_pos(Gtk::POS_BOTTOM);
	sc_vcf_env.set_draw_value();
	sc_vcf_env.set_inverted(); // highest value at top
	sc_vcf_env.set_size_request(-1, range_height);
	sc_vcf_env.signal_value_changed().connect(sigc::mem_fun(*this,
		&mks70_window::on_vcf_env_value_changed));
	vcf_grid.attach(sc_vcf_env, 1, 6, 1, 1);

	// VCF Key Follow
	vcf_grid.attach(vcf_key_label, 2, 5, 1, 1);
	adj_vcf_key = Gtk::Adjustment::create(0.0, 0.0, 127.0, 1.0, 10.0, 0.0);
	sc_vcf_key.set_adjustment(adj_vcf_key);
	sc_vcf_key.set_digits(0);
	sc_vcf_key.set_value_pos(Gtk::POS_BOTTOM);
	sc_vcf_key.set_draw_value();
	sc_vcf_key.set_inverted(); // highest value at top
	sc_vcf_key.set_size_request(-1, range_height);
	sc_vcf_key.signal_value_changed().connect(sigc::mem_fun(*this,
		&mks70_window::on_vcf_key_value_changed));
	vcf_grid.attach(sc_vcf_key, 2, 6, 1, 1);

	// VCF Envelope Dynamics
	vcf_grid.attach(vcf_dyna_label, 0, 7, 1, 1);
	group = rb_vcf_dyna[0].get_group();
	for (i = 1; i < 4; i++) rb_vcf_dyna[i].set_group(group);
	rb_vcf_dyna[0].set_active();
	rb_vcf_dyna[0].set_label("Off");
	rb_vcf_dyna[1].set_label("1");
	rb_vcf_dyna[2].set_label("2");
	rb_vcf_dyna[3].set_label("3");
	for (i = 0; i < 4; i++) {
		rb_vcf_dyna[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_vcf_dyna_button_clicked));
		vcf_grid.attach(rb_vcf_dyna[i], 0, 8 + i, 1, 1);
	}

	// VCF Envelope Mode
	vcf_grid.attach(vcf_env_mode_label, 1, 7, 1, 1);
	group = rb_vcf_env_mode[0].get_group();
	for (i = 1; i < 4; i++) rb_vcf_env_mode[i].set_group(group);
	rb_vcf_env_mode[0].set_label("\\/2");
	rb_vcf_env_mode[1].set_label("/\\2");
	rb_vcf_env_mode[2].set_label("\\/1");
	rb_vcf_env_mode[3].set_label("/\\1");
	rb_vcf_env_mode[3].set_active();
	for (i = 0; i < 4; i++) {
		rb_vcf_env_mode[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_vcf_env_mode_button_clicked));
		vcf_grid.attach(rb_vcf_env_mode[i], 1, 8 + i, 1, 1);
	}
	
	// VCA frame
	vca_frame.set_border_width(1);
	vca_frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	m_Editor_Box.pack_start(vca_frame, Gtk::PACK_SHRINK);
	vca_box.set_border_width(2);
	vca_box.set_spacing(1);
	vca_frame.add(vca_box);

	// VCA Level
	vca_box.pack_start(vca_level_label, Gtk::PACK_SHRINK);
	adj_vca_level = Gtk::Adjustment::create(0.0, 0.0, 127.0, 1.0, 10.0, 0.0);
	sc_vca_level.set_adjustment(adj_vca_level);
	sc_vca_level.set_digits(0);
	sc_vca_level.set_value_pos(Gtk::POS_BOTTOM);
	sc_vca_level.set_draw_value();
	sc_vca_level.set_inverted(); // highest value at top
	sc_vca_level.set_size_request(-1, range_height);
	sc_vca_level.signal_value_changed().connect(sigc::mem_fun(*this,
		&mks70_window::on_vca_level_value_changed));
	vca_box.pack_start(sc_vca_level, Gtk::PACK_SHRINK);

	// VCA Env Mode
	vca_box.pack_start(vca_env_mode_label, Gtk::PACK_SHRINK);
	group = rb_vca_env_mode[0].get_group();
	rb_vca_env_mode[1].set_group(group);
	rb_vca_env_mode[0].set_label("Gate");
	rb_vca_env_mode[1].set_label("Env 2");
	rb_vca_env_mode[1].set_active();
	for (i = 0; i < 2; i++) {
		rb_vca_env_mode[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_vca_env_mode_button_clicked));
		vca_box.pack_start(rb_vca_env_mode[i], Gtk::PACK_SHRINK);
	}

	// VCA Dynamics
	vca_box.pack_start(vca_separator, Gtk::PACK_SHRINK);
	vca_box.pack_start(vca_dyna_label, Gtk::PACK_SHRINK);
	group = rb_vca_dyna[0].get_group();
	for (i = 1; i < 4; i++) rb_vca_dyna[i].set_group(group);
	rb_vca_dyna[0].set_active();
	rb_vca_dyna[0].set_label("Off");
	rb_vca_dyna[1].set_label("1");
	rb_vca_dyna[2].set_label("2");
	rb_vca_dyna[3].set_label("3");
	for (i = 0; i < 4; i++) {
		rb_vca_dyna[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_vca_dyna_button_clicked));
		vca_box.pack_start(rb_vca_dyna[i], Gtk::PACK_SHRINK);
	}

	// LFO Frame
	lfo_frame.set_border_width(1);
	lfo_frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	editor_box2.pack_start(lfo_frame, Gtk::PACK_SHRINK);
	lfo_grid.set_border_width(2);
	lfo_grid.set_column_spacing(1);
	lfo_frame.add(lfo_grid);

	// LFO Waveform
	lfo_grid.attach(lfo_waveform_label, 0, 0, 1, 1);
	group = rb_lfo_waveform[0].get_group();
	rb_lfo_waveform[1].set_group(group);
	rb_lfo_waveform[2].set_group(group);
	rb_lfo_waveform[0].set_label("Random");
	rb_lfo_waveform[1].set_label("Square");
	rb_lfo_waveform[2].set_label("Triangle");
	rb_lfo_waveform[2].set_active();
	for (i = 0; i < 3; i++) {
		rb_lfo_waveform[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_lfo_waveform_button_clicked));
		lfo_grid.attach(rb_lfo_waveform[i], 0, 1 + i, 1, 1);
	}

	// LFO Delay Time
	lfo_grid.attach(lfo_delay_time_label, 1, 0, 1, 1);
	adj_lfo_delay_time = Gtk::Adjustment::create(0.0, 0.0, 127.0, 1.0, 10.0, 0.0);
	sc_lfo_delay_time.set_adjustment(adj_lfo_delay_time);
	sc_lfo_delay_time.set_digits(0);
	sc_lfo_delay_time.set_value_pos(Gtk::POS_BOTTOM);
	sc_lfo_delay_time.set_draw_value();
	sc_lfo_delay_time.set_inverted(); // highest value at top
	sc_lfo_delay_time.set_size_request(-1, range_height);
	sc_lfo_delay_time.signal_value_changed().connect(sigc::mem_fun(*this,
		&mks70_window::on_lfo_delay_time_value_changed));
	lfo_grid.attach(sc_lfo_delay_time, 1, 1, 1, 3);

	// LFO Rate
	lfo_grid.attach(lfo_rate_label, 2, 0, 1, 1);
	adj_lfo_rate = Gtk::Adjustment::create(0.0, 0.0, 127.0, 1.0, 10.0, 0.0);
	sc_lfo_rate.set_adjustment(adj_lfo_rate);
	sc_lfo_rate.set_digits(0);
	sc_lfo_rate.set_value_pos(Gtk::POS_BOTTOM);
	sc_lfo_rate.set_draw_value();
	sc_lfo_rate.set_inverted(); // highest value at top
	sc_lfo_rate.set_size_request(-1, range_height);
	sc_lfo_rate.signal_value_changed().connect(sigc::mem_fun(*this,
		&mks70_window::on_lfo_rate_value_changed));
	lfo_grid.attach(sc_lfo_rate, 2, 1, 1, 3);

	// Envelope Frames
	for (i = 0; i < 2; i++) {
		envelope_grid[i].set_border_width(2);
		envelope_grid[i].set_column_spacing(1);
		envelope_frame[i].set_label("Envelope " + std::to_string(i + 1));
		envelope_frame[i].set_border_width(1);
		envelope_frame[i].set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
		envelope_frame[i].add(envelope_grid[i]);
		editor_box2.pack_start(envelope_frame[i], Gtk::PACK_SHRINK);

		envelope_attack_label[i].set_label("Attack");
		adj_envelope_attack[i] = Gtk::Adjustment::create(0.0, 0.0, 127.0, 1.0, 10.0, 0.0);
		sc_envelope_attack[i] = new Gtk::Scale(adj_envelope_attack[i], Gtk::ORIENTATION_VERTICAL);
		sc_envelope_attack[i]->set_digits(0);
		sc_envelope_attack[i]->set_value_pos(Gtk::POS_BOTTOM);
		sc_envelope_attack[i]->set_draw_value();
		sc_envelope_attack[i]->set_inverted(); // highest value at top
		sc_envelope_attack[i]->set_size_request(-1, range_height);
		sc_envelope_attack[i]->signal_value_changed().connect(sigc::mem_fun(*this,
			&mks70_window::on_envelope_attack_value_changed));
		envelope_grid[i].attach(envelope_attack_label[i], 0, 0, 1, 1);
		envelope_grid[i].attach(*sc_envelope_attack[i], 0, 1, 1, 4);

		envelope_decay_label[i].set_label("Decay");
		adj_envelope_decay[i] = Gtk::Adjustment::create(0.0, 0.0, 127.0, 1.0, 10.0, 0.0);
		sc_envelope_decay[i] = new Gtk::Scale(adj_envelope_decay[i], Gtk::ORIENTATION_VERTICAL);
		sc_envelope_decay[i]->set_digits(0);
		sc_envelope_decay[i]->set_value_pos(Gtk::POS_BOTTOM);
		sc_envelope_decay[i]->set_draw_value();
		sc_envelope_decay[i]->set_inverted(); // highest value at top
		sc_envelope_decay[i]->set_size_request(-1, range_height);
		sc_envelope_decay[i]->signal_value_changed().connect(sigc::mem_fun(*this,
			&mks70_window::on_envelope_decay_value_changed));
		envelope_grid[i].attach(envelope_decay_label[i], 1, 0, 1, 1);
		envelope_grid[i].attach(*sc_envelope_decay[i], 1, 1, 1, 4);

		envelope_sustain_label[i].set_label("Sustain");
		adj_envelope_sustain[i] = Gtk::Adjustment::create(127.0, 0.0, 127.0, 1.0, 10.0, 0.0);
		sc_envelope_sustain[i] = new Gtk::Scale(adj_envelope_sustain[i], Gtk::ORIENTATION_VERTICAL);
		sc_envelope_sustain[i]->set_digits(0);
		sc_envelope_sustain[i]->set_value_pos(Gtk::POS_BOTTOM);
		sc_envelope_sustain[i]->set_draw_value();
		sc_envelope_sustain[i]->set_inverted(); // highest value at top
		sc_envelope_sustain[i]->set_size_request(-1, range_height);
		sc_envelope_sustain[i]->signal_value_changed().connect(sigc::mem_fun(*this,
			&mks70_window::on_envelope_sustain_value_changed));
		envelope_grid[i].attach(envelope_sustain_label[i], 2, 0, 1, 1);
		envelope_grid[i].attach(*sc_envelope_sustain[i], 2, 1, 1, 4);
		
		envelope_release_label[i].set_label("Release");
		adj_envelope_release[i] = Gtk::Adjustment::create(0.0, 0.0, 127.0, 1.0, 10.0, 0.0);
		sc_envelope_release[i] = new Gtk::Scale(adj_envelope_release[i], Gtk::ORIENTATION_VERTICAL);
		sc_envelope_release[i]->set_digits(0);
		sc_envelope_release[i]->set_value_pos(Gtk::POS_BOTTOM);
		sc_envelope_release[i]->set_draw_value();
		sc_envelope_release[i]->set_inverted(); // highest value at top
		sc_envelope_release[i]->set_size_request(-1, range_height);
		sc_envelope_release[i]->signal_value_changed().connect(sigc::mem_fun(*this,
			&mks70_window::on_envelope_release_value_changed));
		envelope_grid[i].attach(envelope_release_label[i], 3, 0, 1, 1);
		envelope_grid[i].attach(*sc_envelope_release[i], 3, 1, 1, 4);

		envelope_key_follow_label[i].set_label("Key Follow");
		envelope_grid[i].attach(envelope_key_follow_label[i], 4, 0, 1, 1);
		group = rb_envelope_key_follow[0][i].get_group();
		for (k = 1; k < 4; k++) rb_envelope_key_follow[k][i].set_group(group);
		rb_envelope_key_follow[0][i].set_label("Off");
		rb_envelope_key_follow[1][i].set_label("1");
		rb_envelope_key_follow[2][i].set_label("2");
		rb_envelope_key_follow[3][i].set_label("3");
		rb_envelope_key_follow[0][i].set_active();
		for (k = 0; k < 4; k++) {
			rb_envelope_key_follow[k][i].signal_clicked().connect(sigc::mem_fun(*this,
				&mks70_window::on_envelope_key_follow_button_clicked));
			envelope_grid[i].attach(rb_envelope_key_follow[k][i], 4, 1 + k, 1, 1);
		}
	}

	// Chorus Frame
	chorus_frame.set_border_width(1);
	chorus_frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	chorus_box.set_border_width(2);
	chorus_frame.add(chorus_box);
	editor_box2.pack_start(chorus_frame, Gtk::PACK_SHRINK);
	chorus_box.pack_start(chorus_label, Gtk::PACK_SHRINK);
	group = rb_chorus[0].get_group();
	rb_chorus[1].set_group(group);
	rb_chorus[2].set_group(group);
	rb_chorus[0].set_label("Off");
	rb_chorus[1].set_label("1");
	rb_chorus[2].set_label("2");
	for (i = 0; i < 3; i++) {
		rb_chorus[i].signal_clicked().connect(sigc::mem_fun(*this,
			&mks70_window::on_chorus_button_clicked));
		chorus_box.pack_start(rb_chorus[i], Gtk::PACK_SHRINK);
	}

	filter = Gtk::FileFilter::create();
	filter->set_name("Roland MKS70 Patch");
	filter->add_pattern("*.mks70");
	filter_any = Gtk::FileFilter::create();
	filter_any->set_name("Any files");
	filter_any->add_pattern("*");

	show_all_children();
}

mks70_window::~mks70_window()
{
	unsigned short i;

	for (i = 0; i < 2; i++) {
		delete sc_envelope_release[i]; sc_envelope_release[i] = 0;
		delete sc_envelope_sustain[i]; sc_envelope_sustain[i] = 0;
		delete sc_envelope_decay[i]; sc_envelope_decay[i] = 0;
		delete sc_envelope_attack[i]; sc_envelope_attack[i] = 0;
		delete sc_mixer_dco[i]; sc_mixer_dco[i] = 0;
		delete sc_dco_envelope[i]; sc_dco_envelope[i] = 0;
		delete sc_dco_lfo[i]; sc_dco_lfo[i] = 0;
		delete sc_dco_tune[i]; sc_dco_tune[i] = 0;
	}
	delete tone; tone = 0;
	delete midiout; midiout = 0;
}

void mks70_window::on_dco_range_button_clicked(unsigned short dco)
{
	unsigned short i;

#ifdef HAVE_DEBUG
	std::cout << "on_dco_range_button_clicked CALLED!" << std::endl;
#endif

	for (i = 0; i < 4; i++)
		if (rb_dco_range[i][dco].get_active()) {
			tone->set_dco_range(dco, i, midi_channel, midiout, true);
			break;
		}
}

void mks70_window::on_dco_waveform_button_clicked(unsigned short dco)
{
	unsigned short i;

#ifdef HAVE_DEBUG
	std::cout << "on_dco_waveform_button_clicked CALLED!" << std::endl;
#endif

	for (i = 0; i < 4; i++)
		if (rb_dco_waveform[i][dco].get_active()) {
			tone->set_dco_wave(dco, i, midi_channel, midiout, true);
			break;
		}
}

void mks70_window::on_action_file_new()
{
	Gtk::MessageDialog* dialog;
	int result;

	dialog = new Gtk::MessageDialog("Reset all controls?", false,
	                               Gtk::MessageType::MESSAGE_QUESTION,
	                               Gtk::ButtonsType::BUTTONS_OK_CANCEL,
	                               true);
	result = dialog->run();
	if (result == Gtk::ResponseType::RESPONSE_OK) {
		delete tone; tone = 0;
		tone = new mks70_tone();
		reset_controllers();
	}
	delete dialog;
}

void mks70_window::on_action_file_open()
{
	Gtk::FileChooserDialog* dialog;
	Gtk::MessageDialog* message;
	int result;
	bool load_ok;
	std::string filename;

	dialog = new Gtk::FileChooserDialog("Open tone",
	                                    Gtk::FileChooserAction::FILE_CHOOSER_ACTION_OPEN);
	dialog->add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog->add_button("Open", Gtk::RESPONSE_OK);
	dialog->add_filter(filter);
	dialog->add_filter(filter_any);

	result = dialog->run();
	if (result == Gtk::ResponseType::RESPONSE_OK) filename = dialog->get_filename();
	delete dialog;

	if (result == Gtk::ResponseType::RESPONSE_OK) {
		load_ok = tone->load_from_file(filename);
		if (load_ok) reset_controllers();
		else {
			message = new Gtk::MessageDialog("Load failed!", false, Gtk::MessageType::MESSAGE_ERROR,
			                                 Gtk::ButtonsType::BUTTONS_OK, true);
			message->run();
			delete message;
		}
	}
}

void mks70_window::on_action_file_save_as()
{
	Gtk::FileChooserDialog* dialog;
	int result;

	dialog = new Gtk::FileChooserDialog("Save tone",
	                                    Gtk::FileChooserAction::FILE_CHOOSER_ACTION_SAVE);
	dialog->add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog->add_button("Save", Gtk::RESPONSE_OK);
	dialog->add_filter(filter);
	dialog->add_filter(filter_any);

	result = dialog->run();
	if (result == Gtk::ResponseType::RESPONSE_OK) {
		tone->save_to_file(dialog->get_filename());
	}

	delete dialog;
}

void mks70_window::on_action_file_send()
{
	Gtk::MessageDialog* dialog;
	int result;

	dialog = new Gtk::MessageDialog("Send this patch?", false,
	                               Gtk::MessageType::MESSAGE_QUESTION,
	                               Gtk::ButtonsType::BUTTONS_OK_CANCEL,
	                               true);
	result = dialog->run();
	if (result == Gtk::ResponseType::RESPONSE_OK)
		tone->apr_send(midi_channel, midiout);

	delete dialog;
}

void mks70_window::on_action_file_preferences()
{
	int result;
	unsigned int old_midi_port = midi_port;
	unsigned short tone_number;

	preferences* pref = new preferences(midi_port_name, midi_port, midi_channel,
	                                    tone->get_tone_number());
	result = pref->run();
	if (result == Gtk::ResponseType::RESPONSE_OK) {
		midi_port = pref->get_midi_port_number ();
		midi_channel = pref->get_midi_channel();
		tone_number = pref->get_tone_number();
		tone->set_tone_number(tone_number);
		status_bar.remove_all_messages();
		if (tone_number == 0) status_bar.push("Tone A");
		else status_bar.push("Tone B");
	}
	delete pref;

	if (midi_port != old_midi_port) {
		try {
			midiout->closePort();
			if (midi_port < number_of_ports) midiout->openPort(midi_port);
		}
		catch(const RtError& error) {
			// TODO: handle error
		}
	}
}

void mks70_window::on_action_help_about()
{
	about* dialog = new about();
	dialog->run();
	delete dialog;
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

void mks70_window::on_dco_tune_value_changed()
{
	unsigned short i;

	for (i = 0; i < 2; i++)
		tone->set_dco_tune(i, adj_dco_tune[i]->get_value(), midi_channel, midiout, true);
}

void mks70_window::on_dco2_ftune_value_changed()
{
	tone->set_dco2_ftune(adj_dco2_ftune->get_value(), midi_channel, midiout, true);
}

void mks70_window::on_dco_lfo_value_changed()
{
	unsigned short i;

	for (i = 0; i < 2; i++)
		tone->set_dco_lfo(i, adj_dco_lfo[i]->get_value(), midi_channel, midiout, true);
}

void mks70_window::on_dco_envelope_value_changed()
{
	unsigned short i;

	for (i = 0; i < 2; i++)
		tone->set_dco_envelope(i, adj_dco_envelope[i]->get_value(), midi_channel, midiout, true);
}

void mks70_window::on_dco_dyna_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 4; i++)
		if (rb_dco_dyna[i].get_active()) {
			tone->set_dco_dyna(i, midi_channel, midiout, true);
			break;
		}
}

void mks70_window::on_dco_mode_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 4; i++)
		if (rb_dco_mode[i].get_active()) {
			tone->set_dco_mode(i, midi_channel, midiout, true);
			break;
		}
}

void mks70_window::on_mixer_dco_value_changed()
{
	unsigned short i;

	for (i = 0; i < 2; i++)
		tone->set_mixer_dco(i, adj_mixer_dco[i]->get_value(), midi_channel, midiout, true);
}

void mks70_window::on_mixer_envelope_value_changed()
{
	tone->set_mixer_envelope(adj_mixer_envelope->get_value(), midi_channel, midiout, true);
}

void mks70_window::on_mixer_dyna_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 4; i++)
		if (rb_mixer_dyna[i].get_active()) {
			tone->set_mixer_dyna(i, midi_channel, midiout, true);
			break;
		}
}

void mks70_window::on_mixer_mode_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 4; i++)
		if (rb_mixer_mode[i].get_active()) {
			tone->set_mixer_mode(i, midi_channel, midiout, true);
			break;
		}
}

void mks70_window::on_vcf_hpf_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 4; i++)
		if (rb_vcf_hpf[i].get_active()) {
			tone->set_vcf_hpf(i, midi_channel, midiout, true);
			break;
		}
}

void mks70_window::on_vcf_cutoff_value_changed()
{
	tone->set_vcf_cutoff(adj_vcf_cutoff->get_value(), midi_channel, midiout, true);
}

void mks70_window::on_vcf_resonance_value_changed()
{
	tone->set_vcf_resonance(adj_vcf_resonance->get_value(), midi_channel, midiout, true);
}

void mks70_window::on_vcf_lfo_value_changed()
{
	tone->set_vcf_lfo(adj_vcf_lfo->get_value(), midi_channel, midiout, true);
}

void mks70_window::on_vcf_env_value_changed()
{
	tone->set_vcf_envelope(adj_vcf_env->get_value(), midi_channel, midiout, true);
}

void mks70_window::on_vcf_key_value_changed()
{
	tone->set_vcf_key(adj_vcf_key->get_value(), midi_channel, midiout, true);
}

void mks70_window::on_vcf_dyna_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 4; i++)
		if (rb_vcf_dyna[i].get_active()) {
			tone->set_vcf_dyna(i, midi_channel, midiout, true);
			break;
		}
}

void mks70_window::on_vcf_env_mode_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 4; i++)
		if (rb_vcf_env_mode[i].get_active()) {
			tone->set_vcf_env_mode(i, midi_channel, midiout, true);
			break;
		}
}

void mks70_window::on_vca_level_value_changed()
{
	tone->set_vca_level(adj_vca_level->get_value(), midi_channel, midiout, true);
}

void mks70_window::on_vca_env_mode_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 2; i++)
		if (rb_vca_env_mode[i].get_active()) {
			tone->set_vca_env_mode(i, midi_channel, midiout, true);
			break;
		}
}

void mks70_window::on_vca_dyna_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 4; i++)
		if (rb_vca_dyna[i].get_active()) {
			tone->set_vca_dyna(i, midi_channel, midiout, true);
			break;
		}
}

void mks70_window::on_lfo_waveform_button_clicked()
{
	unsigned short i;

	for (i = 0; i < 3; i++)
		if (rb_lfo_waveform[i].get_active()) {
			tone->set_lfo_waveform(i, midi_channel, midiout, true);
			break;
		}
}

void mks70_window::on_lfo_delay_time_value_changed()
{
	tone->set_lfo_delay_time(adj_lfo_delay_time->get_value(), midi_channel, midiout, true);
}

void mks70_window::on_lfo_rate_value_changed()
{
	tone->set_lfo_rate(adj_lfo_rate->get_value(), midi_channel, midiout, true);
}

void mks70_window::on_envelope_attack_value_changed()
{
	unsigned short i;

	for (i = 0; i < 2; i++)
		tone->set_envelope_attack(i, adj_envelope_attack[i]->get_value(),
		                          midi_channel, midiout, true);
}

void mks70_window::on_envelope_decay_value_changed()
{
	unsigned short i;

	for (i = 0; i < 2; i++)
		tone->set_envelope_decay(i, adj_envelope_decay[i]->get_value(),
		                          midi_channel, midiout, true);
}

void mks70_window::on_envelope_sustain_value_changed()
{
	unsigned short i;

	for (i = 0; i < 2; i++)
		tone->set_envelope_sustain(i, adj_envelope_sustain[i]->get_value(),
		                          midi_channel, midiout, true);
}

void mks70_window::on_envelope_release_value_changed()
{
	unsigned short i;

	for (i = 0; i < 2; i++)
		tone->set_envelope_release(i, adj_envelope_release[i]->get_value(),
		                          midi_channel, midiout, true);
}

void mks70_window::on_envelope_key_follow_button_clicked()
{
	unsigned short i, k;

	for (i = 0; i < 2; i++) for (k = 0; k < 4; k++) {
		if (rb_envelope_key_follow[k][i].get_active())
			tone->set_envelope_key_follow(i, k, midi_channel, midiout, true);
	}
}

void mks70_window::on_chorus_button_clicked()
{
	unsigned short i;
	for (i = 0; i < 3; i++)
		if (rb_chorus[i].get_active()) {
			tone->set_chorus(i, midi_channel, midiout, true);
			break;
		}
}

void mks70_window::reset_controllers()
{
	unsigned short i;

	for (i = 0; i < 2; i++) {
		rb_dco_range[tone->get_dco_range(i)][i].set_active();
		rb_dco_waveform[tone->get_dco_waveform(i)][i].set_active();
		adj_dco_tune[i]->set_value(64.0);
		adj_dco_lfo[i]->set_value(0.0);
		adj_dco_envelope[i]->set_value(0.0);
		adj_mixer_dco[i]->set_value(0.0);
		adj_envelope_attack[i]->set_value(0.0);
		adj_envelope_decay[i]->set_value(0.0);
		adj_envelope_sustain[i]->set_value(127.0);
		adj_envelope_release[i]->set_value(0.0);
		rb_envelope_key_follow[0][i].set_active();
	}

	rb_crossmod[tone->get_dco2_crossmod()].set_active();
	adj_dco2_ftune->set_value(tone->get_dco2_ftune());
	rb_dco_dyna[0].set_active();
	rb_dco_mode[3].set_active();
	adj_mixer_envelope->set_value(0.0);
	rb_mixer_dyna[0].set_active();
	rb_mixer_mode[3].set_active();
	rb_vcf_hpf[0].set_active();
	adj_vcf_cutoff->set_value(127.0);
	adj_vcf_resonance->set_value(0.0);
	adj_vcf_lfo->set_value(0.0);
	adj_vcf_env->set_value(0.0);
	adj_vcf_key->set_value(0.0);
	rb_vcf_dyna[0].set_active();
	rb_vcf_env_mode[3].set_active();
	adj_vca_level->set_value(0.0);
	rb_vca_env_mode[1].set_active();
	rb_vca_dyna[0].set_active();
	rb_lfo_waveform[2].set_active();
	adj_lfo_delay_time->set_value(0.0);
	adj_lfo_rate->set_value(0.0);
	rb_chorus[0].set_active();
}

const std::string mks70_window::window_title = "Roland MKS-70 Super JX Tone Editor";
