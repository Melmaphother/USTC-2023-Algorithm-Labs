#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <string>

enum Color { RED, BLANK };

template <typename T> struct RBNode {
	T	  key;
	Color color;
	int	  max;
	//* Automatically assigned the initial value 'nullptr'
	std::unique_ptr<RBNode<T>> left;
	std::unique_ptr<RBNode<T>> right;
	RBNode<T>				  *parent; //* it doesn't
	explicit RBNode(const T &key)
		: key(std::make_pair(key.first, key.second)), color(Color::RED),
		  max(key.second), parent(nullptr) {}
};

std::ofstream &operator<<(std::ofstream &out, RBNode<std::pair<int, int>> *p) {
	out << '[' << p->key.first << ", " << p->key.second << ']'
		<< " max: " << p->max << ' ';
	return out;
}

bool operator<(std::pair<int, int> &key1, std::pair<int, int> &key2) {
	return key1.first < key2.first;
}

std::ostream &operator<<(std::ostream &out, std::pair<int, int> &key) {
	out << '[' << key.first << ", " << key.second << ']' << std::endl;
	return out;
}

inline int max(const int a, const int b) { return a > b ? a : b; }

inline int max(const int a, const int b, const int c) {
	return a > b ? (a > c ? a : c) : (b > c ? b : c);
}

template <typename T> class RBTree {
public:
	void RBInsert(const T &key);
	void Display(const std::string LNR_path, const std::string NLR_path,
				 const std::string LOT_path);
	void IntervalSearch(const T &interval, std::vector<T> &result);

private:
	void LeftRotate(std::unique_ptr<RBNode<T>> &&x);
	void RightRotate(std::unique_ptr<RBNode<T>> &&x);

private:
	void RBInsert(std::unique_ptr<RBNode<T>> &&z);
	void RBInsertFixup(std::unique_ptr<RBNode<T>> &&z);

private:
	bool overlap(const T &x, const T &interval);
	void IntervalSearch(const RBNode<T> *root, const T &interval,
						std::vector<T> &result);

private:
	std::unique_ptr<RBNode<T>> root;
};

template <typename T> void RBTree<T>::RBInsert(const T &key) {
	std::unique_ptr<RBNode<T>> new_node = std::make_unique<RBNode<T>>(key);
	RBInsert(std::move(new_node));
	Display("Result/LNR.txt", "Result/NLR.txt", "Result/LOT.txt");
}

template <typename T>
void RBTree<T>::Display(const std::string LNR_path, const std::string NLR_path,
						const std::string LOT_path) {
	std::ofstream f1, f2, f3;
	f1.open(LNR_path, std::ios::out);
	if (f1.is_open()) {
		if (root) {
			std::stack<RBNode<T> *> stack1;
			RBNode<T>			   *Node = root.get();
			while (!stack1.empty() || Node) {
				while (Node) {
					stack1.push(Node);
					Node = Node->left.get();
				}
				Node				   = stack1.top();
				std::string Node_color = Node->color ? "BLACK" : "RED";
				f1 << Node << ' ' << Node_color << std::endl;
				stack1.pop();
				Node = Node->right.get(); //* be care that this is not
										  //* push(Node->right.get())
			}
		}
	} else {
		std::cout << "Can not open " << LNR_path << std::endl;
	}
	f1.close();
	f2.open(NLR_path, std::ios::out);
	if (f2.is_open()) {
		if (root) {
			std::stack<RBNode<T> *> stack2;
			RBNode<T>			   *Node = root.get();
			stack2.push(Node);
			while (!stack2.empty()) {
				Node				   = stack2.top();
				std::string Node_color = Node->color ? "BLACK" : "RED";
				f2 << Node << ' ' << Node_color << std::endl;
				stack2.pop();
				if (Node->right) { stack2.push(Node->right.get()); }
				if (Node->left) { stack2.push(Node->left.get()); }
			}
		}
	} else {
		std::cout << "Can not open " << NLR_path << std::endl;
	}
	f2.close();
	f3.open(LOT_path, std::ios::out);
	if (f3.is_open()) {
		if (root) {
			std::queue<RBNode<T> *> Queue1;
			RBNode<T>			   *Node = root.get();
			Queue1.push(Node);
			while (!Queue1.empty()) {
				Node				   = Queue1.front();
				std::string Node_color = Node->color ? "BLACK" : "RED";
				f3 << Node << ' ' << Node_color << std::endl;
				Queue1.pop();
				if (Node->left) { Queue1.push(Node->left.get()); }
				if (Node->right) { Queue1.push(Node->right.get()); }
			}
		}
	} else {
		std::cout << "Can not open " << LOT_path << std::endl;
	}
	f3.close();
}

