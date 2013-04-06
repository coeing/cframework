#ifndef COFILES_RCPTR_H
#define COFILES_RCPTR_H

namespace CoFiles
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
