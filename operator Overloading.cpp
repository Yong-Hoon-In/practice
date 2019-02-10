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
};
int main() {
	point p1(2, 3), p2(2, 3);
	point result;
	result = ++p1;

	p1.print();

	result.print();

	result = p2++;

	p2.print();

	result.print();
	return 0;
}
