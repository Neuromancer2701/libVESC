

#include "Vesc.h"
#include<iostream>

using std::cout;

int main()
{

    cout << "VESC Test\n";

    Vesc  test;

    test.FindandMapMotorControllers();


    return 0;
}