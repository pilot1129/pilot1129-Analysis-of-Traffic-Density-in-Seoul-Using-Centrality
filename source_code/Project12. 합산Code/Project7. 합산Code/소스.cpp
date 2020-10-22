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
	*	����ϴµ� �־ ����, �����, �Ͽ��� �� 3ȸ �۵����Ѿ���.[���� �������� Nor("����_C.csv") ���� ���� -> ������(�����)�� �����Ұ�]
	*	�׿� ���� ofstream�� "����.csv"�� �̸��� �ٲ������.
	*	�Ʒ��� Bus_Calc �κ� ������� ���� �̿ϼ��̶� �ϼ��� �ۼ��ؾ���.[Line 238~253][��ȭ���� ����߽ɼ� �� ������ �����]
	*	Bus_Calc�� �ð��� �и��Ͽ� �� �߽ɼ��� ����� ����.
	*	Buc_ClacAll�� �ð��� ���� ������ �ջ��Ͽ�, �����庰 �����ϳ�, �ָ��ϳ�, ������ �ϳ��� �ۼ��Ǵ� ��갪��. �̶��� �ð��� ������ �̿ϼ��̶� �ϼ��� �ۼ��ؾ���.[Line 247-251]
	*	�׷����� �ۼ��ϱ� ���� �ϱ� ���� �Ʒ��� ���� ���·� ������ ������
	*
	*	[1] �����ð�.csv																		[2] ��������.csv
	*	From������	�ѱ۸�	Dest������	�ѱ۸�	�߽ɼ�00	�߽ɼ�01	....	�߽ɼ�23	|	From������	�ѱ۸�	Dest������	�ѱ۸�	���߽ɼ�		
	*		0		-			2		-		0.5			0.5					0.5			|		0			-		2			-		1.1
	*		1		-			3		-		0.6			0.6					0.6			|		1			-		3			-		1.2
	*		2		-			4		-		0.7			0.7					0.7			|		2			-		4			-		1.3
*/

vector<string> csv_read_row(istream &file, char delimiter);

