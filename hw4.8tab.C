//-------------------------------------------------------------------
// File		:hw4.C
// Author	:Michael Benson
// Course	:CS1129
// Date		:April 5, 2006
//
//	hw4.C is an object oriented version of hw3.C, a music library 
//	management program
//
//	IMPLIMENTED BONUS FEATURES:
//
//	* Spaces accepted in Title, Artist, and Album fields *
//
//	* Title, Artist, and Album fields have no length limit *
//
//-------------------------------------------------------------------

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cctype>
#include <cstring>

using namespace std;

// Global Variables
const char LIBFILE[] = "musicLibrary.txt";
const int ID_WIDTH = 4;
const int MAXDISP_TITLE = 27;
const int MAXDISP_ALBUM = 24;
const int MAXDISP_ARTIST = 24;
const int GENRE_WIDTH = 15;
const int RATING_WIDTH = 7;
const int BUFFER = 3;
const int MAXLIB_SIZE = 100;
const int BREAK_WIDTH = ID_WIDTH + MAXDISP_TITLE + BUFFER + 
	MAXDISP_ALBUM + BUFFER + MAXDISP_ARTIST + BUFFER + 
	GENRE_WIDTH + RATING_WIDTH;
	

// Class Definitions

class Song
{
	public:
		Song(); // Empty Constructor
		void readInData(ifstream& inStream);
		void writeSongData(ofstream& outStream);
		void addTitle();
		void addArtist();
		void addAlbum();
		void addGenre();
		void addRating();
		void displayGenres();
		bool genreTranslate();
		bool ratingTranslate();
		void displaySongData();
		bool empty();
		void clearData();
		string returnTitle() {return title;}

	private:
		string title;
		string artist;
		string album;
		char genre;
		int rating;
};

class Library
{
	public:
		Library(); // Empty Constructor
		bool openInput();
		bool openOutput();
		void loadLibrary();
		void addSong();
		void removeSong();
		void libStats();
		void writeLib();
		void printHeaders();
		void viewLib();

	private:
		ifstream inStream;
		ofstream outStream;
		
		int libSize;
		Song libData[MAXLIB_SIZE];
};


// General Use Functions
void splashScreen();
int mainMenu();
void menuControl(Library& mainLibrary);
bool confirm();
void getNoError(char&);
void getNoError(string& input);
void leaving(Library& mainLibrary);

int main()
{
	// Create the main Library
	Library mainLibrary;

	// Initalize the library with data from LIBFILE
	mainLibrary.loadLibrary();
	
	// Begin UI
	menuControl(mainLibrary);
}

// Default Library Contructor
Library::Library()
{/*Intentionally Empty*/}

// Default Song Constructor
Song::Song()
{/*Intentionally Empty*/}

//-------------------------------------------------------------------
//	Song::readInData(ifstream&)
//
// Preconditions	:Needs an ifstream pointing at relivent data
//
// Postconditions	:Loads data being pointed at into memory
//-------------------------------------------------------------------

void Song::readInData(ifstream& inStream)
{
	getline(inStream, title);
	getline(inStream, album);
	getline(inStream, artist);
	inStream.get(genre);

	// Skip preceeding white spaces
	while( isspace( inStream.peek() ) )
	{
		inStream.get();
	}
	inStream >> rating;

	// Skip trailing white spaces
	while( isspace( inStream.peek() ) )
	{
		inStream.get();
	}
	
}

//-------------------------------------------------------------------
//	Song::writeSongData(ofstream&)
//
// Preconditions	:None
//
// Postconditions	:Writes the data of the song to the ofstream
//-------------------------------------------------------------------

void Song::writeSongData(ofstream& outStream)
{
	outStream << title << endl;
	outStream << album << endl;
	outStream << artist << endl;
	outStream << genre << endl;
	outStream << rating << endl;
}

//-------------------------------------------------------------------
//	Song::addTitle()
//
// Preconditions	:None
//
// Postconditions	:Loads a new title into memory based on input
//-------------------------------------------------------------------

