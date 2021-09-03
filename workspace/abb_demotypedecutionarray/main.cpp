#include <array>
#include <algorithm>
#include <numeric>
#include <iostream>

template<typename T, auto n>
auto average(std::array<T,n> const &values){
	static_assert(n!=0);
    auto sum=std::accumulate(begin(values),end(values),0);
    return sum/n;
}


int main() {
	//int empty[0];
    std::array<double,10> some{};
    static_assert(some.size() == 0);
    std::array a{1.0,2.,3.,4.};
    std::cout << average(a);
}
