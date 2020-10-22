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
unordered_map<int, int> Scale; // 정류장번호 , 확률(count)
unordered_map<int, int> Index;// 인덱스번호, 정류장번호
unordered_map<int, int> Reverse;// 정류장번호, 인덱스 번호

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

	std::unordered_map<int, int>::const_iterator got6 = Index.find(i); // 인덱스로 정류장번호 찾기
	std::unordered_map<int, int>::const_iterator got7 = Scale.find(got6->second); // 정류장번호로 확률찾기
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
	ifstream Bus("버스노선도.csv");
	ofstream Output("Betweenness.csv");
	unordered_map<string, vector<int>>BusRoute; // 버스번호 , 정류장
	unordered_map<int, vector<int>>CheckSum; // 중복체크용 map[start - destlist]
	vector<string> content;
	vector<string>Busnumber; // 버스번호
	vector<int>Stop; // 정류장
	vector<int>StopList; // 정류장 총 리스트
	bool Check = false; //첫 셀 체크용
	string Busnum = "";
	int Count = 0; // 정류장 갯수 체크용

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
		
		std::unordered_map<int, int>::const_iterator got = Scale.find(stoi(content[1])); // 기존 정류장이 아닌 정류장이면
		if (got== Scale.end()) {
			Scale.insert({ stoi(content[1]),0 }); // 삽입
			StopList.push_back(stoi(content[1]));
			Index.insert({ Count, stoi(content[1]) }); // 인덱스, 정류장번호 삽입
			Count++;
		}
		Check = true;
	}
	Bus.close();

	for (int i = 0; i < Index.size(); i++) {
		std::unordered_map<int, int>::const_iterator got2 = Index.find(i); // 인덱스 탐색
		int a = got2->second;
		Reverse.insert({ a, i }); // 정류장번호 , 인덱스
	}

	
	Edge e;
	vector<Edge> edges; // 중복체크 필요.CheckSum
	vector<int> Destlist;
	for (int i = 0; i < Busnumber.size(); i++) { // 모든 버스 순회
		Busnum = Busnumber[i];
		std::unordered_map<string, vector<int>>::const_iterator got1 = BusRoute.find(Busnum);
		for (int j = 0; j < got1->second.size()-1; j++) { // 해당 버스의 전 정류장 순회
			std::unordered_map<int, int>::const_iterator got4 = Reverse.find(got1->second[j]);
			e.source = got4->second;
			std::unordered_map<int, int>::const_iterator got5 = Reverse.find(got1->second[j+1]);
			e.dest = got5->second;
			e.weight = 1;

			//기존의 것과 중복체크
			std::unordered_map<int, vector<int>>::const_iterator got10 = CheckSum.find(got4->second); // got4에 대한 dest list를 찾음
			if (got10 == CheckSum.end()) // 애초에 e.source가 존재하지 않으면
			{
				edges.push_back(e); // 엣지에 추가 + source , dest는 index로 접근
				Destlist.push_back(e.dest);
				CheckSum.insert({ e.source, Destlist }); // 새로운 벡터 추가
			}
			else// e.source는 존재하는 경우
			{
				bool FindCheck = false;
				for (int i = 0; i < got10->second.size(); i++) // 중복이 있나 검사
				{
					if (got10->second[i] == e.dest)// e.source가 존재하고, e.dest도 똑같은게 존재한다면.
					{
						FindCheck = true;
						break;
					}
				}
				if (!FindCheck) // e.source가 존재하고, e.dest에 존재하지 않는다면.
				{
					Destlist.assign(got10->second.begin(), got10->second.end());// 벡터 복사
					Destlist.push_back(e.dest);
					CheckSum.erase(e.source); // 지우고
					CheckSum.insert({ e.source, Destlist }); // 추가해서 다시만들어줌
				}
			}
			Destlist.clear();
		}
	}
	Graph graph(edges, Count); // count = 노드 수

	
	//전부 순회하는 코드[J에서 I로 가는 루트]
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