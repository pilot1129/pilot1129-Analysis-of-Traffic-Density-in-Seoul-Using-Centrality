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


/*
	*	기본적인 프로그램은 다음과 같음
	*	데이터 셋 자체에서, 승차량의 합과 하차량의 합이 동일하지 않음을 발견
	*	승차량의 합과 하차량의 합을 동일시 하여 종점에 도착시 0명을 만들기 위하여, 일단 승차량과 하차량의 차를 구함.(해당 시간대의)
	*	승차량이 하차량보다 많을 경우, 차이를 정거장별 승차량의 비율을 구하여 해당 비율대로 분배한 후, 분배한 값을 하차량에 더함.
	*	승차량이 하차량보다 적을 경우, 차이를 정거장별 하차량의 비율을 구하여 해당 비율대로 분배한 후, 분배한 값을 승차량에 더함.
	*	
*/

int main()
{
	ifstream Bus("시간별.csv"); // 파일 입력
	ofstream Fix_Bus("수정파일.csv"); // 파일 출력
	//queue<pair<int, vector<int>>>StopList; // 정류장번호 , 24시간 승하차 값들
	//unordered_map<int, vector<int>> StopList; // 정류장번호 , 24시간 승하차 값들
	vector<vector<int>>StopList; // 버스정류장순 24시간정보 = [정류장갯수][24]
	vector<int> BusStop; // 버스정류장 번호
	vector<int> ISum; // 승차 합 = 00시승차 01시승차 02시승차
	vector<int> OSum; // 하차 합 = 00시하차 01시하차 02시하차
	vector<vector<int>>Difference; // 버스정류장 순 변환할 값
	vector<int>Stop_Dif; // 한 버스정류장에서의 24시간 승차-하차
	vector<string> content; // 한줄한줄 읽음
	vector<int> Difout; // 하차 변환값
	vector<int> Difin; // 승차 변환값
	string Busnum = "0"; // 현재 버스번호
	int Count = -1; // 처음 버스 구분용
	int Sum1 = 0; // 승차변수
	int Sum2 = 0; // 하차변수
	int Differ = 0; // 승차-하차
	double Differ1 = 0; //하차 조정용 변수
	double Differ2 = 0; //승차 조정용 변수
	
	//StopList.clear();// 초기화

	while (Bus.good())
	{
		content = csv_read_row(Bus, ',');
		if (Busnum != content[0] && Count != -1) // 새로운 버스라면
		{
			//모든 정류장에서의 승차합과 하차합을 구함.
			for (int i = 0; i < 24; i++) // 24시간 승하차 정보만큼(48번) loop
			{
				for (int j = 0; j < BusStop.size(); j++)// 버스 정류장 갯수만큼 loop
				{
					Sum1 += StopList[j][2*i]; // 승차
					Sum2 += StopList[j][2*i+1]; // 하차
				}
				ISum.push_back(Sum1);
				OSum.push_back(Sum2);
				Stop_Dif.push_back(Sum1 - Sum2); // 24개로 구성되어있음.(00~23)
				Sum1 = 0;//초기화
				Sum2 = 0;//초기화

				Differ = ISum[i] - OSum[i]; //첫차의 승차-하차
				//해당 시간에서 모든 정류장의 승차합 - 하차합을 계산후, 비율을 계산해 승차에서 뺄 값까지 계산.
				for (int j = 0; j < BusStop.size(); j++) // 버스 정류장 갯수만큼 loop => 승차량의 비율만큼 하차량에 더해줌.
				{
					if (Differ >= 0) // 승차량이 하차량보다 많을 경우
					{
						Differ1 = (float)StopList[j][i * 2] / (float)ISum[i] * (float)Differ; // 해당정류소 승차량 / 전체승차량 * 차이
						if (ISum[i] == 0)
							Differ1 = 0;
						Difout.push_back(StopList[j][i * 2 + 1] + Differ1); // 실제 하차량 - 변환량 하차량 변환[첫 정류장부터 나중 정류장까지 계산]
						Difin.push_back(StopList[j][i*2]); // 실제 승차량에서 변화가 없음.
					}
					else
					{
						Differ2 = (float)StopList[j][i * 2+1] / (float)OSum[i] * (float)(abs(Differ)); // 해당정류소 하차량 / 전체하차량 * 차이
						if (OSum[i] == 0)
							Differ2 = 0;
						Difin.push_back(StopList[j][i * 2] + Differ2); // 실제 승차량 + 변환량 승차량 변환[첫 정류장부터 나중 정류장까지 계산]
						Difout.push_back(StopList[j][i * 2 + 1]); // 실제 하차량에서 변화가 없음.
					}
				}
				//승차량 하차량 입력
				Difference.push_back(Difin); // 승차량 00시
				Difference.push_back(Difout); // 하차량 01시
				Difin.clear();
				Difout.clear();
				Differ, Differ1, Differ2 = 0;
			}
			for (int i = 0; i < BusStop.size(); i++)
			{
				Fix_Bus << Busnum << ',' << BusStop[i] << ',';
				for (int j = 0; j < 24; j++) // 24시간 작성
				{
					Fix_Bus << Difference[2 * j][i] << ',' << Difference[2 * j + 1][i] << ',';
				}
				Fix_Bus << endl;
			}

			//초기화
			Busnum = content[0];
			Sum1 = 0;
			Sum2 = 0;
			BusStop.clear();
			ISum.clear();
			OSum.clear();
			Stop_Dif.clear();
			Difout.clear();
			Difin.clear();
			Differ, Differ1, Differ2 = 0;

			for (int i = 0; i < StopList.size(); i++)
				StopList[i].clear();
			StopList.clear();
			for (int i = 0; i < Difference.size(); i++)
				Difference[i].clear();
			Difference.clear();
		}
		else if (Count == -1)
		{
			Count++;
			Busnum = content[0];
		}
		vector<int> Time;
		for (int i = 2; i < 50; i++) // 24시간
		{
			if (content[i] == "")
				content[i] = "0";
			Time.push_back(stoi(content[i]));
		}
		BusStop.push_back(stoi(content[1])); // 정류장 번호 삽입
		StopList.push_back(Time); // 24시간 삽입
	}

	/*
	//StopList.push(pair<int,vector<int>>(stoi(content[1]),Time)); // 정류장, 24시간 삽입
	//StopList.insert(std::make_pair(stoi(content[1]), Time)); // StopList에 다 때려박는중.
	std::unordered_map<std::string, int>::const_iterator got = Bus_Time.find("100");
	cout << got->second << endl;
	*/
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