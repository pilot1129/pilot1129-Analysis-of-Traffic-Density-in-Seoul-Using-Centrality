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
	ifstream Bus("Last_수정파일.csv");
	ifstream Ratio("토평공비율.csv");
	ofstream Sat("토요일.csv");
	ofstream Sun("공휴일.csv");
	ofstream Nor("평일.csv");
	vector<string> content; // 파일 읽기
	vector<vector<vector<double>>>Percent; // [버스][정류장][비율 6개]
	vector<vector<double>>PercentTemp;
	string Busnum = ""; // 버스번호
	int Station = 0; // 정류장번호
	int Count = -1;
	bool Dif = false;
	int BusCount = 0;
	int StationCount = 0;

	while (Ratio.good())
	{
		content = csv_read_row(Ratio, ',');
		if (Busnum != content[0] && Count != -1)
		{
			Percent.push_back(PercentTemp); //[버스][정류장][비율 6개]
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
		vector<double>PP; // 정류장 하나의 비율
		for (int i = 2; i < 8; i++)
			PP.push_back(stod(content[i])); // 비율 삽입(토 상,하 / 평 상,하 / 공 상,하)
		PercentTemp.push_back(PP);
		PP.clear();

		if (content[0] == "중랑01" && content[1] == "106900004")
		{
			Percent.push_back(PercentTemp); //[버스][정류장][비율 6개]
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
		Sat << Busnum << ','; Sun << Busnum << ','; Nor << Busnum << ','; // 버스번호 입력
		Sat << Station << ','; Sun << Station << ','; Nor << Station << ','; // 역ID 입력
		int Up = 0;
		int Down = 0;
		if (Dif) // 버스 번호가 바뀌었으면
		{
			BusCount++;
			StationCount = 0;
			Dif = false;
		}
		for (int i = 1; i < 25; i++) // 상,하 입력
		{
			Up = stoi(content[i * 2]); // 상
			Down = stoi(content[i * 2 + 1]); // 하
			for (int j = 0; j < 3; j++) // 토 - 평 - 공
			{
				switch (j) 
				{
				case 0://토
					Sat << Percent[BusCount][StationCount][2 * j] * Up << ','; //상
					Sat << Percent[BusCount][StationCount][2 * j + 1] * Down << ','; //하
					break;
				case 1://평
					Nor << Percent[BusCount][StationCount][2 * j] * Up << ','; //상
					Nor << Percent[BusCount][StationCount][2 * j + 1] * Down << ','; //하
					break;
				case 2://공
					Sun << Percent[BusCount][StationCount][2 * j] * Up << ','; //상
					Sun << Percent[BusCount][StationCount][2 * j + 1] * Down << ','; //하
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