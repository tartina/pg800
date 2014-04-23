#include <new>
#include "window.h"

MKS70_Window::MKS70_Window()
	: m_Application_Box(Gtk::ORIENTATION_VERTICAL),
	m_Editor_Box(Gtk::ORIENTATION_HORIZONTAL)
{
	tone = new MKS70_tone();

	set_default_size(640, 400);
	set_title("Roland MKS-70 Super JX Tone Editor");
	set_border_width(6);

	add(m_Application_Box);

	m_refActionGroup = Gtk::ActionGroup::create();

	m_refActionGroup->add( Gtk::Action::create("MenuFile", "_File") );
	m_refActionGroup->add( Gtk::Action::create("New", "_New"),
		sigc::mem_fun(*this, &MKS70_Window::on_action_file_new) );
	m_refActionGroup->add( Gtk::Action::create("Open", "_Open"),
		sigc::mem_fun(*this, &MKS70_Window::on_action_file_open) );
	m_refActionGroup->add( Gtk::Action::create("Saveas", "_Save as"),
		sigc::mem_fun(*this, &MKS70_Window::on_action_file_save) );
	m_refActionGroup->add( Gtk::Action::create("Send", "S_end patch"),
		sigc::mem_fun(*this, &MKS70_Window::on_action_file_send) );
	m_refActionGroup->add( Gtk::Action::create("Preferences", "_Preferences"),
		sigc::mem_fun(*this, &MKS70_Window::on_action_file_preferences) );
	m_refActionGroup->add( Gtk::Action::create("Quit", "_Quit"),
		sigc::mem_fun(*this, &MKS70_Window::on_action_file_quit) );

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

	m_DCO1_Frame.set_label("DCO 1");
	m_DCO1_Frame.set_size_request(40, 40);
	m_DCO1_Frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);

	m_DCO2_Frame.set_label("DCO 2");
	m_DCO2_Frame.set_size_request(40, 40);
	m_DCO2_Frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);

	m_Editor_Box.pack_start(m_DCO1_Frame);
	m_Editor_Box.pack_start(m_DCO2_Frame);

	show_all_children();
}

MKS70_Window::~MKS70_Window() {delete tone;}
