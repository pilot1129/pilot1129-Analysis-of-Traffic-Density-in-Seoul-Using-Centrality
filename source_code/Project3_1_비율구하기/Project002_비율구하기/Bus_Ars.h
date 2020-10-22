#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>


using namespace std;

struct Day_by_Day
{
	string Time;			// 1��1��, 1��2��, 1��3��, 1�� 4�� ... ������� ���� ���� ��
	string Up_Nop;
	string Down_Nop;
};

struct ARS
{
	string ARS_no;
	string index = "0";
	bool value = false;
	float S_Down_Ratio = 0.0;
	float S_Up_Ratio = 0.0;
	float W_Down_Ratio = 0.0;
	float W_Up_Ratio = 0.0;
	float H_Down_Ratio = 0.0;
	float H_Up_Ratio = 0.0;
	int Sum_of_Up_H = 0.0;
	int Sum_of_Down_H = 0.0;
	int Sum_of_Up_W = 0.0;
	int Sum_of_Down_W = 0.0;
	int Sum_of_Up_S = 0.0;
	int Sum_of_Down_S = 0.0;


	vector<Day_by_Day> Time_list;
};

class Bus_ARS
{

public:
	string Bus;
	int First_Time;
	int S_Term;
	int W_Term;
	int T_Term;



	vector<ARS> ARS_list;

	// ���ο� ������ �뼱�� �߰��ϴ� �Լ�
	void Add_First(string ars, string INDEX);

	// ������ ������ �뼱�� �߰��Ǵ� �Լ�
	void Add_Second(string ars, string INDEX);

	// ������ ���� �߰��ϴ� �Լ�
	void Add_Ratio(string ars_id, int H_Up, int H_Down, int W_Up, int W_Down, int S_Up, int S_Down, string h_up, string h_down, string w_up, string w_down, string s_up, string s_down);

	// �ð��� �������� �ִ� �Լ�
	void Add_Time_Data(string ars_id, vector<Day_by_Day> time);

	// ������ �ִ� ARS����Ʈ�� ����� ��
	void Delete_empty_ARS(vector<string> D_list);



	// �� ���ں��� �������ִ� ���� �Լ�
	void Sorting();


	Bus_ARS();
	~Bus_ARS();
};

