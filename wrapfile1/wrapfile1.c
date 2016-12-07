//FileName:		wrapfile.c
//Project:		wrapfile.exe
//Author:		Alex Guerrero	
//Date:			December 6, 2013
//Description:	This program reads text files from the command line using argc and argv[] and outputs the contents to the console. 
//				The ouput text is wrapped at a default column width of 40 characters if no command and column width is entered.
//				To enter a specified column width, the user must enter -w or - w followed by a numeric column width followed by the 
//				input file name.  Input files must be located in the current directory for the program to successfully function.
			


//Include files
#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>



//the following piece of code allows for the use of the fopen() function
//and disables the visual studio warning
#pragma warning(disable: 4996)



//Constants
#define kUnsignedIntValue 65535
#define kDefaultLength 40



//function prototype
int getSmallFileLength( const char * fileName );



int main( int argc, char * argv[] )
{
	//variables for use in the program initialized
    FILE *ifp = NULL;
    char * buffer = NULL;
    int argCount = 1;
    int inputFile = 0;
    unsigned int columnWidth = 0;
    int fileSize = 0;
    unsigned int textCount = 0;
    int wrapLength = 0;
	int exitStatus = 0;


	//program will loop while the input files are less than the argument count
    while( argCount < argc )
    {
		//the following code compares the string found in argv[argCount] and -w and - w.
		//If they are equal, the argCount variable is incremented to get the user specified column width.
		//If the argument is not a number or is empty, a message is displayed to inform the user of the proper way to input the wrapfile command
		//and exits, return 2 to main. If the value is a number,
		//the value is assigned to columnWidth and tested to be a value between 1 and 65535. If the value is out of that range
		//an error message is displayed and the program exits.  If the columnWidth value is valid, the argument count is incremented by
		//one to read the input file and the columnWidth is reset to zero to track the column position. 
        if( strcmp( argv[argCount], "-w" ) == 0 || strcmp( argv[argCount], "- w" ) == 0 )
        {
			++argCount;
			if( argv[argCount] == NULL )//if following argument is not there
			{
				printf("Usage: wrapfile [-w width] filename ...");
				exitStatus =2;
				break;
			}
			if( atoi( argv[argCount] ) == 0 )//if argument is not a number
			{
				printf("Usage: wrapfile [-w width] filename ...");
				exitStatus =2;
				break;
			}
			else
			{
				columnWidth = atoi( argv[argCount] );
				wrapLength = columnWidth;
			}
		
            if( columnWidth <= kUnsignedIntValue && columnWidth > 0 )
            {
                columnWidth = 0;
                inputFile = ++argCount;

				if( argv[argCount] == NULL )//if file is not there
				{
					printf("Usage: wrapfile [-w width] filename ...");
					exitStatus =2;
					break;
				}

				//The following code uses a file pointer to receive an address from fopen() if successful to 
				//open a text file for reading.  If the file is not successfully opened, a message is displayed
				//to the user and the program exits.  1 is returned to main indicating the file cannot be opened
                ifp = fopen( argv[inputFile], "r" );
				if( ifp == NULL )
				{
					printf( "File not openable\n" );
					exitStatus = 1; //can't open file
					break;
				}
	
				//The following code passes the input file as a string to the function getSmallFileLength
				//to retrieve the the file size. getSmallFileLength returns -1 if no file size was retrieved.
				//If the fileSize is less than 0, an error message is displayed, the program returns 4 and
				//exits
                fileSize = getSmallFileLength( argv[inputFile] );
				if( fileSize < 0 )
				{
					printf( "File size not found\n" );
					exitStatus = 4;//fatal error
					break;
				}

				//The following code uses malloc() to dynamically allocate space in memory to buffer for the filesize gotten by 
				//getSmallFileLength.
				//If Buffer is NULL, an error message is displayed, the program returns 4 and exits
                buffer = ( char * )malloc( fileSize );
				if( buffer == NULL )
				{
					printf( "No memory allocated for file input\n" );
					exitStatus = 4;//fatal error
					break;
				}

				//The following code uses fgets to read in a string of input at a time from the input file using the
				//file pointer and assigns it to buffer with the file size retrieved previously.  The code counts the
				//length of the string and prints it's contents to the console window. a count of each character is kept 
				//and if the count equals the user defined wraplength, a newline is printed. If the program encounters 
				//a newline at the end of the string, it is replaced with a NULL character and loops to get the next line of input.
				while( fgets( buffer, fileSize, ifp ) != NULL )
                {
                    for( textCount = 0; textCount < strlen( buffer ); ++textCount )
                    {
                        if( columnWidth == wrapLength )
                        {
                            printf("\n");
                            columnWidth = 0;
                        }
                        if( buffer[textCount] == '\n' )
                        {
                            buffer[textCount] = '\0';
                        }
                        printf("%c", buffer[textCount]);
                        ++columnWidth;
                    }
                }
			}

			//If the value is out of a range of 1 and 65535,
			//an error message is displayed, the program exits
			// and returns 4.
			else
			{
				printf( "Width is not a positive integer between 1 and 65535\n" );
				exitStatus = 4;//fatal error
				break;
			}

			//The following code frees the memory allocated by malloc and the input file is closed.
			//if fclose() fails, the program returns 4 and exits
			free( buffer );
			if( fclose( ifp ) != 0 )
			{
				printf( "File was not closed\n" );
				exitStatus = 4; //fatal error
				break;
			}

			//The foloowing code adds a newline to the end of the file on the console window 
			//and increments the argument count to read the next string in argv[].
			//If the next argument in argv[] is null, there are no more files to read and the program exits
            printf( "\n" );
            ++argCount;
            if( argv[argCount] == NULL )
            {
				exitStatus = 0;
				break;
            }
        }

		//The following code compares the string found in argv[] to -w and - w.  If they are equal, a continue statement returns the program to the
		//beginning of the while loop to perform a user specified wrapLength. If they are not equal, 
		//the program checks whether the argument in argv[] is a number or a single "-" character.  If it is a number, the program exits and returns 2 to main.
		//if the argument is a file the program enters the default wraplength section of the code and reads the first argument as the inputfile.
		//In this section the default wraplength is 40 characters.
		if( ( strcmp( argv[argCount], "-w" ) == 0 ) || ( strcmp( argv[argCount], "- w" ) == 0 ) )
		{
			continue;
		}

		if( strcmp( argv[argCount], "-" ) == 0 )//if argument is only a "-" character
		{
			printf("Usage: wrapfile [-w width] filename ...");
			exitStatus = 2;
			break;
		}
 
		if( atoi( argv[argCount] ) != 0 )//if argument is not a file....
		{
			printf("Usage: wrapfile [-w width] filename ...");
			exitStatus = 2;
			break;
		}
		
		if( ( strcmp( argv[argCount], "-w" ) != 0 ) || ( strcmp( argv[argCount], "- w" ) != 0 ) )
        {
			columnWidth = kDefaultLength;
			wrapLength = columnWidth;
			columnWidth = 0;
            inputFile = argCount;

			//The following code uses a file pointer to receive an address from fopen() if successful to 
			//open a text file for reading.  If the file is not successfully opened, a message is displayed
			//to the user and the program exits.  1 is returned to main indicating the file cannot be opened
            ifp = fopen( argv[inputFile], "r" );
			if( ifp == NULL )
			{
				printf( "File not openable\n" );
				break;
				exitStatus = 1;//can't open file
			}

			//The following code passes the input file as a string to the function getSmallFileLength
			//to retrieve the the file size. getSmallFileLength returns -1 if no file size was retrieved.
			//If the fileSize is less than 0, an error message is displayed, the program returns 4 and
			//exits
            fileSize = getSmallFileLength( argv[inputFile] );
			if( fileSize < 0 )
			{
				printf( "File size not found\n" );
				exitStatus = 4;//fatal error
			}

			//The following code uses malloc() to dynamically allocate space in memory to buffer for the filesize gotten by 
			//getSmallFileLength.
			//If Buffer is NULL, an error message is displayed, the program returns 4 and exits
            buffer = ( char * )malloc( fileSize );
			if( buffer == NULL )
			{
				printf( "No memory allocated for file input\n" );
				break;
				exitStatus = 4;//fatal error
			}

			//The following code uses fgets to read in a string of input at a time from the input file using the
			//file pointer and assigns it to buffer with the file size retrieved previously.  The code counts the
			//length of the string and prints it's contents to the console window. a count of each character is kept 
			//and if the count equals the default wraplength of 40 characters, a newline is printed. If the program encounters 
			//a newline at the end of the string, it is replaced with a NULL character and loops to get the next line of input.
            while( fgets(buffer, fileSize, ifp ) != NULL )
            {
				for( textCount = 0; textCount < strlen(buffer); ++textCount )
				{
					if( columnWidth == wrapLength )
					{
						printf( "\n" );
						columnWidth = 0;
					}
					if( buffer[textCount] == '\n' )
					{
						buffer[textCount] = '\0';
					}

                    printf( "%c", buffer[textCount] );
                    ++columnWidth;
					}
			}

			// The following code frees the memory allocated by malloc and the input file is closed.
			//if fclose() fails, the program returns 4 and exits
			free( buffer );
			if( fclose( ifp ) != 0 )
			{
				printf( "File was not closed\n" );
				break;
				exitStatus = 4;//fatal error
			}
			
			//The foloowing code adds a newline to the end of the file on the console window 
			//and increments the argument count to read the next string in argv[].
			//If the next argument in argv[] is null, there are no more files to read and the program exits
            printf( "\n" );
            ++argCount;
            if( argv[argCount] == NULL )
            {
				exitStatus = 0;
                break;
            }
        }
    }
	if( exitStatus == 4 )
	{
		return 4;//fatal error
	}
	if( exitStatus == 1 )
	{
		return 1;//can't open file
	}
	if( exitStatus == 2 )
	{
		return 2;//command line options incorrect
	}
	if( exitStatus == 0 )
	{
		return 0;//Successful exit
	}
}



//FileName:		fileUtilities.c
//Project:		cA5
//Author:		Alex Guerrero	
//Date:			December 2, 2013
//Description:	This function works properly for files that are less than 2 billion bytes in size.  
//				The function retrieves the size of the file through the use of the WIN32_FIND_DATA structure and returns
//				the nfilesizelow field otherwise the function returns -1 if unsuccessful
int getSmallFileLength( const char * fileName )
{
	//variables initialized
    WIN32_FIND_DATA fileData = {0};
    int fileSize = 0;
    HANDLE h = NULL;

	//passes file data to h
    h = FindFirstFile( fileName, &fileData );
	//if no data was retrieved, user is informed with a print statement and -1 is returned
    if( h == INVALID_HANDLE_VALUE )
    {
        printf("file data not found");
        return -1;
    }
	//if data was found, the filesize is returned
    else
    {
		//This function works properly for files less than 2 billion bytes in size
        fileSize = fileData.nFileSizeLow;
        return fileSize;
    }
}