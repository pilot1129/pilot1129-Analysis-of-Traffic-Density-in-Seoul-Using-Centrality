#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<istream>
#include<cmath>
#include<unordered_map>
#include<queue>
using namespace std;

vector<string> csv_read_row(istream &file, char delimiter);

struct Data {
	float Saturation; // ��ȭ��
	string Busnumer; // ������ȣ
	int Stopnumber; // �������ȣ
	int Time; // �ش� �ð�
	string Day; // ����
};

bool operator<(Data a, Data b)
{
	return a.Saturation < b.Saturation;
}

int main()
{
	ifstream Nor("����_C.csv");
	ifstream Sat("�����_C.csv");
	ifstream Sun("������_C.csv");
	ifstream Bus("������.csv");
	ofstream Top("Top40��ȭ��.csv");
	unordered_map<string, vector<int>> BusStop; // ���� ������
	unordered_map<int, string> StationName; // ���� ������ ��ȣ,�̸�
	priority_queue<Data> Best40;
	vector<int>Stop; // ������
	vector<string> Norcontent;
	vector<string> Satcontent;
	vector<string> Suncontent;
	vector<string> content;
	string Busnum = "";
	bool Check = false; // �� ù �� üũ��
	
	 // ������ ������ Ȯ�� �� ���� ������ �̸� ���
	while (Bus.good())
	{
		content = csv_read_row(Bus, ',');
		if (Check && Busnum != content[0]) // ��ȣ�� �ٸ���(�� ������)
		{
			BusStop.insert({ Busnum, Stop });
			Stop.clear();
		}
		Busnum = content[0];
		Stop.push_back(stoi(content[1]));

		StationName.insert({ stoi(content[1]),content[3] });
		Check = true;
	}
	Bus.close();

	//Top 40 ã��
	//����
	while (Nor.good())
	{
		Norcontent = csv_read_row(Nor, ',');
		for (int i = 0; i < 24; i++)
		{
			Data temp;
			temp.Busnumer = Norcontent[0];
			temp.Stopnumber = stoi(Norcontent[1]);
			temp.Saturation = stoi(Norcontent[2+i]);
			temp.Time = i;
			temp.Day = "����";
			Best40.push(temp);
		}
	}
	cout << "���� ����" << endl;
	Nor.close();
	//�ָ�
	while (Sat.good())
	{
		Satcontent = csv_read_row(Sat, ',');
		for (int i = 0; i < 24; i++)
		{
			Data temp;
			temp.Busnumer = Satcontent[0];
			temp.Stopnumber = stoi(Satcontent[1]);
			temp.Saturation = stoi(Satcontent[2 + i]);
			temp.Time = i;
			temp.Day = "�ָ�";
			Best40.push(temp);
		}
	}
	cout << "����� ����" << endl;
	Sat.close();
	//������
	while (Sun.good())
	{
		Suncontent = csv_read_row(Sun, ',');
		for (int i = 0; i < 24; i++)
		{
			Data temp;
			temp.Busnumer = Suncontent[0];
			temp.Stopnumber = stoi(Suncontent[1]);
			temp.Saturation = stoi(Suncontent[2 + i]);
			temp.Time = i;
			temp.Day = "������";
			Best40.push(temp);
		}
	}
	cout << "������ ����" << endl;
	Sun.close();

	//Top100 ���
	Top << "index" << ',' << "Day" << ',' << "Time" << ',' << "Bus" << ',' <<  "Station" << ',' <<  "Saturation" << ',' << endl;
	for (int i = 0; i < 100; i++)
	{
		Top << i + 1 << ','; // index
		Top << Best40.top().Day << ',';
		Top << Best40.top().Time << ',';
		Top << Best40.top().Busnumer << ',';
		Top << Best40.top().Stopnumber << ',';
		Top << Best40.top().Saturation << ',';
		std::unordered_map<int, string>::const_iterator got = StationName.find(Best40.top().Stopnumber);
		Top << got->second << ',' << endl;
		Best40.pop();
	}

	//��� ��ȭ���� ���� ����.







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