template <typename T>
void RBTree<T>::IntervalSearch(const T &interval, std::vector<T> &result) {
	if (interval.first > interval.second) return;
	IntervalSearch(root.get(), interval, result);
}

template <typename T>
void RBTree<T>::LeftRotate(std::unique_ptr<RBNode<T>> &&x) {
	if (x->left) {
		if (x->right) {
			if (x->right->left) {
				x->max = max(x->key.second, x->left->max, x->right->left->max);
			} else {
				x->max = max(x->key.second, x->left->max);
			}
		} else {
			x->max = max(x->key.second, x->left->max);
		}
	} else {
		if (x->right) {
			if (x->right->left) {
				x->max = max(x->key.second, x->right->left->max);
			} else {
				x->max = x->key.second;
			}
		} else {
			x->max = x->key.second;
		}
	}
	if (x->right) { x->right->max = max(x->right->max, x->max); }
	std::unique_ptr<RBNode<T>> y = std::move(x->right);
	x->right					 = std::move(y->left);
	if (x->right) x->right->parent = x.get(); //* warning
	y->parent = x->parent;
	auto xp	  = x->parent; //* avoid that after std::move(x), we cann't find x
	if (!xp) {			   //* x is root
		RBNode<T> *x_tmp   = x.release();
		root			   = std::move(y);
		root->left		   = std::unique_ptr<RBNode<T>>(x_tmp);
		root->left->parent = root.get();
	} else if (x == xp->left) {
		RBNode<T> *x_tmp = x.release();
		xp->left		 = std::move(y);
		/*
		 * 1.
		 * We can not use "xp->left->left = std::move(x);"
		 * assume unique_ptr p1, p2;
		 * The problem is that when p1 = p2, if we change what p1 point to,
		 * p2 will change at the same time. So we should save what p2 point to
		 * as p2_tmp, then modify what p1 point to.
		 * 2.
		 * Also, we can not use
		 * "xp->left->left = std::make_unique<RBNode<T>>(*x_tmp);"
		 * because there is no "default constructor" in struct RBNode
		 * 3. However, we can use
		 * default copy constructor of unique_ptr, like:
		 * unique_ptr<RBNode<T>>(x_tmp);
		 */
		xp->left->left		   = std::unique_ptr<RBNode<T>>(x_tmp);
		xp->left->left->parent = xp->left.get();
	} else {
		RBNode<T> *x_tmp		= x.release();
		xp->right				= std::move(y);
		xp->right->left			= std::unique_ptr<RBNode<T>>(x_tmp);
		xp->right->left->parent = xp->right.get();
	}
}

template <typename T>
void RBTree<T>::RightRotate(std::unique_ptr<RBNode<T>> &&x) {
	if (x->right) {
		if (x->left) {
			if (x->left->right) {
				x->max = max(x->key.second, x->right->max, x->left->right->max);
			} else {
				x->max = max(x->key.second, x->right->max);
			}
		} else {
			x->max = max(x->key.second, x->right->max);
		}
	} else {
		if (x->left) {
			if (x->left->right) {
				x->max = max(x->key.second, x->left->right->max);
			} else {
				x->max = x->key.second;
			}
		} else {
			x->max = x->key.second;
		}
	}
	if (x->left) { x->left->max = max(x->left->max, x->max); }
	std::unique_ptr<RBNode<T>> y = std::move(x->left);
	x->left						 = std::move(y->right);
	if (x->left) x->left->parent = x.get(); //* warning
	y->parent = x->parent;
	auto xp	  = x->parent;
	if (!xp) { //* x is root
		RBNode<T> *x_tmp	= x.release();
		root				= std::move(y);
		root->right			= std::unique_ptr<RBNode<T>>(x_tmp);
		root->right->parent = root.get();
	} else if (x == xp->left) {
		RBNode<T> *x_tmp		= x.release();
		xp->left				= std::move(y);
		xp->left->right			= std::unique_ptr<RBNode<T>>(x_tmp);
		xp->left->right->parent = xp->left.get();
	} else {
		RBNode<T> *x_tmp		 = x.release();
		xp->right				 = std::move(y);
		xp->right->right		 = std::unique_ptr<RBNode<T>>(x_tmp);
		xp->right->right->parent = xp->right.get();
	}
}

