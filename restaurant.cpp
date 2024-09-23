#include "main.h"

int MAXSIZE = 0;
#define ull unsigned long long

class HANDLE;	//xu li cac lenh
class GojoRestaurant;	//nha hang cua gojo
class SukunaRestaurant;	//nha hang cua sukuna    
class HuffmanTree;	//cay huffman

class GojoRestaurant{
private: 
	class BinSearchTree{
	private:
		class Node{
		private:
			int result;
			Node* left;
			Node* right;
			friend class BinSearchTree;	//de class BST co the truy cap vao cac thuoc tinh cua class node
		public:
			Node(int key) : result(key), left(nullptr), right(nullptr){}
		};
		Node* root;
		queue<int> qTime;	//danh sach luu thu tu (thoi gian) den nha hang cua khach hang
	public:
		BinSearchTree() : root(nullptr){}
		~BinSearchTree(){
			while(!qTime.empty()){
				//lay ra khach hang dau tien trong danh sach va xoa 
				int temp = qTime.front();			
				qTime.pop();
				//xoa khach hang do ra khoi nha hang
				root = deleteRec(root ,temp);
			}
		}
		int size(){
			return qTime.size();
		}

		// them node vao cay
		Node* insertRec(Node* node, int key){
			if(node == nullptr){
				Node* newNode = new Node(key);
				return newNode;
			}
			if (key < node->result){
				node->left = insertRec(node->left, key);
			}
			//key lon hon HOAC BANG node->result -> them vao cay con ben phai
			else{
				node->right = insertRec(node->right, key);
			}
			return node;
		}

		void insert(int key){
			qTime.push(key);	//them vao danh sach thu tu khach den nha hang
			this->root = insertRec(this->root, key);
		}

		// xoa node trong cay
		Node* deleteRec(Node* node, int key){
			if (node == nullptr) return node;	//cay rong

			//key nho hon node->result -> tim node can xoa o cay con ben trai
			if (key < node->result){	
				node->left = deleteRec(node->left, key);
			}

			//key lon hon node->result -> tim node can xoa o cay con ben phai
			else if (key > node->result){
				node->right = deleteRec(node->right, key);
			}
			//tim thay node can xoa
			else{
				if (node->left == nullptr){
					Node* temp = node->right;
					delete node;
					return temp;
				}
				else if (node->right == nullptr){
					Node* temp = node->left;
					delete node;
					return temp;
				}

				//tim node co gia tri nho nhat cua cay con ben phai de thay the node can xoa
				Node* temp = node->right;
				while (temp->left != nullptr){
					temp = temp->left;
				}

				node->result = temp->result;	//thay the node can xoa bang node co gia tri nho nhat cua cay con ben phai
				node->right = deleteRec(node->right, temp->result);	//tiep tuc duyet xuong de xoa node co gia tri nho nhat cua cay con ben phai
			}
			return node;
		}	

		/* Tinh so cach de reorder lai cay (dung cho delete Y khach hang trong lenh KOKUSEN) */
		
		// transform tu cay sang mang theo thu tu post-order
		void BST_toVect(Node* node, vector<int>& res){
			if (node == nullptr) 
				return;

			// duyet cay push vao res theo thu tu: left, right, node
			BST_toVect(node->left, res);
			BST_toVect(node->right, res);
			res.push_back(node->result);
		}
		
		// toi uu hoa tinh to hop bang cach su dung cong thuc C(n,k) = C(n-1,k-1) + C(n-1,k)
		ull toHop(int n, int k){
			vector<vector<ull>> C(n + 1, vector<ull>(k + 1, 0));
			for (ull i = 0; i <= (ull)n; i++){
				for (ull j = 0; j <= min(i, (ull)k); j++){
					if (j == 0 || j == i){
						C[i][j] = 1;
					}
					else{
						C[i][j] = C[i - 1][j - 1] %MAXSIZE + C[i - 1][j] %MAXSIZE;
					}
				}
			}
			return C[n][k] % MAXSIZE;
		}