void Song::addTitle()
{
	title.clear();

	// Get title from user
	cout << "Please enter a title : ";
	getNoError(title);

	// Ask for confirmation
	cout << "You entered : " << title << endl;
	if(confirm() == false )
		return addTitle();
}

//-------------------------------------------------------------------
//	Song::addArtist()
//
// Preconditions	:None
//
// Postconditions	:Loads a new artist into memory based on input
//-------------------------------------------------------------------

void Song::addArtist()
{
	artist.clear();
	
	// Get artist from user
	cout << "Please enter an artist : ";
	getNoError(artist);

	// Ask for confirmation
	cout << "You entered : " << artist << endl;
	if ( confirm() == false )
		return addArtist();
}

//-------------------------------------------------------------------
//	Song::addAlbum()
//
// Preconditions	:None
//
// Postconditions	:Loads a new album into memory based on input
//-------------------------------------------------------------------

void Song::addAlbum()
{
	album.clear();
	
	// Get album from user
	cout << "Please enter a album : ";
	getNoError(album);

	// Ask for confirmation
	cout << "You entered : " << album << endl;
	if(confirm() == false )
		return addAlbum();
}

//-------------------------------------------------------------------
//	Song::addGenre()
//
// Preconditions	:None
//
// Postconditions	:Loads a genre into memory based on user input
//-------------------------------------------------------------------

void Song::addGenre()
{
	// Show the user possable genres
	cout << endl;
	displayGenres();
	cout << endl;

	// Get genre from user
	cout << "Please enter a genre : ";
	getNoError(genre);

	// Check for valid genre and translate it
	cout << "You entered : ";
	if (genreTranslate() == false)
		return addGenre();
	
	// Ask for confirmation
	cout << endl;
	if(confirm() == false)
		return addGenre();
}

//-------------------------------------------------------------------
//	Song::addRating()
//
// Preconditions	:None
//
// Postconditions	:Prompts for and stores a new rating for 
// 			 the song
//-------------------------------------------------------------------

void Song::addRating()
{
	char input;
	rating = 0;

	// Ask for rating from user
	cout << "Please enter a rating (e.g. ***) : ";
	cin.get(input);

	// Skip pre-ceeding returns
	while (cin.peek() == '\n')
	{
		cin.ignore(1);
	}
	
	// Make sure user only inputs * characters/increments rating
	while (input != '\n')
	{
		if (input == '*')
			rating++;
		else
		{
			cout << endl << "Invalid Rating" << endl;
			return addRating();
		}
		cin.get(input);
	}
	
	// Translate the rating and check it is valid
	cout << "You entered : ";
	if (ratingTranslate() == false)
	{
		cout << endl;
		return addRating();
	}
	cout << endl;
	
	// Ask for confirmation
	if(confirm() == false)
		return addRating();
}

//-------------------------------------------------------------------
//	Song::displayGenres()
//
// Preconditions	:None
//
// Postconditions	:Prints out in plain text the Genre options
//-------------------------------------------------------------------

void Song::displayGenres()
{
	// Display possable genres
	cout << "a - Alternative" << endl;
	cout << "b - Blues" << endl;
	cout << "c - Classical" << endl;
	cout << "d - Country" << endl;
	cout << "e - Hip-Hop" << endl;
	cout << "f - Jazz" << endl;
	cout << "g - Pop" << endl;
	cout << "h - Rap" << endl;
	cout << "i - Reggae" << endl;
	cout << "j - Religious" << endl;
	cout << "k - Rock" << endl;
	cout << "l - R&B" << endl;
	cout << "m - Techno" << endl;
	cout << "n - World" << endl;
	cout << "o - Other" << endl;
}

//-------------------------------------------------------------------
//	Song::genreTranslate()
//
// Preconditions	:None
//
// Postconditions	:Prints out the Plain Text versoin of the 
// 			 char genre and returns a bool true if it 
// 			 is a valid genre
//-------------------------------------------------------------------

