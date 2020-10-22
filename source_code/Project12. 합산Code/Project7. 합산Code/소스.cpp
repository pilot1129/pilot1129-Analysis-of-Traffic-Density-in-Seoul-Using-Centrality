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

/*	=================================================================================Readme.txt=========================================================
	*	사용하는데 있어서 평일, 토요일, 일요일 총 3회 작동시켜야함.[만일 귀찮으면 Nor("평일_C.csv") 에서 평일 -> 공휴일(토요일)로 수정할것]
	*	그에 따라 ofstream의 "최종.csv"의 이름도 바꿔줘야함.
	*	아래쪽 Bus_Calc 부분 연산식이 아직 미완성이라 완성시 작성해야함.[Line 238~253][포화도랑 노드중심성 둘 사이의 관계식]
	*	Bus_Calc는 시간을 분리하여 총 중심성을 계산한 값임.
	*	Buc_ClacAll은 시간을 일정 비율로 합산하여, 정류장별 평일하나, 주말하나, 공휴일 하나로 작성되는 계산값임. 이또한 시간별 비율이 미완성이라 완성시 작성해야함.[Line 247-251]
	*	그래프에 작성하기 쉽게 하기 위해 아래와 같은 형태로 파일을 제작함
	*
	*	[1] 최종시간.csv																		[2] 최종총합.csv
	*	From정류장	한글명	Dest정류장	한글명	중심성00	중심성01	....	중심성23	|	From정류장	한글명	Dest정류장	한글명	총중심성		
	*		0		-			2		-		0.5			0.5					0.5			|		0			-		2			-		1.1
	*		1		-			3		-		0.6			0.6					0.6			|		1			-		3			-		1.2
	*		2		-			4		-		0.7			0.7					0.7			|		2			-		4			-		1.3
*/

vector<string> csv_read_row(istream &file, char delimiter);

