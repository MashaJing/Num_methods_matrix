#include <iostream>
#include <fstream>
#include <vector>
#include <float.h>
#include <time.h>  
#include "CSIR.h"

#define EPS 1e+3

using namespace std;

int main()
{
	try
	{
		//Задание 1
		//CSIR a("data\\");

		CSIR x;
		x.generate(1600);
		vector<vector<double>> T;
		vector<double> y, result;
		
		//Задание 2
		T = x.to_dens();

		//Задание 3, 4
		for (int i = 0; i < x.size; i++)
			y.push_back(rand()%10+1);

		unsigned int fTimeStart = clock();
		x.matrix_mult_vector(y, result); //CSIR формат
		unsigned int fTimeStop = clock();

		unsigned int timeProc1 = fTimeStop - fTimeStart;

		fTimeStart = clock();
		densit_mult_vector(y, T, result); //плотный формат
		fTimeStop = clock();

		unsigned int timeProc2 = fTimeStop - fTimeStart;
		cout << "CSIR: " << timeProc1 << endl;
		cout << "Densit: " << timeProc2 << endl;

		//Задание 5
		y.clear();
		y.push_back(1);
		for (int i = 0; i < x.size - 1; i++)
			y.push_back(0);


		//цикл 1
		double N = 0;
		int ind = 0;
		while (1)
		{
			ind++;
			x.matrix_mult_vector(y, result);

			for (int i = 0; i < result.size(); i++)
			{
				result[i] *= result[i];
				if (N>1e+307) cout << DBL_MAX - result[i] - N << "  " << DBL_MAX - result[i] << "  "<< DBL_MAX - N << endl;
				if (DBL_MAX - (N + result[i]) <= EPS) throw "Double overflow";
				N += result[i];
				
			}
			N = sqrt(N);
			cout << "N=" << N << " ";
			cout << "x1=" << y[0] << endl;
			if (y[0] >= DBL_MAX / 10) throw "Double overflow";
			y[0] *= 10;
		}
		
		//цикл 2
	/*	while (1)
		{
			ind++;
			int i = 0;
			x.matrix_mult_vector(y, result);

			while (i < result.size())
			{
				result[i] *= result[i];
				if (fabs(result[i] - DBL_MIN) ) throw "Double underflow";
				N = N + result[i];
				i++;
			}
			N = sqrt(N);
			cout << "N=" << N << " ";
			cout << "x1=" << y[0] << endl;
			y[0] /= 10;
			if (y[0] == 0) throw "Double underflow";
		}*/
	}
	catch (exception & ex)
	{
		cout << ex.what() << endl;
		system("pause");
	}
}