bool Song::genreTranslate()
{
	switch(genre)
	{
		case 'a':
			cout << "Alternative";
			break;
		case 'b':
			cout << "Blues";
			break;
		case 'c':
			cout << "Classical";
			break;
		case 'd':
	            cout << "Country";
	            break;
	        case 'e':
	            cout << "Hip-Hop";
	            break;
	        case 'f':
	            cout << "Jazz";
	            break;
	        case 'g':
	            cout << "Pop";
	            break;
	        case 'h':
	            cout << "Rap";
	            break;
	        case 'i':
	            cout << "Reggae";
	            break;
	        case 'j':
	            cout << "Religious";
	            break;
	        case 'k':
	            cout << "Rock";
	            break;
	        case 'l':
	            cout << "R&B";
	            break;
	        case 'm':
	            cout << "Techno";
	            break;
	        case 'n':
	            cout << "World";
		    break;
	        case 'o':
        	    cout << "Other";
	            break;
		default:
		    cout << "Invalid Genre";
		    return false;
	}
	return true;
}	
			
//-------------------------------------------------------------------
//	Song::ratingTranslate()
//
// Preconditions	:None
//
// Postconditiosn	:Prints out a Plain Text version of the rating
// 			 data in the song and returns a bool 
// 			 true/false based on if its a valid rating
//-------------------------------------------------------------------

bool Song::ratingTranslate()
{
	switch(rating)
	{
		case 1:
			cout << "*";
			break;
		case 2:
			cout << "**";
			break;
		case 3:
			cout << "***";
			break;
		case 4:
			cout << "****";
			break;
		case 5:
			cout << "*****";
			break;
		default:
			cout << "Invalid Rating";
			return false;
	}
	return true;
}

//-------------------------------------------------------------------
//	Song::displaySongData()
//
// Preconditions	:None
//
// Postconditions	:Prints out formatted song data
//-------------------------------------------------------------------

void Song::displaySongData()
{
	
	cout.width(MAXDISP_TITLE + BUFFER);
	// Test if the length of the title is too long to display
	if (title.length() > MAXDISP_TITLE)
		cout << title.substr(0,MAXDISP_TITLE)+"..";
	else
		cout << title;
	
	cout.width(MAXDISP_ALBUM + BUFFER);
	// Test if the length of the album is too long to display
	if (album.length() > MAXDISP_ALBUM)
		cout << album.substr(0,MAXDISP_ALBUM)+"..";
	else 
		cout << album;
	
	cout.width(MAXDISP_ARTIST + BUFFER);
	// Test if the length of the artist is too long to display
	if (artist.length() > MAXDISP_ARTIST)
		cout << artist.substr(0,MAXDISP_ARTIST)+"..";
	else
		cout << artist;
	
	cout.width(GENRE_WIDTH);
	genreTranslate();
	ratingTranslate();
}

//-------------------------------------------------------------------
//	Song::empty()
//
// Preconditions	:None
//
// Postconditions	:Returns a boolean for existance of song data
//-------------------------------------------------------------------

bool Song::empty()
{
	if (title.empty())
		return true;
	else 
		return false;
}

//-------------------------------------------------------------------
//	Song::clearData()
//
// Preconditions	:None
//
// Postconditions	:Resets all data in a song to null and empty
//-------------------------------------------------------------------

void Song::clearData()
{
	title.clear();
	album.clear();
	artist.clear();
	genre = '\0';
	rating = '\0';
}	

//-------------------------------------------------------------------
//	Library::openInput()
//
// Preconditions	:Needs ifstream in Library and the global 
//			 variable LIBFILE
//
// Postconditions	:Opens the ifstream.  If unable to open the
// 			 LIBFILE for reading, returns a bool false
//-------------------------------------------------------------------

