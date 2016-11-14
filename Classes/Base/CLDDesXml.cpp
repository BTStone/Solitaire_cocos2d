#include "CLDDesXml.h"

#define CDX_ALIGNMENT_8BIT(len)   ((len & 0x7) == 0 ? len : (len + 8 - (len & 0x7)))

FILE* TiXmlFOpen( const char* filename, const char* mode );

CLDDesXml::CLDDesXml()
: TiXmlDocument()
{
}

bool CLDDesXml::LoadDesFile(const char * fileName, TiXmlEncoding encoding )
{
	TIXML_STRING filename( fileName );
	value = filename;

	// Delete the existing data:
	Clear();
	location.Clear();
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	// reading in binary mode so that tinyxml can normalize the EOL
	FILE* file = TiXmlFOpen( value.c_str (), "rb" );	
	if ( !file )
	{
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
    	fclose( file );
		return false;
	}
#endif
	
	// Delete the existing data:
	Clear();
	location.Clear();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	long length = 0;
    char * buf = (char*)CCFileUtils::sharedFileUtils()->getFileData(fileName,"rb", &length );
#else
	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = 0;
	fseek( file, 0, SEEK_END );
	length = ftell( file );
	fseek( file, 0, SEEK_SET );

	// Strange case, but good to handle up front.
	if ( length <= 0 )
	{
		SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, TIXML_ENCODING_UNKNOWN );
    	fclose( file );
		return false;
	}

    long fixLen = CDX_ALIGNMENT_8BIT(length + 1); 
	char* buf = new char[ fixLen ];
	buf[0] = 0;

	if ( fread( buf, length, 1, file ) != 1 ) {
		delete [] buf;
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
    	fclose( file );
		return false;
	}
#endif
    
    // 解密文件数据
    //CLDSecurity::desDe(buf, buf, length); 

	bool result = LoadDesData( buf, length, encoding );
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	delete buf; 
#else
	delete [] buf;
	fclose( file );
#endif
	return result;
}

bool CLDDesXml::LoadDesData(char * buf, long bufLen, TiXmlEncoding encoding )
{
	if ( !buf ) 
	{
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
	}

	// Process the buffer in place to normalize new lines. (See comment above.)
	// Copies from the 'p' to 'q' pointer, where p can advance faster if
	// a newline-carriage return is hit.
	//
	// Wikipedia:
	// Systems based on ASCII or a compatible character set use either LF  (Line feed, '\n', 0x0A, 10 in decimal) or 
	// CR (Carriage return, '\r', 0x0D, 13 in decimal) individually, or CR followed by LF (CR+LF, 0x0D 0x0A)...
	//		* LF:    Multics, Unix and Unix-like systems (GNU/Linux, AIX, Xenix, Mac OS X, FreeBSD, etc.), BeOS, Amiga, RISC OS, and others
    //		* CR+LF: DEC RT-11 and most other early non-Unix, non-IBM OSes, CP/M, MP/M, DOS, OS/2, Microsoft Windows, Symbian OS
    //		* CR:    Commodore 8-bit machines, Apple II family, Mac OS up to version 9 and OS-9

	const char* p = buf;	// the read head
	char* q = buf;			// the write head
	const char CR = 0x0d;
	const char LF = 0x0a;

    long length = bufLen; 
	buf[length] = 0;
	while( *p ) {
		assert( p < (buf+length) );
		assert( q <= (buf+length) );
		assert( q <= p );

		if ( *p == CR ) {
			*q++ = LF;
			p++;
			if ( *p == LF ) {		// check for CR+LF (and skip LF)
				p++;
			}
		}
		else {
			*q++ = *p++;
		}
	}
	assert( q <= (buf+length) );
	*q = 0;

	Parse( buf, 0, encoding );

	return !Error();
}


bool CLDDesXml::DesEncodeFile(FILE * file)
{
    if ( !file )
    {
		SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
    }

	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = 0;
	fseek( file, 0, SEEK_END );
	length = ftell( file );
	fseek( file, 0, SEEK_SET );

	// Strange case, but good to handle up front.
	if ( length <= 0 )
	{
		SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, TIXML_ENCODING_UNKNOWN );
		return false;
	}

    long fixLen = CDX_ALIGNMENT_8BIT(length + 1); 
	char* buf = new char[ fixLen ];
    memset(buf, 0, fixLen); 

	if ( fread( buf, length, 1, file ) != 1 ) {
		delete [] buf;
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN );
    	fclose( file );
		return false;
	}
    
    // 加密文件数据
    //CLDSecurity::desEn(buf, buf, length); 

    fseek( file, 0, SEEK_SET );
    fwrite(buf, fixLen, 1, file); 
    delete [] buf;
    return true; 
}

bool CLDDesXml::SaveDesFile(const char * fileName)
{
	// The old c stuff lives on...
	FILE* fp = TiXmlFOpen( fileName, "wb+" );
	if ( !fp ) return false; 

	bool result = SaveFile( fp );
    if (!result)
    {
	    fclose( fp );
        return false; 
    }

    result = DesEncodeFile(fp); 
	fclose( fp );
	return result;
}
