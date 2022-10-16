#include <tokens.hpp>
#include <lookup.hpp>
#include <string_view>
#include <string>
#include <stack>
#include <push_back_stream.hpp>

#include <iostream>

namespace algoritmik {
	namespace {
		const lookup<std::string_view, reserved_token> operator_token_map {

			{"+", reserved_token::add},
			{"-", reserved_token::sub},
			{"*", reserved_token::mul},
			{"/", reserved_token::div},
			{"div", reserved_token::idiv},
			{"mod", reserved_token::mod},
			
			
			{"<-", reserved_token::assign},
			{"..", reserved_token::traversal},
			
			
			{"not", reserved_token::logical_not},
			{"and", reserved_token::logical_and},
			{"or", reserved_token::logical_or},
			
			{"=", reserved_token::eq},
			{"!=", reserved_token::ne},
			{"<", reserved_token::lt},
			{">", reserved_token::gt},
			{"<=", reserved_token::le},
			{">=", reserved_token::ge},
			
			{"?", reserved_token::question},
			{":", reserved_token::colon},
			
			{",", reserved_token::comma},
			
			{";", reserved_token::semicolon},
			{":", reserved_token::colon},
			
			{"(", reserved_token::open_round},
			{")", reserved_token::close_round},
			
			{"{", reserved_token::open_curly},
			{"}", reserved_token::close_curly},
			
			{"[", reserved_token::open_square},
			{"]", reserved_token::close_square},
		};
		
		const lookup<std::string_view, reserved_token> keyword_token_map {
			{"sizeof", reserved_token::kw_sizeof},
			{"tostring", reserved_token::kw_tostring},
		
			{"if", reserved_token::kw_if},
			{"else", reserved_token::kw_else},
			// {"else if", reserved_token::kw_elif},

			{"depends on", reserved_token::kw_switch},
			{"case", reserved_token::kw_case},
			{"default", reserved_token::kw_default},

			{"for", reserved_token::kw_for},
			{"while", reserved_token::kw_while},
			{"do", reserved_token::kw_do},
			{"repeat", reserved_token::kw_repeat},
			{"times", reserved_token::kw_times},
			{"iterate", reserved_token::kw_do},
			{"until", reserved_token::kw_until},
			{"stop", reserved_token::kw_stop},
			{"traversal", reserved_token::kw_traversal},


			{"->", reserved_token::kw_return},

			{"function", reserved_token::kw_function},
			{"procedure", reserved_token::kw_procedure},
			{"program", reserved_token::kw_program},

			{"kamus", reserved_token::kw_kamus},
			{"kamus lokal", reserved_token::kw_kamus_lokal},
			{"algoritma", reserved_token::kw_algoritma},
			
			{"void", reserved_token::kw_void},
			{"number", reserved_token::kw_number},
			{"string", reserved_token::kw_string},
			
		};
		
		// 
		const lookup<reserved_token, std::string_view> token_string_map = ([](){
			std::vector<std::pair<reserved_token, std::string_view>> container;
			container.reserve(operator_token_map.size() + keyword_token_map.size());
			for (const auto& p : operator_token_map) {
				container.emplace_back(p.second, p.first);
			}
			for (const auto& p : keyword_token_map) {
				container.emplace_back(p.second, p.first);
			}
			return lookup<reserved_token, std::string_view>(std::move(container));
		})();
	}
	
	std::optional<reserved_token> get_keyword(std::string_view word) {
		auto it = keyword_token_map.find(word);
		return it == keyword_token_map.end() ? std::nullopt : std::make_optional(it->second);
	}
	
