#include <vector>
#include <algorithm>
#include <array>
#include <iostream>

auto constexpr print= [](std::ostream &out,auto vec){
    for (auto const &e:vec){
        out << e << ", ";
    }
    out << '\n';
};

auto constexpr printfor= [](std::ostream &out,auto const & vec){
    for (auto it = begin(vec); it != end(vec); ++it){
        //*it += 2;
        out << (*it) << ", ";
    }
    out << '\n';
};


int main() {
    int a[5]{5,4,3,2,1};
    std::array arr{6,5,4,3,2,1};
    begin(arr);
    begin(a);
    std::vector v{1,2,3,4,5,6,7,8};
    printfor(std::cout, arr);

    //transform(begin(v),end(v),begin(v),[](auto e){return e*e;});
    print(std::cout, v);

}
