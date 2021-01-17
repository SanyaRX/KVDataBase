#include "DBServer.h"


int main()
{
	DBServer server(8000);

	server.startServer();
	server.closeServer();

	return 0;
}
