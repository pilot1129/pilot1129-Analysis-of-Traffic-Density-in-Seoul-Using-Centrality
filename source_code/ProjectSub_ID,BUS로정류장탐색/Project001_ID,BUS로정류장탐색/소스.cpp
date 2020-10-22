#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>


using namespace std;
struct ID
{
	string ID_no;				// 정류장 Id 번호
	string ID_Name;				// 정류장 이름
	string ars;					// 정류장 ARS
};
struct Bus
{
	string Bus_no;				// 버스 번호
	vector<ID> ID_list;			// ARS List 즉 노선도임
	string Name;				// 버스 이름 ??????? -> 안씀
};
vector<string> csv_read_row(istream &file, char delimiter);

int main()
{
	ifstream file("평일.csv");

	vector<Bus> Bus_List;			// 버스 목록

	vector<string> content;

	int i = 0;

	bool asdf = false;

	/*
		csv 파일에서 처음에 뿌려주는 애
	*/
	while (file.good())
	{
		content = csv_read_row(file, ',');

		// 기존의 버스에 추가되는지 새로운 버스와 함께 추가되는지 조건 걸어 주는 것
		if (Bus_List.size() != 0 && Bus_List[i].Bus_no != content[0])
		{
			i++;
			asdf = true;
		}
		else
		{
			asdf = false;
		}
		if (Bus_List.size() == 0)
		{
			Bus bus;
			bus.Bus_no = content[0];
			ID ars;
			ars.ID_no = content[1];
			bus.ID_list.push_back(ars);
			Bus_List.push_back(bus);
		}
		else if (asdf == true)
		{
			Bus bus;
			bus.Bus_no = content[0];
			ID ars;
			ars.ID_no = content[1];
			bus.ID_list.push_back(ars);
			Bus_List.push_back(bus);
		}
		else
		{
			ID ars;
			ars.ID_no = content[1];
			Bus_List[i].ID_list.push_back(ars);
		}
	}
	cout << "뿌리기 끝" << endl;
	ifstream file2("노선도.csv");
	int count = 0;
	bool value = false;
	content = csv_read_row(file2, ',');

	/*
		버스 + 정류장 ID의 조합을 바탕으로 정류장 이름과 정류장 ARS 번호를 긁음

	*/
	/// 찾으면 break 안 건 이유는 중복된 정류장인 경우도 있을 수 있기 때문
	while (file2.good())
	{

		content = csv_read_row(file2, ',');
		for (int i = 0; i < Bus_List.size(); i++)
		{
			if (Bus_List[i].Bus_no == content[0])
			{
				for (int j = 0; j < Bus_List[i].ID_list.size(); j++)
				{
					if (Bus_List[i].ID_list[j].ID_no == content[1])
					{
						Bus_List[i].ID_list[j].ars = content[2];
						Bus_List[i].ID_list[j].ID_Name = content[3];
					}
				}
			}
		}
		count++;
		if (count % 3693 == 1)
			cout << count / 369 << "% 완료" << endl;
	}

	// 결과 저장
	ofstream file3("평일(정류장id + 역명).csv");
	for (int i = 0; i < Bus_List.size(); i++)
	{
		for (int j = 0; j < Bus_List[i].ID_list.size(); j++)
		{
			file3 << Bus_List[i].Bus_no << ',' << Bus_List[i].ID_list[j].ID_no << ',' << Bus_List[i].ID_list[j].ars << ',' << Bus_List[i].ID_list[j].ID_Name << ',' << endl;
		}
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