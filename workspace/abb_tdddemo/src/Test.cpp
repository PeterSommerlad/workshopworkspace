#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#include <string>
using namespace std::literals;

std::string fizzbuzz(unsigned i) {
	std::string result{};
	if (i % 3 == 0) result += "Fizz";
	if (i % 5 == 0) result += "Buzz";
	return result.empty()?std::to_string(i):result;
}

struct fizzbuzz_table {
	std::string const expected;
	int const input;
	cute::test_failure const fail; // to keep track of entry location
};

// DDT generates failure message linking back to table entry.
fizzbuzz_table const theFizzBuzzTests[]={
		{"1"s,    1,DDT()},
		{"2"s,    2,DDT()},
		{"Fizz"s, 3,DDT()},
		{"4"s,    4,DDT()},
		{"Buzz"s, 5,DDT()},
		{"Fizz"s, 6,DDT()},
		{"7"s,    7,DDT()},
		{"8"s,    8,DDT()},
		{"Fizz"s, 9,DDT()}
};
void FizzBuzzAllTable(){
	for(auto const &test:theFizzBuzzTests){
		ASSERT_EQUAL_DDT(test.expected, fizzbuzz(test.input), test.fail);
	}
}

void testFizzBuzzWithZero() {
	std::string result = fizzbuzz(0u);
	ASSERT_EQUAL("0",result);
}
void testFizzBuzzWithOneIs1(){
	std::string result = fizzbuzz(1u);
	ASSERT_EQUAL("1",result);
}
void testFizzBuzzWithTwoIs2(){
	std::string result = fizzbuzz(2u);
	ASSERT_EQUAL("2",result);
}
void testFizzBuzzWithThreeIsFizz(){
	ASSERT_EQUAL("Fizz",fizzbuzz(3u));
}
void testFizzBuzzWithFiveIsBuzz(){
	ASSERT_EQUAL("Buzz",fizzbuzz(5u));
}





bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(testFizzBuzzWithZero));
	s.push_back(CUTE(testFizzBuzzWithOneIs1));
	s.push_back(CUTE(testFizzBuzzWithTwoIs2));
	s.push_back(CUTE(testFizzBuzzWithThreeIsFizz));
	s.push_back(CUTE(testFizzBuzzWithFiveIsBuzz));
	s.push_back(CUTE(FizzBuzzAllTable));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
