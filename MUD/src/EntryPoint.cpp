#include "../include/MUD_main.h"

int main(int argc, char **argv)
{
	OnInitialize(argc, argv);

	OnStart();

	OnUninitialize();
	return 0;
}