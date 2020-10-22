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
	string ID_no;				// ������ Id ��ȣ
	string ID_Name;				// ������ �̸�
	string ars;					// ������ ARS
};
struct Bus
{
	string Bus_no;				// ���� ��ȣ
	vector<ID> ID_list;			// ARS List �� �뼱����
	string Name;				// ���� �̸� ??????? -> �Ⱦ�
};
vector<string> csv_read_row(istream &file, char delimiter);

int main()
{
	ifstream file("����.csv");

	vector<Bus> Bus_List;			// ���� ���

	vector<string> content;

	int i = 0;

	bool asdf = false;

	/*
		csv ���Ͽ��� ó���� �ѷ��ִ� ��
	*/
	while (file.good())
	{
		content = csv_read_row(file, ',');

		// ������ ������ �߰��Ǵ��� ���ο� ������ �Բ� �߰��Ǵ��� ���� �ɾ� �ִ� ��
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
	cout << "�Ѹ��� ��" << endl;
	ifstream file2("�뼱��.csv");
	int count = 0;
	bool value = false;
	content = csv_read_row(file2, ',');

	/*
		���� + ������ ID�� ������ �������� ������ �̸��� ������ ARS ��ȣ�� ����

	*/
	/// ã���� break �� �� ������ �ߺ��� �������� ��쵵 ���� �� �ֱ� ����
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
			cout << count / 369 << "% �Ϸ�" << endl;
	}

	// ��� ����
	ofstream file3("����(������id + ����).csv");
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