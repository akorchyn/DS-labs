#pragma once

template <class T>
class ICollection
{
public:
	virtual ~ICollection() {}

	virtual void add(const T& item) = 0;
	virtual bool contains(const T& item) const = 0;
	virtual bool remove(const T& item) = 0;
	virtual size_t size() const = 0;
};