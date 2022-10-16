#ifndef tokens_hpp
#define tokens_hpp

#include <optional>
#include <string_view>
#include <ostream>
#include <variant>

namespace algoritmik {
	enum struct reserved_token {

		add,
		sub,
		mul,
		div,
		idiv,
		mod,
		
		assign,
		traversal,

		logical_not,
		logical_and,
		logical_or,
		
		eq,
		ne,
		lt,
		gt,
		le,
		ge,
		
		question,
		colon,
		
		comma,
		
		semicolon,
		
		open_round,
		close_round,
		
		open_curly,
		close_curly,
		
		open_square,
		close_square,
		

		kw_sizeof,
		kw_tostring,
		
		kw_if,
		kw_else,
		// kw_elif,

		kw_switch,
		kw_case,
		kw_default,

		kw_for,
		kw_while,
		kw_do,
		kw_repeat,
		kw_times,
		kw_until,
		kw_iterate,
		kw_stop,
		kw_traversal,


		kw_return,

		kw_function,
		kw_procedure,
		kw_program,

		kw_kamus,
		kw_kamus_lokal,
		kw_algoritma,
		
		kw_void,
		kw_number,
		kw_string,
		
	};
	
	class push_back_stream;
	
	std::ostream& operator<<(std::ostream& os, reserved_token t);
	
	std::optional<reserved_token> get_keyword(std::string_view word);
	
	std::optional<reserved_token> get_operator(push_back_stream& stream);
	
	struct identifier{
		std::string name;
	};
	
	bool operator==(const identifier& id1, const identifier& id2);
	bool operator!=(const identifier& id1, const identifier& id2);
	
	struct eof{
	};
	
	bool operator==(const eof&, const eof&);
	bool operator!=(const eof&, const eof&);

	using token_value = std::variant<reserved_token, identifier, long long, double, std::string, eof>;

	class token {
	private:
		token_value _value;
		size_t _line_number;
		size_t _char_index;
	public:
		token(token_value value, size_t line_number, size_t char_index);
		
		bool is_reserved_token() const;
		bool is_identifier() const;
		bool is_number() const;
		bool is_string() const;
		bool is_eof() const;
		
		reserved_token get_reserved_token() const;
		const identifier& get_identifier() const;
		double get_number() const;
		const std::string& get_string() const;
		const token_value& get_value() const;
		
		size_t get_line_number() const;
		size_t get_char_index() const;

		bool has_value(const token_value& value) const;
	};
	std::string to_string(algoritmik::reserved_token t);
	std::string to_string(const algoritmik::token_value& t);
}

#endif /* tokens_hpp */
