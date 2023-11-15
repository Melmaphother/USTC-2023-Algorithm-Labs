#ifndef HUFFMAN
#define HUFFMAN

#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

std::map<char, unsigned>	freq;
std::map<char, std::string> code;

class IO {
	friend class HuffmanCode;

private:
	std::ifstream in;
	std::ofstream out;

public:
	void read(const std::string &data_path);
	void write(const std::string				 &store_path,
			   const std::map<char, std::string> &code);
};

typedef struct HuffmanNode {
	char		 character;
	unsigned	 freq;
	HuffmanNode *left, *right;
	HuffmanNode(char character, unsigned freq)
		: character(character), freq(freq), left(nullptr), right(nullptr) {}
} *pHuffmanNode;

struct compare {
	//* default is "less", l->freq > r->freq represents the priority of l is
	//* less than r
	bool operator()(pHuffmanNode l, pHuffmanNode r) {
		return (l->freq > r->freq);
	}
};

class HuffmanCode {
private:
	std::priority_queue<pHuffmanNode, std::vector<pHuffmanNode>, compare>
				 minHeap;
	pHuffmanNode root;
	IO			 io;

public:
	HuffmanCode(const std::string &data_path, const std::string &store_path);
	void CalcCompressRatio();

private:
	void BuildHuffmanTree();
	void GenerateCode(pHuffmanNode &root, std::string str = "");
};

#endif // HUFFMAN