// PRhomework.cpp : 定义控制台应用程序的入口点。
//鸢尾花数据集分解聚类

#include "stdafx.h"
#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
#include<map>
#include<math.h>



class irisData
{
public:
	irisData() = default;
	irisData(std::vector<double> data,const std::string &s,unsigned n1,unsigned n2):
		iris(data),iClass(s),tag1(n1),tag2(n2){}

	//irisData(std::istream &s);
	

private:
	std::vector<double> iris;
	std::string iClass;
	unsigned tag1;
	unsigned tag2;

public:
	std::vector<double> get_data() const { return iris; }
	std::string get_class()const { return iClass; }
	unsigned get_tag1() const { return tag1; }
	unsigned get_tag2() const { return tag2; }
	void change_tag1(unsigned val) { tag1 = val; }
	void change_tag2(unsigned val) { tag2 = val; }
	//析构函数
};

void readFile(std::map<unsigned, irisData> *p, std::string fileName)
{
	std::ifstream inFile("fileName");
	unsigned count = 0;
	const size_t size = 150;
	std::string line;
	while (count<size)
	{
		std::string line;
		getline(inFile, line);
		auto itr = line.begin();
		std::vector<double> vec;
		auto itr_ = itr;

		unsigned count_ = 0;

		while (count_<4)
		{
			if (*itr_ == ',')
			{
				std::string str(itr, itr_);
				vec.push_back(std::stod(str));
				itr = ++itr_;
				++count_;
			}
			else
			{
				++itr_;
			}
		}
		std::string str(itr_, line.end());
		irisData temp(vec,str,1,1);
		(*p).insert({ count++,temp });
	}
	inFile.close();
}

void writeFile(const std::map<unsigned,irisData> *p)
{
	std::ofstream write;
	write.open("myReult.txt");
	if (write.is_open())
	{
		for (auto e : *p)
		{
			write << e.first << ' ' << e.second.get_class() << ' ' << e.second.get_tag1() << ' '<<e.second.get_tag2()<<std::endl;
		}
		write.close();
	}
	else
	{
		std::cout << "cannot open the file" << std::endl;
	}
}

std::vector<double> x_1Means(const unsigned n1,const std::vector<double> &x1_m,const std::vector<double> x_i)//means
{
	std::vector<double> vec;
	for (size_t i = 0; i != x_i.size(); ++i)
	{
		double temp = (x1_m[i] - x_i[i]) / (n1-1) ;
		vec.push_back(x1_m[i]+  temp);
	}
	return vec;
}

std::vector<double> x_2Means(const unsigned n2, const std::vector<double> &x2_m, const std::vector<double> x_i)//计算x2均值
{
	std::vector<double> vec;
	for (size_t i = 0; i != x_i.size(); ++i)
	{
		double temp = (x2_m[i] - x_i[i]) / (n2 + 1);
		vec.push_back(x2_m[i] - temp);
	}
	return vec;
}

double vecMul(const std::vector<double> vec1, const std::vector<double> vec2)//product 乘法
{
	double sum = 0;
	int t = vec1.size();
	for (size_t i = 0; i != t; ++i)
	{
		sum += vec1[i] * vec2[i];
	}
	return sum;
}

double calE(const unsigned n1,const unsigned n2,const double vec_product)//计算E值
{
	double temp = (n1 - 1)*(n2 + 1);
	temp/=(n1 + n2);
	temp*=vec_product;
	return temp;
}

std::vector<double> vecMinus(const std::vector<double> vec_1,const std::vector<double> vec_2)
{
	std::vector<double> re;
	for (size_t i = 0; i != vec_1.size(); ++i)
	{
		re.push_back(vec_1[i] - vec_2[i]);
	}
	return re;
}

void printVec(std::vector<double> vec)
{
	for (auto e : vec)
		std::cout << e << ' ';
	std::cout << std::endl;
}

std::vector<double> vecPlus(const std::vector<double> vec1, const std::vector<double> vec2)//向量加法
{
	if (vec1.size() != vec2.size())
	{
		std::cout << "vectors don't match" << std::endl;
	}
	std::vector<double> vec;
	for (size_t i = 0; i != vec1.size(); ++i)
	{
		vec.push_back(vec1[i] + vec2[i]);
	}
	return vec;
}

void vec_plus(std::vector<double> &vec, const std::vector<double> vec_)
{
	for (size_t i = 0; i != vec.size(); ++i)
	{
		vec[i] += vec_[i];
	}
}