int main()
{
	ifstream Nor("������_C.csv"); // ������ȣ, �������ȣ, 00~23�� ��ȭ��    //����_C , �����_C , ������_C
	//ifstream Sat("�����_C.csv");
	//ifstream Sun("������_C.csv");
	ifstream Bus("������.csv"); // ������ȣ , �������ȣ , ������Id, ������ �ѱ۸�
	ifstream Bet("Betweenness.csv"); // �������ȣ , ����߽ɼ�
	ofstream SolT("�����ð�.csv"); // ������, �ѱ�, ������, �ѱ�, �߽ɼ� 24h
	//ofstream SolA("��������.csv"); // ������, �ѱ�, ������, �ѱ�, �߽ɼ�

	unordered_map<int, string>StationName; // <�������ȣ><�ѱ۸�>
	unordered_map<int, vector<int>>BusComb; // <��������> <��������>
	unordered_map<string, vector<float>>Bus_Sat; // <��������-��������><��ȭ������>
	unordered_map<int, int>Bus_bet; //<�������ȣ><bet��갪>
	unordered_map<string, vector<float>>Bus_Satsum; // <��������-��������><��ȭ�����>
	unordered_map<string, int>Bus_betsum; // <��������-��������><bet�ջ갪>
	unordered_map<string, vector<float>>Bus_Calc; // <��������-��������><���갪(bet *���*��ȭ�����)> 
	unordered_map<string, float>Bus_CalcAll; // <��������-��������><���갪(bet *���*��ȭ�����) + �ð��� ���� ����> 
	unordered_map<int, string>Bus_Find; // <�ε���><��������-��������>

	vector<string> content; // CSV �б��
	vector<int>Temp;
	vector<float>Tempf;
	string Busnum = "";
	int Stop_before = 0;
	int Stop_after = 0;
	string StopCon = "";
	bool Check = false;
	int Count = 0; // Index �ο��� ����
	float Temporary = 0;

	//Betweenness���Ͽ��� Bus_bet�� ä��� ���� loop
	while (Bet.good())
	{
		content = csv_read_row(Bet, ',');
		if (content[0] == "")
			break;
		Bus_bet.insert({stoi(content[0]), stoi(content[1])});
	}
	Bet.close();

	//StationName�� BusComb�� ä��� ���� loop
	while (Bus.good())
	{
		content = csv_read_row(Bus, ',');
		if (Busnum != content[0] && Check) //������ ó�� �ɷ����� �� �����뼱�� ó�� �ɷ�����
		{
			Stop_before = 0;
			Stop_after = 0;
		}
		Busnum = content[0];
		Check = true;

		//StationName ä���ֱ�
		//unordered_map<int, string>StationName; // <�������ȣ><�ѱ۸�>
		std::unordered_map<int, string>::const_iterator SearchStop = StationName.find(stoi(content[1])); // �ش� �������ȣ�� �̹� �Է��ߴ��� Ȯ��
		if (SearchStop == StationName.end()) // ó�� ��ȣ�� �Է��ϴ� �� �Ͻ�
			StationName.insert({stoi(content[1]), content[3]});

		//BusComb ä���ֱ�
		if (Stop_after == 0) // �� �뼱�� �� ó���Ͻ�
			Stop_after = stoi(content[1]);
		else // �� �뼱�� ó���� �ƴҽ�
		{
			Stop_before = Stop_after; // �� ���������� �Ű���
			Stop_after = stoi(content[1]); // after�� �� ���������� ����
			std::unordered_map<int, vector<int>>::const_iterator Search = BusComb.find(Stop_before); // Before�뼱�� �ִ��� �˻�
			if (Search == BusComb.end()) // Stop_before�� �ش��ϴ� �ε����� ���ٸ�
			{
				Temp.push_back(Stop_after); // �� �������� ���Ϳ� ����
				BusComb.insert({ Stop_before,Temp }); // �� �������� ����[��-�� ����]
				Temp.clear();//init
			}
			else // Stop_before�� �ش��ϴ� �ε����� �ִٸ�
			{
				Temp.assign(Search->second.begin(), Search->second.end()); // ���� ����
				vector<int>::iterator it = find(Temp.begin(), Temp.end(), Stop_after); // Stop_after�� �� �����忡 �ֳ� �˻�
				vector<int>::difference_type index = distance(Temp.begin(), it); // index ����
				if (index == Temp.size()) // Before�� �ִµ� After�� ������
				{
					Temp.push_back(Stop_after);
					BusComb.erase(Stop_before); // �ش� index ����
					BusComb.insert({ Stop_before, Temp }); // ���� �߰�
				}
				//Before�� �ְ� After�� ������ �Ѿ.
				Temp.clear();
			}
		}
	}
	Bus.close();

	Stop_before = 0; Stop_after = 0; Check = false; Busnum = ""; StopCon = "";

	//Bus_Sat�� ä��� ���� loop
	while (Nor.good())
	{
		content = csv_read_row(Nor, ',');
		if (Busnum != content[0] && Check) //������ ó�� �ɷ����� �� �����뼱�� ó�� �ɷ�����
		{
			Stop_before = 0;
			Stop_after = 0;
			StopCon = "";
		}
		Busnum = content[0];
		Check = true;

		if (Stop_after == 0) // �� �뼱�� �� ó���Ͻ�
			Stop_after = stoi(content[1]);
		else // �� �뼱�� ó���� �ƴҽ�
		{
			//StopCon ����[��������-��������]
			Stop_before = Stop_after; // �� ���������� �Ű���
			Stop_after = stoi(content[1]); // after�� �� ���������� ����
			StopCon = to_string(Stop_before) + "-";
			StopCon += to_string(Stop_after);

			//Bus_Sat ä���
			std::unordered_map<string, vector<float>>::const_iterator Search = Bus_Sat.find(StopCon); // ������ ��ϵ� ������ �ִ��� �˻�
			if (Search == Bus_Sat.end()) // StopCon�� �ش��ϴ� �ε����� ���ٸ�
			{
				//vector ����
				for (int i = 0; i < 25; i++)
				{
					if (i == 0)
						Tempf.push_back(1); // ���߿� ��ճ��� ���� Count��ġ
					else // 00��~23�� ��ȭ�� ����
					{
						Temporary = stof(content[i + 1]);;
						Tempf.push_back(Temporary);
					}
				}
				Bus_Sat.insert({ StopCon,Tempf }); // ����
				Tempf.clear();

				//Bus_Find ä���
				Bus_Find.insert({ Count, StopCon }); // <�ε���><��������-��������>����
				Count++;
			}
			else // StopCon�� �ش��ϴ� �ε����� �ִٸ�
			{
				//�ش� ���Ϳ� ��ȭ�� �� ��ø
				for (int i = 0; i < 25; i++) // Count + 24�ð�
				{
					if (i == 0) // �� ó�� Count ��ġ
						Tempf.push_back(Search->second[i] + 1);
					else
						Tempf.push_back(Search->second[i] + stof(content[i + 1])); // ��ȭ���ױ�
				}
				Bus_Sat.erase(StopCon); // �ش� index ����
				Bus_Sat.insert({ StopCon,Tempf }); // ���� �߰�
				Tempf.clear();
			}
		}
	}

	//Bus_Satsum ��� <��������-��������><��ȭ�����>
	for (int i = 0; i < Bus_Sat.size(); i++)
	{
		int Devide = 0;
		std::unordered_map<int, string>::const_iterator Search = Bus_Find.find(i); // �ε�����  �ش��ϴ� <��������-��������> ã��
		std::unordered_map<string, vector<float>>::const_iterator Search_Stop = Bus_Sat.find(Search->second); // <��������-��������>�� �ش��ϴ� ��ȭ������ ã��
		for (int j = 0; j < 25; j++)
		{
			if (j == 0)
				Devide = Search_Stop->second[j]; // Count�� ����
			else
				Tempf.push_back(Search_Stop->second[j] / Devide);
		}
		Bus_Satsum.insert({ Search->second, Tempf });
		Tempf.clear();
	}

	//Bus_betsum ��� <��������-��������><bet�ջ갪>
	for (int i = 0; i < Bus_Find.size(); i++) // <��������-��������>������ŭ ����
	{
		string All;
		string Before_Stop;
		string After_Stop;
		float Sumbet = 0;
		Stop_before = 0;
		Stop_after = 0;

		//�������� , �������� �и�(String -> int)
		std::unordered_map<int, string>::const_iterator Search = Bus_Find.find(i); // �ε�����  �ش��ϴ� <��������-��������> ã��
		All = Search->second;
		string::size_type pos = All.find("-");
		After_Stop = All.substr(pos+1); // -���ĺ��� �Է�[After]
		All.erase(pos, 10);
		Before_Stop = All; // -�������� �Է�[Before]
		Stop_before = stoi(Before_Stop); // intȭ
		Stop_after = stoi(After_Stop); // intȭ
		
		//Bet ��갪�� �ջ����� ����
		std::unordered_map<int, int>::const_iterator SearchB = Bus_bet.find(Stop_before); // �� �����忡 �ش��ϴ� bet�� ã��
		std::unordered_map<int, int>::const_iterator SearchA = Bus_bet.find(Stop_after); // �� �����忡 �ش��ϴ� bet�� ã��
		Sumbet = (SearchB->second + SearchA->second)/2; // ���������� bet���� ���������� bet�� �ջ� �� ���� �и�
		
		//Bus_betsum �Է�
		Bus_betsum.insert({ Search->second, Sumbet });
	}

	//Bus_Calc �� Bus_CalcAll ��� <��������-��������><���갪>
	for (int i = 0; i < Bus_Find.size(); i++)
	{
		StopCon = "";
		std::unordered_map<int, string>::const_iterator Search_Index = Bus_Find.find(i); // �ε�����  �ش��ϴ� <��������-��������> ã��
		StopCon = Search_Index->second;
		std::unordered_map<string, vector<float>>::const_iterator Search_Satsum = Bus_Satsum.find(StopCon); // <��������-��������>�� �ش��ϴ� [��ȭ�� ���] ã��
		std::unordered_map<string, int>::const_iterator Search_Betsum = Bus_betsum.find(StopCon); // <��������-��������>�� �ش��ϴ� [bet �ջ갪] ã��

		//����
		float Final = 0;
		for (int i = 0; i < 24; i++)
			Tempf.push_back( (((15000 / 14493) * Search_Betsum->second) + (1499237 / 1499300)) * Search_Satsum->second[i]);

		 // Bus_Calc ���
		Bus_Calc.insert({ StopCon,Tempf });

		//Bus_CalcAll ���
		/*
		for (int i = 0; i < 24; i++)  // �ش� ������ ������ �ʿ��� �����Դϴ� �ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�
		{
			if (i == 6 || i == 7 || i == 8) // 6,7,8���� ���(���)
				Final += Tempf[i] * 111111111; // ������ ������ �Է�
			else if (i == 19 || i == 20 || i == 18) // 18, 19,20���� ���(���)
				Final += Tempf[i] * 112121212; // ������ ������ �Է�
			else // ������
				Final += Tempf[i]; // ������ ������ �Է�
		}
		Tempf.clear();
		Bus_CalcAll.insert({ StopCon,Final });
		*/
		Tempf.clear();
	}

	//Output ���� ���
	for (int i = 0; i < Bus_Calc.size(); i++) // <��������-��������>������ŭ ����
	{
		string All;
		string Before_Stop;
		string After_Stop;
		Stop_before = 0;
		Stop_after = 0;

		//�������� , �������� �и�(String -> int)
		std::unordered_map<int, string>::const_iterator Search = Bus_Find.find(i); // �ε�����  �ش��ϴ� <��������-��������> ã��
		All = Search->second;
		string::size_type pos = All.find("-");
		After_Stop = All.substr(pos + 1); // -���ĺ��� �Է�[After]
		All.erase(pos, 10);
		Before_Stop = All; // -�������� �Է�[Before]
		Stop_before = stoi(Before_Stop); // intȭ
		Stop_after = stoi(After_Stop); // intȭ

		//Bet ��갪�� �ջ����� ����
		std::unordered_map<int, string>::const_iterator SearchB = StationName.find(Stop_before); // �� �����忡 �ش��ϴ� �̸� ã��
		std::unordered_map<int, string>::const_iterator SearchA = StationName.find(Stop_after); // �� �����忡 �ش��ϴ� �̸� ã��
		std::unordered_map<string, vector<float>>::const_iterator SearchC = Bus_Calc.find(Search->second); // <��������-��������>�� �ش��ϴ� �߽ɼ� ã��
		std::unordered_map<string, float>::const_iterator SearchD = Bus_CalcAll.find(Search->second); // <��������-��������>�� �ش��ϴ� �߽ɼ� ã��

		//solT ���� ���
		SolT << Stop_before << ',' << SearchB->second << ',' << Stop_after << ',' << SearchA->second << ','; // ��ȣ �̸� ��ȣ �̸�
		for (int i = 0; i < 24; i++)
			SolT << SearchC->second[i] << ',';
		SolT << endl;

		//SolA ���� ���
		/*
		SolA << Stop_before << ',' << SearchB->second << ',' << Stop_after << ',' << SearchA->second << ','; // ��ȣ �̸� ��ȣ �̸�
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