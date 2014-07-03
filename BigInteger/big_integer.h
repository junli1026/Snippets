#include <vector>
#include <string>

class BigInteger{
private:
	std::vector<int> data_;
	bool negative;
	void copyFrom(const BigInteger& b);
	BigInteger(std::vector<int> data, bool isNegative);

public:
	BigInteger(int d);
	BigInteger(const BigInteger& b);
	BigInteger(const std::string& str);
	BigInteger& operator=(const BigInteger& b);
	bool isNegative() const;
	int get(int p) const;
	int length() const;
	void print();
	const std::vector<int>& getData() const;
	BigInteger abs() const;

	BigInteger operator+(const BigInteger& rhs);
	BigInteger operator-(const BigInteger& rhs);
	BigInteger operator*(const BigInteger& rhs);
	BigInteger operator/(const BigInteger& rhs);
};

