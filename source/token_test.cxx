#include <iostream>
#include <module.hpp>
#include <standard_functions.hpp>

#include <push_back_stream.hpp>
#include <tokenizer.hpp>
#include <errors.hpp>

class file{
	file(const file&) = delete;
	void operator=(const file&) = delete;
private:
	FILE* _fp;
public:
	file(const char* path):
		_fp(fopen(path, "rt"))
	{
		if (!_fp) {
			throw algoritmik::file_not_found(std::string("'") + path + "' not found");
		}
	}
	
	~file() {
		if (_fp) {
			fclose(_fp);
		}
	}
	
	int operator()() {
		return fgetc(_fp);
	}
};

void load(const char* path) {
	std::cout << path << "\n";
	using get_character = std::function<int()>;
	file f(path);
	get_character get = [&](){
		return f();
	};
	algoritmik::push_back_stream stream(&get);
	
	algoritmik::tokens_iterator it(stream);

	while (it) {
		std::cout << algoritmik::to_string(it->get_value()) << " " << it->get_line_number() << ":" << it->get_char_index() << "\n";
		++it;
	}
}

void test_token(std::string path) {
	std::ostream* err = &std::cerr;
	try {
			load(path.c_str());
	} catch(const algoritmik::file_not_found& e) {
		if (err) {
			*err << e.what() << std::endl;
		}
	} catch(const algoritmik::error& e) {
		if (err) {
			file f(path.c_str());
			format_error(
				e,
				[&](){
					return f();
				},
				*err
			);
		}
	}
}

int main(int argc, char** argv) {
	if (argc < 1){
		// help message
	} else {
		// butuh sink

		test_token(argv[1]);
	}

	// std::string path = __FILE__;
	// path = path.substr(0, path.find_last_of("/\\") + 1) + "test.stk";
	
	// using namespace algoritmik;
	
	// algoritmik_module m;
	
	// add_standard_functions(m);
	
	// m.add_external_function("greater", std::function<number(number, number)>([](number x, number y){
	// 	return x > y;
	// }));
	
	// auto s_main = m.create_public_function_caller<void>("main");
	
	// if (m.try_load(path.c_str(), &std::cerr)) {
	// 	s_main();
	// }
	
	
	return 0;
}
