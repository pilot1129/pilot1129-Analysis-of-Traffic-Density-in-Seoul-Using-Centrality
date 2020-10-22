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
	ifstream Sat("토요일.csv");
	ifstream Sun("공휴일.csv");
	ifstream Nor("평일.csv");
	ifstream BusTime("첫차.csv");
	ifstream BusRange("배차.csv");
	ofstream SatR("토요일_C.csv");
	ofstream SunR("공휴일_C.csv");
	ofstream NorR("평일_C.csv");
	unordered_map<string, int> Bus_Time; // 버스 첫차시간
	unordered_map<string, int> Bus_Normal; // 버스 평
	unordered_map<string, int> Bus_Sat; // 버스 토
	unordered_map<string, int> Bus_Sun; // 버스 공
	vector<string> content; // 파일 읽기
	vector<string> Timecontent; // 첫차시간
	vector<string> Rangecontent; // 배차간격
	float Upsum[24] = { 0 }; // 승차 합
	float Downsum[24] = { 0 }; // 하차 합
	float Fix[24] = { 0 }; // 승차 - 하차 누적
	float Seq[24] = { 0 }; // 순서대로 정렬
	int First = 0; // 첫차 시간
	int Range = 0; // 배차간격
	string Busnum = "";

	while (BusTime.good()) // 첫차시간 검색
	{
		Timecontent = csv_read_row(BusTime, ',');
		Bus_Time.insert({ Timecontent[0], stoi(Timecontent[1]) });
	}
	BusTime.close();

	while (BusRange.good())
	{
		Rangecontent = csv_read_row(BusRange, ',');
		string temp = Rangecontent[0];
		Bus_Normal.insert({ temp, ceil(60/stoi(Rangecontent[1])) }); // 평일 대수
		if (Rangecontent[2] == "0")
			Bus_Sat.insert({ temp, 0 }); // 토욜 대수
		else
			Bus_Sat.insert({ temp, ceil(60 / stoi(Rangecontent[2])) }); // 토욜 대수
		
		if (Rangecontent[3] == "0")
			Bus_Sun.insert({ temp, 0 }); // 토욜 대수
		else
			Bus_Sun.insert({ temp, ceil(60 / stoi(Rangecontent[3])) }); // 일욜 대수
	}
	BusRange.close();

	//SaturDay
	while (Sat.good())
	{
		content = csv_read_row(Sat, ',');
		SatR << content[0] << ',' << content[1] << ','; // 버스번호, 정류장번호
		std::unordered_map<std::string, int>::const_iterator got = Bus_Time.find(content[0]);
		std::unordered_map<std::string, int>::const_iterator gotS = Bus_Sat.find(content[0]);
		Range = gotS->second;
		First = got->second;
		if (First == 24)
			First = 0;
		else
			First--; // 2시 -> 2

		if (Busnum != content[0]) // 버스가 바뀐 경우
		{
			Busnum = content[0];
			for (int i = 0; i < 24; i++) //initialize
			{
				Fix[i] = { 0 };
				Seq[i] = { 0 };
			}
		}

		for (int i = 0; i < 24; i++) // 00시부터 23시까지 기록
		{
			Upsum[i] = stof(content[2*i+2]);
			Downsum[i] = stof(content[2*i+3]);
			Fix[i] += (Upsum[i] - Downsum[i]); // 각 시간 별 차이
		}
		for (int i = First; i < 24; i++)
			Seq[i] = Fix[i - First] / 55 * 100; // 해당 시간대의 포화도
		for (int i = 0; i < First; i++)
			Seq[i] = Fix[24 + i - First] / 55 * 100; // 해당 시간대의 포화도
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
		SunR << content[0] << ',' << content[1] << ','; // 버스번호, 정류장번호
		std::unordered_map<std::string, int>::const_iterator got = Bus_Time.find(content[0]);
		std::unordered_map<std::string, int>::const_iterator gotS = Bus_Sun.find(content[0]);
		Range = gotS->second;
		First = got->second;
		if (First == 24)
			First = 0;
		else
			First--; // 2시 -> 2

		if (Busnum != content[0]) // 버스가 바뀐 경우
		{
			Busnum = content[0];
			for (int i = 0; i < 24; i++) //initialize
			{
				Fix[i] = { 0 };
				Seq[i] = { 0 };
			}
		}

		for (int i = 0; i < 24; i++) // 00시부터 23시까지 기록
		{
			Upsum[i] = stof(content[2 * i + 2]);
			Downsum[i] = stof(content[2 * i + 3]);
			Fix[i] += (Upsum[i] - Downsum[i]); // 각 시간 별 차이
		}
		for (int i = First; i < 24; i++)
			Seq[i] = Fix[i - First] / 55 * 100; // 해당 시간대의 포화도
		for (int i = 0; i < First; i++)
			Seq[i] = Fix[24 + i - First] / 55 * 100; // 해당 시간대의 포화도
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
		NorR << content[0] << ',' << content[1] << ','; // 버스번호, 정류장번호
		std::unordered_map<std::string, int>::const_iterator got = Bus_Time.find(content[0]);
		std::unordered_map<std::string, int>::const_iterator gotS = Bus_Normal.find(content[0]);
		Range = gotS->second;
		First = got->second;
		if (First == 24)
			First = 0;
		else
			First--; // 2시 -> 2

		if (Busnum != content[0]) // 버스가 바뀐 경우
		{
			Busnum = content[0];
			for (int i = 0; i < 24; i++) //initialize
			{
				Fix[i] = { 0 };
				Seq[i] = { 0 };
			}
		}

		for (int i = 0; i < 24; i++) // 00시부터 23시까지 기록
		{
			Upsum[i] = stof(content[2 * i + 2]);
			Downsum[i] = stof(content[2 * i + 3]);
			Fix[i] += (Upsum[i] - Downsum[i]); // 각 시간 별 차이
		}
		for (int i = First; i < 24; i++)
			Seq[i] = Fix[i - First] / 55 * 100; // 해당 시간대의 포화도
		for (int i = 0; i < First; i++)
			Seq[i] = Fix[24 + i - First] / 55 * 100; // 해당 시간대의 포화도
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