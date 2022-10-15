#ifndef push_back_stream_h
#define push_back_stream_h
#include <stack>
#include <functional>

namespace algoritmik {
	// masih ga paham yang ni
	using get_character = std::function<int()>;

	class push_back_stream {
	private:
		const get_character& _input;
		std::stack<int> _stack;
		size_t _line_number;
		size_t _char_index;

	public:
		/*
		Konstruktor
		*/
		push_back_stream(const get_character* input);
		
		/*
		Functors?
		dari yang gw baca, ini ngebuat objek yang dibuat dari class
		jadi bisa dipanggil kayak fungsi, uniknya pas dipanggil
		si Functors ini masih punya state jadinya dipanggil ke satu
		ke satu lainnya bisa aja beda?

		return posisi?
		*/
		int operator()();
		
		void push_back(int c);
		
		size_t line_number() const;
		size_t char_index() const;
	};
}

#endif /* push_back_stream_h */
