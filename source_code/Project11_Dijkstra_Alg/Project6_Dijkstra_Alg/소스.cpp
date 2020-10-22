#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <istream>
using namespace std;

vector<string> csv_read_row(istream &file, char delimiter);
unordered_map<int, int> Scale; // �������ȣ , Ȯ��(count)
unordered_map<int, int> Index;// �ε�����ȣ, �������ȣ
unordered_map<int, int> Reverse;// �������ȣ, �ε��� ��ȣ

// Data structure to store graph edges
struct Edge {
	int source, dest, weight;
};

// data structure to store heap nodes
struct Node {
	int vertex, weight;
};

// class to represent a graph object
class Graph
{
public:
	// construct a vector of vectors of Edge to represent an adjacency list
	vector<vector<Edge>> adjList;

	// Graph Constructor
	Graph(vector<Edge> const &edges, int N)
	{
		// resize the vector to N elements of type vector<Edge>
		adjList.resize(N);

		// add edges to the undirected graph
		for (Edge const &edge : edges)
		{
			// insert at end
			adjList[edge.source].push_back(edge);
		}
	}
};

void print_route(vector<int> const &prev, int i)
{
	if (i < 0)
		return;

	print_route(prev, prev[i]);

	std::unordered_map<int, int>::const_iterator got6 = Index.find(i); // �ε����� �������ȣ ã��
	std::unordered_map<int, int>::const_iterator got7 = Scale.find(got6->second); // �������ȣ�� Ȯ��ã��
	++Scale[got6->second];
}

// Comparison object to be used to order the heap
struct comp
{
	bool operator()(const Node &lhs, const Node &rhs) const
	{
		return lhs.weight > rhs.weight;
	}
};

// Run Dijkstra's algorithm on given graph
void shortestPath(Graph const& graph, int source, int N)
{
	// create min heap and push source node having distance 0
	priority_queue<Node, vector<Node>, comp> min_heap;
	min_heap.push({ source, 0 });

	// set infinite distance from source to v initially
	vector<int> dist(N, INT_MAX);

	// distance from source to itself is zero
	dist[source] = 0;

	// boolean array to track vertices for which minimum
	// cost is already found
	vector<bool> done(N, false);
	done[0] = true;

	// stores predecessor of a vertex (to print path)
	vector<int> prev(N, -1);

	// run till min_heap is not empty
	while (!min_heap.empty())
	{
		// Remove and return best vertex
		Node node = min_heap.top();
		min_heap.pop();

		// get vertex number
		int u = node.vertex;

		// do for each neighbor v of u
		for (auto i : graph.adjList[u])
		{
			int v = i.dest;
			int weight = i.weight;

			// Relaxation step
			if (!done[v] && (dist[u] + weight) < dist[v])
			{
				dist[v] = dist[u] + weight;
				prev[v] = u;
				min_heap.push({ v, dist[v] });
			}
		}

		// marked vertex u as done so it will not get picked up again
		done[u] = true;
	}

	for (int i = 1; i < N; ++i)
	{
		print_route(prev, i);
	}
}

// main function
int main()
{
	// initialize edges as per above diagram
	ifstream Bus("�����뼱��.csv");
	ofstream Output("Betweenness.csv");
	unordered_map<string, vector<int>>BusRoute; // ������ȣ , ������
	unordered_map<int, vector<int>>CheckSum; // �ߺ�üũ�� map[start - destlist]
	vector<string> content;
	vector<string>Busnumber; // ������ȣ
	vector<int>Stop; // ������
	vector<int>StopList; // ������ �� ����Ʈ
	bool Check = false; //ù �� üũ��
	string Busnum = "";
	int Count = 0; // ������ ���� üũ��

	while (Bus.good()) {
		content = csv_read_row(Bus, ',');
		if (Check&&Busnum != content[0])
		{
			BusRoute.insert({ Busnum, Stop });
			Busnumber.push_back(Busnum);
			Stop.clear();
		}
		Busnum = content[0];
		Stop.push_back(stoi(content[1]));
		
		std::unordered_map<int, int>::const_iterator got = Scale.find(stoi(content[1])); // ���� �������� �ƴ� �������̸�
		if (got== Scale.end()) {
			Scale.insert({ stoi(content[1]),0 }); // ����
			StopList.push_back(stoi(content[1]));
			Index.insert({ Count, stoi(content[1]) }); // �ε���, �������ȣ ����
			Count++;
		}
		Check = true;
	}
	Bus.close();

	for (int i = 0; i < Index.size(); i++) {
		std::unordered_map<int, int>::const_iterator got2 = Index.find(i); // �ε��� Ž��
		int a = got2->second;
		Reverse.insert({ a, i }); // �������ȣ , �ε���
	}

	
	Edge e;
	vector<Edge> edges; // �ߺ�üũ �ʿ�.CheckSum
	vector<int> Destlist;
	for (int i = 0; i < Busnumber.size(); i++) { // ��� ���� ��ȸ
		Busnum = Busnumber[i];
		std::unordered_map<string, vector<int>>::const_iterator got1 = BusRoute.find(Busnum);
		for (int j = 0; j < got1->second.size()-1; j++) { // �ش� ������ �� ������ ��ȸ
			std::unordered_map<int, int>::const_iterator got4 = Reverse.find(got1->second[j]);
			e.source = got4->second;
			std::unordered_map<int, int>::const_iterator got5 = Reverse.find(got1->second[j+1]);
			e.dest = got5->second;
			e.weight = 1;

			//������ �Ͱ� �ߺ�üũ
			std::unordered_map<int, vector<int>>::const_iterator got10 = CheckSum.find(got4->second); // got4�� ���� dest list�� ã��
			if (got10 == CheckSum.end()) // ���ʿ� e.source�� �������� ������
			{
				edges.push_back(e); // ������ �߰� + source , dest�� index�� ����
				Destlist.push_back(e.dest);
				CheckSum.insert({ e.source, Destlist }); // ���ο� ���� �߰�
			}
			else// e.source�� �����ϴ� ���
			{
				bool FindCheck = false;
				for (int i = 0; i < got10->second.size(); i++) // �ߺ��� �ֳ� �˻�
				{
					if (got10->second[i] == e.dest)// e.source�� �����ϰ�, e.dest�� �Ȱ����� �����Ѵٸ�.
					{
						FindCheck = true;
						break;
					}
				}
				if (!FindCheck) // e.source�� �����ϰ�, e.dest�� �������� �ʴ´ٸ�.
				{
					Destlist.assign(got10->second.begin(), got10->second.end());// ���� ����
					Destlist.push_back(e.dest);
					CheckSum.erase(e.source); // �����
					CheckSum.insert({ e.source, Destlist }); // �߰��ؼ� �ٽø������
				}
			}
			Destlist.clear();
		}
	}
	Graph graph(edges, Count); // count = ��� ��

	
	//���� ��ȸ�ϴ� �ڵ�[J���� I�� ���� ��Ʈ]
	int Percent = 1;
	for (int j = 0; j < Count; j++)
	{
		if (j / (Count/100) > Percent)
		{
			cout << Percent << "% path calculate" << endl;
			Percent++;
		}
		shortestPath(graph, j, Count);
	}

	for (int i = 0; i < StopList.size(); i++) {
		std::unordered_map<int, int>::const_iterator got10 = Index.find(i);
		Output << StopList[i] << ',' << Scale[got10->second] << ',' << endl;
	}

	return 0;
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