		// tinh so cach reorder cay
		ull numWaysReorder(vector<int>& v){
			if (v.size() <= 2) 	//chi co root hoac root va 1 node con -> chi co 1 cach reorder
				return 1;

			vector<int> vleft;
			vector<int> vright;
			int root = v[v.size() - 1];    //root la phan tu cuoi cung cua mang

			//chia mang thanh 2 phan
			for (unsigned int i = 0; i < v.size() - 1; i++){
				int x = v[i];
				//neu phan tu nho hon root -> them vao vleft
				if (x < root){
					vleft.push_back(x);
				} 
				//neu phan tu lon hon hoac bang root -> them vao vright
				else if (x >= root){
					vright.push_back(x);
				}
			}

			// tinh so cach reorder tren 2 mang con vleft & vright
			ull waysL = numWaysReorder(vleft)%MAXSIZE;
			ull waysR = numWaysReorder(vright)%MAXSIZE;

			// to hop cac cach chon goc cua 2 mang con vleft & vright
			ull tohop = toHop(vleft.size() + vright.size(), vleft.size()) %MAXSIZE;

			return (waysL * waysR * tohop)%MAXSIZE;
		}
		/* End of Tinh so cach de reorder lai cay */

		//xoa Y (so cach reorder cay) khach hang ra khoi nha hang Gojo
		void deleteYCus(){
			if (this->size() == 0) return;	//cay rong
			//dem so luong node can xoa
			vector<int> vect;
			BST_toVect(this->root, vect);
			ull numWays = numWaysReorder(vect)%MAXSIZE;	//numWays = Y khach hang can duoi
			
			//duoi Y khach hang ra khoi nha hang Gojo
			while (numWays > 0 && !qTime.empty()){		
				int temp = qTime.front();
				qTime.pop();
				this->root = deleteRec(this->root, temp);
				numWays--;
			}
		}

		//in cay in-order
		void printInOrd(Node* node){	
			if(node == nullptr)
				return;
			printInOrd(node->left);
			cout << node->result << endl;
			printInOrd(node->right);
		}

		void print(){
			if (this->size() == 0) return;	//cay rong
			printInOrd(root);
		}
	};

	vector<BinSearchTree> areaInRes;	//danh sach cac khu vuc trong nha hang Gojo, moi khu vuc dc quan li boi 1 cay BST
public:
	GojoRestaurant():areaInRes(MAXSIZE + 1){}	//id tu 1 -> MAXSIZE

	void insertArea(int result){
		//khach hang dc nhan vien sap xep vao khu vuc nhat dinh ID = result % MAXSIZE + 1
		int ID = result % MAXSIZE + 1;	
		areaInRes[ID].insert(result);
	}

	void deleteKOKUSEN(){
		//duyet qua tung khu vuc, duoi Y gian diep ra khoi khu vuc do
		for (int i = 1; i <= MAXSIZE; i++){
			areaInRes[i].deleteYCus();
		}
	}

	//di den khu vuc num, in ra danh sach khach hang trong khu vuc num
	void printLIMITLESS(int num){	
		if (num <= 0 || num > MAXSIZE) return;	//index khu vuc khong hop le
		areaInRes[num].print();
	}
};

class SukunaRestaurant{
private:
	class Node{
	private:
		int ID;		//id cua khu vuc
		list<int> resList;		//head luu danh sach result cua khach hang
		friend class SukunaRestaurant;	// cho phep class Sukuna_RESTAURANT truy cap vao cac thuoc tinh cua class Node
	public:
		Node(int ID) : ID(ID){}
		const int size(){
			return resList.size();
		}
		//them dau
		void insert(int result){
			resList.push_front(result);
		}
		//xoa n khach hang vao som nhat (cuoi ds)
		void deleteEarlyCus(int num){
			for (int i = 0; i < num; i++){
				if (!resList.empty()){
					cout << resList.back() << "-" << this->ID << endl;	//in thong tin khach bi duoi theo dinh dang "Result-ID/n"
					resList.pop_back();
				}
				else break;
			}
		}
		//ra number khách hàng mới đến gần nhất theo cơ chế LIFO (vao sau in truoc)
		void print(int num){
			for (list<int>::iterator it = resList.begin(); num > 0 && it != resList.end(); it++, num--){
				//định dạng: "ID-Result/n" với ID là số thứ tự của từng khu vực, Result là giá trị được tính ở lệnh LAPSE.
				cout << this->ID << "-";
				cout << *it << endl;	//result
			}
		}
	};
private:
	vector<Node*> areaInRes;	//danh sach cac khu vuc trong nha hang Sukuna, moi khu vuc dc quan li boi 1 list
	list<Node*> usedFreq;	//de tim ra khu vuc nao chua duoc su dung lau nhat, khu vuc nao vua dc su dung se o dau danh sach
private:
	bool cmp(int idx1, int idx2){
		if (areaInRes[idx1]->size() != areaInRes[idx2]->size()){
			return areaInRes[idx1]->size() < areaInRes[idx2]->size();
		}
		//neu 2 khu vuc co size bang nhau thi chon khu vuc nao co khach vao gan nhat dung truoc
		else{	
			//tim node o trong danh sach tan suat su dung
			auto it1 = find(usedFreq.begin(), usedFreq.end(), areaInRes[idx1]);
			auto it2 = find(usedFreq.begin(), usedFreq.end(), areaInRes[idx2]);
			return it1 == usedFreq.end() || (it2 != usedFreq.end() && distance(usedFreq.begin(), it1) > distance(usedFreq.begin(), it2));
		}
	}

