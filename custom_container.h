#pragma once
#include <memory>
#include <vector>

namespace allocator
{
	template<typename T, typename ALLOC = std::allocator<T>>
	class custom_container
	{
	public:
		using pointer	= T * ;
		using reference = T & ;
		using size_type = size_t;

		custom_container() = default;

		~custom_container()
		{
			for (auto i : storage)
			{
				alloc.destroy(i);
				alloc.deallocate(i, 1);
			}
		}

		ALLOC get_allocator() { return alloc; }

		size_type size() const noexcept { return storage.size(); }

		reference at(size_type pos)
		{
			if (pos >= size()) throw std::out_of_range("");
			return *storage[pos];
		}

		void push_back(const T& value)
		{
			auto ptr_item = alloc.allocate(1);
			alloc.construct(ptr_item, value);

			storage.push_back(ptr_item);
		}

	private:
		ALLOC alloc;
		std::vector<pointer> storage;
	};
}