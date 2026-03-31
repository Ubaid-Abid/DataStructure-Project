#include<iostream>
#include<string>
#include<queue>
using namespace std;

class productRecord {
private:
	int pid;
	string name;
	string category;
public:
	productRecord(int pid,string name,string category):pid(pid),name(name),category(category){}
	int get_id() { return pid; }
	void print_product() {
		cout << "ID of product:" << pid << "\nName of product:" << name << "\nCategory of product:" << category << endl;
	}
	
};

class inventoryBST {
private:
	class tnode {
	public:
		int pid;
		productRecord* prodrec;
		tnode* left, * right;
		tnode(int pid,productRecord*prod,tnode*left=nullptr,tnode*right=nullptr):pid(pid),prodrec(prod),left(left),right(right){}

	};
	tnode* root;
	int size;
	void print_post(tnode* root) {
		if (root)
		{
			cout << root->prodrec->get_id() << " ";
			print_post(root->left);
			print_post(root->right);
		}
	}
	bool remove(int id, tnode*& root) {
		if (root) {
			if (root->pid == id) {
				//case where node has been found
				if (root->left && root->right) {
					tnode* temp = root->left;
					while (temp->right)
						temp = temp->right;
					root->pid = temp->pid;
					productRecord* tmpRec = root->prodrec;
					root->prodrec = temp->prodrec;
					temp->prodrec = tmpRec;
					return  remove(temp->pid, root->left);
				}
				else if (root->left) {
					tnode* temp = root;
					root = root->left;
					delete temp->prodrec;
					delete temp;
				}
				else if (root->right) {
					tnode* temp = root;
					root = root->right;
					delete temp->prodrec;
					delete temp;
				}
				else {
					delete root->prodrec;
					delete root;
					root = nullptr;

				}
				return true;
			}
			else if (root->pid > id)
				return remove(id, root->left);
			else
				return remove(id, root->right);
		}
		else return false;
		
	}
	void store_values(vector<int>& arr, tnode* root) {
		if (root->left) {
			store_values(arr, root->left);
		}
		arr.push_back(root->pid);
		if (root->right)
			store_values(arr, root->right);
	}
	tnode* insert(productRecord*& product, tnode*& root) {
		if (root == nullptr) {
			//base case
			size++;
			cout << "Product added in record successfully!\n";
			return root = new tnode(product->get_id(), product);

		}
		tnode* child;
		if (product->get_id() == root->pid) {
			cout << "As id matched thus not inserting in record!\n";
			delete product;
			product = nullptr;
			return root;
		}
		else if (product->get_id() < root->pid)
			child = insert(product, root->left);
		else child = insert(product, root->right);
		if (root->left == child)
			rightRotate(root);
		else if(root->right==child)
			leftRotate(root);
		return root;
	}
	void leftRotate(tnode*& node) {
		if (node && node->right) {
			tnode* child = node->right;
			tnode* orphan = child->left;
			child->left = node;
			node->right = orphan;
			node = child;
		}
	}
	void rightRotate(tnode*& node) {
		if (node && node->left) {
			tnode* child = node->left;
			tnode* orphan = child->right;
			child->right = node;
			node->left = orphan;
			node = child;
		}
	}
	void printAll(tnode* root) {
		if (root) {
			printAll(root->left);
			cout << root->pid << " ";
			printAll(root->right);
		}
	}
	bool search(int pid, int& level, tnode*& root,int currentlevel=0) {
		if (root) {
			bool f;
			if (root->pid == pid) {
				cout << "Root found!Details are as follows:\n";
				root->prodrec->print_product();
				return true;
			}
			else if (root->pid > pid) {
				
				f= search(pid, level, root->left, currentlevel + 1);
				
			}
			else {
				f= search(pid, level, root->right, currentlevel + 1);
			}

			if (f) {
				//means we have to check for level and rotate accordingly
				if (currentlevel >= level) {
					//means we have to rotate
					if (root->left && root->left->pid == pid) {
						rightRotate(root);
					}
					else if(root->right&& root->right->pid==pid){
						leftRotate(root);
					}
				}
				
				return true;
			}
			else return false;
		}
		else return false;
	}

