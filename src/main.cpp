//TODO include licenses and copyright

#include <stdexcept>
#include <iostream>

#include "application.h"
int main()
{
    try {
		Application app;
		app.run();
	} catch (std::exception& e)	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
        return -1;
	}

    return 0;
}
 
