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
	float Saturation; // 포화도
	string Busnumer; // 버스번호
	int Stopnumber; // 정류장번호
	int Time; // 해당 시간
	string Day; // 요일
};

bool operator<(Data a, Data b)
{
	return a.Saturation < b.Saturation;
}

int main()
{
	ifstream Nor("평일_C.csv");
	ifstream Sat("토요일_C.csv");
	ifstream Sun("공휴일_C.csv");
	ifstream Bus("정류장.csv");
	ofstream Top("Top40포화도.csv");
	unordered_map<string, vector<int>> BusStop; // 버스 정류장
	unordered_map<int, string> StationName; // 버스 정류장 번호,이름
	priority_queue<Data> Best40;
	vector<int>Stop; // 정류장
	vector<string> Norcontent;
	vector<string> Satcontent;
	vector<string> Suncontent;
	vector<string> content;
	string Busnum = "";
	bool Check = false; // 맨 첫 셀 체크용
	
	 // 버스별 정류장 확인 및 버스 정류장 이름 출력
	while (Bus.good())
	{
		content = csv_read_row(Bus, ',');
		if (Check && Busnum != content[0]) // 번호가 다르면(새 버스면)
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

	//Top 40 찾기
	//평일
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
			temp.Day = "평일";
			Best40.push(temp);
		}
	}
	cout << "평일 종료" << endl;
	Nor.close();
	//주말
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
			temp.Day = "주말";
			Best40.push(temp);
		}
	}
	cout << "토요일 종료" << endl;
	Sat.close();
	//공휴일
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
			temp.Day = "공휴일";
			Best40.push(temp);
		}
	}
	cout << "공휴일 종료" << endl;
	Sun.close();

	//Top100 출력
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

	//평균 포화도가 높은 버스.







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