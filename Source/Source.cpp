#include<iostream>
#include <memory>
#include "Definitions.h"
#include "Engine.h"


using namespace std;
int main(int argc, char *args[])
{
	unique_ptr<Engine> engine = std::make_unique<Engine>();

	if (!engine->initialize())
	{
		cout << "Engine could not initialize!";
		exit(1);
	}

	while (engine->run()) {}

	engine = nullptr;

	system("PAUSE");
	return 0;
}