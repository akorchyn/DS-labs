#pragma once

#include <cstdint>
#include <limits>

template < class T >
class IContainer
{
public:
	virtual ~IContainer() {}

    virtual void insert(const T&, size_t index) = 0;
    virtual void erase(size_t index) = 0;
    virtual void replace(const T&, size_t) = 0;

    virtual T sum() const = 0;
    virtual size_t find(const T&) const = 0;
	static constexpr size_t invalid_index = std::numeric_limits<size_t>::max();
};