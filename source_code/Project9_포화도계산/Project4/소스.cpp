#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<istream>
#include<cmath>
#include<unordered_map>
using namespace std;

vector<string> csv_read_row(istream &file, char delimiter);

int main()
{
	ifstream Sat("�����.csv");
	ifstream Sun("������.csv");
	ifstream Nor("����.csv");
	ifstream BusTime("ù��.csv");
	ifstream BusRange("����.csv");
	ofstream SatR("�����_C.csv");
	ofstream SunR("������_C.csv");
	ofstream NorR("����_C.csv");
	unordered_map<string, int> Bus_Time; // ���� ù���ð�
	unordered_map<string, int> Bus_Normal; // ���� ��
	unordered_map<string, int> Bus_Sat; // ���� ��
	unordered_map<string, int> Bus_Sun; // ���� ��
	vector<string> content; // ���� �б�
	vector<string> Timecontent; // ù���ð�
	vector<string> Rangecontent; // ��������
	float Upsum[24] = { 0 }; // ���� ��
	float Downsum[24] = { 0 }; // ���� ��
	float Fix[24] = { 0 }; // ���� - ���� ����
	float Seq[24] = { 0 }; // ������� ����
	int First = 0; // ù�� �ð�
	int Range = 0; // ��������
	string Busnum = "";

	while (BusTime.good()) // ù���ð� �˻�
	{
		Timecontent = csv_read_row(BusTime, ',');
		Bus_Time.insert({ Timecontent[0], stoi(Timecontent[1]) });
	}
	BusTime.close();

	while (BusRange.good())
	{
		Rangecontent = csv_read_row(BusRange, ',');
		string temp = Rangecontent[0];
		Bus_Normal.insert({ temp, ceil(60/stoi(Rangecontent[1])) }); // ���� ���
		if (Rangecontent[2] == "0")
			Bus_Sat.insert({ temp, 0 }); // ��� ���
		else
			Bus_Sat.insert({ temp, ceil(60 / stoi(Rangecontent[2])) }); // ��� ���
		
		if (Rangecontent[3] == "0")
			Bus_Sun.insert({ temp, 0 }); // ��� ���
		else
			Bus_Sun.insert({ temp, ceil(60 / stoi(Rangecontent[3])) }); // �Ͽ� ���
	}
	BusRange.close();

	//SaturDay
	while (Sat.good())
	{
		content = csv_read_row(Sat, ',');
		SatR << content[0] << ',' << content[1] << ','; // ������ȣ, �������ȣ
		std::unordered_map<std::string, int>::const_iterator got = Bus_Time.find(content[0]);
		std::unordered_map<std::string, int>::const_iterator gotS = Bus_Sat.find(content[0]);
		Range = gotS->second;
		First = got->second;
		if (First == 24)
			First = 0;
		else
			First--; // 2�� -> 2

		if (Busnum != content[0]) // ������ �ٲ� ���
		{
			Busnum = content[0];
			for (int i = 0; i < 24; i++) //initialize
			{
				Fix[i] = { 0 };
				Seq[i] = { 0 };
			}
		}

		for (int i = 0; i < 24; i++) // 00�ú��� 23�ñ��� ���
		{
			Upsum[i] = stof(content[2*i+2]);
			Downsum[i] = stof(content[2*i+3]);
			Fix[i] += (Upsum[i] - Downsum[i]); // �� �ð� �� ����
		}
		for (int i = First; i < 24; i++)
			Seq[i] = Fix[i - First] / 55 * 100; // �ش� �ð����� ��ȭ��
		for (int i = 0; i < First; i++)
			Seq[i] = Fix[24 + i - First] / 55 * 100; // �ش� �ð����� ��ȭ��
		for (int i = 0; i < 24; i++)
		{
			if(Range == 0)
				SatR << 0 << ',';
			else
				SatR << Seq[i] / Range << ',';
		}
			
		SatR << endl;
	}
	Sat.close();

	for (int i = 0; i < 24; i++) //initialize
	{
		Upsum[i] = { 0 };
		Downsum[i] = { 0 };
		Fix[i] = { 0 };
		Seq[i] = { 0 };
	}
	content.clear();

	//SunDay
	while (Sun.good())
	{
		content = csv_read_row(Sun, ',');
		SunR << content[0] << ',' << content[1] << ','; // ������ȣ, �������ȣ
		std::unordered_map<std::string, int>::const_iterator got = Bus_Time.find(content[0]);
		std::unordered_map<std::string, int>::const_iterator gotS = Bus_Sun.find(content[0]);
		Range = gotS->second;
		First = got->second;
		if (First == 24)
			First = 0;
		else
			First--; // 2�� -> 2

		if (Busnum != content[0]) // ������ �ٲ� ���
		{
			Busnum = content[0];
			for (int i = 0; i < 24; i++) //initialize
			{
				Fix[i] = { 0 };
				Seq[i] = { 0 };
			}
		}

		for (int i = 0; i < 24; i++) // 00�ú��� 23�ñ��� ���
		{
			Upsum[i] = stof(content[2 * i + 2]);
			Downsum[i] = stof(content[2 * i + 3]);
			Fix[i] += (Upsum[i] - Downsum[i]); // �� �ð� �� ����
		}
		for (int i = First; i < 24; i++)
			Seq[i] = Fix[i - First] / 55 * 100; // �ش� �ð����� ��ȭ��
		for (int i = 0; i < First; i++)
			Seq[i] = Fix[24 + i - First] / 55 * 100; // �ش� �ð����� ��ȭ��
		for (int i = 0; i < 24; i++)
		{
			if (Range == 0)
				SunR << 0 << ',';
			else
				SunR << Seq[i] / Range << ',';
		}
		SunR << endl;
	}
	Sun.close();

	for (int i = 0; i < 24; i++) //initialize
	{
		Upsum[i] = { 0 };
		Downsum[i] = { 0 };
		Fix[i] = { 0 };
		Seq[i] = { 0 };
	}
	content.clear();
	
	//NormalDay
	while (Nor.good())
	{
		content = csv_read_row(Nor, ',');
		NorR << content[0] << ',' << content[1] << ','; // ������ȣ, �������ȣ
		std::unordered_map<std::string, int>::const_iterator got = Bus_Time.find(content[0]);
		std::unordered_map<std::string, int>::const_iterator gotS = Bus_Normal.find(content[0]);
		Range = gotS->second;
		First = got->second;
		if (First == 24)
			First = 0;
		else
			First--; // 2�� -> 2

		if (Busnum != content[0]) // ������ �ٲ� ���
		{
			Busnum = content[0];
			for (int i = 0; i < 24; i++) //initialize
			{
				Fix[i] = { 0 };
				Seq[i] = { 0 };
			}
		}

		for (int i = 0; i < 24; i++) // 00�ú��� 23�ñ��� ���
		{
			Upsum[i] = stof(content[2 * i + 2]);
			Downsum[i] = stof(content[2 * i + 3]);
			Fix[i] += (Upsum[i] - Downsum[i]); // �� �ð� �� ����
		}
		for (int i = First; i < 24; i++)
			Seq[i] = Fix[i - First] / 55 * 100; // �ش� �ð����� ��ȭ��
		for (int i = 0; i < First; i++)
			Seq[i] = Fix[24 + i - First] / 55 * 100; // �ش� �ð����� ��ȭ��
		for (int i = 0; i < 24; i++)
		{
			if (Range == 0)
				NorR << 0 << ',';
			else
				NorR << Seq[i] / Range << ',';
		}
		NorR << endl;
	}
	Nor.close();

	for (int i = 0; i < 24; i++) //initialize
	{
		Upsum[i] = { 0 };
		Downsum[i] = { 0 };
		Fix[i] = { 0 };
		Seq[i] = { 0 };
	}
	content.clear();

	return 0;
}
vector<string> csv_read_row(istream &file, char delimiter)
{
	stringstream ss;
	bool inquotes = false;
	vector<string> row;

	while (file.good())
	{
		char c = file.get();
		if (!inquotes && c == '"')
			inquotes = true;
		else if (inquotes && c == '"')
		{
			if (file.peek() == '"')
				ss << (char)file.get();
			else
				inquotes = false;
		}
		else if (!inquotes&& c == delimiter)
		{
			row.push_back(ss.str());
			ss.str("");
		}
		else if (!inquotes && (c == '\r' || c == '\n'))
		{
			if (file.peek() == '\n')
				file.get();
			row.push_back(ss.str());
			return row;
		}
		else
			ss << c;
	}
}