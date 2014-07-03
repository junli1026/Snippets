#include "big_integer.h"
#include <vector>
#include <iostream>
#include <assert.h>

//vector[0] keeps the least significant number

int compare(const std::vector<int>& a, const std::vector<int>& b){
	if(a.size() > b.size()){
		return 1;
	}else if(a.size() < b.size()){
		return -1;
	}else{
		for(int i = a.size() -1; i >= 0; i--){
			if(a[i] < b[i]){
				return -1;
			}else if(a[i] > b[i]){
				return 1;
			}
		}
		return 0;
	}
}

void remove_zero_ending(std::vector<int>& a){
	int count = 0;
	for(int i = a.size() -1; i >= 0; i--){
		if(a[i] == 0){
			count ++;
		}else{
			break;
		}
	}
	a.resize(a.size() - count);
}

std::vector<int> add(const std::vector<int>& a, const std::vector<int>& b){
	std::vector<int> ret;
	int carry = 0;
	for(int i = 0, j = 0; i < a.size() || j < b.size(); i++, j++){
		int sum = carry;
		if(i < a.size()) sum += a[i];
		if(j < b.size()) sum += b[j];
		ret.push_back(sum % 10);
		carry = sum / 10;
	}
	if(carry){
		ret.push_back(carry);
	}
	return ret;
}

std::vector<int> minus(const std::vector<int>& a, const std::vector<int>& b){
	assert(compare(a, b) == 1); //correctly works only when a > b
	std::vector<int> ret;
	int carry = 0;
	int res;
	for(int i = 0; i < a.size(); i++){
		if(i < b.size()) res = a[i] - b[i] + carry;
		else res = a[i] - 0 + carry;

		if(res < 0){
			ret.push_back(res + 10);
			carry = -1;
		}else{
			ret.push_back(res);
			carry = 0;
		}
	}
	assert(carry == 0);
	remove_zero_ending(ret);
	return ret;
}

std::vector<int> multiply(const std::vector<int>& a, const std::vector<int>& b){
	std::vector<int> ret(a.size()  + b.size(), 0);
	for(int i = 0; i < a.size(); i++){
		for(int j = 0; j < b.size(); j++){
			ret[i+j] += a[i] * b[j];
		}
	}

	int carry = 0;
	for(int i = 0; i < ret.size(); i++){
		int k = ret[i] + carry;
		ret[i] = k % 10;
		carry = k / 10;
	}
	remove_zero_ending(ret);
	return ret;
}

std::vector<int> divide_by_minus(const std::vector<int>& a, const std::vector<int>& b, int& quotient){
	std::vector<int> remain(a);
	int count = 0;
	while(compare(remain, b) >= 0){ //remain >= b
		remain = minus(remain, b);
		count ++;
	}
	quotient = count;
	return remain;
}

std::vector<int> divide(const std::vector<int>& a, const std::vector<int>& b){
	std::vector<int> ret;
	std::vector<int> d;
	int quotient;
	for(int i = a.size()-1; i >= 0; i--){
		d.insert(d.begin(), a[i]);
		d = divide_by_minus(d, b, quotient);
		assert(quotient < 10);
		ret.push_back(quotient);
	}
	std::reverse(ret.begin(), ret.end());
	remove_zero_ending(ret);
	return ret;
}

void BigInteger::copyFrom(const BigInteger& b){
	this->data_ = b.getData();
	this->negative = b.isNegative();
}

BigInteger::BigInteger(std::vector<int> data, bool isNegative){
	this->data_ = data;
	this->negative = isNegative;
}

BigInteger::BigInteger(int d){
	if(d < 0){
		this->negative = true;
		d = -1 * d;
	}else{
		this->negative = false;
	}
	while(d > 0){
		this->data_.push_back(d % 10);
		d = d / 10;
	}
}

BigInteger::BigInteger(const std::string& str){
	for(int i = str.length() -1; i >= 0; i--){
		this->data_.push_back(str[i] - '0');
		this->negative = false;
	}
}

BigInteger::BigInteger(const BigInteger& b){
	copyFrom(b);
}

BigInteger& BigInteger::operator=(const BigInteger& b){
	copyFrom(b);
	return *this;
}

bool BigInteger::isNegative() const{
	return this->negative;
}

int BigInteger::get(int p) const{
	if(p < this->data_.size()){
		return this->data_[p];
	}else{
		return 0;
	}
}

int BigInteger::length() const{
	return this->data_.size();
}

const std::vector<int>& BigInteger::getData() const{
	return this->data_;
}

void BigInteger::print(){
	if(negative){
		std::cout << "-";
	}
	for(int i = 0; i < this->data_.size(); i++){
		std::cout << this->data_[this->data_.size()-1-i];
	}
}

BigInteger BigInteger::abs() const{
	return BigInteger(this->data_, false);	
}

BigInteger BigInteger::operator+(const BigInteger& rhs){
	if(rhs.isNegative() && !this->isNegative()){
		return *this - rhs;
	}else if(!rhs.isNegative() && this->isNegative()){
		return BigInteger(rhs) - *this;
	}else{
		std::vector<int> v = add(this->data_, rhs.getData());
		return BigInteger(v, this->isNegative());
	}
}

BigInteger BigInteger::operator-(const BigInteger& rhs){
	if(this->isNegative() && rhs.isNegative()){
		return BigInteger((this->abs() + rhs.abs()).getData(), true);
	}else if(this->isNegative() && !rhs.isNegative()){
		return BigInteger((this->abs() + rhs).getData(), true);
	}else if(!this->isNegative() && rhs.isNegative()){
		return *this + rhs.abs();
	}else{
		
		std::vector<int> v = minus(this->getData(), rhs.getData());
		return BigInteger(v, false);
	}
}

BigInteger BigInteger::operator*(const BigInteger& rhs){
	return BigInteger(multiply(this->getData(), rhs.getData()), false);
}

BigInteger BigInteger::operator/(const BigInteger& rhs){
	return BigInteger(divide(this->getData(), rhs.getData()), false);
}
