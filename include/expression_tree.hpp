#ifndef expression_tree_hpp
#define expression_tree_hpp
#include <memory>
#include <variant>
#include <vector>
#include "tokens.hpp"
#include "types.hpp"

namespace algoritmik {

	enum struct node_operation {
		param,

		positive,
		negative,
		lnot,
		size,
		tostring,
		
		add,
		sub,
		mul,
		div,
		idiv,
		mod,

		assign,

		eq,
		ne,
		lt,
		gt,
		le,
		ge,

		comma,
		land,
		lor,

		index,

		traversal,
		
		ternary,
		
		call,
		
		init,
	};
	
	struct node;
	using node_ptr=std::unique_ptr<node>;
	
	using node_value=std::variant<node_operation, std::string, double, identifier>;
	
	class compiler_context;
	
	struct node {
	private:
		node_value _value;
		std::vector<node_ptr> _children;
		type_handle _type_id;
		bool _lvalue;
		size_t _line_number;
		size_t _char_index;
	public:
		node(compiler_context& context, node_value value, std::vector<node_ptr> children, size_t line_number, size_t char_index);
		
		const node_value& get_value() const;
		
		bool is_node_operation() const;
		bool is_identifier() const;
		bool is_integer() const;
		bool is_real() const;
		bool is_character() const;
		bool is_boolean() const;
		bool is_string() const;
		
		node_operation get_node_operation() const;
		std::string_view get_identifier() const;
		long long get_integer() const;
		double get_real() const;
		char get_character() const;
		bool get_boolean() const;
		std::string_view get_string() const;

		const std::vector<node_ptr>& get_children() const;
		
		type_handle get_type_id() const;
		bool is_lvalue() const;
		
		size_t get_line_number() const;
		size_t get_char_index() const;
		
		void check_conversion(type_handle type_id, bool lvalue) const;
	};

}

#endif /* expression_tree_hpp */
