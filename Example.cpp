#include <iostream>
#include "Matrix.hpp"
using namespace std;

int main(int argc, char const *argv[])
{
	Matrix a = Matrix({{21,26},{37,46}});
	a *= 5;
	a.print();
}
/*
{{21,26},{37,46}}/{{2,3},{4,5}} = {{3,4},{5,6}}
{{21,26},{37,46}}/{{3,4},{5,6}} = {{2,3},{4,5}}
*/