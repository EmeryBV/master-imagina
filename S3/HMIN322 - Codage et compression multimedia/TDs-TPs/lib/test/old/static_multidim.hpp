#ifndef __STATIC_MULTIDIM_ARRAY__
#define __STATIC_MULTIDIM_ARRAY__

#include <array>
#include <vector>
#include <utility>
#include <iostream>

/*
template<typename T, size_t dim>
struct getTypeAtDim { using type = T; };

template<typename T, size_t N>
struct getTypeAtDim<T[N], 1> { using type = T; };

template<typename T, size_t dim, size_t N>
struct getTypeAtDim<T[N], dim> : getTypeAtDim<T, dim-1> {};

template<typename T, size_t dim>
using typeAtDim = typename getTypeAtDim<T, dim>::type;

template<typename T> 
typeAtDim<T, 1>& indexed(T& arr, const int& first)
{
    return arr[first];
}

template<typename T, typename... Args> 
typeAtDim<T, sizeof...(Args) + 1>& indexed(T& arr, const int& first, const Args& ...rest)
{
    return indexed(arr[first], rest...);
}

template <typename T, size_t PrimaryDimension, size_t... Dimensions>
struct MultiDim_impl 
{
  using Nested = typename MultiDim_impl<T, Dimensions...>::type;
  using type = Nested[PrimaryDimension];
};

template <typename T, size_t PrimaryDimension>
struct MultiDim_impl<T, PrimaryDimension> 
{
  using type = T[PrimaryDimension];
};

template<typename T, size_t PrimaryDimension, size_t... Dimensions>
using MultiDimArray = typename MultiDim_impl<T, PrimaryDimension, Dimensions...>::type;

template<typename T, size_t PrimaryDimension, size_t... Dimensions>
class Multidim_array
{
public:
	using type = MultiDimArray<T, PrimaryDimension, Dimensions...>;
	using value_type = T;
	using size_type = decltype(PrimaryDimension);
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = pointer;
  	using const_iterator = const_pointer;
  	using reverse_iterator = std::reverse_iterator<iterator>;
  	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  	// STL INTERFACE
	
	// Accessors
	// reference at(size_t pos)				{ return this->m_data.at(pos); }
	// const_reference at(size_t pos) const	{ return this->m_data.at(pos); }

	reference operator[](size_t pos) 				{ return this->m_data + pos; }	
	const_reference operator[](size_t pos) const 	{ return this->m_data + pos; }

	//reference front()				{ return this->m_data; }
	//const_reference front() const 	{ return this->m_data; }
	
	//reference back()				{ return this->m_data + (PrimaryDimension * ... * Dimensions) - 1; }
	//const_reference back() const 	{ return this->m_data + (PrimaryDimension * ... * Dimensions) - 1; }

	pointer data() 				{ return this->m_data; }
	const_pointer data() const 	{ return this->m_data; }

	// Iterators
	iterator begin() 				{ return this->data(); }
	const_iterator begin() const 	{ return this->data(); }

	iterator end() 				{ return this->data() + (PrimaryDimension * ... * Dimensions); }
	const_iterator end() const 	{ return this->data() + (PrimaryDimension * ... * Dimensions); }

	//Capacity
	constexpr bool empty() const { return begin() == end(); }
	constexpr size_type size() const { return std::distance(begin(), end()); }
	constexpr size_type max_size() const { return std::distance(begin(), end()); }

	// MULTIDIM_ARRAY INTERFACE
	
	typeAtDim<T, 1>& indexed(const int& first)
	{
	    return this->m_data[first];
	}

	template<typename... Args> 
	typeAtDim<T, sizeof...(Args) + 1>& indexed(const int& first, const Args& ...rest)
	{
	    return indexed(this->m_data[first], rest...);
	}

	template<typename... Args> 
	typeAtDim<T, sizeof...(Args) + 1>& operator()(const int& first, const Args& ...rest)
	{
	    return indexed(this->m_data[first], rest...);
	}

private:
	MultiDimArray<T, PrimaryDimension, Dimensions...> m_data;
};
*/
template <typename T, size_t PrimaryDimension, size_t... Dimensions>
struct Multidim_array_t_impl 
{
  using Nested = typename Multidim_array_t_impl<T, Dimensions...>::type;
  using type = std::array<Nested, PrimaryDimension>;
};

template <typename T, size_t PrimaryDimension>
struct Multidim_array_t_impl<T, PrimaryDimension> 
{
  using type = std::array<T, PrimaryDimension>;
};

template <typename T, size_t PrimaryDimension, size_t... Dimensions>
using Multidim_array_t = typename Multidim_array_t_impl<T, PrimaryDimension, Dimensions...>::type;

template<typename T, size_t PrimaryDimension, size_t... Dimensions>
struct Multidim_array : Multidim_array_t<T, PrimaryDimension, Dimensions...> {};


/*

z * height + y * width + x

	// STL INTERFACE
	
	// Accessors
	reference at(size_t pos)				{ return this->m_data.at(pos); }
	const_reference at(size_t pos) const	{ return this->m_data.at(pos); }

	reference operator[](size_t pos) 				{ return this->m_data[pos]; }	
	const_reference operator[](size_t pos) const 	{ return this->m_data[pos]; }

	reference front()				{ return this->m_data.front(); }
	const_reference front() const 	{ return this->m_data.front(); }
	
	reference back()				{ return this->m_data.back(); }
	const_reference back() const 	{ return this->m_data.back(); }

	pointer data() 				{ return this->m_data.data(); }
	const_pointer data() const 	{ return this->m_data.data(); }

	// Iterators
	iterator begin() 				{ return this->data(); }
	const_iterator begin() const 	{ return this->data(); }

	iterator end() 				{ return this->data() + this->size(); }
	const_iterator end() const 	{ return this->data() + this->size(); }

	//Capacity
	constexpr bool empty() const { return this->m_data.empty(); }
	constexpr size_type size() const { return this->m_data.size(); }
	constexpr size_type max_size() const { return this->m_data.max_size(); }
*/

#endif
