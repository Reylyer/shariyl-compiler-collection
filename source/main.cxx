#include <iostream>
#include <module.hpp>
#include <standard_functions.hpp>

#include <push_back_stream.hpp>
#include <tokenizer.hpp>
#include <errors.hpp>


int main(int argc, char** argv) {
	if (argc < 1){
		// help message
	} else {
		// butuh sink
	}

	std::string path = __FILE__;
	path = path.substr(0, path.find_last_of("/\\") + 1) + "test.stk";
	
	using namespace algoritmik;
	
	algoritmik_module m;
	
	add_standard_functions(m);
	
	m.add_external_function("greater", std::function<number(number, number)>([](number x, number y){
		return x > y;
	}));
	
	auto s_main = m.create_public_function_caller<void>("main");
	
	if (m.try_load(path.c_str(), &std::cerr)) {
		s_main();
	}
	
	
	return 0;
}