	namespace {
		class maximal_munch_comparator{
		private:
			size_t _idx;
		public:
			maximal_munch_comparator(size_t idx) :
				_idx(idx)
			{
			}
			// operand operang
			bool operator()(char l, char r) const {
				return l < r;
			}
			// subtree operand
			bool operator()(std::pair<std::string_view, reserved_token> l, char r) const {
				return l.first.size() <= _idx || l.first[_idx] < r;
			}
			// operand subtree
			bool operator()(char l, std::pair<std::string_view, reserved_token> r) const {
				return r.first.size() > _idx && l < r.first[_idx];
			}
			// subtree subtree
			bool operator()(std::pair<std::string_view, reserved_token> l, std::pair<std::string_view, reserved_token> r) const {
				return r.first.size() > _idx && (l.first.size() < _idx || l.first[_idx] < r.first[_idx]);
			}
		};
	}
	
	std::optional<reserved_token> get_operator(push_back_stream& stream) {
		auto candidates = std::make_pair(operator_token_map.begin(), operator_token_map.end());
		
		std::optional<reserved_token> ret;
		size_t match_size = 0;
		
		std::stack<int> chars;
		
		for (size_t idx = 0; candidates.first != candidates.second; ++idx) {
			chars.push(stream());
			
			candidates = std::equal_range(candidates.first, candidates.second, char(chars.top()), maximal_munch_comparator(idx));
			
			//              belom abis                                panjangnya sama?      
			if (candidates.first != candidates.second && candidates.first->first.size() == idx + 1) {
				match_size = idx + 1;
				ret = candidates.first->second;
			}
		}
		
		while (chars.size() > match_size) {
			stream.push_back(chars.top());
			chars.pop();
		}
		
		return ret;
	}
	
	token::token(token_value value, size_t line_number, size_t char_index) :
		_value(std::move(value)),
		_line_number(line_number),
		_char_index(char_index)
	{
	}
	
	bool token::is_reserved_token() const {
		return std::holds_alternative<reserved_token>(_value);
	}
	
	bool token::is_identifier() const {
		return std::holds_alternative<identifier>(_value);
	}
	
	bool token::is_number() const {
		return std::holds_alternative<double>(_value);
	}
	
	bool token::is_string() const {
		return std::holds_alternative<std::string>(_value);
	}
	
	bool token::is_eof() const {
		return std::holds_alternative<eof>(_value);
	}
	
	reserved_token token::get_reserved_token() const {
		return std::get<reserved_token>(_value);
	}
	
	const identifier& token::get_identifier() const {
		return std::get<identifier>(_value);
	}
	
	double token::get_number() const {
		return std::get<double>(_value);
	}
	
	const std::string& token::get_string() const {
		return std::get<std::string>(_value);
	}
	
	const token_value& token::get_value() const {
		return _value;
	}
	
	size_t token::get_line_number() const {
		return _line_number;
	}

	size_t token::get_char_index() const {
		return _char_index;
	}
	
	bool token::has_value(const token_value& value) const {
		return _value == value;
	}
	
	bool operator==(const identifier& id1, const identifier& id2) {
		return id1.name == id2.name;
	}
	
	bool operator!=(const identifier& id1, const identifier& id2) {
		return id1.name != id2.name;
	}
	
	bool operator==(const eof&, const eof&) {
		return true;
	}
	
	bool operator!=(const eof&, const eof&) {
		return false;
	}

	std::string algoritmik::to_string(reserved_token t) {
		return std::string(token_string_map.find(t)->second);
	}
	
	// bisa diprint
	std::string algoritmik::to_string(const token_value& t) {
		return std::visit(
			[](const auto& t) {
				if constexpr (std::is_same_v<decltype(t), const reserved_token&>) {
					return algoritmik::to_string(t);
				} else if constexpr (std::is_same_v<decltype(t), const long long&>) {
					return std::to_string(t);
				} else if constexpr (std::is_same_v<decltype(t), const double&>) {
					return std::to_string(t);
				} else if constexpr (std::is_same_v<decltype(t), const std::string&>) {
					return t;
				} else if constexpr (std::is_same_v<decltype(t), const identifier&>) {
					return t.name;
				} else if constexpr (std::is_same_v<decltype(t), const eof&>) {
					return std::string("<EOF>");
				}
			},
			t
		);
	}

}

