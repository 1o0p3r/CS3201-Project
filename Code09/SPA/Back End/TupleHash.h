#pragma once

//code snippet for hashing tuple from stackoverflow
#include <tuple>
#include <vector>
namespace std {
	namespace
	{
		// Reciprocal of the golden ratio helps spread entropy
		//     and handles duplicates.
		// See Mike Seymour in magic-numbers-in-boosthash-combine:
		//     http://stackoverflow.com/questions/4948780

		template <class T>
		inline void hash_combine(std::size_t& seed, T const& v)
		{
			seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

		// Recursive template code derived from Matthieu M.
		template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
		struct HashValueImpl
		{
			static void apply(size_t& seed, Tuple const& tuple)
			{
				HashValueImpl<Tuple, Index - 1>::apply(seed, tuple);
				hash_combine(seed, std::get<Index>(tuple));
			}
		};

		template <class Tuple>
		struct HashValueImpl<Tuple, 0>
		{
			static void apply(size_t& seed, Tuple const& tuple)
			{
				hash_combine(seed, std::get<0>(tuple));
			}
		};
	}

	template <typename ... TT>
	struct hash<std::tuple<TT...>>
	{
		size_t
			operator()(std::tuple<TT...> const& tt) const
		{
			size_t seed = 0;
			HashValueImpl<std::tuple<TT...> >::apply(seed, tt);
			return seed;
		}

	};
	
	//code from stackoverflow, cartesian product
	// cross_imp(f, v...) means "do `f` for each element of cartesian product of v..."
	template<typename F>
	inline void cross_imp(F f) {
		f();
	}
	template<typename F, typename H, typename... Ts>
	inline void cross_imp(F f, std::vector<H> const& h,
		std::vector<Ts> const&... t) {
		for (H const& he : h)
			cross_imp([&](Ts const&... ts) {
			f(he, ts...);
		}, t...);
	}

	template<typename... Ts>
	std::vector<std::tuple<Ts...>> cross(std::vector<Ts> const&... in) {
		std::vector<std::tuple<Ts...>> res;
		cross_imp([&](Ts const&... ts) {
			res.emplace_back(ts...);
		}, in...);
		return res;
	}

	template <typename T> vector<T> intersectionT(vector<T> v1, vector<T> v2)
	{
		vector<T> v3;

		sort(v1.begin(), v1.end());
		sort(v2.begin(), v2.end());

		set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

		return v3;
	}


}
