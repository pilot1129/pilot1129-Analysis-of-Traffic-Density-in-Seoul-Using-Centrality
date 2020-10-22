#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<istream>
#include<unordered_map>
using namespace std;

vector<string> csv_read_row(istream &file, char delimiter);

int main()
{
	ifstream Bus("시간별.csv"); // 24시간 파일
	ifstream FirstTime("첫차.csv"); // 첫차파일

	ofstream FileOut("Fix_시간별.csv");

	unordered_map<string, int> Bus_Time; // 버스 배차간격
	vector<string> content;
	vector<string> Timecontent;
	int BusTimeList[48] = { 0 };
	string Busnum = "0";
	int Count = 0;
	int F_Time = 0;

	while (FirstTime.good()) // 첫차시간 검색
	{
		Timecontent = csv_read_row(FirstTime, ',');
		Bus_Time.insert({ Timecontent[0], stoi(Timecontent[1]) });
	}

	FirstTime.close();
	/*
	std::unordered_map<std::string, int>::const_iterator got = Bus_Time.find("100");
	cout << got->second << endl;
	*/

	while (Bus.good())
	{
		content = csv_read_row(Bus, ',');
		if (content[2] == "")
			continue;
		Count = 0;
		FileOut << content[0] << ',' << content[1] << ','; // 버스번호, 정류장번호 삽입
		Busnum = content[0]; // 버스번호
		std::unordered_map<std::string, int>::const_iterator got = Bus_Time.find(Busnum);
		F_Time = got->second;
		if (F_Time == 0)
			F_Time = 24;
		for (int i = ((F_Time) * 2); i < 50; i++) {
			BusTimeList[Count] = stoi(content[i]);
			Count++;
		}
		for (int j = 2; j < (F_Time) * 2; j++) {
			BusTimeList[Count] = stoi(content[j]);
			Count++;
		}
		for (int k = 0; k < 48; k++) {
			FileOut << BusTimeList[k] << ',';
		}
		for(int i = 0; i<48; i++)
			BusTimeList[i] = { 0 };
		FileOut << endl;
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