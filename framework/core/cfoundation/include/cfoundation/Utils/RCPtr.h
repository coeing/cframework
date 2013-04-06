#ifndef CFoundation_RCPTR_H
#define CFoundation_RCPTR_H

namespace CFoundation
{
	template <class T>
	class RCPtr
	{
	public:
		RCPtr( T *pT = 0);
		RCPtr( const RCPtr &rhs );
		~RCPtr();

		RCPtr& operator=( const RCPtr &rhs );

		T* operator->() const;
		T& operator*() const;

	private:
		void Init();

		T	*m_pT;
	};
};

#endif
