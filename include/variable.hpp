#ifndef variable_hpp
#define variable_hpp

#include <memory>
#include <deque>
#include <vector>
#include <functional>
#include <string>

namespace algoritmik {

	class variable;
	
	using variable_ptr = std::shared_ptr<variable>;

	template <typename T>
	class variable_impl;
	
	class runtime_context;
	
	using integer = long long;
	using real = double;
	using character = char;
	using boolean = bool;
	using string = std::shared_ptr<std::string>;
	using array = std::deque<variable_ptr>;
	using function = std::function<void(runtime_context&)>;
	using tuple = array;
	using initializer_list = array;
	
	using lvalue = variable_ptr;
	using linteger = std::shared_ptr<variable_impl<integer> >;
	using lreal = std::shared_ptr<variable_impl<real> >;
	using lcharacter = std::shared_ptr<variable_impl<character> >;
	using lboolean = std::shared_ptr<variable_impl<boolean> >;
	using lstring = std::shared_ptr<variable_impl<string> >;
	using larray = std::shared_ptr<variable_impl<array> >;
	using lfunction = std::shared_ptr<variable_impl<function> >;
	using ltuple = std::shared_ptr<variable_impl<tuple> >;

	class variable: public std::enable_shared_from_this<variable> {
	private:
		variable(const variable&) = delete;
		void operator=(const variable&) = delete;
	protected:
		variable() = default;
	public:
		virtual ~variable() = default;

		template <typename T>
		T static_pointer_downcast() {
			return std::static_pointer_cast<
				variable_impl<typename T::element_type::value_type>
			>(shared_from_this());
		}
		
		virtual variable_ptr clone() const = 0;
		
		virtual string to_string() const = 0;
	};
	
	template<typename T>
	class variable_impl: public variable {
	public:
		using value_type = T;
		
		value_type value;
		
		variable_impl(value_type value);
		
		variable_ptr clone() const override;
	
		string to_string() const override;
	};
	
	integer clone_variable_value(integer value);
	real clone_variable_value(real value);
	character clone_variable_value(character value);
	boolean clone_variable_value(boolean value);
	string clone_variable_value(const string& value);
	function clone_variable_value(const function& value);
	array clone_variable_value(const array& value);
	
	template <class T>
	T clone_variable_value(const std::shared_ptr<variable_impl<T> >& v) {
		return clone_variable_value(v->value);
	}
	
	string convert_to_string(integer value);
	string convert_to_string(real value);
	string convert_to_string(character value);
	string convert_to_string(boolean value);
	string convert_to_string(const string& value);
	string convert_to_string(const function& value);
	string convert_to_string(const array& value);
	string convert_to_string(const lvalue& var);
}

#endif /* variable_hpp */