	bool organize(int id, tnode*& root, int& current) {
		if (root) {
			if (current == id) {
				cout << "new root :" << root->pid << endl;
				return true;
			}
			else {
				if (organize(id,root->left,++current)) {
					rightRotate(root);
					return true;
				}
				else {
					
					if (organize(id, root->right, ++current)) {
						leftRotate(root);
						return true;
					}
				}
			}
		}
		else return false;//this is for nullptr
	}
	void clear(tnode*& root) {
		if (root) {
			if (root->left)
				clear(root->left);
			if (root->right)
				clear(root->right);
			delete root->prodrec;
			delete root;
			root = nullptr;
		}
		else root = nullptr;
	}

public:
	inventoryBST():root(nullptr),size(0){}
	~inventoryBST() {
		clear(root);
	}
	
	void insert(productRecord*&product) {
		root=insert(product, root);
		
	}
	
	void remove(int id) {
		if (root) {
			
			if (remove(id, root))
			{	
				cout << "Product deleted!\n";
				size--;
			}
			else
				cout << "Product not found!\n";
		}
		else {
			cout << "There is no record at all :(\n";
		}
	
	}
	
	void print(int id) {
		if (root) {
			tnode* temp = root;
			bool flag = true;//this print is using a normal loop no recursion
			while (flag) {
				if (temp) {
					if (temp->pid == id) {
						temp->prodrec->print_product();
						flag = false;
					}
					else if (temp->pid < id)
						temp =temp->right;
					else
						temp= temp->left;
				}
				else flag = false;
			}
		}
		else {
			cout << "No Record of any product :(\n";
		}
	}
	void printall() {
		if (root)
		{
			cout << "Head is :" << root->pid << endl;
			printAll(root), cout << endl;
		}
		else cout << "There is no record at all!\n";
	}
	void search(int pid, int level) {
		if (root) {
			
			
			
			if (search(pid, level, root)) {
				//this is just so that product is found and also so that we can see new tree as well
				cout << "Now tree after searching:\n";
				print_post(root);
				cout << endl;
			}
			else cout << "No record of this product found :(\n";
		}
		else cout << "Record book is empty!\n";
	}
	
	void organize() {
		if (root) {
			vector<int>arr;
			store_values(arr,root);
			int median = arr.size() / 2;
			int median_root = arr[median];
			search(median_root, 0);
            //here i have used search function which will automatically bring median value to top and total complexity is still
			//O(n) ,first o(n) for vector creation and second for search but search will take less time in most cases
		}
	}

};
void insert_record(inventoryBST& record) {
	int id;
	string name, category;
	do {
		cout << "Enter product id:";
		cin >> id;
	} while (id < 0);
	cin.ignore();
    cout << "Enter name:";
    getline(cin, name);
	cout << "Enter product category:";
	getline(cin, category);
	productRecord* temp = new productRecord(id, name, category);//creating product record
	record.insert(temp);

}
void remove_record(inventoryBST& record) {
	int id;
	cout << "Enter product id:";
	cin >> id;
	record.remove(id);
}
void search_record(inventoryBST& record) {
	int pid, level;
	do {
		cout << "Enter product record:";
		cin >> pid;
	} while (pid < 0);
	do {
		cout << "Enter level:";
		cin >> level;
	} while (level < 0);
	record.search(pid, level);
}
void print_product(inventoryBST& record) {
	int pid;
	do {
		cout << "Enter product id to print:";
		cin >> pid;
	} while (pid < 0);
	record.print(pid);
}

void running() {
	inventoryBST record = inventoryBST();
	bool flag = true;
	int option;
	while (flag) {
		cout << "------------Select from following options:------------\n";
		cout << "1-Insert product.\n2-Remove product.\n3-Search product.\n4-Print product.\n5-Print record.\n6-Organize record.\n7-Exit.\n";
		//i have make function so that we can ensure encapsulation and abstraction 
		do {
			cout << "Enter your choice here:";
			cin >> option;
		} while (option<1 || option>7);
		if (option == 1) {
			insert_record(record);
		}
		else if (option == 2) {
			remove_record(record);
		}
		else if (option == 3) {
			search_record(record);
		}
		else if (option == 4) {
			print_product(record);
		}
		else if (option == 5) {
			record.printall();
		}
		else if (option == 6) {
			record.organize();
		}
		else if (option == 7) {
			flag = false;
		}
	}
}


int main() {
	//abstraction is performed here
	running();

}
