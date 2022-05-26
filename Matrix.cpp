
#include <iostream>
#include <vector>
#include <initializer_list>

using namespace std;

void lol(initializer_list<initializer_list<double>> lst)
{
	cout << lst.size() << endl;
}

int main(int argc, char const *argv[])
{
	lol({{1,2},{3,4}});
	return 0;
}