bool Library::openInput()
{
	inStream.open(LIBFILE);

	if ( inStream.fail() )
	{
		cout << endl;
		cout << "*** musicLibrary.txt COULD NOT ";
		cout <<"BE READ ***" << endl;
		cout << "Please ensure that another program ";
		cout << "is not" << endl;
		cout << "using it and that it is in the ";
		cout << "local dir." << endl;
		cout << "OPENING AN EMPTY MEDIA LIBRARY" << endl;
		return false;
	}

	else
		return true;
}

//-------------------------------------------------------------------
//	Library::openOutput()
//
// Preconditions	:Needs outStream defined in Library
//
// Postconditions	:Loads the output file into outStream.  
// 			 Returns a false boolean if the load failed.
//-------------------------------------------------------------------

bool Library::openOutput()
{
	outStream.open(LIBFILE);

	if (outStream.fail())
	{
		cout << "Failed to create musicLibrary.txt, " << endl;
		cout << "ending save function" << endl;
		return false;
	}
	return true;
}

//-------------------------------------------------------------------
//	Library::loadLibrary()
//
// Preconditions	:Needs inStream defined in Library
//
// Postconditions	:Loads the data from LIBFILE into memory
//-------------------------------------------------------------------

void Library::loadLibrary()
{
	// Initalize libSize in case LIBFILE cannot be opened
	libSize = 0;
	
	if (openInput())
	{
		inStream >> libSize;

		// Skip preceeding white spaces
		while( isspace( inStream.peek() ) )
		{
			inStream.get();
		}

		// Load # songs equal to libSize into memory
		for (int i = 0; i < libSize; i++)
		{
			libData[i].readInData(inStream);
		}
	}
}

//-------------------------------------------------------------------
//	Library::addSong()
//
// Preconditions	:None
//
// Postconditions	:Loads a new song into memory based on 
// 			 user input
//-------------------------------------------------------------------

void Library::addSong()
{
	int addLocation = 0;
	int test = 0;
	
	// Look for the first available open Song
	while (addLocation == 0 && libSize < MAXLIB_SIZE)
	{
		if (libData[test].empty() == true)
			addLocation = test;
		test++;
	}

	// If there are no openings return an error
	if (addLocation == 0)
	{
		cout << "Error: Media Library Full" << endl;
	}
	
	// If there is an opening load data into the song
	else
	{
		libData[addLocation].addTitle();
		libData[addLocation].addAlbum();
		libData[addLocation].addArtist();
		libData[addLocation].addGenre();
		libData[addLocation].addRating();
	}

	// Incriment the Library size
	libSize++;
}

//-------------------------------------------------------------------
//	Library::removeSong()
//
// Preconditions	:None
//
// Postconditions	:Removes user specified song from the library
//-------------------------------------------------------------------

void Library::removeSong()
{
	string toRemove;
	int removeLocation = -1;

	// Show the Library
	viewLib();
	
	// Ask the user for the song title
	cout << endl << "Enter the title of the song ";
	cout << "you would like to remove : ";
	getNoError(toRemove);

	// Prompt for confirmation
	cout << endl << "You entered : " << toRemove << endl;
	if ( confirm() == false)
		return removeSong();
	
	// Search for the song specified
	for ( int i = 0; i < MAXLIB_SIZE && removeLocation == -1; i++)
	{
		if (toRemove == libData[i].returnTitle() )
			removeLocation = i;
	}

	// If the song is not found return an error
	if (removeLocation == -1)
		cout << "Error: Song not found in Library" << endl;

	// If the song is found display its data
	else
	{
		// Display the song's data
		cout.setf(ios::left);
		printHeaders();
		cout.width(ID_WIDTH);
		cout << removeLocation;
		libData[removeLocation].displaySongData();
		cout << endl;
		cout.unsetf(ios::left);

		// Prompt for user confirmation to delete
		cout << endl << "YOU ARE ABOUT TO DELETE ";
		cout << "THIS SONG" << endl;
		if ( confirm() == false)
			removeLocation = -1;
	}

	// If user confirms deletion, delete the song
	if (removeLocation != -1)
	{
		libData[removeLocation].clearData();
		cout << endl << "Song Deleated" << endl;
		libSize--;
	}

	// If deletion not confirmed, confirm abortion
	else
		cout << endl << "Song Not Removed" << endl;
	
}

