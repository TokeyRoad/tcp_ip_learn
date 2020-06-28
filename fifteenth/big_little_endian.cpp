/*************************************************************************
	> File Name: big_little_endian.cpp
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年06月28日 星期日 11时07分27秒
 ************************************************************************/

#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>

int main(){
	short sa = 0x1234;
	std::cout << htons(sa) << std::endl;
	std::cout << htonl(sa) << std::endl;

	long sa2 = 0x12345678;
	std::cout << htons(sa2) << std::endl;
	std::cout << htonl(sa2) << std::endl;

	short a1 = 13330;
	long a2 = 873594880;
	std::cout << ntohs(a1) << std::endl;
	std::cout << ntohl(a2) << std::endl;

	short s1 = 30806;
	long s2 = 2018915346;
	std::cout << ntohs(s1) << std::endl;
	std::cout << ntohl(s2) << std::endl;
	return 0;

}

