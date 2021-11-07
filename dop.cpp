#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <fstream>
#include <memory>
#include <functional>

namespace std {
	template<typename t1, typename t2>
	std::ostream& operator<<(std::ostream& os, const std::pair<t1, t2> pair)
	{
		return os << pair.first << " " << *pair.second;
	}
}

class Val {
public:
	virtual std::ostream& print(std::ostream& os) const {
		return os;
	}
	friend std::ostream& operator<<(std::ostream& out, const Val& S) {
		return S.print(out);
	}
	virtual std::string ret_type() = 0;
	virtual Val& from_string(std::string str) = 0;
	

};

class V_int : public Val {
private:
	int val;
public:

	V_int(int valu) {
		val = valu;
	}
	V_int() {
		val = NULL;
	}
	virtual std::ostream& print(std::ostream& out) const {
		out << val;
		return out;
	}
	virtual std::string ret_type() {

		return typeid(this->val).name();
	}

	friend std::string ret_type(V_int& v) {

		return typeid(v.val).name();
	}
	virtual std::string to_string() {
		return std::to_string(val);
	}
	virtual V_int& from_string(std::string str) {
		auto it = str.begin();
		int Int = *it - '0';
		for (it = str.begin() + 1; it != str.end(); ++it) {
			Int = Int * 10 + *it - '0';
		}
		
		this->val = Int;

		return *this;
	}


};

class V_set : public Val {
private:
	std::set<int> val;
public:
	
	V_set(int valu) {
		val.insert(valu);
	}
	V_set() {
		
	}
	
	virtual std::ostream& print(std::ostream& out) const {
		
		for (auto it = val.begin(); it != val.end(); ++it)
			out << *it << " ";
		
		return out;
	}
	virtual std::string ret_type() {

		return typeid(this->val).name();
	}

	friend std::string ret_type(V_set& v) {

		return typeid(v.val).name();
	}
	
	virtual V_set& from_string(std::string str) {
		std::string tmp;

		for (int i = 0; i != str.length(); i++)
		{
			if (str[i] == ' ')
			{

				auto it = tmp.begin();
				int Int = *it - '0';
				for (it = tmp.begin() + 1; it != tmp.end(); ++it) {
					Int = Int * 10 + *it - '0';
				}

				val.insert(Int);
				tmp.clear();
			}
			else
				tmp += str[i];
		}
		
		return *this;
	}


};



template <class T>
class setting {
private:
	std::multimap <std::string, const std::shared_ptr<Val>> mas;
	T f;
public:
	setting(T f) : f(f) {};

	void add(const std::string& name, const std::shared_ptr<Val> val) {
	
		mas.insert(std::pair<std::string, const std::shared_ptr<Val>>(name, val));

	}
	friend std::ostream& operator<<(std::ostream& out, const setting& S)
	{

		for (auto it1 = S.mas.begin(); it1 != S.mas.end(); ++it1) std::cout << *it1 << std::endl;

		return out;
	}
	auto read_file(std::string file_name) {

		std::ifstream file(file_name);
		std::string str, name;
		int j = 0;

		if (!file.is_open())
			std::cout << "error\n";
		else {
			while (true) {

				if (!file.eof()) {

					std::getline(file, str);

					if (j == 0)  name = str; else {
						std::shared_ptr<Val> p1 = this->f(name);
						p1->from_string(str);
						this->add(name, p1);
					}
					j++;
					str.clear();
				}
				else break;
			}

			file.close();

		}


	}

};
int main() {
	std::string j = "int";
	auto myLambda = [](std::string type) -> std::shared_ptr<Val>
	{
		if (type == "int")
		{
			return std::make_shared<V_int>();
		}
		if (type == "set")
		{
			return std::make_shared<V_set>();
		}
		else
		{
			return nullptr;
		}
	};
	setting<decltype(myLambda)> D(myLambda);

	D.read_file("C://lab18.txt");
	std::cout << D;
	
}