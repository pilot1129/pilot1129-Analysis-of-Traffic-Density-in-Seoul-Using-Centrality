#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<istream>
#include<unordered_map>
#include<queue>
using namespace std;

vector<string> csv_read_row(istream &file, char delimiter);

int main()
{
	ifstream Bus("Last_��������.csv");
	ifstream Ratio("���������.csv");
	ofstream Sat("�����.csv");
	ofstream Sun("������.csv");
	ofstream Nor("����.csv");
	vector<string> content; // ���� �б�
	vector<vector<vector<double>>>Percent; // [����][������][���� 6��]
	vector<vector<double>>PercentTemp;
	string Busnum = ""; // ������ȣ
	int Station = 0; // �������ȣ
	int Count = -1;
	bool Dif = false;
	int BusCount = 0;
	int StationCount = 0;

	while (Ratio.good())
	{
		content = csv_read_row(Ratio, ',');
		if (Busnum != content[0] && Count != -1)
		{
			Percent.push_back(PercentTemp); //[����][������][���� 6��]
			for (int i = 0; i < PercentTemp.size(); i++)
				PercentTemp[i].clear();
			PercentTemp.clear();
			Busnum = content[0];
		}
		else
		{
			Count++;
			Busnum = content[0];
		}
		vector<double>PP; // ������ �ϳ��� ����
		for (int i = 2; i < 8; i++)
			PP.push_back(stod(content[i])); // ���� ����(�� ��,�� / �� ��,�� / �� ��,��)
		PercentTemp.push_back(PP);
		PP.clear();

		if (content[0] == "�߶�01" && content[1] == "106900004")
		{
			Percent.push_back(PercentTemp); //[����][������][���� 6��]
			for (int i = 0; i < PercentTemp.size(); i++)
				PercentTemp[i].clear();
			PercentTemp.clear();
			Busnum = content[0];
		}
	}
	Ratio.close();

	Count = 0;
	content.clear();
	while (Bus.good()) 
	{
		content = csv_read_row(Bus, ',');
		if (Busnum != content[0]&& Count != 0)
			Dif = true;
		Busnum = content[0];
		Station = stoi(content[1]);
		Sat << Busnum << ','; Sun << Busnum << ','; Nor << Busnum << ','; // ������ȣ �Է�
		Sat << Station << ','; Sun << Station << ','; Nor << Station << ','; // ��ID �Է�
		int Up = 0;
		int Down = 0;
		if (Dif) // ���� ��ȣ�� �ٲ������
		{
			BusCount++;
			StationCount = 0;
			Dif = false;
		}
		for (int i = 1; i < 25; i++) // ��,�� �Է�
		{
			Up = stoi(content[i * 2]); // ��
			Down = stoi(content[i * 2 + 1]); // ��
			for (int j = 0; j < 3; j++) // �� - �� - ��
			{
				switch (j) 
				{
				case 0://��
					Sat << Percent[BusCount][StationCount][2 * j] * Up << ','; //��
					Sat << Percent[BusCount][StationCount][2 * j + 1] * Down << ','; //��
					break;
				case 1://��
					Nor << Percent[BusCount][StationCount][2 * j] * Up << ','; //��
					Nor << Percent[BusCount][StationCount][2 * j + 1] * Down << ','; //��
					break;
				case 2://��
					Sun << Percent[BusCount][StationCount][2 * j] * Up << ','; //��
					Sun << Percent[BusCount][StationCount][2 * j + 1] * Down << ','; //��
					break;
				default://error
					break;
				}
			}
		}
		Sat << endl;
		Nor << endl;
		Sun << endl;
		Count++;
		StationCount++;
	}
	Bus.close();
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