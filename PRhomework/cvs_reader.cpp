// cvs reading test

#include<ifstream>
#include<vector>
#include<string>
#include<iostream>

int cvs_reader(std::string filePath, std::vector<std::string> vec) {
	std::fstream read(filePath);
	for (int i = 0; i != 4; ++i)
	{
		std::string line;
		getline(filePath, line);
		vec.push_back(line);
	}
	read.close();
	return 0;
}

int main() {
	std::vector<std::string> vec;
	std::string str("C:\Users\Jasmine\Downloads\movies.csv");
	cvs_reader(str, vec);
	for (auto e : vec) std::cout << e << std::endl;
	return 0;
}


