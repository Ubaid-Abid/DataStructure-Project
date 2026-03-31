#include<iostream>
#include<fstream>
#include<vector>
#include<utility>
#include<string>
#include<list>
#include<iomanip>
using namespace std;
class point {
private:
	
		
public:
	float x, y;
	point(float x, float y):x(x),y(y) {}
	point(const point& other):x(other.x),y(other.y) {}
};
class group {
private:
	
	float** distances;
	vector<list<point>> groups;
	int M;
	int N;
	void regrouping(int row, int col) {
		/*in this function i will add elements of groups[col] in groups[row] and then will delete
		groups[col] also will reduce N and thats it we have merged two groups so yeah*/
		groups[row].splice(groups[row].end(), groups[col]);
		groups.erase(groups.begin()+ col);
		empty();
		--N;
		distances = new float* [N];
		for (int i = 0; i < N; ++i)
			distances[i] = new float[N]();

	}


public:
	group():distances(nullptr),N(0),M(0){}
	bool empty() {
		if (distances) {
			for (int i = 0; i < N; ++i) 
				delete[] distances[i];
			delete[] distances;
			distances = nullptr;
		}
		return false;
	}
	bool loadData() {
		if (distances) return false;//bcz we have already read file once so dobara is not possible
		else {
			ifstream file;
			file.open("points.txt");
			if (!file.is_open()) return false;
			string line;
			getline(file, line);
			char temp;
			int i = 0;
			for (; line[i] != ','; ++i) {//this loop will load N
			    temp = line[i];
				if (temp < '0' || temp>'9')
					return false;
				N *= 10;
				N += (temp-'0');
			}
			++i;//now loading M
			for (; line[i] != '\0'; ++i) {
				temp = line[i];
				if (temp < '0' || temp>'9')
					return false;
				M *= 10;
				M += temp-'0';
			}
			if (N < M)//bcz M will be less than or equal to N
				return false;
			distances = new float* [N];
			for (int j = 0; j < N; ++j)
				distances[j] = new float[N]();
			i = 0;//this counter will make sure that points are equal to N and not greater
			while (getline(file, line) && i<N) {
				++i;
				int j = 0;
				float x=0, y=0;
				for (; line[j] != ','; ++j) {
					if (line[j] >= '0' && line[j] <= '9') {
						x *= 10;
						x += line[j] - '0';
					}
					else if (line[j] == '.') {
						//case where we will have to make it float
						++j;//pointing at decimal value
						if (line[j] < '0' || line[j]>'9') return empty();//checking we have number after . or not
						float val = line[j] - '0';
						val /= 10.0;
						x += val;
						//now in my file there can be only one digit after decimal and if it violates then return false
						if (line[j+1] != ',')
							return empty();
					}
					else return empty();
				}//now x value has been stored and going for y value
				++j;//now pointing at value next to ,
				for (; line[j] != '\0'; ++j) {
					if (line[j] >= '0' && line[j] <= '9') {
						y *= 10;
						y += line[j] - '0';
					}
					else if (line[j] == '.') {
						//case where we will have to make it float
						++j;//pointing at decimal value
						if (line[j] < '0' || line[j]>'9') return empty();//same as above
						float val = line[j] - '0';
						val /= 10.0;
						y += val;
						//now in my file there can be only one digit after decimal and if it violates then return false
						if (line[j+1] != '\0')
							return empty();
					}
					else return empty();
				}
				//now storing it in vector
				//cout << "value of x=" << x << ",value of y=" << y << endl;
				

				groups.push_back(list<point>());
				groups[groups.size() - 1].push_back(point(x, y));
				
			}
			cout << "Value of i=" << i << endl;
			if (i != N) {
				//this means something is wrong with input file so exit 
				return empty();
			}
			return true;
		}
	}
	float getDistance(list<point>& first, list<point>& second) {
		float min = INT_MAX;
		for (const auto& a:first) {
			for (const auto& b : second) {
				float dis = sqrtf(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
				if (dis < min) 
					min = min < dis ? min : dis;	
			}
		}
		return min;
	}
	void compute_distances() {
		for (int i = 0; i < groups.size(); ++i) {
			distances[i][i] = 0.0;
			for (int j = i + 1; j < groups.size(); ++j) {
				//cout << groups[j].get_head()->point.first << "," << groups[j].get_head()->point.second << ")\n";
				float dist = getDistance(groups[i], groups[j]);
				//cout << "Distance for point " << i << "," << j << "=" << dist << endl;
				distances[i][j] = dist;
				distances[j][i] = dist;//just for safety as we will consider only upper triangle
			}
		}
	}
	void print() {
		for (int i = 0; i < groups.size(); ++i) {
			cout << "Group " << i + 1 << ":";
			bool first = false;//this will make sure that we do not print comma unevenly
			for (auto s : groups[i]) {
				if (first)cout << ", ";
				cout << "(" << s.x << "," << s.y << ")";
				first = true;
			}
			cout << endl;
		}
	}
	void make_groups() {
		compute_distances();
		while (N != M){
			//now we will find minimum in our matrix
			int row = 0, col = 0;float min = INT_MAX;
			//cout << groups.size() <<N<< endl;
			for (int i = 0; i < N; i++) {
				for (int j = i + 1; j < N; j++) {
					//cout << "for row =" << i << " and col=" << j << "value=" << distances[i][j] << " and min="<<min<<"\n";
					if (distances[i][j] < min)
						min = distances[i][j], row = i, col = j;
				}
			
			}
			//cout << "Min=" << min <<"row= "<<row <<"col = "<<col<<endl;
			//now we have to merge and in merging i will merge col in row
			regrouping(row, col);
			compute_distances();
		}
	}
};

int main() {
	group p;
	if (p.loadData())
	{
		cout << "File read successfuly:)\n";
		//p.print();
		p.compute_distances();
		//p.print();
		p.make_groups();
		p.print();
		p.empty();
	}
	else cout << "Something went wrong!\n";
	
}
