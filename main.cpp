#include <iostream>
#include <map>

#include "reserving_allocator.h"
#include "custom_container.h"

// debug output to console
#define NDEBUG 1 

int main(int, char *[])
{
	const size_t BUFFER_SIZE = 10;
	
	using custom_allocator_int_t = allocator::reserving_allocator<int, BUFFER_SIZE >;
	using custom_allocator_pair_t = 
			allocator::reserving_allocator<std::pair<const int, int>, BUFFER_SIZE>;
	   
	using tested_map_t			= std::map<int, int, std::less<int>, custom_allocator_pair_t>;
	using tested_container_t	= allocator::custom_container<int, custom_allocator_int_t>;

	try
	{
		{
			auto map_custom_no_reserve = tested_map_t{};

			//no call get_allocator().Reserve();
			//default allocating strategy

			for (int i = 0, fact = 1; i < BUFFER_SIZE; fact *= ++i)
			{
				map_custom_no_reserve[i] = fact;
			}

			for (auto iter : map_custom_no_reserve)
			{
				std::cout << iter.first << ' ' << iter.second << '\n';
			}
		}

		{
			auto m_custom_uses_reserve = tested_map_t{};

			//same allocator, reserve called
			m_custom_uses_reserve.get_allocator().reserve();

			for (int i = 0, fact = 1; i < BUFFER_SIZE; fact *= ++i)
			{
				m_custom_uses_reserve[i] = fact;
			}

			for (auto iter : m_custom_uses_reserve)
			{
				std::cout << iter.first << ' ' << iter.second << '\n';
			}
		}

		{
			auto cc_default_alloc = tested_container_t();

			// or
			// auto cc_default_alloc = allocator::custom_container<int>();

			//no call get_allocator().Reserve();
			//default allocating strategy

			for (int i = 0; i < BUFFER_SIZE; ++i)
			{
				cc_default_alloc.push_back(i);
			}

			for (size_t i = 0; i < cc_default_alloc.size(); ++i)
			{
				std::cout << cc_default_alloc.at(i) << ' ';
			}
			std::cout << std::endl;
		}

		{
			auto cc_custom_alloc = tested_container_t();

			//same allocator, reserve called
			cc_custom_alloc.get_allocator().reserve();

			for (int i = 0; i < BUFFER_SIZE; ++i)
			{
				cc_custom_alloc.push_back(i);
			}

			for (size_t i = 0; i < cc_custom_alloc.size(); ++i)
			{
				std::cout << cc_custom_alloc.at(i) << ' ';
			}
			std::cout << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