	void reHeapUp(int idx){		//minheap
		if (idx == 0) return;	//root
		int parentIdx = (idx - 1) / 2;
		// neu node con nho hon node cha -> doi cho node con va node cha
		if (cmp(idx, parentIdx)){
			swap(areaInRes[idx], areaInRes[parentIdx]);
			reHeapUp(parentIdx);
		}
	}

	void reHeapDown(int idx){	//minheap
		unsigned int leftChildIdx = 2 * idx + 1;	
		unsigned int rightChildIdx = 2 * idx + 2;
	    int smallestIdx = idx;

		// neu left child nho hon idx thi cap nhat smallestIdx
		if (leftChildIdx < areaInRes.size() && cmp(leftChildIdx, smallestIdx)){
			smallestIdx = leftChildIdx;
		}

		// neu right child nho hon node idx thi cap nhat smallestIdx
		if (rightChildIdx < areaInRes.size() && cmp(rightChildIdx, smallestIdx)){
			smallestIdx = rightChildIdx;
		}

		// neu idx ko phai la node nho nhat thi doi cho node nho nhat va node idx
		if (smallestIdx != idx){
			swap(areaInRes[idx], areaInRes[smallestIdx]);
			reHeapDown(smallestIdx);
		}
	}

	/*neu node chua ton tai trong danh sach usedFreq thi them vao, 
	neu da ton tai thi di chuyen no len dau danh sach*/
	//de tim ra khu vuc nao chua duoc su dung lau nhat (o cuoi danh sach)
	void moveToTop(Node* node){
		// tim node can move to top trong danh sach tan suat su dung
		auto it = find(usedFreq.begin(), usedFreq.end(), node);
		
		//neu tim thay thi xoa no
		if (it != usedFreq.end()){
			usedFreq.erase(it);
		}

		// them node vao dau danh sach tan suat su dung
		usedFreq.push_front(node);
	}

	//xoa node ra khoi danh sach tan suat su dung
	void deleteNode(Node* node){
		// tim node can xoa
		auto it = find(usedFreq.begin(), usedFreq.end(), node);

		// neu tim thay thi xoa node
		if (it != usedFreq.end()){
			usedFreq.erase(it);
		}
	}

public:
	SukunaRestaurant(){}
	~SukunaRestaurant(){
		for(unsigned int i = 0; i < areaInRes.size(); i++) delete areaInRes[i];
	}
	
	void insertArea(int result){
		//khach hang dc nhan vien sap xep vao khu vuc nhat dinh ID = result % MAXSIZE + 1
		int ID = result % MAXSIZE + 1;	
		// duyet qua danh sach cac khu vuc, kiem tra xem khu vuc ID da co trong nha hang chua
		int idx = -1;	//tim thay thi luu vi tri heap vao idx
		for(unsigned int i = 0; i < areaInRes.size(); i++){
			if(areaInRes[i]->ID == ID){
				idx = i;
				break;
			}
		}
		// neu khu vuc ID chua co trong minheap thi them vao 
		if(idx == -1){
			areaInRes.push_back(new Node(ID));
			idx = areaInRes.size() - 1;
			areaInRes[idx]->insert(result);
			this->moveToTop(areaInRes[idx]);
			this->reHeapUp(idx);
		}
		
		else{// them khach hang moi vao khu vuc ID và reHeapDown khu vuc này xuống vì có số khách đông hơn
			areaInRes[idx]->insert(result);
			this->moveToTop(areaInRes[idx]);	// di chuyen khu vuc ID len dau danh sach tan suat su dung
			this->reHeapDown(idx);
		}
	}

