#include "Bus_ARS.h"



Bus_ARS::Bus_ARS()
{
}


Bus_ARS::~Bus_ARS()
{
}


void Bus_ARS::Add_First(string ars, string INDEX)
{
	ARS* a = new ARS;
	a->ARS_no = ars;
	a->index = INDEX;
	ARS_list.push_back(*a);

}

void Bus_ARS::Add_Second(string ars, string INDEX)
{
	ARS* a = new ARS;
	a->ARS_no = ars;
	a->index = INDEX;
	ARS_list.push_back(*a);

}

void Bus_ARS::Add_Ratio(string ars_id, int H_Up, int H_Down, int W_Up, int W_Down, int S_Up, int S_Down, string h_up, string h_down, string w_up, string w_down, string s_up, string s_down)
{

	//ofstream file0("Only_Ratio.txt",ios::app);
	//ofstream file2("Two_Lines.txt", ios::app);
	// �������� �� �ִٸ� �� ����� ������ִ� ����Ʈ
	// ������ �־��ٴ� ���� Ȯ���ϱ� ���ߴ� �κ��̹Ƿ� �ʿ� X

	vector<int> index_list;

	for (int i = 0; i < ARS_list.size(); i++)
	{
		if (ars_id == ARS_list[i].ARS_no)
			index_list.push_back(i);
	}

	// �������� �ִ� ��� ** ��ǻ� �����ص� ��
	if (index_list.size() == 0)
	{
		//file0 << Bus << "\t" << ars_id << endl;
		//file0.close();
	}
	// �� �ϳ��� ��Ī�� �Ǵ� ���
	else if (index_list.size() == 1)
	{
		ARS_list[index_list[0]].H_Up_Ratio = stof(h_up);
		ARS_list[index_list[0]].H_Down_Ratio = stof(h_down);
		ARS_list[index_list[0]].W_Up_Ratio = stof(w_up);
		ARS_list[index_list[0]].W_Down_Ratio = stof(w_down);
		ARS_list[index_list[0]].S_Up_Ratio = stof(s_up);
		ARS_list[index_list[0]].S_Down_Ratio = stof(s_down);

		ARS_list[index_list[0]].Sum_of_Down_H = H_Down;
		ARS_list[index_list[0]].Sum_of_Up_H = H_Up;
		ARS_list[index_list[0]].Sum_of_Down_S = S_Down;
		ARS_list[index_list[0]].Sum_of_Up_S = S_Up;
		ARS_list[index_list[0]].Sum_of_Down_W = W_Down;
		ARS_list[index_list[0]].Sum_of_Up_W = W_Up;


		ARS_list[index_list[0]].value = true; // �������� �ִ� ��

	}
	// �뼱���� �Ȱ����� �ΰ� �̻� �ִ� ���
	else
	{
		/*
			������
		*/
		if (index_list.size() == 2)
		{
			if ((index_list[0] <= 10) && (index_list[index_list.size() - 1] - 10 >= ARS_list.size()))
			{
				ARS_list[index_list[0]].H_Up_Ratio = stof(h_up) * 0.7;
				ARS_list[index_list[0]].H_Down_Ratio = stof(h_down) * 0.3;
				ARS_list[index_list[0]].W_Up_Ratio = stof(w_up) * 0.7;
				ARS_list[index_list[0]].W_Down_Ratio = stof(w_down) * 0.3;
				ARS_list[index_list[0]].S_Up_Ratio = stof(s_up) * 0.7;
				ARS_list[index_list[0]].S_Down_Ratio = stof(s_down) * 0.3;
				ARS_list[index_list[0]].Sum_of_Down_H = H_Down;
				ARS_list[index_list[0]].Sum_of_Up_H = H_Up;
				ARS_list[index_list[0]].Sum_of_Down_S = S_Down;
				ARS_list[index_list[0]].Sum_of_Up_S = S_Up;
				ARS_list[index_list[0]].Sum_of_Down_W = W_Down;
				ARS_list[index_list[0]].Sum_of_Up_W = W_Up;

				ARS_list[index_list[0]].value = true;

				ARS_list[index_list[1]].H_Up_Ratio = stof(h_up) * 0.3;
				ARS_list[index_list[1]].H_Down_Ratio = stof(h_down) * 0.7;
				ARS_list[index_list[1]].W_Up_Ratio = stof(w_up) * 0.3;
				ARS_list[index_list[1]].W_Down_Ratio = stof(w_down) * 0.7;
				ARS_list[index_list[1]].S_Up_Ratio = stof(s_up) * 0.3;
				ARS_list[index_list[1]].S_Down_Ratio = stof(s_down) * 0.7;
				ARS_list[index_list[1]].Sum_of_Down_H = H_Down;
				ARS_list[index_list[1]].Sum_of_Up_H = H_Up;
				ARS_list[index_list[1]].Sum_of_Down_S = S_Down;
				ARS_list[index_list[1]].Sum_of_Up_S = S_Up;
				ARS_list[index_list[1]].Sum_of_Down_W = W_Down;
				ARS_list[index_list[1]].Sum_of_Up_W = W_Up;

				ARS_list[index_list[1]].value = true;
			}
			else
			{
				ARS_list[index_list[0]].H_Up_Ratio = stof(h_up) * 0.5;
				ARS_list[index_list[0]].H_Down_Ratio = stof(h_down) * 0.5;
				ARS_list[index_list[0]].W_Up_Ratio = stof(w_up) * 0.5;
				ARS_list[index_list[0]].W_Down_Ratio = stof(w_down) * 0.5;
				ARS_list[index_list[0]].S_Up_Ratio = stof(s_up) * 0.5;
				ARS_list[index_list[0]].S_Down_Ratio = stof(s_down) * 0.5;
				ARS_list[index_list[0]].Sum_of_Down_H = H_Down;
				ARS_list[index_list[0]].Sum_of_Up_H = H_Up;
				ARS_list[index_list[0]].Sum_of_Down_S = S_Down;
				ARS_list[index_list[0]].Sum_of_Up_S = S_Up;
				ARS_list[index_list[0]].Sum_of_Down_W = W_Down;
				ARS_list[index_list[0]].Sum_of_Up_W = W_Up;
				ARS_list[index_list[0]].value = true;

				ARS_list[index_list[1]].H_Up_Ratio = stof(h_up) * 0.5;
				ARS_list[index_list[1]].H_Down_Ratio = stof(h_down) * 0.5;
				ARS_list[index_list[1]].W_Up_Ratio = stof(w_up) * 0.5;
				ARS_list[index_list[1]].W_Down_Ratio = stof(w_down) * 0.5;
				ARS_list[index_list[1]].S_Up_Ratio = stof(s_up) * 0.5;
				ARS_list[index_list[1]].S_Down_Ratio = stof(s_down) * 0.5;
				ARS_list[index_list[1]].Sum_of_Down_H = H_Down;
				ARS_list[index_list[1]].Sum_of_Up_H = H_Up;
				ARS_list[index_list[1]].Sum_of_Down_S = S_Down;
				ARS_list[index_list[1]].Sum_of_Up_S = S_Up;
				ARS_list[index_list[1]].Sum_of_Down_W = W_Down;
				ARS_list[index_list[1]].Sum_of_Up_W = W_Up;
				ARS_list[index_list[1]].value = true;
			}
		}
		else if (index_list.size() == 3)
		{
			ARS_list[index_list[0]].H_Up_Ratio = stof(h_up) / 3.0;
			ARS_list[index_list[0]].H_Down_Ratio = stof(h_down) / 3.0;
			ARS_list[index_list[0]].W_Up_Ratio = stof(w_up) / 3.0;
			ARS_list[index_list[0]].W_Down_Ratio = stof(w_down) / 3.0;
			ARS_list[index_list[0]].S_Up_Ratio = stof(s_up) / 3.0;
			ARS_list[index_list[0]].S_Down_Ratio = stof(s_down) / 3.0;
			ARS_list[index_list[0]].Sum_of_Down_H = H_Down;
			ARS_list[index_list[0]].Sum_of_Up_H = H_Up;
			ARS_list[index_list[0]].Sum_of_Down_S = S_Down;
			ARS_list[index_list[0]].Sum_of_Up_S = S_Up;
			ARS_list[index_list[0]].Sum_of_Down_W = W_Down;
			ARS_list[index_list[0]].Sum_of_Up_W = W_Up;
			ARS_list[index_list[0]].value = true;


			ARS_list[index_list[1]].H_Up_Ratio = stof(h_up) / 3.0;
			ARS_list[index_list[1]].H_Down_Ratio = stof(h_down) / 3.0;
			ARS_list[index_list[1]].W_Up_Ratio = stof(w_up) / 3.0;
			ARS_list[index_list[1]].W_Down_Ratio = stof(w_down) / 3.0;
			ARS_list[index_list[1]].S_Up_Ratio = stof(s_up) / 3.0;
			ARS_list[index_list[1]].S_Down_Ratio = stof(s_down) / 3.0;
			ARS_list[index_list[1]].Sum_of_Down_H = H_Down;
			ARS_list[index_list[1]].Sum_of_Up_H = H_Up;
			ARS_list[index_list[1]].Sum_of_Down_S = S_Down;
			ARS_list[index_list[1]].Sum_of_Up_S = S_Up;
			ARS_list[index_list[1]].Sum_of_Down_W = W_Down;
			ARS_list[index_list[1]].Sum_of_Up_W = W_Up;
			ARS_list[index_list[1]].value = true;

			ARS_list[index_list[2]].H_Up_Ratio = stof(h_up) / 3.0;
			ARS_list[index_list[2]].H_Down_Ratio = stof(h_down) / 3.0;
			ARS_list[index_list[2]].W_Up_Ratio = stof(w_up) / 3.0;
			ARS_list[index_list[2]].W_Down_Ratio = stof(w_down) / 3.0;
			ARS_list[index_list[2]].S_Up_Ratio = stof(s_up) / 3.0;
			ARS_list[index_list[2]].S_Down_Ratio = stof(s_down) / 3.0;
			ARS_list[index_list[2]].Sum_of_Down_H = H_Down;
			ARS_list[index_list[2]].Sum_of_Up_H = H_Up;
			ARS_list[index_list[2]].Sum_of_Down_S = S_Down;
			ARS_list[index_list[2]].Sum_of_Up_S = S_Up;
			ARS_list[index_list[2]].Sum_of_Down_W = W_Down;
			ARS_list[index_list[2]].Sum_of_Up_W = W_Up;
			ARS_list[index_list[2]].value = true;
		}
	}
}