//-------------------------------------------------------------------
//	Library::libStats()
//
// Preconditions	:None
//
// Postconditions	:Prints out media library avalability info.
//-------------------------------------------------------------------
void Library::libStats()
{
	cout << "MEDIA LIBRARY STATS" << endl;
	cout << "Songs = " << libSize << endl;
	cout << "Capacity = " << MAXLIB_SIZE << endl;
	cout << "Available = " << MAXLIB_SIZE-libSize << endl;
	cout << "Version 1.10" << endl;
}

//-------------------------------------------------------------------
//	Library::writeLib()
//
// Preconditions	:Needs outStream defined in Library
//
// Postconditions	:Saves the current data in libData into a 
//					 formatted text file
//-------------------------------------------------------------------

void Library::writeLib()
{
	openOutput();
	
	// Write library size
	outStream << libSize << endl;

	outStream.setf(ios::left);

	// Write songs to ofstream
	for ( int i = 0; i < MAXLIB_SIZE; i++)
	{
		// If a song has data write the data
		if (libData[i].empty() == false)
			libData[i].writeSongData(outStream);
	}

	outStream.unsetf(ios::left);

	// Confirm saveing
	outStream.close();
	cout << "Library saved!" << endl;
}
	
//-------------------------------------------------------------------
//	Library::printHeaders()
// 
// Preconditions	:None
//
// Postconditions	:Prints out the formatted library headers for
// 			 song data viewing
//-------------------------------------------------------------------

void Library::printHeaders()
{
	cout.width(ID_WIDTH);
	cout << "ID";
	cout.width(MAXDISP_TITLE + BUFFER );
	cout << "Title";
	cout.width(MAXDISP_ALBUM + BUFFER );
	cout << "Album";
	cout.width(MAXDISP_ARTIST + BUFFER );
	cout << "Artist";
	cout.width(GENRE_WIDTH);
	cout << "Genre";
	cout.width(RATING_WIDTH);
	cout << "Rating" << endl;

	for (int i = 0; i < 115; i++)
	{
        	cout << "~";
	}
	cout << endl;
}

//-------------------------------------------------------------------
//	Library::viewLib()
//
// Preconditions	:None
//
// Postconditions	:Displays the library contents in a well
// 			 formatted maner
//-------------------------------------------------------------------
void Library::viewLib()
{
	cout.setf(ios::left);
	
	// Print out song data headers
	printHeaders();
	
	// Move through the libData array
	for (int i = 0; i < MAXLIB_SIZE; i++)
	{
		// If a song has data, display the data
		if ( libData[i].empty() == false )
		{
			cout.width(ID_WIDTH);
			cout << i;
			libData[i].displaySongData();
			cout << endl;
		}
	}
	cout.unsetf(ios::left);
	
}

//-------------------------------------------------------------------
//	splashScreen()
//
// Preconditions	:None
//
// Postconditions	:Prints out ASCII splash screen
//-------------------------------------------------------------------

void splashScreen()
{
        cout << "           #        #######          \n";
        cout << "           #           #             \n";
        cout << "           #           #             \n";
        cout << "# ### ###  #           #   #  #   #  \n";
        cout << "##   #   # #           #    ##   ### \n";
        cout << "#    #   # #      ##   #    ##    #  \n";
        cout << "#    #   # ###### ##   #   #  #   #  \n";
}

//-------------------------------------------------------------------
//      mainMenu(int&)
//
// Preconditions        :Takes an int arguement to be modified
//
// Postconditions       :Prints out the main menu and prompts for
//                       input. Modifies int select based on input
//-------------------------------------------------------------------