	void deleteKEITEIKEN(int num){
		if (areaInRes.size() <= 0) return;	//nha hang rong
		
		// duoi num khach hang tai num khu vuc chua duoc su dung lau nhat va co so khach it nhat ra khoi nha hang
		vector<Node* > saveHeap(areaInRes.begin(), areaInRes.end());
		queue<Node*> delList;	//danh sach num khu vuc nho nhat de xoa (sorted)
		for(int i = 0; areaInRes.size() != 0; i++){
			if (i >= num) break;	//da duoi duoc num khu vuc

			Node* delArea = areaInRes[0];
			swap(areaInRes[0], areaInRes[areaInRes.size() - 1]);
			areaInRes.pop_back();
			this->reHeapDown(0);

			//them vao danh sach cac khu vuc dc xoa
			delList.push(delArea);
		}

		areaInRes = saveHeap;	//dung heap luu tru gan lai vao heap cu

		while(delList.size() != 0){
			//lay khu vuc o dau queue ra 
			Node* delArea = delList.front();
			delList.pop();

			//tim idx cua khu vuc can xoa
			int idx = 0;
			while(areaInRes[idx] != delArea) idx++;
			
			//xoa num khach o dau resList
			delArea->deleteEarlyCus(num);
			
			//neu xoa het khach trong khu vuc do thi xoa khu vuc do ra khoi heap
			if (delArea->size() == 0){
				//xoa khu vuc het khach trong heap
				swap(areaInRes[idx], areaInRes[areaInRes.size() - 1]);
				this->deleteNode(areaInRes[areaInRes.size() - 1]);	//xoa khoi khu vuc khoi nha hang sukuna
				delete areaInRes[areaInRes.size() - 1];
				areaInRes.pop_back();	//xoa khoi heap
			}
			else{
				this->moveToTop(areaInRes[idx]);	//di chuyen khu vuc ID len dau danh sach tan suat su dung
			}
			//reheapDown khu vuc xuong vi dang o dau hang
			this->reHeapDown(idx);
		}
	}	
	

	void printPreOrd(int idx, int num){	
		if((unsigned int)idx >= this->areaInRes.size()) return;
		//in num khach hang moi den gan nhat trong khu vuc idx (lifo: last in first print)
		this->areaInRes[idx]->print(num);	//in node hien tai
		printPreOrd(idx * 2 + 1, num);	//in node con ben trai
		printPreOrd(idx * 2 + 2, num);	//in node con ben phai
	}

	void printCLEAVE(int num){
		if (num <= 0) return;
		printPreOrd(0, num);	
	}
};

bool cmp(const pair<char, int>& a, const pair<char, int>& b){
	if (a.second != b.second)
		return a.second > b.second;
	else{
		if (isupper(a.first) && islower(b.first))
			return true;
		if (islower(a.first) && isupper(b.first))
			return false;
		return a.first > b.first;
	}
}

class HuffmanTree{
private:
	class Node{
	public:
		int weight;
		char key;
		Node* left;
		Node* right;
		friend class HuffmanTree;
	public:
		Node(int weight, char c = '\0',Node* left = nullptr, Node* right = nullptr ):  weight(weight), key(c), left(left), right(right){}
	};
	
	Node* root = nullptr;

public:
	~HuffmanTree(){
		destroyRec(root);
	}
	void destroyRec(Node* node){
		if (node == nullptr) return;
		destroyRec(node->left);
		destroyRec(node->right);
		delete node;
	}

	/*----- START: CHUAN HOA TEN -----*/
	static bool cmpCall(const pair<char, int>& a, const pair<char, int>& b){
		return cmp(a, b);
	}

	// ma hoa caesar tung ki tu
	void caesarEncode(char& c, int shift){
		if (isalpha(c)){	//chi ma hoa chu cai
			if (islower(c)){
				c = (c - 'a' + shift) % 26 + 'a'; 	//chuyen ve so, cong shift, chuyen ve chu
			}
			else{
				c = (c - 'A' + shift) % 26 + 'A';
			}
		}
	}

