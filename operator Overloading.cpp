#include <iostream>
using namespace std;
class point {
	int x;
	int y;
public:
	point(int a = 0, int b = 0) :x(a), y(b) {}
	void print()const { cout << x << ',' << y << endl; }
	const point& operator++() {
		++x;
		++y;
		return *this;
	}
	const point operator++(int) {
		point pt(x, y);
		x++;
		y++;
		return pt;
	}
	const point operator--() {
		x--;
		y--;
		return *this;
	}
	const point operator--(int) {
		point pt(x, y);
		x--;
		y--;
		return pt;
	}
	const point operator+(point pt){
		x = x + pt.x;
		y = y + pt.y;
		return *this;
	}
	const point operator-(point pt) {
		x = x - pt.x;
		y = y - pt.y;
		return *this;
	}
	bool operator==(const point &pt) const {
		return x == pt.x&&y == pt.y ? true : false;
	}
	bool operator!=(const point &pt) const {
		return x == pt.x&&y == pt.y ? false : true;
	}
	int operator[](int index) {
		if (index == 0)
			return x;
		else if (index == 1)
			return y;
		else
			throw"error";
	}
	int getx()const {
		return x;
	}
	int gety()const {
		return y;
	}
};
const point operator*(const point p1,const point p2) {
	return point(p1.getx() * p2.getx(), p1.getx() * p2.gety());
}
int main() {
	point p1(5,6), p2(2, 3);
	point result;
	result = ++p1;
	p1.print();
	result.print();
	result = p2++;
	p2.print();
	result.print();
	result=p1 + p2;
	result.print();
	result = p1 - p2;
	result.print();
	bool a = (p1 == p2);
	bool b = (p1 != p2);
	result = p1 * p2;
	result.print();
	cout << a << b << endl;
	cout << p1[1] << p1[0] << endl;
	return 0;
}
