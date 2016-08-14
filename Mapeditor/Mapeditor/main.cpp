#include "Application.hpp"
#include "Configuration.hpp"

int main()
{
	Application app(1000, cfg::gui_height);
	app.run();

	return 0;
}
