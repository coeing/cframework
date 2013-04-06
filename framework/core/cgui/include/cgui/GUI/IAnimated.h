#ifndef CLIB_IANIMATED_H
#define CLIB_IANIMATED_H

namespace CGui
{
	class AnimateParams;

	class IAnimated
	{
	public:
		virtual void Animate( const AnimateParams &clParams ) = 0;
	};
};

#endif