void vecMeansCritification1(unsigned n1, std::vector<double> x_1, const std::vector<double> vec)
{
	for (size_t i = 0; i != x_1.size(); ++i)
	{
		double temp = (x_1[i] - vec[i]) / (n1 - 1);
		x_1[i] += temp;
	}
}

void vecMeansCritification2(unsigned n2, std::vector<double> x_2, const std::vector<double> vec)
{
	for (size_t i = 0; i != x_2.size(); ++i)
	{
		double temp = (x_2[i] - vec[i]) / (n2 + 1);
		x_2[i] -= temp;
	}
}

void print(const std::pair<unsigned,irisData> e)
{
	//std::cout << "第" << e.first << "条数据类别为：" << e.second.get_class<< ",聚类结果为：" << e.second.get_tag1 << std::endl;

}

std::vector<double> vecMeansInitialize(const std::map<unsigned, irisData> *data,const unsigned totalNum)
{
	std::vector<double> x_1(4, 0);
	for (auto e : *data)
	{
		x_1 = vecPlus(x_1, e.second.get_data());
	}
	for (auto &e : x_1)
	{
		e /= totalNum;
	}
	return x_1;
}

void compCluster1(std::map<unsigned, irisData> *data, unsigned &totalNum, std::vector<double> x_1, std::vector<double> x_2, const unsigned tag, const unsigned into)
{
	unsigned n1 = totalNum;
	unsigned n2 = totalNum - n1;
	double Ep = 0;
	double Ec = 0;
	while (n1)
	{
		Ep = Ec;
		double emax = 0;
		unsigned index;
		std::vector<double> x_2(4, 0);

		for (auto e : *data)
		{
			unsigned tag1 = e.second.get_tag1();
			std::vector<double> vec = e.second.get_data();
			if (tag1 == tag)
			{
				std::vector<double> xm1=x_1Means(n1, x_1, vec);
				std::vector<double> xm2=x_2Means(n2, x_2, vec);

				//printVec(xm1);
				//printVec(xm2);

				std::vector<double> vec_minus(vecMinus(xm1, xm2));
				double product = vecMul(vec_minus, vec_minus);
				//printVec(vec_minus);
				//system("pause");
				double enow = calE(n1, n2, product);
				if (enow > Ec)
				{
					index = e.first;
					Ec = enow;
				}
			}
			
		}
		if (Ec > Ep)
		{
			std::vector<double> vec = (*data)[index].get_data();
			(*data)[index].change_tag1(into);
			unsigned temp = (*data)[index].get_tag1();
			vecMeansCritification1(n1, x_1, vec);
			vecMeansCritification2(n2, x_2, vec);
			--n1;
			++n2;
		}
		else
		{
			totalNum = n2;
			break;
		}
	}
}

void compCluster2(std::map<unsigned, irisData> *data, unsigned totalNum, std::vector<double> x_1, std::vector<double> x_2, const unsigned tag, const unsigned into)
{
	unsigned n1 = totalNum;
	unsigned n2 = totalNum - n1;
	double Ep = 0;
	double Ec = 0;
	while (n1)
	{
		Ep = Ec;
		double emax = 0;
		unsigned index;
		std::vector<double> x_2(4, 0);

		for (auto e : *data)
		{
			unsigned tag1 = e.second.get_tag2();
			std::vector<double> vec = e.second.get_data();
			if (tag1 == tag)
			{
				std::vector<double> xm1 = x_1Means(n1, x_1, vec);
				std::vector<double> xm2 = x_2Means(n2, x_2, vec);


				std::vector<double> vec_minus(vecMinus(xm1, xm2));

				printVec(vec_minus);

				double product = vecMul(vec_minus, vec_minus);
				double enow = calE(n1, n2, product);
				if (enow > Ec)
				{
					index = e.first;
					Ec = enow;
				}
			}

		}
		if (Ec > Ep)
		{
			std::vector<double> vec = (*data)[index].get_data();
			(*data)[index].change_tag2(into);
			unsigned temp = (*data)[index].get_tag2();
			vecMeansCritification1(n1, x_1, vec);
			vecMeansCritification2(n2, x_2, vec);
			--n1;
			++n2;
		}
		else
		{
			break;
		}
	}
}

void vecCenterDiv(std::vector<double> &vec, const unsigned n)
{
	for (auto &e : vec)
	{
		e /= n;
	}
}

