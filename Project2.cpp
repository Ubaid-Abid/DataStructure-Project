#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include<list>
#include<fstream>
using namespace std;

struct point {
public:
	int g1, g2;
	point(int g1 = -1, int g2 = -1) :g2(g2), g1(g1) {}
};

class heap {
private:
	pair<float, point>* heaping;
	int totalElemnts;
	int currSize;
	inline int parent(int i)const { return (i - 1) / 2; }
	inline int left(int i)const { return (i * 2) + 1; }
	inline int right(int i)const { return (i * 2) + 2; }

	void heapifyUp(int idx, int**& matrix) {
		if (idx > 0) {
			int p = parent(idx);
			if (heaping[idx].first < heaping[p].first) {
				point child = heaping[idx].second;
				point parent = heaping[p].second;

				swap(heaping[p], heaping[idx]);
				//here as indexes are swapped thus also adjusting them in array as well
				matrix[child.g1][child.g2] = p;
				matrix[child.g2][child.g1] = p;

				matrix[parent.g1][parent.g2] = idx;
				matrix[parent.g2][parent.g1] = idx;

				heapifyUp(p, matrix);
			}
		}
	}

	void heapifyDown(int idx, int**& matrix) {
		int l = left(idx);
		int r = right(idx);
		int minChild = idx;

		if (l < currSize && heaping[l].first < heaping[minChild].first)
			minChild = l;
		if (r < currSize && heaping[r].first < heaping[minChild].first)
			minChild = r;

		if (minChild != idx) {
			point current = heaping[idx].second;
			point child = heaping[minChild].second;

			swap(heaping[idx], heaping[minChild]);

			matrix[current.g1][current.g2] = minChild;
			matrix[current.g2][current.g1] = minChild;

			matrix[child.g1][child.g2] = idx;
			matrix[child.g2][child.g1] = idx;

			heapifyDown(minChild, matrix);
		}
	}

public:
	heap(int n) {
		totalElemnts = n * (n - 1) / 2;
		currSize = 0;
		heaping = new pair<float, point>[totalElemnts];
	}

	bool insert(const float distance, pair<int, int> p, int**& matrix) {
		if (currSize < totalElemnts) {
			heaping[currSize] = make_pair(distance, point(p.first, p.second));
			matrix[p.first][p.second] = currSize;
			matrix[p.second][p.first] = currSize;
			heapifyUp(currSize, matrix);
			currSize++;
			return true;
		}
		return false;
	}

	bool isEmpty() {
		return currSize == 0;
	}

	bool removeMin(point& out, int**& matrix) {
		if (isEmpty()) return false;
		out = heaping[0].second;
		heaping[0] = heaping[--currSize];

		if (currSize > 0) {
			point moved = heaping[0].second;
			matrix[moved.g1][moved.g2] = 0;
			matrix[moved.g2][moved.g1] = 0;
		}

		heapifyDown(0, matrix);
		return true;
	}

	void updateDistance(int g1, int g2, float newDist, int**& matrix) {
		int idx = matrix[g1][g2];
		if (idx == 0 || idx >= currSize) return;

		heaping[idx].first = newDist;
		heapifyUp(idx, matrix);
		heapifyDown(idx, matrix);
	}
};

class actual {
private:
	heap* s;
	vector<list<int>> v;
	int** matrix;
	pair<float, float>* points;
	int N, M;

	float getDistance(list<int> first, list<int> second) {
		float min = INT_MAX;
	
		for (auto& f : first) {
			for (auto& s_idx : second) {
				float distance = sqrt(pow(points[f].first - points[s_idx].first, 2) + pow(points[f].second - points[s_idx].second, 2));
				if (distance < min) {
					min = distance;
				}
			}
		}
		
		return  min ;
	}

public:
	actual() :N(-1), M(-1), s(nullptr), points(nullptr) {}

	void updateHash() {
		for (int i = 0; i < N; ++i) {
			if (!v[i].empty()) {
				for (int j = i + 1; j < N; ++j) {
					if (!v[j].empty()) {
						
						float t = getDistance(v[i], v[j]);
						s->insert(t, { i, j }, matrix);
					}
				}
			}
		}
	}

	bool loadData() {
		if (s) return false;
		else {
			ifstream file;
			file.open("points2.txt");
			if (!file.is_open()) return false;
			string line;
			getline(file, line);
			char temp;
			int i = 0;
			for (; line[i] != ','; ++i) {
				temp = line[i];
				if (temp < '0' || temp>'9') 
					return false;
				if (N == -1)
					N = 0;
				N *= 10;
				N += (temp - '0');
			}
			++i;//now gonna read M and for that have to move to next index in first line
			for (; line[i] != '\0'; ++i) {
				temp = line[i];
				if (temp < '0' || temp>'9') 
					return false;
				if (M == -1)
					M = 0;
				M *= 10;
				M += temp - '0';
			}
			if (N < M) return false;

			matrix = new int* [N];
			//cout << "Value of N=" << N << " and M=" << M << endl;
			for (int j = 0; j < N; ++j) {
				matrix[j] = new int[N]();
				
			}
			points = new pair<float, float>[N];
			s = new heap(N);

			i = 0;
			
			while (getline(file, line) && i < N) {
				int j = 0;
				float x = 0, y = 0;
				for (; line[j] != '\t'; ++j) {
					if (line[j] >= '0' && line[j] <= '9') {
						x = x * 10 + (line[j] - '0');
					}
					else if (line[j] == '.') {
						++j;
						float dec = line[j] - '0';
						x += dec / 10.0;
					}
				}
				++j;
				for (; line[j] != '\0'; ++j) {
					if (line[j] >= '0' && line[j] <= '9') {
						y = y * 10 + (line[j] - '0');
					}
					else if (line[j] == '.') {
						++j;
						float dec = line[j] - '0';
						y += dec / 10.0;
					}
				}
				
				points[i] = make_pair(x, y);	
				v.push_back({ i });
				i++;
			}
			updateHash();
			return true;
		}
	}

	void resizing() {
		int activeGroups = N;
		while (activeGroups > M) {
			point min;
			if (s->removeMin(min, matrix)) {
				int g1 = min.g1;
				int g2 = min.g2;

				if (v[g1].empty() || v[g2].empty()) continue;

				v[g1].splice(v[g1].end(), v[g2]);
				v[g2].clear(); // this will delete list and put nullptr important so that we avoid stupid indexing

				for (int k = 0; k < N; ++k) {
					if (k == g1 || k == g2 || v[k].empty()) continue;
					float newDistData = getDistance(v[g1], v[k]);
					s->updateDistance(g1, k, newDistData, matrix);
				}
				activeGroups--;
			}
		}
	}


	void printGroups() {
		int groupNum = 1;
		for (int i = 0; i < N; i++) {
			if (!v[i].empty()) {
				cout << "Group " << groupNum++ << ": ";

				bool first = true;
				for (auto idx : v[i]) {
					if (!first) cout << ", ";
					cout << "(" << points[idx].first << "," << points[idx].second << ")";
					first = false;
				}
				cout << endl;
			}
		}
	}
};

int main() {
	actual a;
	if (a.loadData()) {
		a.resizing();
		a.printGroups();
	}
	else
		cout << "Failed to load data." << endl;

}
