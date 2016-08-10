#include "Application.hpp"
#include <iostream>

int main()
{
	int x;
	float rx = -0.3f;
	x = rx / 1;
	std::cout << x << "\trx=" << rx;

	Application app(800, 600);
	app.run();

	return 0;
}
