#ifndef CFOUNDATION_ITERATOR_H
#define CFOUNDATION_ITERATOR_H

namespace CFoundation
{
	template<class T>
	class Iterator
	{
	public:
		T		GetFirst();
		const T	GetFirst() const;
		T		GetNext();
		const T	GetNext() const;
		bool	IsDone() const;
		T		GetCurrent() const;
	};
}

#endif
