#include <gtkmm.h>
#include "window.h"

int main(int argc, char *argv[])
{
	Glib::RefPtr<Gtk::Application> app =
		Gtk::Application::create(argc, argv, "it.heavyware.applications.pg800");

	mks70_window window;

	return app->run(window);
}
