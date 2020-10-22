#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>


using namespace std;

vector<string> csv_read_row(istream &file, char delimiter);

struct ID
{
	string ID_no;
	float percent;
};
class Bus
{
public:
	string Bus_no;
	vector<ID> ID_list;
};


void main()
{
	ifstream file("평일_C.csv"); // 1111111111111111111111

	vector<Bus> Bus_List;

	vector<string> content;

	bool asdf = false;
	int i = 0;
	int pre = i;
	int as = 0;
	while (file.good())
	{
		content = csv_read_row(file, ',');

		// 기존의 버스에 추가되는지 새로운 버스와 함께 추가되는지 조건 걸어 주는 것
		if (Bus_List.size() != 0 && Bus_List[i].Bus_no != content[0])
		{
			pre = i;
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

			if (stof(content[10]) > 30) // 333333333333
			{
				ID ars;
				ars.ID_no = content[1];
				ars.percent = stof(content[10]); // 22222222222222
				bus.ID_list.push_back(ars);
				Bus_List.push_back(bus);

			}
		}
		else if (asdf == true)
		{
			Bus bus;
			bus.Bus_no = content[0];
			if (stof(content[10]) > 30) // 33333333333333
			{
				ID ars;
				ars.ID_no = content[1];
				ars.percent = stof(content[10]); // 22222222222222
				bus.ID_list.push_back(ars);
				Bus_List.push_back(bus);
			}
			else
			{
				pre = i;
				i--;
			}
		}
		else
		{
			if (stof(content[10]) > 30) // 33333333333333
			{
				ID ars;
				ars.ID_no = content[1];
				ars.percent = stof(content[10]); // 22222222222222
				Bus_List[i].ID_list.push_back(ars);
			}
		}
	}

	cout << "끝! -> 저장 시작" << endl;


	ofstream file2("평일 돌린거.csv"); // 4444444444

	for (int j = 0; j < Bus_List.size(); j++)
	{
		for (int k = 0; k < Bus_List[j].ID_list.size(); k++)
		{
			file2 << Bus_List[j].Bus_no << ',' << Bus_List[j].ID_list[k].ID_no << ',' << Bus_List[j].ID_list[k].percent << ',' << endl;
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