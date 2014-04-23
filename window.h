#ifndef MKS70_WINDOW_H
#define MKS70_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/widget.h>

#include "tone.h"

class MKS70_Window : public Gtk::Window
{
	public:
		MKS70_Window();
		virtual ~MKS70_Window();

	protected:
		Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
		Glib::RefPtr<Gtk::UIManager> m_refUIManager;
		Gtk::Box m_Application_Box;
		Gtk::Box m_Editor_Box;
		Gtk::Frame m_DCO1_Frame;
		Gtk::Frame m_DCO2_Frame;

		void on_action_file_open() {};
		void on_action_file_new() {};
		void on_action_file_save() {};
		void on_action_file_send() {};
		void on_action_file_preferences() {};
		void inline on_action_file_quit() {hide();};
	
	private:
		MKS70_tone* tone;
};

#endif
