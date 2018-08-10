#pragma once
#include <memory>
#include <iostream>

#include "ndebug.h"

namespace allocator
{
	template<typename T, size_t BUF> class allocator_buffer
	{
	public:
		allocator_buffer			(const allocator_buffer&) = delete;
		allocator_buffer& operator=	(const allocator_buffer&) = delete;

		allocator_buffer() = default;

		T* TryGetMemory(size_t n)
		{
			if (num_reserved >= n)
			{
				auto addr = buffer.get() + BUF - num_reserved;
#ifndef NDEBUG
				std::cout << "use pre-allocated: [n = " << n << "] " << addr << std::endl;
#endif
				num_reserved -= n;
				return addr;
			}
			return nullptr;
		}

		void CreateBuffer(T* buf)
		{
			buffer = std::shared_ptr<T>(buf, [](T* p)
			{

					std::free(p);
#ifndef NDEBUG
					if (p)
					{
						std::cout << "free buffer memory: [n = " << BUF << "] " << p << std::endl;
					}
#endif
			});
			num_reserved = BUF;
		}

		bool ContainsAddress(T* addr) const
		{
			return addr >= buffer.get() && addr < buffer.get() + BUF;
		}
	private:
		std::shared_ptr<T> buffer = std::shared_ptr<T>(nullptr);
		size_t num_reserved = 0;
	};
}