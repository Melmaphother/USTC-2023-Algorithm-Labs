#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <string>
using namespace std;

enum Color { RED, BLANK };

template <typename T> struct RBNode {
	T	  key;
	Color color;
	//* Automatically assigned the initial value 'nullptr'
	std::unique_ptr<RBNode<T>> left;
	std::unique_ptr<RBNode<T>> right;
	RBNode<T>				  *parent; //* it doesn't
	explicit RBNode(const T &key)
		: key(key), color(Color::RED), parent(nullptr) {}
};

template <typename T> class RBTree {
public:
	void RBInsert(const T &key);
	void Display(const string LNR_path, const string NLR_path,
				 const string LOT_path);

private:
	void LeftRotate(unique_ptr<RBNode<T>> &&x);
	void RightRotate(unique_ptr<RBNode<T>> &&x);

private:
	void RBInsert(unique_ptr<RBNode<T>> &&z);
	void RBInsertFixup(unique_ptr<RBNode<T>> &&z);

private:
	unique_ptr<RBNode<T>> root;
};

template <typename T> void RBTree<T>::RBInsert(const T &key) {
	unique_ptr<RBNode<T>> new_node = make_unique<RBNode<T>>(key);
	RBInsert(move(new_node));
}

template <typename T>
void RBTree<T>::Display(const string LNR_path, const string NLR_path,
						const string LOT_path) {
	ofstream f1, f2, f3;
	f1.open(LNR_path, ios::out);
	if (f1.is_open()) {
		if (root) {
			stack<RBNode<T> *> Stack1;
			RBNode<T>		  *Node = root.get();
			while (!Stack1.empty() || Node) {
				while (Node) {
					Stack1.push(Node);
					Node = Node->left.get();
				}
				Node			  = Stack1.top();
				string Node_color = Node->color ? "BLACK" : "RED";
				f1 << Node->key << ' ' << Node_color << endl;
				Stack1.pop();
				Node = Node->right.get(); //* be care that this is not
										  //* push(Node->right.get())
			}
		}
	} else {
		cout << "Can not open " << LNR_path << endl;
	}
	f1.close();
	f2.open(NLR_path, ios::out);
	if (f2.is_open()) {
		if (root) {
			stack<RBNode<T> *> Stack2;
			RBNode<T>		  *Node = root.get();
			Stack2.push(Node);
			while (!Stack2.empty()) {
				Node			  = Stack2.top();
				string Node_color = Node->color ? "BLACK" : "RED";
				f2 << Node->key << ' ' << Node_color << endl;
				Stack2.pop();
				if (Node->right) { Stack2.push(Node->right.get()); }
				if (Node->left) { Stack2.push(Node->left.get()); }
			}
		}
	} else {
		cout << "Can not open " << NLR_path << endl;
	}
	f2.close();
	f3.open(LOT_path, ios::out);
	if (f3.is_open()) {
		if (root) {
			queue<RBNode<T> *> Queue1;
			RBNode<T>		  *Node = root.get();
			Queue1.push(Node);
			while (!Queue1.empty()) {
				Node			  = Queue1.front();
				string Node_color = Node->color ? "BLACK" : "RED";
				f3 << Node->key << ' ' << Node_color << endl;
				Queue1.pop();
				if (Node->left) { Queue1.push(Node->left.get()); }
				if (Node->right) { Queue1.push(Node->right.get()); }
			}
		}
	} else {
		cout << "Can not open " << LOT_path << endl;
	}
	f3.close();
}

template <typename T> void RBTree<T>::LeftRotate(unique_ptr<RBNode<T>> &&x) {
	unique_ptr<RBNode<T>> y = move(x->right);
	x->right				= move(y->left);
	if (x->right) x->right->parent = x.get(); //* warning
	y->parent = x->parent;
	auto xp	  = x->parent; //* avoid that after move(x), we cann't find x
	if (!xp) {			   //* x is root
		RBNode<T>			 *x_tmp = x.release();
		root			   = move(y);
		root->left		   = unique_ptr<RBNode<T>>(x_tmp);
		root->left->parent = root.get();
	} else if (x == xp->left) {
		RBNode<T> *x_tmp = x.release();
		xp->left		 = move(y);
		/*
		 * 1.
		 * We can not use "xp->left->left = move(x);"
		 * assume unique_ptr p1, p2;
		 * The problem is that when p1 = p2, if we change what p1 point to,
		 * p2 will change at the same time. So we should save what p2 point to
		 * as p2_tmp, then modify what p1 point to.
		 * 2.
		 * Also, we can not use
		 * "xp->left->left = make_unique<RBNode<T>>(*x_tmp);"
		 * because there is no "default constructor" in struct RBNode
		 * 3. However, we can use
		 * default copy constructor of unique_ptr, like:
		 * unique_ptr<RBNode<T>>(x_tmp);
		 */
		xp->left->left		   = unique_ptr<RBNode<T>>(x_tmp);
		xp->left->left->parent = xp->left.get();
	} else {
		RBNode<T> *x_tmp		= x.release();
		xp->right				= move(y);
		xp->right->left			= unique_ptr<RBNode<T>>(x_tmp);
		xp->right->left->parent = xp->right.get();
	}
}

