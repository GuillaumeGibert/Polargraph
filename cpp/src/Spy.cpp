#include "Spy.h"

Spy::Spy()
{
}

Spy::~Spy()
{
}

void Spy::setSignalValues(float timestamp, std::vector<float> vInputData)
{
    std::cout << "Spy received: timestamp = "<<  timestamp << ", vInputData = (";
    for (std::vector<float>::iterator it = vInputData.begin() ; it != vInputData.end(); ++it)
        std::cout << *it << " ";
	std::cout << ")" << std::endl;
}
