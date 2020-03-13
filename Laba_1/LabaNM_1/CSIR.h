#pragma once
#include <vector>
#include <fstream>

using namespace std;

class CSIR
{

public:
	vector<double> di;
	vector<double> altr;
	vector<double> autr;
	vector<int> iptr;
	vector<int> jptr;
	int size;

	void generate(int size);
	double get_elem(int Row, int Col);
	const vector <vector<double>> to_dens();
	void matrix_mult_vector(const std::vector<double>& X, std::vector<double>& Res);

	CSIR(const std::string& PATH);
	CSIR();
	~CSIR();
};

void densit_mult_vector(const std::vector<double>& X, const std::vector <vector<double>> &Y, std::vector<double> &Res);