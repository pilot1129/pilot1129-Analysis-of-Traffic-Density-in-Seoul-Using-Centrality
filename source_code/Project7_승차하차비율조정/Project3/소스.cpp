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
	*	�⺻���� ���α׷��� ������ ����
	*	������ �� ��ü����, �������� �հ� �������� ���� �������� ������ �߰�
	*	�������� �հ� �������� ���� ���Ͻ� �Ͽ� ������ ������ 0���� ����� ���Ͽ�, �ϴ� �������� �������� ���� ����.(�ش� �ð�����)
	*	�������� ���������� ���� ���, ���̸� �����庰 �������� ������ ���Ͽ� �ش� ������� �й��� ��, �й��� ���� �������� ����.
	*	�������� ���������� ���� ���, ���̸� �����庰 �������� ������ ���Ͽ� �ش� ������� �й��� ��, �й��� ���� �������� ����.
	*	
*/

int main()
{
	ifstream Bus("�ð���.csv"); // ���� �Է�
	ofstream Fix_Bus("��������.csv"); // ���� ���
	//queue<pair<int, vector<int>>>StopList; // �������ȣ , 24�ð� ������ ����
	//unordered_map<int, vector<int>> StopList; // �������ȣ , 24�ð� ������ ����
	vector<vector<int>>StopList; // ����������� 24�ð����� = [�����尹��][24]
	vector<int> BusStop; // ���������� ��ȣ
	vector<int> ISum; // ���� �� = 00�ý��� 01�ý��� 02�ý���
	vector<int> OSum; // ���� �� = 00������ 01������ 02������
	vector<vector<int>>Difference; // ���������� �� ��ȯ�� ��
	vector<int>Stop_Dif; // �� ���������忡���� 24�ð� ����-����
	vector<string> content; // �������� ����
	vector<int> Difout; // ���� ��ȯ��
	vector<int> Difin; // ���� ��ȯ��
	string Busnum = "0"; // ���� ������ȣ
	int Count = -1; // ó�� ���� ���п�
	int Sum1 = 0; // ��������
	int Sum2 = 0; // ��������
	int Differ = 0; // ����-����
	double Differ1 = 0; //���� ������ ����
	double Differ2 = 0; //���� ������ ����
	
	//StopList.clear();// �ʱ�ȭ

	while (Bus.good())
	{
		content = csv_read_row(Bus, ',');
		if (Busnum != content[0] && Count != -1) // ���ο� �������
		{
			//��� �����忡���� �����հ� �������� ����.
			for (int i = 0; i < 24; i++) // 24�ð� ������ ������ŭ(48��) loop
			{
				for (int j = 0; j < BusStop.size(); j++)// ���� ������ ������ŭ loop
				{
					Sum1 += StopList[j][2*i]; // ����
					Sum2 += StopList[j][2*i+1]; // ����
				}
				ISum.push_back(Sum1);
				OSum.push_back(Sum2);
				Stop_Dif.push_back(Sum1 - Sum2); // 24���� �����Ǿ�����.(00~23)
				Sum1 = 0;//�ʱ�ȭ
				Sum2 = 0;//�ʱ�ȭ

				Differ = ISum[i] - OSum[i]; //ù���� ����-����
				//�ش� �ð����� ��� �������� ������ - �������� �����, ������ ����� �������� �� ������ ���.
				for (int j = 0; j < BusStop.size(); j++) // ���� ������ ������ŭ loop => �������� ������ŭ �������� ������.
				{
					if (Differ >= 0) // �������� ���������� ���� ���
					{
						Differ1 = (float)StopList[j][i * 2] / (float)ISum[i] * (float)Differ; // �ش������� ������ / ��ü������ * ����
						if (ISum[i] == 0)
							Differ1 = 0;
						Difout.push_back(StopList[j][i * 2 + 1] + Differ1); // ���� ������ - ��ȯ�� ������ ��ȯ[ù ��������� ���� ��������� ���]
						Difin.push_back(StopList[j][i*2]); // ���� ���������� ��ȭ�� ����.
					}
					else
					{
						Differ2 = (float)StopList[j][i * 2+1] / (float)OSum[i] * (float)(abs(Differ)); // �ش������� ������ / ��ü������ * ����
						if (OSum[i] == 0)
							Differ2 = 0;
						Difin.push_back(StopList[j][i * 2] + Differ2); // ���� ������ + ��ȯ�� ������ ��ȯ[ù ��������� ���� ��������� ���]
						Difout.push_back(StopList[j][i * 2 + 1]); // ���� ���������� ��ȭ�� ����.
					}
				}
				//������ ������ �Է�
				Difference.push_back(Difin); // ������ 00��
				Difference.push_back(Difout); // ������ 01��
				Difin.clear();
				Difout.clear();
				Differ, Differ1, Differ2 = 0;
			}
			for (int i = 0; i < BusStop.size(); i++)
			{
				Fix_Bus << Busnum << ',' << BusStop[i] << ',';
				for (int j = 0; j < 24; j++) // 24�ð� �ۼ�
				{
					Fix_Bus << Difference[2 * j][i] << ',' << Difference[2 * j + 1][i] << ',';
				}
				Fix_Bus << endl;
			}

			//�ʱ�ȭ
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
		for (int i = 2; i < 50; i++) // 24�ð�
		{
			if (content[i] == "")
				content[i] = "0";
			Time.push_back(stoi(content[i]));
		}
		BusStop.push_back(stoi(content[1])); // ������ ��ȣ ����
		StopList.push_back(Time); // 24�ð� ����
	}

	/*
	//StopList.push(pair<int,vector<int>>(stoi(content[1]),Time)); // ������, 24�ð� ����
	//StopList.insert(std::make_pair(stoi(content[1]), Time)); // StopList�� �� �����ڴ���.
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