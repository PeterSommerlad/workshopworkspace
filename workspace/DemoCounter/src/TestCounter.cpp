#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <optional>
namespace freefunctions {
struct Counter {
	unsigned value{};
};
struct FloatCounter {
	float value{};
};


template<typename ACounter>
auto valueOf(ACounter const &theCounter) {
	return theCounter.value;
}
template<typename ACounter>
void increment(ACounter &theCounter) {
	theCounter.value++;
}

void demoTemporary(){
	//increment(Counter{});
	ASSERT_EQUAL(0,valueOf(Counter{}));
}


void createDefaultCounterIsZero() {
	Counter const theCounter { };
	ASSERT_EQUAL(0u,valueOf(theCounter));
}

void incrementedCounterIsOne(){
	Counter theCounter{};
	increment(theCounter);
	ASSERT_EQUAL(1u,valueOf(theCounter));
}

void incrementedCounterIsOneFloat(){
	FloatCounter theCounter{};
	increment(theCounter);
	ASSERT_EQUAL(1.0,valueOf(theCounter));
}

void createDefaultCounterIsZeroFloat() {
	FloatCounter const theCounter { };
	ASSERT_EQUAL(0.0,valueOf(theCounter));
}



void incrementedManyTimesMatches(){
	Counter theCounter{};
	constexpr auto theAmount{42};
	for(auto i=0; i < theAmount ; ++i){
		increment(theCounter);
	}
	ASSERT_EQUAL(theAmount,valueOf(theCounter));

}
void incrementedManyTimesMatchesFloat(){
	FloatCounter theCounter{};
	constexpr auto theAmount{42};
	for(auto i=0; i < theAmount ; ++i){
		increment(theCounter);
	}
	ASSERT_EQUAL(theAmount,valueOf(theCounter));
}

}
namespace memberfunctions {
struct Counter {
	Counter() = default;
	unsigned valueOf() const {
		return value;
	}
	void increment() ;
	// have a factory function, because no exceptions
	static
	std::optional<Counter> makeCounter(unsigned initialvalue){
		if (initialvalue > 10) {
			//return Counter{10}; // default value
			return std::nullopt; // extra value
			// throw "an exception"; // throw - could be from constructor
		}
		return Counter{initialvalue};
	}

private:
	explicit Counter(unsigned initialvalue) noexcept
	: value{initialvalue}
	{
	}
	unsigned value{};
};


void Counter::increment() {
	this->value++;
}

void TestMakeCounterWith10WillWork(){
	auto c = Counter::makeCounter(10);
	;
	ASSERT(c.has_value());
	ASSERT_EQUALM("we expect the initial value to be tranferred",10,(*c).valueOf());
}
void TestMakeCounterFailsWithGreaterValue(){
	auto c = Counter::makeCounter(11);
	ASSERT(not c.has_value());
}


Counter acceptACounter(Counter c){
	Counter{} = c; // this shouldn't compile
	auto cc {c};
	c = cc;
	return cc;
}

void demoTemporaryClass(){
	acceptACounter(Counter{});
	Counter{}.increment() ; // SHOULD not work
	ASSERT_EQUAL(0,Counter{}.valueOf());
}

void createDefaultCounterIsZero() {
	Counter const theCounter { };
	ASSERT_EQUAL(0u,theCounter.valueOf());
}

void incrementedCounterIsOne(){
	Counter theCounter{};
	theCounter.increment();
	ASSERT_EQUAL(1u,theCounter.valueOf());
}
}
namespace operatorfunctions {
struct Counter {
	Counter() = default;
	explicit
	Counter(unsigned initialvalue):value{initialvalue}{}
	unsigned operator*() const {
		return value;
	}
	Counter & operator++() & { // prefix
		value++;
		return *this;
	}
	Counter operator++(int) & { // postfix
		auto old = *this;
		++(*this);
		return old;
	}
	friend
	bool operator==(Counter const &lhs, Counter const&rhs) {
		return lhs.value == rhs.value && lhs.maximumvalue == rhs.maximumvalue;
	}
	friend
	bool operator!=(Counter const &lhs,Counter const & rhs)  {
		return ! (lhs == rhs);
	}
	friend
	std::ostream& operator<<(std::ostream& out, Counter const &c){
		return out << "Counter: " << *c;
	}
	explicit
	operator bool() const {
		return !(value == maximumvalue);
	}

private:
	unsigned value{};
	unsigned maximumvalue{};
};



void createDefaultCounterIsZero() {
	Counter const theCounter { };
	ASSERT_EQUAL(0u,*theCounter);
}

void incrementedCounterIsOne(){
	Counter theCounter{};
	++theCounter;
	theCounter.operator++();
	theCounter++;
	ASSERT_EQUAL(Counter{3},theCounter);
	ASSERT(Counter{2} != theCounter);
}

}


bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(freefunctions::createDefaultCounterIsZero));
	s.push_back(CUTE(freefunctions::incrementedCounterIsOne));
	s.push_back(CUTE(freefunctions::incrementedManyTimesMatches));
	s.push_back(CUTE(memberfunctions::createDefaultCounterIsZero));
	s.push_back(CUTE(memberfunctions::incrementedCounterIsOne));
	s.push_back(CUTE(operatorfunctions::incrementedCounterIsOne));
	s.push_back(CUTE(operatorfunctions::createDefaultCounterIsZero));
	s.push_back(CUTE(freefunctions::createDefaultCounterIsZeroFloat));
	s.push_back(CUTE(freefunctions::incrementedManyTimesMatchesFloat));
	s.push_back(CUTE(freefunctions::incrementedCounterIsOneFloat));
	s.push_back(CUTE(freefunctions::demoTemporary));
	s.push_back(CUTE(memberfunctions::TestMakeCounterWith10WillWork));
	s.push_back(CUTE(memberfunctions::TestMakeCounterFailsWithGreaterValue));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
