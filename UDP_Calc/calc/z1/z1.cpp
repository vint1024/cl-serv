// z1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "my_calc.h"

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Input F(x)  ";
	char F[1000];
	cin.getline(F, 1000);
	int x=0;
	//cout << "Input X  ";
	//cin >> x;
	cout << calculate(F) << endl;

	system("pause");
	return 0;
}