void cMeansCenter(std::map<unsigned, irisData> *const p, std::vector<double> &center_1, std::vector<double> &center_2, std::vector<double> &center_3)//求类中心
{
	unsigned n1 = 0;
	unsigned n2(n1);
	unsigned n3(n2);
	center_1 = { 0,0,0,0 };
	center_2 = center_1;
	center_3 = center_1;

	for (auto e : *p)
	{
		unsigned i = e.second.get_tag2();
		std::vector<double> vec = e.second.get_data();
		if (i == 1)
		{
			center_1 = vecPlus(center_1, vec);
			std::cout << "center_1" << std::endl;
			printVec(center_1);
			++n1;
			std::cout << n1 << std::endl;
		}
		if (i == 2)
		{
			 vec_plus(center_2, vec); 
			++n2;
		}
		if (i == 3)
		{
			vec_plus(center_3, vec); 
			++n3;
		}
	}
	vecCenterDiv(center_1, n1);
	vecCenterDiv(center_2, n2);
	vecCenterDiv(center_3, n3);
}

double centerIteration(const std::vector<double> vec1, const std::vector<double> vec2)
{
	double sum = 0;
	for (size_t i = 0; i != 4; ++i) 
	{
		double temp = vec1[i] - vec2[i];
		temp *= (vec1[i] - vec2[i]);
		sum += temp;
	}
		
	return sum;
}

double vecDistance(const std::vector<double>vec, const std::vector<double> center)//计算点与中心点的距离
{
	std::vector<double> minus = vecMinus(vec, center);
	double distance = sqrt(vecMul(minus, minus));
	return distance;
}

int main()
{
	std::map<unsigned, irisData> iris;
	std::map<unsigned, irisData> *pt = &iris;
	std::ifstream inFile("IrisDataSet.txt");
	unsigned count = 0;
	unsigned size = 150;
	std::string line;
	while (count<size)
	{
		std::string line;
		getline(inFile, line);
		auto itr = line.begin();
		std::vector<double> vec;
		auto itr_ = itr;

		unsigned count_ = 0;

		while (count_<4)
		{
			if (*itr_ == ',')
			{
				std::string str(itr, itr_);
				vec.push_back(std::stod(str));
				itr = ++itr_;
				++count_;
			}
			else
			{
				++itr_;
			}
		}
		std::string str(itr_, line.end());
		irisData temp(vec, str, 1, 1);
		iris.insert({ count++,temp });
	}
	inFile.close();
	//std::cout << iris[149].get_class() << std::endl;
	//system("pause");
	std::vector<double> x_1=vecMeansInitialize(pt,size);

	//printVec(x_1);

	std::vector<double> x_2(4, 0);
	compCluster1(pt, size, x_1, x_2, 1, 2);
	std::vector<double> x_3(4, 0);
	unsigned n1 = 150 - size;
	
	compCluster1(pt, n1, x_1, x_3, 1, 3);
	std::vector<double> x_4(4, 0);
	compCluster2(pt, size, x_2, x_4, 2, 3);

	bool flag = true;//cmeans cluster
	std::vector<double> center1(iris[24].get_data());//primer center 
	std::vector<double> center2(iris[74].get_data());
	std::vector<double> center3(iris[124].get_data());
	while (flag)
	{
		for (auto &e : iris)
		{
			std::vector<double> vec = e.second.get_data();
			double dis1 = vecDistance(vec, center1);
			double dis2 = vecDistance(vec, center2);
			double dis3 = vecDistance(vec, center3);
			unsigned val=0;
			if (dis1 < dis2)
				val = dis1 < dis3 ? 1 : 3;
			else
				val = dis2 < dis3 ? 2 : 3;
			e.second.change_tag2(val);
			unsigned tag = e.second.get_tag2();
			std::cout << tag << ' ';
		}
		unsigned tempcount = 1;
		std::cout << iris[77].get_tag2();
		std::cout << std::endl << std::endl;
		for (auto e : iris)
		{
			if (!tempcount % 50)
				std::cout << std::endl;
			std::cout << e.second.get_tag2() << ' ';
			++tempcount;
		}
		//writeFile(pt);
		
		std::map<unsigned, irisData> *pt = &iris;
		std::vector<double> c1 = center1;
		std::vector<double> c2(center2);
		std::vector<double> c3(center3);
		cMeansCenter(pt, center1, center2, center3);
		std::cout << "c1" << std::endl;
		printVec(c1);
		printVec(center1);
		std::cout << "c2" << std::endl;
		printVec(c2);
		printVec(center2);
		std::cout << "c3" << std::endl;
		printVec(c3);
		printVec(center3);
		///flag = false;
		if (c1 == center1 || c2 == center2 || c3 == center3)
			flag = false;
	}
	return 0;
}