template <typename T> void RBTree<T>::RBInsert(std::unique_ptr<RBNode<T>> &&z) {
	RBNode<T> *y = nullptr;
	RBNode<T> *x = root.get();
	while (x) {
		x->max = max(x->max, z->key.second); //* important
		y	   = x;
		if (z->key < x->key) {
			x = x->left.get();
		} else {
			x = x->right.get();
		}
	}
	z->parent = y;
	if (!y) {
		root = std::move(z);
		RBInsertFixup(std::move(root));
	} else if (z->key < y->key) {
		y->left = std::move(z);
		RBInsertFixup(std::move(y->left));
	} else {
		y->right = std::move(z);
		RBInsertFixup(std::move(y->right));
	}
}

template <typename T>
void RBTree<T>::RBInsertFixup(std::unique_ptr<RBNode<T>> &&z) {
	RBNode<T> *zp	 = z->parent;
	RBNode<T> *z_tmp = z.get();
	while (zp && zp->color == Color::RED) {
		RBNode<T> *zpp = zp->parent;
		if (zp == zpp->left.get()) { //* case 1, 2, 3
			RBNode<T> *y = zpp->right.get();
			if (y && y->color == Color::RED) { //* case 1
#ifdef DEBUG
				std::cout << "case 1 ";
#endif
				y->color   = Color::BLANK;
				zp->color  = Color::BLANK;
				zpp->color = Color::RED;
				z_tmp	   = zpp;
				zp		   = zpp->parent;
			} else { //* case 2, 3
				bool is_3 = true;
				if (z_tmp == zp->right.get()) { //* case 2
#ifdef DEBUG
					std::cout << "case 2 ";
#endif
					is_3 = false;
					LeftRotate(std::move(zpp->left));
					zp = zpp->left.get();
				}
				if (is_3) {
#ifdef DEBUG
					std::cout << "case 3 ";
#endif
				}
				zp->color		= Color::BLANK; //* case 3
				zpp->color		= Color::RED;
				RBNode<T> *zppp = zpp->parent;
				if (!zppp) { //* zpp is root
					RightRotate(std::move(root));
				} else if (zpp == zppp->left.get()) {
					RightRotate(std::move(zppp->left));
				} else {
					RightRotate(std::move(zppp->right));
				}
			}
		} else { //* case 4, 5, 6
			RBNode<T> *y = zpp->left.get();
			if (y && y->color == Color::RED) { //* case 4
#ifdef DEBUG
				std::cout << "case 4 ";
#endif
				y->color   = Color::BLANK;
				zp->color  = Color::BLANK;
				zpp->color = Color::RED;
				z_tmp	   = zpp;
				zp		   = zpp->parent;
			} else { //* case 5, 6
				bool is_6 = true;
				if (z_tmp == zp->left.get()) { //* case 5
#ifdef DEBUG
					std::cout << "case 5 ";
#endif
					is_6 = false;
					RightRotate(std::move(zpp->right));
					zp = zpp->right.get();
				}
				if (is_6) {
#ifdef DEBUG
					std::cout << "case 6 ";
#endif
				}
				zp->color		= Color::BLANK; //* case 6
				zpp->color		= Color::RED;
				RBNode<T> *zppp = zpp->parent;
				if (!zppp) { //* zpp is root
					LeftRotate(std::move(root));
				} else if (zpp == zppp->left.get()) {
					LeftRotate(std::move(zppp->left));
				} else {
					LeftRotate(std::move(zppp->right));
				}
			}
		}
	}
	root->color = Color::BLANK;
}

template <typename T>
inline bool RBTree<T>::overlap(const T &x, const T &interval) {
	return !(x.second < interval.first || interval.second < x.first);
}

template <typename T>
void RBTree<T>::IntervalSearch(const RBNode<T> *root, const T &interval,
							   std::vector<T> &result) {
	if (overlap(root->key, interval)) { result.push_back(root->key); }
	if (root->left && root->left->max >= interval.first) {
		IntervalSearch(root->left.get(), interval, result);
	}
	if (root->right && root->right->max >= interval.first) {
		IntervalSearch(root->right.get(), interval, result);
	}
}