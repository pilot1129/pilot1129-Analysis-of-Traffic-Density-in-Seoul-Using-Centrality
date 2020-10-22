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
	// 여러개가 들어가 있다면 그 목록을 출력해주는 리스트
	// 문제가 있었다는 것을 확인하기 위했던 부분이므로 필요 X

	vector<int> index_list;

	for (int i = 0; i < ARS_list.size(); i++)
	{
		if (ars_id == ARS_list[i].ARS_no)
			index_list.push_back(i);
	}

	// 비율에만 있는 경우 ** 사실상 무시해도 됨
	if (index_list.size() == 0)
	{
		//file0 << Bus << "\t" << ars_id << endl;
		//file0.close();
	}
	// 딱 하나씩 매칭이 되는 경우
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


		ARS_list[index_list[0]].value = true; // 비율에는 있는 것

	}
	// 노선도에 똑같은게 두개 이상 있는 경우
	else
	{
		/*
			시작쪽
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
	// ofstream file("시간별에는 있는데 노선도+공토평비율에는 없는 경우.txt", ios::app);
	// 오류를 체크하기 위한 코드로 현재는 필요X

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
	// ARS_list의 크기와 지워야할 Delete_List의 크기를 저장해둠
	int ars_size = ARS_list.size();
	int d_size = D_list.size();

	for (int i = 0; i < ars_size; i++)
	{
		for (int j = 0; j < d_size; j++)
		{
			if (ARS_list[i].ARS_no == D_list[j])
			{
				// 일치하는 인덱스를 지우고
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
		ARS_list[i].Sum_of_Down_H /= 4;	// 하루에 4 나눈거만큼 타고 내림
		ARS_list[i].Sum_of_Up_H /= 4;	// 


	}
}