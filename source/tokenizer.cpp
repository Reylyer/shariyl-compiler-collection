#include <tokenizer.hpp>
#include <map>
#include <string>
#include <cctype>
#include <stack>
#include <cstdlib>
#include <push_back_stream.hpp>
#include <errors.hpp>

#include <iostream>

namespace algoritmik {
	namespace {
		enum struct character_type {
			eof,
			space,
			alphanum,
			punct,
		};
		
		/*
		eof, space, valid word, punct
		*/
		character_type get_character_type(int c) {
			if (c < 0) {
				return character_type::eof;
			}
			if (std::isspace(c)) {
				return character_type::space;
			}
			if (std::isalpha(c) || std::isdigit(c) || c == '_') {
				return character_type::alphanum;
			}
			return character_type::punct;
		}
		
		/*
		pinjem stream buat ambil word sekarang
		*/
		token fetch_word(push_back_stream& stream) {
			size_t line_number = stream.line_number();
			size_t char_index = stream.char_index();

			std::string word;
			
			// ambil sampel
			int c = stream();
			
			do {
				// masukin ke stack
				word.push_back(char(c));
				// ambil sampel lagi
				c = stream();

				if(std::isspace(c) && word == "depends") {
					// handle depends on
				}
				// handling double titik
				if (c == '.' && word.back() == '.') {
					stream.push_back(word.back());
					word.pop_back();
					break;
				}
				
			//                      kata                                          angka
			} while (get_character_type(c) == character_type::alphanum || c == '.');
			
			stream.push_back(c);
			
			// check dia reserved ato bukan
			if (std::optional<reserved_token> t  = get_keyword(word)) {
				return token(*t, line_number, char_index);
			} else {
				// sebuah angka
				if (std::isdigit(word.front())) {
					char* endptr;
					long long inum = strtol(word.c_str(), &endptr, 0);
					// strtol kalo abis jadi angka ptr nunjuk ke 0
					if (*endptr != 0) { // floating point
						double dnum = strtod(word.c_str(), &endptr);
						if (*endptr != 0) { // ga bisa di parse
							size_t remaining = word.size() - (endptr - word.c_str());
							throw unexpected_error(
								std::string(1, char(*endptr)),
								stream.line_number(),
								stream.char_index() - remaining
							);
						}
						return token(dnum, line_number, char_index);
					}
					return token(inum, line_number, char_index);
				} else { // sebuah identifier, bisa nama variable	
					return token(identifier{std::move(word)}, line_number, char_index);
				}
			}
		}
		
		token fetch_operator(push_back_stream& stream) {
			size_t line_number = stream.line_number();
			size_t char_index = stream.char_index();

			if (std::optional<reserved_token> t = get_operator(stream)) {
				return token(*t, line_number, char_index);
			} else {
				std::string unexpected;
				size_t err_line_number = stream.line_number();
				size_t err_char_index = stream.char_index();
				for (int c = stream(); get_character_type(c) == character_type::punct; c = stream()) {
					unexpected.push_back(char(c));
				}
				std::cout << "fucked!\n";

				throw unexpected_error(unexpected, err_line_number, err_char_index);
			}
		}
		
		token fetch_string(push_back_stream& stream) {
			size_t line_number = stream.line_number();
			size_t char_index = stream.char_index();

			std::string str;
			
			bool escaped = false;
			int c = stream();
			for (; get_character_type(c) != character_type::eof; c = stream()) {
				if (c == '\\') {
					escaped = true;
				} else {
					if (escaped) {
						switch(c) {
							case 't':
								str.push_back('\t');
								break;
							case 'n':
								str.push_back('\n');
								break;
							case 'r':
								str.push_back('\r');
								break;
							case '0':
								str.push_back('\0');
								break;
							default:
								str.push_back(c);
								break;
						}
						escaped = false;
					} else {
						switch (c) {
							case '\t':
							case '\n':
							case '\r':
								stream.push_back(c);
								throw parsing_error("Tidak menemukan pasangan kutip '\"'", stream.line_number(), stream.char_index());
							case '"':
								return token(std::move(str), line_number, char_index);
							default:
								str.push_back(c);
						}
					}
				}
			}
			stream.push_back(c);
			throw parsing_error("Tidak menemukan pasangan kutip '\"'", stream.line_number(), stream.char_index());
		}
		
		void skip_block_comment(push_back_stream& stream) {
			bool closing = false;
			int c;
			do {
				c = stream();
				if (c == '}') {
					return;
				}
			} while (get_character_type(c) != character_type::eof);

			stream.push_back(c);
			throw parsing_error("Tidak menemukan pasangan kurung kurawal '}'", stream.line_number(), stream.char_index());
		}
	
		token tokenize(push_back_stream& stream) {
			while (true) {
				size_t line_number = stream.line_number();
				size_t char_index = stream.char_index();
				int c = stream();
				switch (get_character_type(c)) {
					case character_type::eof:
						return {eof(), line_number, char_index};
					case character_type::space:
						continue;
					case character_type::alphanum:
						stream.push_back(c);
						return fetch_word(stream);
					case character_type::punct:
						switch (c) {
							case '"':
								return fetch_string(stream);
							case '{':
								skip_block_comment(stream);
								continue;
							default:
								stream.push_back(c);
								return fetch_operator(stream);
						}
						break;
				}
			}
		}
	}
	
	tokens_iterator::tokens_iterator(push_back_stream& stream):
		_current(eof(), 0, 0),
		_get_next_token([&stream](){
			return tokenize(stream);
		})
	{
		++(*this);
	}
	
	tokens_iterator::tokens_iterator(std::deque<token>& tokens):
		_current(eof(), 0, 0),
		_get_next_token([&tokens](){
			if (tokens.empty()) {
				return token(eof(), 0, 0);
			} else {
				token ret = std::move(tokens.front());
				tokens.pop_front();
				return ret;
			}
		})
	{
		++(*this);
	}

	tokens_iterator& tokens_iterator::operator++() {
		_current = _get_next_token();
		return *this;
	}
	
	const token& tokens_iterator::operator*() const {
		return _current;
	}
	
	const token* tokens_iterator::operator->() const {
		return &_current;
	}

	tokens_iterator::operator bool() const {
		return !_current.is_eof();
	}
}

