#ifndef COFILES_BITMAKROS_H
#define COFILES_BITMAKROS_H

#define BITS_ON( _u32BitWord, _u32Bits )        { ( _u32BitWord ) |= ( _u32Bits ); }
#define BITS_OFF( _u32BitWord, _u32Bits )       { ( _u32BitWord ) &= ~( _u32Bits ); }
#define BITS_TOGGLE( _u32BitWord, _u32Bits )    { ( _u32BitWord ) ^= ( _u32Bits ); }

#define TEST_BITS_ON( _u32BitWord, _u32Bits )   ( ( ( _u32BitWord ) & ( _u32Bits ) ) == ( _u32Bits ) )

#endif