	//chuan hoa ten
	vector<pair<char, int>> standardizeName(string& name){
		// liet ke tan suat xuat hien cua cac ki tu trong ten theo thu tu tang dan
		map<char, int> freq_map;
		for (char c : name){
			freq_map[c]++;
		}   
		vector<pair<char, int>> freq_preCaesar(freq_map.begin(), freq_map.end());

		if (freq_preCaesar.size() < 3) return{};	//ten k	du de build cay
		//caesar
		for (pair<char, int> &it : freq_preCaesar){
			caesarEncode(it.first, it.second);
		}
		for (char &c : name){
			caesarEncode(c, freq_map[c]);
		}

		//ghep cac phan tu trung nhau sau khi ma hoa caesar
		map<char, int> combine_map;
		for (const auto& fp : freq_preCaesar){
			combine_map[fp.first] += fp.second;
		}
		vector<pair<char, int>> X_list(combine_map.begin(), combine_map.end());

		// sort giam dan theo tan suat, neu tan suat bang nhau thi sap xep theo thu tu alphabet
		sort(X_list.begin(), X_list.end(), cmpCall);
		return X_list;
	}
	/*----- END: CHUAN HOA TEN -----*/


	/*----- START: XAY DUNG CAY HUFFMAN -----*/
	int height(Node* node){
		if (!node) 
			return 0;
		return 1 + max(height(node->left), height(node->right));    //root + chieu cao cay con trai, phai
	}

	Node* rotateRight(Node* root){
		// x la node ben trai cua root, y la node ben phai cua root
		if (root == nullptr || root->left == nullptr) return root;
		Node* x = root->left;
		root->left = x->right;
		x->right = root;
		
		return x;
	}

	Node* rotateLeft(Node* root){
		// x la node ben trai cua root, y la node ben phai cua root
		if (root == nullptr || root->right == nullptr) return root; 
		Node* y = root->right; 
		root->right = y->left;
		y->left = root;
		
		return y;
	}

	Node* rotateNode(Node* node, int& cntRotate){
		if (node == nullptr) return nullptr;	//cay rong
		int leftHeight = (node->left != nullptr) ? height(node->left) : 0;
		int rightHeight = (node->right != nullptr) ? height(node->right) : 0;
		int balance = leftHeight - rightHeight;
		//lech ve phia ben trai
		if (balance > 1){
			int LLeftH = (node->left->left != nullptr) ? height(node->left->left) : 0;
			int LRightH = (node->left->right != nullptr) ? height(node->left->right) : 0;
			if (LLeftH >= LRightH){
				cntRotate++;
				return rotateRight(node);
			} 
			else{
				cntRotate++;
				node->left = rotateLeft(node->left); 
				return rotateRight(node);
			}
		} 
		//lech ve phia ben phai
		else if (balance < -1){
			int RRightH = (node->right->right != nullptr) ? height(node->right->right) : 0;
			int RLeftH = (node->right->left != nullptr) ? height(node->right->left) : 0;
			if (RRightH >= RLeftH){
				cntRotate++;
				return rotateLeft(node);
			} 
			else{
				cntRotate++;
				node->right = rotateRight(node->right);
				return rotateLeft(node);
			}
		}
		return node;
	}

	// balance theo preOrder, cntRotate >= 3 thi dung
	Node* balanceHuff(Node* node, int cntRotate=0){
		if (node == nullptr || cntRotate == 1){
			return node;
		}

		node = rotateNode(node, cntRotate);
		node->left = balanceHuff(node->left, cntRotate);
		node->right = balanceHuff(node->right, cntRotate);
		return node;
	}

	Node* buildHuffmanTree(vector<pair<char, int>> v){
		vector<Node*> huff;
		for(pair<char,int>& p : v){
			Node* newNode = new Node(p.second, p.first);
			huff.push_back(newNode);
		}

		while(huff.size() > 1){
			// lấy ra node nhỏ nhất thứ nhất và nhỏ nhất thứ 2 (phần tử cuối vector)
			Node* nodeleft = huff.back();
			huff.pop_back();
			if (huff.empty()) break;
			Node* noderight = huff.back();
			huff.pop_back();
			// tạo ra node nới có con bên trái là node nhỏ nhất và bên phải là node nhỏ nhất thứ 2 -> cập nhật weight, height của node mới
		
			Node* newNode = new Node(nodeleft->weight+noderight->weight, '\0', nodeleft, noderight);

			int cntRotate = 0;
			newNode = balanceHuff(newNode, cntRotate);
			newNode = balanceHuff(newNode, cntRotate);
			newNode = balanceHuff(newNode, cntRotate);

			vector<Node*>::iterator it;
			for (it = huff.begin(); it != huff.end(); it++){
				if ((*it)->weight <= newNode->weight){
					break;
				}
			}
			huff.insert(it, newNode);
		}       
		return huff[0];
	}	
	/*----- END: XAY DUNG CAY HUFFMAN -----*/


