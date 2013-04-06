#ifndef COFILES_FILE_H
#define COFILES_FILE_H

namespace CoFiles
{
	class File
	{
	public:
		enum State
		{
			STATE_READ,
			STATE_READ_WRITE
		};

		File( const String &sFileName, State eState = STATE_READ_WRITE );

		
	};
}

#endif
