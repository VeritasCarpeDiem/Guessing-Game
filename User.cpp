#include "User.h"

User::User(string tname)
{
	name = tname;
}

void User::sayHello()
{
	std::cout << "Hello! from " << name << std::endl;
}
