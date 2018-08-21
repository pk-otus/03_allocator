#pragma once
#include <iostream>
#include "ndebug.h"
#include "allocator_buffer.h"

namespace allocator
{
	class iBufferedAllocator 
	{
	public:
		virtual ~iBufferedAllocator() = default;
		virtual void reserve() const = 0;
	};

	template<typename T, size_t BUF> class reserving_allocator : public iBufferedAllocator
	{
	public:
		using value_type		= T;
		using pointer			= T * ;
		using const_pointer		= const T*;
		using reference			= T & ;
		using const_reference	= const T&;

		template<typename U> struct rebind
		{
			using other = reserving_allocator<U, BUF>;
		};

		template<typename U> reserving_allocator(const reserving_allocator<U, BUF>& src) :
			parent(&src),
			buffer(new allocator_buffer<T, BUF>())
		{
#ifndef NDEBUG 
			std::cout << "copy constructor" << std::endl;
#endif
		}

		reserving_allocator() :
			parent(nullptr),
			buffer(new allocator_buffer<T, BUF>())
		{
		}
		~reserving_allocator() = default;

		T *allocate(size_t n) const
		{
			if (auto temp = buffer->TryGetMemory(n)) return temp;

			auto p = std::malloc(n * sizeof(T));
			if (!p)
				throw std::bad_alloc();

#ifndef NDEBUG 
			std::cout << "allocate: [n = " << n << "] " << p << std::endl;
#endif
			return static_cast<T*>(p);
		}

		void deallocate(T *p, size_t n)
		{
			if (buffer->ContainsAddress(p)) return;
#ifndef NDEBUG 
			std::cout << "deallocate: [n  = " << n << "] " << p << std::endl;
#endif
			std::free(p);
		}

		template<typename U, typename ...Args>	void construct(U *p, Args &&...args)
		{
#ifndef NDEBUG 
			std::cout << "construct" << std::endl;
#endif
			new(p) U(std::forward<Args>(args)...);
		}

		void destroy(T *p)
		{
#ifndef NDEBUG 
			std::cout << "destroy" << std::endl;
#endif
			p->~T();
		}

		void reserve() const override
		{
			if (parent) 
				parent->reserve();

			buffer->CreateBuffer(allocate(BUF));
		}


	private:
		const iBufferedAllocator*	parent;
		std::shared_ptr<allocator_buffer<T, BUF>>	buffer;
	};
}