template <typename T> void RBTree<T>::RightRotate(unique_ptr<RBNode<T>> &&x) {
	unique_ptr<RBNode<T>> y = move(x->left);
	x->left					= move(y->right);
	if (x->left) x->left->parent = x.get(); //* warning
	y->parent = x->parent;
	auto xp	  = x->parent;
	if (!xp) { //* x is root
		RBNode<T> *x_tmp	= x.release();
		root				= move(y);
		root->right			= unique_ptr<RBNode<T>>(x_tmp);
		root->right->parent = root.get();
	} else if (x == xp->left) {
		RBNode<T> *x_tmp		= x.release();
		xp->left				= move(y);
		xp->left->right			= unique_ptr<RBNode<T>>(x_tmp);
		xp->left->right->parent = xp->left.get();
	} else {
		RBNode<T> *x_tmp		 = x.release();
		xp->right				 = move(y);
		xp->right->right		 = unique_ptr<RBNode<T>>(x_tmp);
		xp->right->right->parent = xp->right.get();
	}
}

template <typename T> void RBTree<T>::RBInsert(unique_ptr<RBNode<T>> &&z) {
	RBNode<T> *y = nullptr;
	RBNode<T> *x = root.get();
	while (x) {
		y = x;
		if (z->key < x->key) {
			x = x->left.get();
		} else {
			x = x->right.get();
		}
	}
	z->parent = y;
	if (!y) {
		root = move(z);
		RBInsertFixup(move(root));
	} else if (z->key < y->key) {
		y->left = move(z);
		RBInsertFixup(move(y->left));
	} else {
		y->right = move(z);
		RBInsertFixup(move(y->right));
	}
}

template <typename T> void RBTree<T>::RBInsertFixup(unique_ptr<RBNode<T>> &&z) {
	RBNode<T> *zp	 = z->parent;
	RBNode<T> *z_tmp = z.get();
	while (zp && zp->color == Color::RED) {
		RBNode<T> *zpp = zp->parent;
		if (zp == zpp->left.get()) { //* case 1, 2, 3
			RBNode<T> *y = zpp->right.get();
			if (y && y->color == Color::RED) { //* case 1
				cout << "case 1 ";
				y->color   = Color::BLANK;
				zp->color  = Color::BLANK;
				zpp->color = Color::RED;
				z_tmp	   = zpp;
				zp		   = zpp->parent;
			} else { //* case 2, 3
				bool is_3 = true;
				if (z_tmp == zp->right.get()) { //* case 2
					cout << "case 2 ";
					is_3 = false;
					LeftRotate(move(zpp->left));
					zp = zpp->left.get();
				}
				if (is_3) cout << "case 3 ";
				zp->color		= Color::BLANK; //* case 3
				zpp->color		= Color::RED;
				RBNode<T> *zppp = zpp->parent;
				if (!zppp) { //* zpp is root
					RightRotate(move(root));
				} else if (zpp == zppp->left.get()) {
					RightRotate(move(zppp->left));
				} else {
					RightRotate(move(zppp->right));
				}
			}
		} else { //* case 4, 5, 6
			RBNode<T> *y = zpp->left.get();
			if (y && y->color == Color::RED) { //* case 4
				cout << "case 4 ";
				y->color   = Color::BLANK;
				zp->color  = Color::BLANK;
				zpp->color = Color::RED;
                z_tmp	   = zpp;
				zp		   = zpp->parent;
			} else { //* case 5, 6
				bool is_6 = true;
				if (z_tmp == zp->left.get()) { //* case 5
					cout << "case 5 ";
					is_6 = false;
					RightRotate(move(zpp->right));
					zp = zpp->right.get();
				}
				if (is_6) cout << "case 6 ";
				zp->color		= Color::BLANK; //* case 6
				zpp->color		= Color::RED;
				RBNode<T> *zppp = zpp->parent;
				if (!zppp) { //* zpp is root
					LeftRotate(move(root));
				} else if (zpp == zppp->left.get()) {
					LeftRotate(move(zppp->left));
				} else {
					LeftRotate(move(zppp->right));
				}
			}
		}
	}
	root->color = Color::BLANK;
}

template <typename T>
void InsertAllNodes(RBTree<T> &RBT, const string Insert_path) {
	ifstream f;
	int		 num;
	T		 key;
	f.open(Insert_path, ios::in);
	if (f.is_open()) {
		f >> num;
		// cout << num << endl;
		for (int i = 0; i < num; i++) {
			f >> key;
			cout << key << ' ';
			RBT.RBInsert(key);
			cout << endl;
		}
	} else {
		cout << "Can not open" << Insert_path << endl;
	}
	f.close();
}

int main(int argc, char **argv) {
	RBTree<int> RBT;
	string		Insert_path = "insert.txt";
	string		LNR_path	= "Result/LNR.txt";
	string		NLR_path	= "Result/NLR.txt";
	string		LOT_path	= "Result/LOT.txt";
	InsertAllNodes<int>(RBT, Insert_path);
	RBT.Display(LNR_path, NLR_path, LOT_path);
	return 0;
}