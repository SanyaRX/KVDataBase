#include "DBServer.h"


int main()
{
	DBServer server(8000, "../../dbtest");

	server.startServer();
	server.closeServer();

	return 0;
}