	/*----- START: GIAI MA TEN THEO CAY HUFFMAN -----*/
	// duyet cay de luu ma hoa nhi phan cua tung ki tu
	void binaryPerKey(vector<string>& binEncode, Node* node, string s){
		if (!node) return;

		if (!node->left && !node->right){
			binEncode[(unsigned char)node->key] = s;
		}   

		binaryPerKey(binEncode, node->left, s + "0");
		binaryPerKey(binEncode, node->right, s + "1");
	}

	int decodeHuffman(Node * root,string nameCaesar){
		if(root->left == nullptr && root->right == nullptr) return 0;

		//ma hoa nhi phan cua tung ki tu
		vector<string> binEncoded(256, "");
		binaryPerKey(binEncoded, root, "");

		//duyet nguoc de cong chuoi nhi phan
		string bin10bit = "";
		for (int i = nameCaesar.size()-1; i >= 0; i--){
			string rev = binEncoded[(unsigned char)nameCaesar[i]];
			reverse(rev.begin(), rev.end());
			bin10bit += rev;
			if (bin10bit.size() >= 10){
				bin10bit = bin10bit.substr(0, 10);
				break;
			}
		}

		//chuyen tu nhi phan sang thap phan
		int res = 0;
		for (char c : bin10bit){
			res = res * 2 + (c - '0');
		}

		return res;
	}
	/*----- END: GIAI MA TEN THEO CAY HUFFMAN -----*/

	int encodeCusName(string name){	
		//chuẩn hóa tên theo mã hóa caesar và xây dựng danh sách X
		vector<pair<char, int>> X_list = this->standardizeName(name);
		if(X_list.size() == 0) return -1;

		//huy cay cu
		this->destroyRec(root); 
		// xây dung cây huffman dựa trên danh sách X
		root = this->buildHuffmanTree(X_list);
	
		if(root->left == nullptr && root->right == nullptr) return 0; // chỉ có 1 node
		
		// giai ma ten theo cay huffman
		int result = this->decodeHuffman(root ,name);
		return result;
	}

	void printRec(Node* node){
		if(node == nullptr) return;
		printRec(node->left);
		if(node->key == '\0') cout << node->weight << endl;
		else cout << node->key << endl;
		printRec(node->right);
	}

	void printHUFFMAN(){
		printRec(root);
	}
};


class HANDLE{
private:
	GojoRestaurant gojoRes;	//hash table
	SukunaRestaurant sukunaRes;	//heap
	HuffmanTree newCus;
public:
	void LAPSE(string name){
		int result = newCus.encodeCusName(name);
		if(result == -1) return;	//res ko hop le

		if (result % 2 == 1) gojoRes.insertArea(result);
		else sukunaRes.insertArea(result);
		return;		
	}

	/*	GOJO RESTAURANT	  */
	void KOKUSEN(){
		gojoRes.deleteKOKUSEN();
	}
	
	void LIMITLESS(int num){
		gojoRes.printLIMITLESS(num);
	}

	/*	SUKUNA RESTAURANT	*/
	void KEITEIKEN(int num){
		sukunaRes.deleteKEITEIKEN(num);
	}
	
	void CLEAVE(int num){
		sukunaRes.printCLEAVE(num);
	}

	/* HUFFMAN */
	void HAND(){
		newCus.printHUFFMAN();
	}
};

void simulate(string filename)
{
	ifstream ss(filename);
	if (ss.fail())
		return;
	string str, name;
	int num;

	ss >> str;
	ss >> MAXSIZE;

	HANDLE* cmd = new HANDLE();
	
	while(ss >> str)
	{ 
        if(str == "LAPSE") // LAPSE <NAME>
       {
            ss >> name;
            cmd->LAPSE(name);
    	}
		else if(str == "KOKUSEN") // KOKUSEN
    	{
    		cmd->KOKUSEN();
		}
    	else if(str == "KEITEIKEN") // KEITEIKEN <NUM>
    	{
			ss >> num;
			cmd->KEITEIKEN(num);
		}
    	
		else if(str == "HAND") // HAND
		{
			cmd->HAND();	
		}
    	else if(str == "LIMITLESS") // LIMITLESS <NUM>
     	{   	
			ss >> num;
			cmd->LIMITLESS(num);
    	}
    	else // CLEAVE <NUM>
    	{
			ss >> num;
    		cmd->CLEAVE(num);
    	}
    }
	delete cmd;
}

