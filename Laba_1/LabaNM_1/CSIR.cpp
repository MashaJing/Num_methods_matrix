#include "CSIR.h"

CSIR::CSIR()
{
}

CSIR::~CSIR()
{
}



CSIR::CSIR(const std::string& PATH)
{
	//размер матрицы
	std::ifstream Reader(PATH + "size.bin", std::ios::binary);
	if (!Reader.is_open())
		throw std::exception("File size.bin was not found...");
	Reader.read((char*)&size, sizeof(int));
	Reader.close();
	//di
	Reader.open(PATH + "di.bin", std::ios::binary);
	if (!Reader.is_open())
		throw std::exception("File di.bin was not found...");
	di.resize(size);
	for (int i = 0; i < size; i++)
		Reader.read((char*)&di[i], sizeof(double));
	Reader.close();
	//iptr
	Reader.open(PATH + "iptr.bin", std::ios::binary);
	if (!Reader.is_open())
		throw std::exception("File iptr.bin was not found...");
	iptr.resize(size + 1);
	for (int i = 0; i < size + 1; i++)
		Reader.read((char*)&iptr[i], sizeof(int));
	Reader.close();
	//jptr
	Reader.open(PATH + "jptr.bin", std::ios::binary);
	if (!Reader.is_open())
		throw std::exception("File jptr.bin was not found...");
	int JPTR_SIZE = iptr[size] - 1;
	jptr.resize(JPTR_SIZE);
	for (int i = 0; i < JPTR_SIZE; i++)
		Reader.read((char*)&jptr[i], sizeof(int));
	Reader.close();
	//autr
	Reader.open(PATH + "autr.bin", std::ios::binary);
	if (!Reader.is_open())
		throw std::exception("File autr.bin was not found...");
	autr.resize(JPTR_SIZE);
	for (int i = 0; i < JPTR_SIZE; i++)
		Reader.read((char*)&autr[i], sizeof(double));
	Reader.close();
	//altr
	Reader.open(PATH + "altr.bin", std::ios::binary);
	if (!Reader.is_open())
		throw std::exception("File altr.bin was not found...");
	altr.resize(JPTR_SIZE);
	for (int i = 0; i < JPTR_SIZE; i++)
		Reader.read((char*)&altr[i], sizeof(double));
	Reader.close();

}


void CSIR::generate(int size)
{
	if (di.size() != 0)
		di.clear();

	if (altr.size() != 0)
		altr.clear();
	
	if (autr.size() != 0)
		autr.clear();

	if (iptr.size() != 0)
		iptr.clear();

	if (jptr.size() != 0)
		jptr.clear();

	this->size = size;
	di.resize(size);
	for (int i = 0; i < size; i++) //Заполняем диагональ
		di[i] = rand()%50+1;//WHAT
	

	iptr.push_back(1);
	int k = 1;
	for (int i = 0; i < size; i++) //проходимся одновременно по i-ой строке НТ и i-ому столбцу ВТ
	{		
		for (int j = 0; j < i; j++) //условие продолжения: пока j не достигнет количества элементов в i-ой строке/столбце
		{
			bool a = rand()%2; //0 или 1
			if (a == 1)
			{
				jptr.push_back(j);
				autr.push_back(rand()%10+1); //Случайным образом в i-ой строчке/столбце выбираются ненулевые элементы
				altr.push_back(rand()%10+1); //Любое ненулевое double значение 
				k++;
			}
		}
		iptr.push_back(k);
	}
}
//метод доступа к элементу матрицы в формате csir
//Row – номер строки, Column – номер столбца (начинаются с 0)
double CSIR::get_elem(int Row, int Column)
{
	//диагональный элемент
	if (Row == Column) return di[Row];
	//нижний треугольник
	if (Row > Column)
	{
		//число элементов в строке
		int Num = iptr[Row + 1] - iptr[Row];
		if (Num == 0) return 0;
		//ищем столбец Column в массиве jptr
		for (int ind = iptr[Row] - 1, k = 0; k < Num; ind++, k++)
			if (Column == (jptr[ind] - 1)) return altr[iptr[Row] - 1 + k];
	}
	//верхний треугольник
	if (Row < Column)
	{
		//число элементов в столбце
		int Num = iptr[Column + 1] - iptr[Column];
		if (Num == 0) return 0;
		//ищем строку Row в массиве jptr
		for (int ind = iptr[Column] - 1, k = 0; k < Num; ind++, k++)
			if (Row == (jptr[ind] - 1)) return autr[iptr[Column] - 1 + k];
	}
	return 0;
}


//Метод чтения данных из файлов

//Процедура умножения матрицы на вектор
void CSIR:: matrix_mult_vector(const std::vector<double>& X, std::vector<double>& Res)
{
	Res.resize(size);
	//размер матрицы
	int n = size;
	//инициализация результата через умножения вектора на диагональ
	for (int i = 0; i < n; i++) Res[i] = X[i] * di[i];
	//проход по всем строкам и столбцам с учётом формата
	for (int i = 0; i < n; i++)
		for (int j = iptr[i] - 1; j < iptr[i + 1] - 1; j++)
		{
			Res[i] += X[jptr[j]] * altr[j];
			Res[jptr[j]] += X[i] * autr[j];
		}
}

void densit_mult_vector(const std::vector<double>& X, const std::vector <vector<double>> &Y, std::vector<double> &Res)
{
	Res.clear();
	for (int i = 0; i < Y.size(); i++)
	{
		Res.push_back(0);
		for (int j = 0; j < Y[i].size(); j++)
			Res[i] += Y[i][j] * X[j];
	}
}

//Метод перевода в плотный формат
const vector<vector<double>> CSIR::to_dens()
{
	vector<vector<double>> res;

	for (int i = 0; i < size; i++)
	{
		vector<double> temp;
		for (int j = 0; j < size; j++)
		{
			temp.push_back(get_elem(i, j));
		}
		res.push_back(temp);
	}
	return res;
}