int main()
{
	ifstream Nor("공휴일_C.csv"); // 버스번호, 정류장번호, 00~23시 포화도    //평일_C , 토요일_C , 공휴일_C
	//ifstream Sat("토요일_C.csv");
	//ifstream Sun("공휴일_C.csv");
	ifstream Bus("정류장.csv"); // 버스번호 , 정류장번호 , 정류장Id, 정류장 한글명
	ifstream Bet("Betweenness.csv"); // 정류장번호 , 노드중심성
	ofstream SolT("최종시간.csv"); // 정류장, 한글, 정류장, 한글, 중심성 24h
	//ofstream SolA("최종총합.csv"); // 정류장, 한글, 정류장, 한글, 중심성

	unordered_map<int, string>StationName; // <정류장번호><한글명>
	unordered_map<int, vector<int>>BusComb; // <전정류장> <후정류장>
	unordered_map<string, vector<float>>Bus_Sat; // <전정류장-후정류장><포화도모음>
	unordered_map<int, int>Bus_bet; //<정류장번호><bet계산값>
	unordered_map<string, vector<float>>Bus_Satsum; // <전정류장-후정류장><포화도평균>
	unordered_map<string, int>Bus_betsum; // <전정류장-후정류장><bet합산값>
	unordered_map<string, vector<float>>Bus_Calc; // <전정류장-후정류장><연산값(bet *상수*포화도평균)> 
	unordered_map<string, float>Bus_CalcAll; // <전정류장-후정류장><연산값(bet *상수*포화도평균) + 시간당 비율 적용> 
	unordered_map<int, string>Bus_Find; // <인덱스><전정류장-후정류장>

	vector<string> content; // CSV 읽기용
	vector<int>Temp;
	vector<float>Tempf;
	string Busnum = "";
	int Stop_before = 0;
	int Stop_after = 0;
	string StopCon = "";
	bool Check = false;
	int Count = 0; // Index 부여용 변수
	float Temporary = 0;

	//Betweenness파일에서 Bus_bet을 채우기 위한 loop
	while (Bet.good())
	{
		content = csv_read_row(Bet, ',');
		if (content[0] == "")
			break;
		Bus_bet.insert({stoi(content[0]), stoi(content[1])});
	}
	Bet.close();

	//StationName과 BusComb를 채우기 위한 loop
	while (Bus.good())
	{
		content = csv_read_row(Bus, ',');
		if (Busnum != content[0] && Check) //파일의 처음 걸러내기 및 버스노선의 처음 걸러내기
		{
			Stop_before = 0;
			Stop_after = 0;
		}
		Busnum = content[0];
		Check = true;

		//StationName 채워넣기
		//unordered_map<int, string>StationName; // <정류장번호><한글명>
		std::unordered_map<int, string>::const_iterator SearchStop = StationName.find(stoi(content[1])); // 해당 정류장번호를 이미 입력했는지 확인
		if (SearchStop == StationName.end()) // 처음 번호를 입력하는 것 일시
			StationName.insert({stoi(content[1]), content[3]});

		//BusComb 채워넣기
		if (Stop_after == 0) // 각 노선의 맨 처음일시
			Stop_after = stoi(content[1]);
		else // 각 노선의 처음이 아닐시
		{
			Stop_before = Stop_after; // 전 정류장으로 옮겨줌
			Stop_after = stoi(content[1]); // after을 현 정류장으로 변경
			std::unordered_map<int, vector<int>>::const_iterator Search = BusComb.find(Stop_before); // Before노선이 있는지 검색
			if (Search == BusComb.end()) // Stop_before에 해당하는 인덱스가 없다면
			{
				Temp.push_back(Stop_after); // 뒤 정류장을 벡터에 넣음
				BusComb.insert({ Stop_before,Temp }); // 뒤 정류장을 삽입[전-후 생성]
				Temp.clear();//init
			}
			else // Stop_before에 해당하는 인덱스가 있다면
			{
				Temp.assign(Search->second.begin(), Search->second.end()); // 벡터 복사
				vector<int>::iterator it = find(Temp.begin(), Temp.end(), Stop_after); // Stop_after이 후 정류장에 있나 검색
				vector<int>::difference_type index = distance(Temp.begin(), it); // index 설정
				if (index == Temp.size()) // Before은 있는데 After이 없으면
				{
					Temp.push_back(Stop_after);
					BusComb.erase(Stop_before); // 해당 index 삭제
					BusComb.insert({ Stop_before, Temp }); // 새로 추가
				}
				//Before도 있고 After도 있으면 넘어감.
				Temp.clear();
			}
		}
	}
	Bus.close();

	Stop_before = 0; Stop_after = 0; Check = false; Busnum = ""; StopCon = "";

	//Bus_Sat을 채우기 위한 loop
	while (Nor.good())
	{
		content = csv_read_row(Nor, ',');
		if (Busnum != content[0] && Check) //파일의 처음 걸러내기 및 버스노선의 처음 걸러내기
		{
			Stop_before = 0;
			Stop_after = 0;
			StopCon = "";
		}
		Busnum = content[0];
		Check = true;

		if (Stop_after == 0) // 각 노선의 맨 처음일시
			Stop_after = stoi(content[1]);
		else // 각 노선의 처음이 아닐시
		{
			//StopCon 생성[전정류장-후정류장]
			Stop_before = Stop_after; // 전 정류장으로 옮겨줌
			Stop_after = stoi(content[1]); // after을 현 정류장으로 변경
			StopCon = to_string(Stop_before) + "-";
			StopCon += to_string(Stop_after);

			//Bus_Sat 채우기
			std::unordered_map<string, vector<float>>::const_iterator Search = Bus_Sat.find(StopCon); // 이전에 등록된 조합이 있는지 검색
			if (Search == Bus_Sat.end()) // StopCon에 해당하는 인덱스가 없다면
			{
				//vector 생성
				for (int i = 0; i < 25; i++)
				{
					if (i == 0)
						Tempf.push_back(1); // 나중에 평균내기 위한 Count수치
					else // 00시~23시 포화도 삽입
					{
						Temporary = stof(content[i + 1]);;
						Tempf.push_back(Temporary);
					}
				}
				Bus_Sat.insert({ StopCon,Tempf }); // 삽입
				Tempf.clear();

				//Bus_Find 채우기
				Bus_Find.insert({ Count, StopCon }); // <인덱스><전정류장-후정류장>삽입
				Count++;
			}
			else // StopCon에 해당하는 인덱스가 있다면
			{
				//해당 벡터에 포화도 값 중첩
				for (int i = 0; i < 25; i++) // Count + 24시간
				{
					if (i == 0) // 맨 처음 Count 수치
						Tempf.push_back(Search->second[i] + 1);
					else
						Tempf.push_back(Search->second[i] + stof(content[i + 1])); // 포화도쌓기
				}
				Bus_Sat.erase(StopCon); // 해당 index 삭제
				Bus_Sat.insert({ StopCon,Tempf }); // 새로 추가
				Tempf.clear();
			}
		}
	}

	//Bus_Satsum 계산 <전정류장-후정류장><포화도평균>
	for (int i = 0; i < Bus_Sat.size(); i++)
	{
		int Devide = 0;
		std::unordered_map<int, string>::const_iterator Search = Bus_Find.find(i); // 인덱스에  해당하는 <전정류장-후정류장> 찾기
		std::unordered_map<string, vector<float>>::const_iterator Search_Stop = Bus_Sat.find(Search->second); // <전정류장-후정류장>에 해당하는 포화도벡터 찾기
		for (int j = 0; j < 25; j++)
		{
			if (j == 0)
				Devide = Search_Stop->second[j]; // Count수 측정
			else
				Tempf.push_back(Search_Stop->second[j] / Devide);
		}
		Bus_Satsum.insert({ Search->second, Tempf });
		Tempf.clear();
	}

	//Bus_betsum 계산 <전정류장-후정류장><bet합산값>
	for (int i = 0; i < Bus_Find.size(); i++) // <전정류장-후정류장>갯수만큼 조정
	{
		string All;
		string Before_Stop;
		string After_Stop;
		float Sumbet = 0;
		Stop_before = 0;
		Stop_after = 0;

		//전정류장 , 후정류장 분리(String -> int)
		std::unordered_map<int, string>::const_iterator Search = Bus_Find.find(i); // 인덱스에  해당하는 <전정류장-후정류장> 찾기
		All = Search->second;
		string::size_type pos = All.find("-");
		After_Stop = All.substr(pos+1); // -이후부터 입력[After]
		All.erase(pos, 10);
		Before_Stop = All; // -이전까지 입력[Before]
		Stop_before = stoi(Before_Stop); // int화
		Stop_after = stoi(After_Stop); // int화
		
		//Bet 계산값을 합산으로 변형
		std::unordered_map<int, int>::const_iterator SearchB = Bus_bet.find(Stop_before); // 전 정류장에 해당하는 bet값 찾기
		std::unordered_map<int, int>::const_iterator SearchA = Bus_bet.find(Stop_after); // 후 정류장에 해당하는 bet값 찾기
		Sumbet = (SearchB->second + SearchA->second)/2; // 전정류장의 bet값과 후정류장의 bet값 합산 후 절반 분리
		
		//Bus_betsum 입력
		Bus_betsum.insert({ Search->second, Sumbet });
	}

	//Bus_Calc 및 Bus_CalcAll 계산 <전정류장-후정류장><연산값>
	for (int i = 0; i < Bus_Find.size(); i++)
	{
		StopCon = "";
		std::unordered_map<int, string>::const_iterator Search_Index = Bus_Find.find(i); // 인덱스에  해당하는 <전정류장-후정류장> 찾기
		StopCon = Search_Index->second;
		std::unordered_map<string, vector<float>>::const_iterator Search_Satsum = Bus_Satsum.find(StopCon); // <전정류장-후정류장>에 해당하는 [포화도 평균] 찾기
		std::unordered_map<string, int>::const_iterator Search_Betsum = Bus_betsum.find(StopCon); // <전정류장-후정류장>에 해당하는 [bet 합산값] 찾기

		//연산
		float Final = 0;
		for (int i = 0; i < 24; i++)
			Tempf.push_back( (((15000 / 14493) * Search_Betsum->second) + (1499237 / 1499300)) * Search_Satsum->second[i]);

		 // Bus_Calc 계산
		Bus_Calc.insert({ StopCon,Tempf });

		//Bus_CalcAll 계산
		/*
		for (int i = 0; i < 24; i++)  // 해당 수식은 수정이 필요한 수식입니다 ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
		{
			if (i == 6 || i == 7 || i == 8) // 6,7,8시의 경우(출근)
				Final += Tempf[i] * 111111111; // 임의의 비율값 입력
			else if (i == 19 || i == 20 || i == 18) // 18, 19,20시의 경우(퇴근)
				Final += Tempf[i] * 112121212; // 임의의 비율값 입력
			else // 나머지
				Final += Tempf[i]; // 임의의 비율값 입력
		}
		Tempf.clear();
		Bus_CalcAll.insert({ StopCon,Final });
		*/
		Tempf.clear();
	}

	//Output 파일 출력
	for (int i = 0; i < Bus_Calc.size(); i++) // <전정류장-후정류장>갯수만큼 조정
	{
		string All;
		string Before_Stop;
		string After_Stop;
		Stop_before = 0;
		Stop_after = 0;

		//전정류장 , 후정류장 분리(String -> int)
		std::unordered_map<int, string>::const_iterator Search = Bus_Find.find(i); // 인덱스에  해당하는 <전정류장-후정류장> 찾기
		All = Search->second;
		string::size_type pos = All.find("-");
		After_Stop = All.substr(pos + 1); // -이후부터 입력[After]
		All.erase(pos, 10);
		Before_Stop = All; // -이전까지 입력[Before]
		Stop_before = stoi(Before_Stop); // int화
		Stop_after = stoi(After_Stop); // int화

		//Bet 계산값을 합산으로 변형
		std::unordered_map<int, string>::const_iterator SearchB = StationName.find(Stop_before); // 전 정류장에 해당하는 이름 찾기
		std::unordered_map<int, string>::const_iterator SearchA = StationName.find(Stop_after); // 후 정류장에 해당하는 이름 찾기
		std::unordered_map<string, vector<float>>::const_iterator SearchC = Bus_Calc.find(Search->second); // <전정류장-후정류장>에 해당하는 중심성 찾기
		std::unordered_map<string, float>::const_iterator SearchD = Bus_CalcAll.find(Search->second); // <전정류장-후정류장>에 해당하는 중심성 찾기

		//solT 파일 출력
		SolT << Stop_before << ',' << SearchB->second << ',' << Stop_after << ',' << SearchA->second << ','; // 번호 이름 번호 이름
		for (int i = 0; i < 24; i++)
			SolT << SearchC->second[i] << ',';
		SolT << endl;

		//SolA 파일 출력
		/*
		SolA << Stop_before << ',' << SearchB->second << ',' << Stop_after << ',' << SearchA->second << ','; // 번호 이름 번호 이름
		SolA << SearchD->second << ',' << endl;
		*/
	}




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