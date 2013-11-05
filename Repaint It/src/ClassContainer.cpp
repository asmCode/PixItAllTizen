#include "ClassContainer.h"

ClassContainer::~ClassContainer()
{
	std::map<std::string, unsigned char*>::iterator it;
	for (it = classes.begin(); it != classes.end(); it++)
		delete [] it ->second;
}