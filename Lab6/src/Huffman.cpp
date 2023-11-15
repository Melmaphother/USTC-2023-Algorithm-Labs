#include "Huffman.h"
#include <cassert>
#include <cmath>

#define sep 20

std::map<char, unsigned>	freq;
std::map<char, std::string> code;

void IO::read(const std::string &data_path) {
	std::cout << "data_path: " << data_path <<' ' << data_path.size() << std::endl;
	this->in.open(data_path, std::ios::in);
	assert(this->in.is_open());
	char c;
	while (this->in.get(c)) {
		if (c != EOF) {
			if (c < 33 || c > 126) { continue; }
			freq[c]++;
		}
	}
	this->in.close();
}

void IO::write(const std::string				 &store_path,
			   const std::map<char, std::string> &code) {
	this->out.open(store_path, std::ios::out);
	this->out.setf(std::ios::left);
	this->out.width(sep);
	this->out << "Character";
	this->out.setf(std::ios::left);
	this->out.width(sep);
	this->out << "Frequency";
	this->out.setf(std::ios::left);
	this->out.width(sep);
	this->out << "Code" << std::endl;
	assert(this->out.is_open());
	for (const auto &[character, str] : code) {
		this->out.setf(std::ios::left);
		this->out.width(sep);
		this->out << character;
		this->out.setf(std::ios::left);
		this->out.width(sep);
		this->out << freq[character];
		this->out.setf(std::ios::left);
		this->out.width(sep);
		this->out << str << std::endl;
	}
	this->out.close();
}

HuffmanCode::HuffmanCode(const std::string &data_path,
						 const std::string &store_path) {
	io.read(data_path);
	BuildHuffmanTree();
	io.write(store_path, code);
}

void HuffmanCode::BuildHuffmanTree() {
	for (const auto &[character, freq] : freq) {
		this->minHeap.push(new HuffmanNode(character, freq));
	}
	while (this->minHeap.size() > 1) {
		auto left = this->minHeap.top();
		this->minHeap.pop();
		auto right = this->minHeap.top();
		this->minHeap.pop();
		auto new_node	= new HuffmanNode('\0', left->freq + right->freq);
		new_node->left	= left;
		new_node->right = right;
		this->minHeap.push(new_node);
	}
	this->root = this->minHeap.top();
	GenerateCode(this->root);
}

void HuffmanCode::GenerateCode(pHuffmanNode &root, std::string str) {
	std::string left_str  = str + "0";
	std::string right_str = str + "1";
	if (root->left == nullptr && root->right == nullptr) {
		code[root->character] = str;
		return;
	}
	if (root->left != nullptr) GenerateCode(root->left, left_str);
	if (root->right != nullptr) GenerateCode(root->right, right_str);
}

void HuffmanCode::CalcCompressRatio() {
	size_t origin	  = 0;
	size_t compressed = 0;
	size_t size		  = freq.size();
	assert(size > 0);
	size_t bits = static_cast<size_t>(std::ceil(std::log2(size)));
	for (const auto &[character, freq] : freq) { origin += freq * bits; }
	for (const auto &[character, str] : code) {
		compressed += freq[character] * str.size();
	}
	std::cout << "origin: " << origin << std::endl;
	std::cout << "compressed: " << compressed << std::endl;
	std::cout << "ratio: " << static_cast<double>(compressed) / origin
			  << std::endl;
}