#include "Server.h"

int main(int argc, char *argv[])
{
	CServer Server;

	if (Server.Start())
	{
		Server.Run(argc, argv);
	}

	return 0;
}