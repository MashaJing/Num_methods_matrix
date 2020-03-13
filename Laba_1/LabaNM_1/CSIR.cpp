#include "CSIR.h"

CSIR::CSIR()
{
}

CSIR::~CSIR()
{
}



CSIR::CSIR(const std::string& PATH)
{
	//������ �������
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
	for (int i = 0; i < size; i++) //��������� ���������
		di[i] = rand()%50+1;//WHAT
	

	iptr.push_back(1);
	int k = 1;
	for (int i = 0; i < size; i++) //���������� ������������ �� i-�� ������ �� � i-��� ������� ��
	{		
		for (int j = 0; j < i; j++) //������� �����������: ���� j �� ��������� ���������� ��������� � i-�� ������/�������
		{
			bool a = rand()%2; //0 ��� 1
			if (a == 1)
			{
				jptr.push_back(j);
				autr.push_back(rand()%10+1); //��������� ������� � i-�� �������/������� ���������� ��������� ��������
				altr.push_back(rand()%10+1); //����� ��������� double �������� 
				k++;
			}
		}
		iptr.push_back(k);
	}
}
//����� ������� � �������� ������� � ������� csir
//Row � ����� ������, Column � ����� ������� (���������� � 0)
double CSIR::get_elem(int Row, int Column)
{
	//������������ �������
	if (Row == Column) return di[Row];
	//������ �����������
	if (Row > Column)
	{
		//����� ��������� � ������
		int Num = iptr[Row + 1] - iptr[Row];
		if (Num == 0) return 0;
		//���� ������� Column � ������� jptr
		for (int ind = iptr[Row] - 1, k = 0; k < Num; ind++, k++)
			if (Column == (jptr[ind] - 1)) return altr[iptr[Row] - 1 + k];
	}
	//������� �����������
	if (Row < Column)
	{
		//����� ��������� � �������
		int Num = iptr[Column + 1] - iptr[Column];
		if (Num == 0) return 0;
		//���� ������ Row � ������� jptr
		for (int ind = iptr[Column] - 1, k = 0; k < Num; ind++, k++)
			if (Row == (jptr[ind] - 1)) return autr[iptr[Column] - 1 + k];
	}
	return 0;
}


//����� ������ ������ �� ������

//��������� ��������� ������� �� ������
void CSIR:: matrix_mult_vector(const std::vector<double>& X, std::vector<double>& Res)
{
	Res.resize(size);
	//������ �������
	int n = size;
	//������������� ���������� ����� ��������� ������� �� ���������
	for (int i = 0; i < n; i++) Res[i] = X[i] * di[i];
	//������ �� ���� ������� � �������� � ������ �������
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

//����� �������� � ������� ������
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