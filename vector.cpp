#include <iostream>
#include <vector>
using namespace std;
int main() {
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);
	v.push_back(60);
	vector<int> v2(5, 3);
	for (vector<int>::size_type i = 0; i < v.size(); ++i)
		cout << v[i] << ' ';
	cout << endl;
	for (vector<int>::size_type i = 0; i < v2.size(); ++i)
		cout << v2[i] << ' ';
	cout << endl;
	cout << typeid(vector<int>::size_type).name() << endl;
	cout << "v.capacity():"<<v.capacity() << endl;
	cout << "v.size():"<< v.size() << endl;
	cout <<"v.max_size():"<< v.max_size() << endl;
	v.reserve(15);
	cout <<"after reserve v.capacity():"<< v.capacity() << endl;
	cout << "v2.size():" << v2.size() << endl;
	v2.resize(15);
	cout << "after resize v2.size():" << v2.size() << endl;
	v.clear();
	cout << "after clear v.empty():" << v.empty() << endl;
	cout << "after clear v.capacity():" << v.capacity() << endl;
	vector<int>().swap(v);
	cout << "after swap with 0 v.capacity:" << v.capacity() << endl;
	cout << "v2.front():"<< v2.front()<<" v2.back():"  << v2.back() << endl;
	cout << "v2[0]:" << v2[0] << " v2.at(3):" << v2.at(3) << endl;
	v.assign(5,2);
	for (vector<int>::iterator iter = v.begin(); iter != v.end(); iter++)
		cout << *iter << " ";
	cout << endl;
	vector<int>::iterator ite = v.begin();
	cout << *ite << ++*ite << endl;
	vector<int>::reverse_iterator riter;
	for (riter=v.rbegin();riter!=v.rend();++riter)
		cout << *riter << " ";
	vector<int>::iterator iter = v.begin();


	vector<int> v3;
	v3.reserve(5);
	v3.push_back(10);
	v3.push_back(20);
	v3.push_back(30);
	v3.push_back(40);
	v3.push_back(50);
	vector<int>v4;
	v4.assign(v.begin(), v.end());
	if (v3 == v4)
		cout << "v3==v4" << endl;
	if (v3 != v4)
		cout << "v3!=v4" << endl;
	if (v3 < v4)
		cout << "v3<v4" << endl;
	return 0;
}