void Bus_ARS::Add_Time_Data(string ars_id, vector<Day_by_Day> time)
{
	// ofstream file("�ð������� �ִµ� �뼱��+������������� ���� ���.txt", ios::app);
	// ������ üũ�ϱ� ���� �ڵ�� ����� �ʿ�X

	bool value = false;
	for (int i = 0; i < ARS_list.size(); i++)
	{
		if (ARS_list[i].ARS_no == ars_id)
		{
			ARS_list[i].Time_list = time;
		}
	}
	if (value == false)
	{
		// file << Bus << "\t" << ars_id << endl;
	}
}

void Bus_ARS::Delete_empty_ARS(vector<string> D_list)
{
	// ARS_list�� ũ��� �������� Delete_List�� ũ�⸦ �����ص�
	int ars_size = ARS_list.size();
	int d_size = D_list.size();

	for (int i = 0; i < ars_size; i++)
	{
		for (int j = 0; j < d_size; j++)
		{
			if (ARS_list[i].ARS_no == D_list[j])
			{
				// ��ġ�ϴ� �ε����� �����
				ARS_list.erase(ARS_list.begin() + i);
				ars_size--;
				d_size--;
				j--;
			}
		}
	}
}



void Bus_ARS::Sorting()
{
	for (int i = 0; i < ARS_list.size(); i++)
	{
		ARS_list[i].Sum_of_Down_H /= 4;	// �Ϸ翡 4 �����Ÿ�ŭ Ÿ�� ����
		ARS_list[i].Sum_of_Up_H /= 4;	// 


	}
}