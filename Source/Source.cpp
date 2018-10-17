#include<iostream>
#include <memory>
#include "Definitions.h"
#include "Engine.h"


using namespace std;
int main(int argc, char *args[])
{
	unique_ptr<Engine> engine = std::make_unique<Engine>();

	if (!engine->initialize("./Assets/Config/mainScreen.xml"))
	{
		cout << "Engine could not initialize!";
		exit(1);
	}

	while (engine->run()) {}

	if (engine) engine = nullptr;

	system("PAUSE");
	return 0;
}