int mainMenu()
{
	char temp;
	
	// Display Menu Options
	cout << endl;
	cout << "mL.Txt Music Library Main Menu\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << endl;
	cout << "Please make your selection: \n";
	cout << endl;
	cout << "\t1 - Add a new song to the library\n";
	cout << "\t2 - Delete a song\n";
	cout << "\t3 - View library\n";
	cout << "\t4 - Library Stats\n";
	cout << "\t5 - Save\n";
	cout << "\t6 - Quit\n";
	cout << endl;
	cout << "Selection : ";

	// Get user input (read to a char)
	cin >> temp;

	// Check user entered only one thing
	while ( isalnum(cin.peek()) )
	{
		cout << "Invalid Selection";
		while (cin.get() != '\n')
		{}
		
		return mainMenu();
	}

	// Convert the ASCII value of the char to the related int
	return temp-48;
}

//-------------------------------------------------------------------
//      menuControl(Library mainLibrary)
//
// Preconditions        :None
//
// Postconditions       :Calls mainMenu(int) and redirects the
//                       user to the desired functions untill quit
//                       is selected.
//-------------------------------------------------------------------

void menuControl(Library& mainLibrary)
{
	int select = 0;
	
	while (select != 6)
	{
		// Display options and prompt for input
		switch( select =  mainMenu() )
		{
			case 1:
				mainLibrary.addSong();
				cout << endl;
				break;
			case 2:
				mainLibrary.removeSong();
				cout << endl;
				break;
			case 3:
				cout << endl;
				mainLibrary.viewLib();
				cout << endl;
				break;
			case 4:
				cout << endl << endl;
				mainLibrary.libStats();
				cout << endl;
				break;
			case 5:
				mainLibrary.writeLib();
				cout << endl;
				break;
			case 6:
				leaving(mainLibrary);
				cout << endl;
				break;
			default:
				cout << "Invalid Selection";
				cout << endl;
				break;
		}
	}
}

//-------------------------------------------------------------------
//	confirm()
// 
// Preconditions	:None
//
// Postconditions	:Returs a boolean based on user input
//-------------------------------------------------------------------

bool confirm()
{
	char check;

	// Ask for user confirmation
	cout << "Is this correct? [y/N] : ";
	getNoError(check);
	cout << endl;
	
	// Check for confirmation, default is false
	if ( check == 'Y' || check == 'y' )
		return true;
	else
		return false;
}

//-------------------------------------------------------------------
//	getNoError(char&)
// 
// Preconditions	:Must be passed a char to stor input in
//
// Postconditions	:Reads userinput into the passed char
//
// NOTE			:Used to prevent the cin << doulbe /n
// 			 infinite loop bug in cin
//-------------------------------------------------------------------

void getNoError(char& input)
{
	// Skip any endl's in the stream
	while (cin.peek() == '\n')
	{
		cin.ignore(1);
	}

	// Get user input
	cin.get(input);

	// Skip any remaining endl's in the stream
	while (cin.get() != '\n')
	{}
}

//-------------------------------------------------------------------
//	getNoError(char&)
// 
// Preconditions	:Needs a reference to a string object
//
// Postconditions	:Reads userinput into the passed string
//
// NOTE			:Used to prevent the cin << doulbe /n
//	 		 infinite loop bug in iostream
//-------------------------------------------------------------------

void getNoError(string& input)
{
	// Skip all leading endl's in the stream
	while (cin.peek() == '\n')
	{
		cin.ignore(1);
	}

	// Get user input
	getline(cin, input);
}

//-------------------------------------------------------------------
//	leaving(Library&)
//
// Preconditions	:Needs a reference to the lbrary object
//
// Postconditions	:Performs a save of the library if requested 
// 			 by the user before exiting
//-------------------------------------------------------------------

void leaving(Library& mainLibrary)
{
	char check;
	cout << endl << "Would you like to save before ";
	cout << "leaving [y/N] : ";
	getNoError(check);
	cout << endl;
	if ( check == 'Y' || check == 'y' )
		mainLibrary.writeLib();
}

