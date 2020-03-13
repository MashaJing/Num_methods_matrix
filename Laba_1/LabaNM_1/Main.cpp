#include <iostream>
#include <fstream>
#include <vector>
#include <float.h>
#include <time.h>  
#include "CSIR.h"

using namespace std;

int main()
{		//Задание 1
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

	/*	try
		{
			while (1)
			{
				N = 0;
				ind++;
				x.matrix_mult_vector(y, result);

				for (int i = 0; i < result.size(); i++)
				{
					result[i] *= result[i];
					//if (N > 1e+307) cout << N << "  " << result[i] << endl;
					if ((N >= DBL_MAX) || (result[i] >= DBL_MAX)) throw ind;
					N += result[i];

				}
				N = sqrt(N);
				cout << "N=" << N << " ";
				cout << "x1=" << y[0] << endl;
				if (y[0] >= DBL_MAX) throw ind;
				y[0] *= 10;
			}
		}
		catch (int ind)
		{
			cout << "Double overflow at iteration " << ind << endl;
			system("pause");
		}*/

		//цикл 2
	try 
	{
		while (1)
		{
			N = 0;
			ind++;
			x.matrix_mult_vector(y, result);
			for (int i = 0; i < result.size(); i++)
			{
				result[i] *= result[i];
				N += result[i];
			}
			N = sqrt(N);
			if (N == 0) throw ind;
			if (y[0] == 0) throw ind;
			cout << "N=" << N << " ";
			cout << "y1=" << y[0] << endl;
			y[0] /= 10;
		
		}
	}
	catch (int ind)
	{
		cout << "Double underflow at iteration "<< ind << endl;
		system("pause");
	}
}
