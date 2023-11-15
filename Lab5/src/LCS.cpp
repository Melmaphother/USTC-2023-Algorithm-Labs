#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>

class LCS {
public:
	LCS(std::string data_path);
	std::pair<std::string, int> LCSNaive();
	int							LCSSaveSpace();
	int							LCSSaveSpaceFurther();

public:
	std::string getText1() const { return this->text1; }
	std::string getText2() const { return this->text2; }

private:
	std::string text1;
	std::string text2;
};

LCS::LCS(std::string data_path) {
	std::ifstream data_file(data_path);
	std::string	  line1, line2;
	if (data_file.is_open()) {
		std::getline(data_file, line1);
		std::getline(data_file, line2);
		this->text1 = line1.size() >= line2.size() ? line1 : line2; // * longer
		this->text2 = line1.size() >= line2.size() ? line2 : line1; //* shorter
		data_file.close();
	} else {
		std::cout << "Unable to open file";
	}
}

std::pair<std::string, int> LCS::LCSNaive() {
	int							  m = this->text1.size();
	int							  n = this->text2.size();
	std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			if (this->text1[i - 1] == this->text2[j - 1]) {
				dp[i][j] = dp[i - 1][j - 1] + 1;
			} else {
				dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
			}
		}
	}

	std::string lcs;
	int			i = m, j = n;
	while (i > 0 && j > 0) {
		if (this->text1[i - 1] == this->text2[j - 1]) {
			lcs.push_back(this->text1[i - 1]);
			i--;
			j--;
		} else if (dp[i - 1][j] > dp[i][j - 1]) {
			i--;
		} else {
			j--;
		}
	}

	std::reverse(lcs.begin(), lcs.end());
	return std::make_pair(lcs, dp[m][n]);
}

int LCS::LCSSaveSpace() {
	int				 m = this->text1.size();
	int				 n = this->text2.size();
	std::vector<int> dp_pre(n + 1, 0), dp_now(n + 1, 0);
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			if (this->text1[i - 1] == this->text2[j - 1]) {
				dp_now[j] = dp_pre[j - 1] + 1;
			} else {
				dp_now[j] = std::max(dp_pre[j], dp_now[j - 1]);
			}
		}
		/*
		 * Direct assignment operations between STL composed of
		 * statically allocated data types are overloaded in c++
		 */
		dp_pre = dp_now;
	}
	return dp_now[n];
}

int LCS::LCSSaveSpaceFurther() {
	int				 m = this->text1.size();
	int				 n = this->text2.size();
	std::vector<int> dp_now(n + 1, 0);
	int				 pre, now;
	for (int i = 1; i <= m; i++) {
		pre		  = 0;
		dp_now[0] = 0; // * not need but for clear
		for (int j = 1; j <= n; j++) {
			if (this->text1[i - 1] == this->text2[j - 1]) {
				now = pre + 1;
			} else {
				now = std::max(dp_now[j], dp_now[j - 1]);
			}
			pre		  = dp_now[j];
			dp_now[j] = now;
		}
	}
	return dp_now[n];
}

void ConsolePrint(LCS &lcs) {
	std::cout.setf(std::ios::left);
	std::cout.width(25);
	std::cout << "Text1: ";
	std::cout << lcs.getText1() << std::endl;
	std::cout.width(25);
	std::cout << "Text2: ";
	std::cout << lcs.getText2() << std::endl;
	auto lcs_pair = lcs.LCSNaive();
	std::cout.width(25);
	std::cout << "Naive dp: ";
	if (lcs_pair.first.size() > 0) {
		std::cout << "lcs: " << lcs_pair.first << std::endl;
		std::cout.width(25);
		std::cout << " ";
	}
	std::cout << "length: " << lcs_pair.second << std::endl;
	std::cout.width(25);
	std::cout << "Save space dp: ";
	std::cout << "length: " << lcs.LCSSaveSpace() << std::endl;
	std::cout.width(25);
	std::cout << "Further save space dp: ";
	std::cout << "length: " << lcs.LCSSaveSpaceFurther() << std::endl;
}

bool cmdOptionExists(char **begin, char **end, const std::string &option) {
	return std::find(begin, end, option) != end;
}

int main(int argc, char **argv) {
	std::string data_path = "data.txt";
	if (cmdOptionExists(argv, argv + argc, "-h")) {
		std::cout << "-h : help message" << std::endl;
		std::cout << "-f [file path] : input file path" << std::endl;
	}
	if (cmdOptionExists(argv, argv + argc, "-f")) {
		if (argc >= 3) {
			data_path = argv[2];
		} else {
			std::cout << "Lack of parameter." << std::endl;
			std::cout << "Use \"-h\" to get some help." << std::endl;
		}
	}
	LCS lcs(data_path);
	ConsolePrint(lcs);
}