#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

class Helper
{
public:
	static int StringLenght( char* str )
	{
		//	RETURNS THE LENGTH OF THE WORD
		int strlen = 0;
		for (char* i = str; *i != '\0'; i++)
		{
			strlen++;
		}
		return strlen;
	}

	static char* GetStringFromBuffer(char* str)
	{	
		// RETURNS CHAR POINTER WITH NO EXTRA SPACES
		int strlen = StringLenght(str) + 1;
		char* arr = new char[strlen];

		char* set = arr;
		for (char* i = str; *i != '\0'; i++, set++)
		{
			*set = *i;
		}
		*set = '\0';

		return arr;
	}

};


class WordPuzzleCreation
{
private:
	char** inputwords; // FOR INPUTTING WORDS FROM FILE INTO 2D ARRAY
	char** Grid; // FOR CREATION OF THE WORD PUZZLE
	int rows; // TOTAL ROWS
	int columns; // TOTAL COLUMNS
	int count; // FOR COUNTING THE WORD IN PROGRESS
	int totalWords; // Total number of Words


public:

	WordPuzzleCreation()
	{
		// INITIALIZATION / DEFAULT CONSTRUCTOR
		inputwords = 0;
		Grid = 0;
		rows = 0;
		count = 0;
		totalWords = 0;
	}
	
	~WordPuzzleCreation()
	{
		// DESTRUCTOR TO AVOID ANY MEMORY LEAKAGE
		for ( int i = 0; i < totalWords; i++)
		{
			 delete[] inputwords[i];
			 inputwords[i] = 0;
		}

		delete[] inputwords;
		inputwords = 0;

		for ( int i = 0; i < rows; i++)
		{
			 delete[] Grid[i];
			 Grid[i] = 0;
		}

		delete[] Grid;
		Grid = 0;
	}

	void GetDataFromFile(char fileName[40])
	{
		// SAVES THE WORDS FROM THE FILE INTO inputwords
		ifstream input(fileName);
		

		inputwords = new char*[20];
		for(int i = 0; i < totalWords; i++) // loop for storing words from file 
		{
			char temp[25];
			input.getline(temp, 25);
			int strlen = Helper::StringLenght(temp);
			inputwords[i] = Helper::GetStringFromBuffer(temp);
		}
		inputwords[totalWords] = '\0';

	}

	void Capitalization()
	{
		// CAPITALIZES ALL THE CHARACTERS WRITTEN IN LOWERCASE IN FILE
		for(int i = 0; i < totalWords; i++)
		{
			int strlen = Helper::StringLenght(inputwords[i]);
			for(int j = 0; j < strlen; j++)
			{
				int asciinum = inputwords[i][j];
				if(asciinum >=97 && asciinum <= 122) // RANGE OF LOWERCASE LETTERS
				{
					asciinum -= 32; // COVERSION INTO UPPERCASE
					inputwords[i][j] = asciinum;
				}
			}
		}
	}

	void GridGeneration()
	{
		// FOR THE INITIALIZATION OF GRID TO 32 (SPACE)
		Grid = new char*[rows + 1];
		for(int i = 0; i < rows; i++)
		{
			Grid[i] = new char[columns + 1];
			Grid[i][columns] = '\0';
		}
		Grid[rows] = '\0';

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0 ; j < columns; j++)
			{
				Grid[i][j] = 32;
			}
		}

	}

	void Sorting()
	{
		// SORTING THE WORDS ACCORDING TO THEIR LENGTH
		char* temp;
		for (int i = 0; i < totalWords; i++) 
		{
			for (int j = 0; j < totalWords-i-1; j++)
			{
				if (Helper::StringLenght(inputwords[j]) <= Helper::StringLenght(inputwords[j + 1]))
				{
					temp = inputwords[j];
					inputwords[j] = inputwords[j+1];
					inputwords[j+1] = temp;

				}
			}
		}
	}

	bool Horizontal()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM LEFT TO RIGHT
		int strlen = Helper::StringLenght(inputwords[count]);
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				if(Grid[i][j] != 32 && Grid[i][j] != inputwords[count][0]) 
				{
					// CHECKS WHETHER LETTER MATCHES THE SPACE OR FIRST LETTER OF THE WORD IN PROGRESS 
					continue;
				}
				if ((strlen-1) + j >= columns) // CHECKING THAT WORDS STAYS WITHIN THE GRID
				{
					continue;
				}
	
				int k = 0;
				int indexcheck = 0;
				for(int l = j; l < j + strlen; l++)
				{
					if(Grid[i][l] == 32 || Grid[i][l] == inputwords[count][k])
					{
						indexcheck++;
						k++;
					}
				}
				k = 0;
				if(indexcheck == strlen)
				{
					for(int l = j; l < j + strlen; l++)
					{
						Grid[i][l] = inputwords[count][k];
						k++;
					}
					return true;
				}
			}
		}
		return false;
	}

	bool ReverseHorizontal()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM RIGHT TO LEFT
		int strlen = Helper::StringLenght(inputwords[count]);
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				if(Grid[i][j] != 32 && Grid[i][j] != inputwords[count][0])
				{
					// CHECKS WHETHER LETTER MATCHES THE SPACE OR FIRST LETTER OF THE WORD IN PROGRESS 
					continue;
				}
				if (j - (strlen-1) < 0) // CHECKING THAT WORDS STAYS WITHIN THE GRID
				{
					continue;
				}
	
				int k = 0;
				int indexcheck = 0;
				for(int l = j; l > j - strlen; l--)
				{
					if(Grid[i][l] == 32 || Grid[i][l] == inputwords[count][k])
					{
						indexcheck++;
						k++;
					}
				}
				k = 0;
				if(indexcheck == strlen)
				{
					for(int l = j; l > j - strlen; l--)
					{
						Grid[i][l] = inputwords[count][k];
						k++;
					}
					return true;
				}
			}
		}
		return false;
	}

	bool Vertical()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM TOP TO BOTTOM
		int strlen = Helper::StringLenght(inputwords[count]);
		for (int i = 0; i < columns; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				if(Grid[j][i] != 32 && Grid[j][i] != inputwords[count][0])
				{
					// CHECKS WHETHER LETTER MATCHES THE SPACE OR FIRST LETTER OF THE WORD IN PROGRESS 
					continue;
				}
				if ((strlen-1) + j >= rows) // CHECKING THAT WORDS STAYS WITHIN THE GRID
				{
					continue;
				}
	
				int k = 0;
				int indexcheck = 0;
				for(int l = j; l < j + strlen; l++)
				{
					if(Grid[l][i] == 32 || Grid[l][i] == inputwords[count][k])
					{
						indexcheck++;
						k++;
					}
				}
				k = 0;
				if(indexcheck == strlen)
				{
					for(int l = j; l < j + strlen; l++)
					{
						Grid[l][i] = inputwords[count][k];
						k++;
					}
					return true;
				}
			}
		}
		return false;
	}

	bool ReverseVertical()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM BOTTOM TO TOP
		int strlen = Helper::StringLenght(inputwords[count]);
		for (int i = 0; i < columns; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				if(Grid[j][i] != 32 && Grid[j][i] != inputwords[count][0])
				{
					// CHECKS WHETHER LETTER MATCHES THE SPACE OR FIRST LETTER OF THE WORD IN PROGRESS 
					continue;
				}
				if (j - (strlen-1) < 0) // CHECKING THAT WORDS STAYS WITHIN THE GRID
				{
					continue;
				}
	
				int k = 0;
				int indexcheck = 0;
				for(int l = j; l > j - strlen; l--)
				{
					if(Grid[l][i] == 32 || Grid[l][i] == inputwords[count][k])
					{
						indexcheck++;
						k++;
					}
				}
				k = 0;
				if(indexcheck == strlen)
				{
					for(int l = j; l > j - strlen; l--)
					{
						Grid[l][i] = inputwords[count][k];
						k++;
					}
					return true;
				}
			}
		}
		return false;

	}

	bool TowardsRightBottom()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM TOPLEFT TO RIGHTBOTTOM
		int strlen = Helper::StringLenght(inputwords[count]);
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				if(Grid[i][j] != 32 && Grid[i][j] != inputwords[count][0])
				{
					// CHECKS WHETHER LETTER MATCHES THE SPACE OR FIRST LETTER OF THE WORD IN PROGRESS 
					continue;
				}
				if ((strlen-1) + j >= rows || (strlen -1) + i >= rows) // CHECKING THAT WORDS STAYS WITHIN THE GRID
				{
					continue;
				}
	
				int k = 0;
				int indexcheck = 0;
				for(int l = j, m = i; l < j + strlen, m < i + strlen; l++, m++)
				{
					if(Grid[m][l] == 32 || Grid[m][l] == inputwords[count][k])
					{
						indexcheck++;
						k++;
					}
				}
				k = 0;
				if(indexcheck == strlen)
				{
					for(int l = j, m = i; l < j + strlen, m < i + strlen; l++, m++)
					{
						Grid[m][l] = inputwords[count][k];
						k++;
					}
					return true;
				}
			}
		}
		return false;

	}

	bool TowardsRightTop()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM BOTTOMLEFT TO RIGHTTOP
		int strlen = Helper::StringLenght(inputwords[count]);
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				if(Grid[i][j] != 32 && Grid[i][j] != inputwords[count][0])
				{
					// CHECKS WHETHER LETTER MATCHES THE SPACE OR FIRST LETTER OF THE WORD IN PROGRESS 
					continue;
				}
				if ((strlen-1) + j >= rows || i - (strlen -1) < 0) // CHECKING THAT WORDS STAYS WITHIN THE GRID
				{
					continue;
				}
	
				int k = 0;
				int indexcheck = 0;
				for(int l = j, m = i; l < j + strlen, m > i - strlen; l++, m--)
				{
					if(Grid[m][l] == 32 || Grid[m][l] == inputwords[count][k])
					{
						indexcheck++;
						k++;
					}
				}
				k = 0;
				if(indexcheck == strlen)
				{
					for(int l = j, m = i; l < j + strlen, m > i - strlen; l++, m--)
					{
						Grid[m][l] = inputwords[count][k];
						k++;
					}
					return true;
				}
			}
		}
		return false;
	}

	bool TowardsBottomLeft()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM TOPRIGHT TO LEFTBOTTOM
		int strlen = Helper::StringLenght(inputwords[count]);
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				if(Grid[i][j] != 32 && Grid[i][j] != inputwords[count][0])
				{
					// CHECKS WHETHER LETTER MATCHES THE SPACE OR FIRST LETTER OF THE WORD IN PROGRESS 
					continue;
				}
				if ( j - (strlen-1) < 0 || i + (strlen -1) >= rows) // CHECKING THAT WORDS STAYS WITHIN THE GRID
				{
					continue;
				}
	
				int k = 0;
				int indexcheck = 0;
				for(int l = j, m = i; l > j - (strlen-1), m < i + strlen; l--, m++)
				{
					if(Grid[m][l] == 32 || Grid[m][l] == inputwords[count][k])
					{
						indexcheck++;
						k++;
					}
				}
				k = 0;
				if(indexcheck == strlen)
				{
					for(int l = j, m = i; l > j - (strlen-1), m < i + strlen; l--, m++)
					{
						Grid[m][l] = inputwords[count][k];
						k++;
					}
					return true;
				}
			}
		}
		return false;

	}

	bool TowardsLeftTop()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM RIGHTBOTTOM TO LEFT TOP
		int strlen = Helper::StringLenght(inputwords[count]);
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				if(Grid[i][j] != 32 && Grid[i][j] != inputwords[count][0])
				{
					// CHECKS WHETHER LETTER MATCHES THE SPACE OR FIRST LETTER OF THE WORD IN PROGRESS 
					continue;
				}
				if (j - (strlen-1) < 0 || i - (strlen -1) < 0) // CHECKING THAT WORDS STAYS WITHIN THE GRID
				{
					continue;
				}
	
				int k = 0;
				int indexcheck = 0;
				for(int l = j, m = i; l > j - strlen, m > i - strlen; l--, m--)
				{
					if(Grid[m][l] == 32 || Grid[m][l] == inputwords[count][k])
					{
						indexcheck++;
						k++;
					}
				}
				k = 0;
				if(indexcheck == strlen)
				{
					for(int l = j, m = i; l > j - strlen, m > i - strlen; l--, m--)
					{
						Grid[m][l] = inputwords[count][k];
						k++;
					}
					return true;
				}
			}
		}
		return false;

	}

	void OrganisationOfWords()
	{
		// PLACING WORDS INTO THE GRID IN RANDOM ORDER
		srand(time(0)); // FOR RANDOM ORDER
		while(count != totalWords)
		{
			int temp = (rand() % 8) + 1; // FOR RANDOM ORDER

			if(temp == 1)
			{
				if(Horizontal())
				{
					count++;
					if(count == totalWords)
					{
						break;
					}
				}
			}

			if(temp == 2)
			{
				if(Vertical())
				{
					count++;
					if(count == totalWords)
					{
						break;
					}
				}
			}

			if(temp == 3)
			{
				if(ReverseVertical())
				{
					count++;
					if(count == totalWords)
					{
						break;
					}
				}
			}

			if(temp == 4)
			{
				if(ReverseHorizontal())
				{
					count++;
					if(count == totalWords)
					{
						break;
					}
				}
			}

			if(temp == 5)
			{
				if(TowardsLeftTop())
				{
					count++;
					if(count == totalWords)
					{
						break;
					}

				}
			}

			if(temp == 6)
			{
				if(TowardsBottomLeft())
				{
					count++;
					if(count == totalWords)
					{
						break;
					}
				}
			}

			if(temp == 7)
			{
				if(TowardsRightTop())
				{
					count++;
					if(count == totalWords)
					{
						break;
					}
				}
			}
		
			if(temp == 8)
			{
				if(TowardsRightBottom())
				{
					count++;
					if(count == totalWords)
					{
						break;
					}
				}
			}
		}
	}

	void FillingRandomCharacters()
	{
		 // PLACING RANDOM WORDS IN THE EMPTY SPACES
		srand(time(0));
		for (int i = 0; i < rows; i++)
		{
			for(int j = 0; j < columns; j++)
			{
				if(Grid[i][j] == 32)
				{
					int temp = 64 + (rand() % (65 - 91) + 1 );
					Grid[i][j] = temp;
				}
			}
		}
	}

	void OutputFile(char outputfileName[40])
	{
		// SAVING DATA ON THE OUTPUT FILE "Grid.txt"
		ofstream output(outputfileName);
		
		for(int i = 0; i < rows; i++)
		{
			for(int j = 0; j < columns; j++)
			{
				output << Grid[i][j] << " ";
			}
			output << endl;
		}
	}

	void CreatingWordSearchPuzzle(char inputfileName[40], char outputfileName[40], int _rows, int _cols, int _totalWords)
	{
		rows = _rows; // Saving rows
		columns = _cols; // Saving columns
		totalWords = _totalWords; // Saving total number of words
		GetDataFromFile(inputfileName); // GET'S THE WORDS FROM THE FILE AND STORE INTO inputwords(2D Array)
		Capitalization();  // CAPITALIZES ALL THE CHARACTERS WRITTEN IN LOWERCASE IN FILE
		GridGeneration();  // FOR THE INITIALIZATION OF GRID
		Sorting();         // SORTING THE WORDS ACCORDING TO THEIR LENGTH

		int longestWordLength = Helper::StringLenght(inputwords[0]);
		if(longestWordLength > rows &&  longestWordLength > columns)
		{
			cout << "Grid cannot be formed, press any key to return to the home screen \n";
		}

		else
		{
			OrganisationOfWords(); // PLACING WORDS INTO THE GRID IN RANDOM ORDER
			cout << endl << "GRID GENERATED!!!\nPLEASE CHECK THE FILE 'Grid.txt' FOR THE RESULT\nOR REFER THE GRID BELOW :)";
			FillingRandomCharacters(); // PLACING RANDOM WORDS IN THE EMPTY SPACES
			OutputFile(outputfileName); // SAVING DATA ON THE OUTPUT FILE "Grid.txt"
		}
	}
};


class WordPuzzuleSearch
{
private:
	char** Grid; // GRID ON WHICH THE PUZZLE IS PRESENT
	int rows; // ROWS ENTERED BY THE USER
	int columns; // COLUMNS ENTERED BY THE USER
	char** userEnteredWords; //  USED FOR SAVING THE WORDS USER HAS ENTERED
	int noOfWords; // TOTAL WORDS USER HAS ENTERED
	int startOfWordR; // START ROW INDEX FOR CHECKING THE PRESENCE OF THE WORD
	int startOfWordC; // START COLUMN INDEX FOR CHECKING THE PRESENCE OF THE WORD
	int endOfWordR; // END ROW INDEX FOR CHECKING THE PRESENCE OF THE WORD
	int endOfWordC; // END ROW INDEX FOR CHECKING THE PRESENCE OF THE WORD
	int count; // FOR CHECKING THE WORD IN PROGRESS

public:

	WordPuzzuleSearch()
	{
		// DEFAULT CONSTRUCTOR / INITIALIZATION
		Grid = 0;
		rows = 0;
		columns =  0;
		userEnteredWords =  0;
		noOfWords = 0;
		startOfWordR = 0;
		startOfWordC = 0;
		endOfWordR = 0;
		endOfWordC = 0;
		count = 0;
	}

	~WordPuzzuleSearch()
	{
		// DESTRUCTOR
		for ( int i = 0; i < rows; i++)
		{
			 delete[] Grid[i];
			 Grid[i] = 0;
		}

		delete[] Grid;
		Grid = 0;

		for ( int i = 0; i < noOfWords; i++)
		{
			delete[] userEnteredWords[i];
			userEnteredWords[i] = 0;
		}

		delete[] userEnteredWords;
		userEnteredWords = 0;
	}

	void copyDataFromFile(char inputfile[40])
	{
		// COPYING THE GRID FROM THE FILE ONTO THE 2D ARRAY GRID
		ifstream input(inputfile);

		Grid = new char* [rows + 1];
		for (int i = 0; i < rows; i++)
		{
			Grid[i] = new char [columns + 1];
		}
		Grid[rows] = '\0';

		for ( int i = 0; i < rows; i++)
		{
			for( int j = 0; j < columns; j++)
			{
				input >> Grid[i][j];
	
			}
			Grid[i][columns] = '\0';
		}
	}

	void Input(char wordsfileName[40])
	{
		// ALL INPUTS OF WORDS ENTERED BY USER SAVED IN userenteredwords 2D array 
		ifstream inputfile(wordsfileName);

		inputfile >> rows >> columns >> noOfWords;
		userEnteredWords = new char*[noOfWords];
		inputfile.ignore();
		for(int i  = 0; i < noOfWords; i++)
		{
			char temp[25];
			inputfile.getline(temp, 25);
			int strlen = Helper::StringLenght(temp);
			userEnteredWords[i] = Helper::GetStringFromBuffer(temp);

		}

	}

	bool Horizontal()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM LEFT TO RIGHT
		int k = 0;
		int strlength = Helper::StringLenght(userEnteredWords[count]);
		if((strlength -1) + startOfWordC >= columns)
		{
			return false;
		}
		for(int i = startOfWordC; i < startOfWordC+strlength; i++)
			{
				if(i < columns  && i >= 0)
				{
					if(Grid[startOfWordR][i] != userEnteredWords[count][k])
					{
						return false;
					}
					k++;
				}
			}
		
		return true;
	}

	bool ReverseHorizontal()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM RIGHT TO LEFT
		int k = 0;
		int strlength = Helper::StringLenght(userEnteredWords[count]);
		if(startOfWordC - (strlength - 1) < 0)
		{
			return false;
		}
		for(int i = startOfWordC; i > startOfWordC - strlength; i--)
			{
				if(i < columns  && i >= 0)
				{
					if(Grid[startOfWordR][i] != userEnteredWords[count][k])
					{
						return false;
					}
					k++;
				}
			}
		
		return true;
	}

	bool TowardsRightBottom()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM TOPLEFT TO RIGHTBOTTOM
		int strlength = Helper::StringLenght(userEnteredWords[count]);
		int k = 0;
		if((strlength-1) + startOfWordR >= rows || (strlength-1) + startOfWordC >= columns)
		{
			return false;
		}
		
		for(int i = startOfWordR, j = startOfWordC; i < startOfWordR+strlength, j < startOfWordC+strlength; i++, j++)
		{
			if(i < rows && i >= 0 && j < columns && j >= 0)
			{
				if(Grid[i][j] != userEnteredWords[count][k])
				{
					return false;
				}
				k++;
			}
		}
		
		return true;
	}

	bool TowardsLeftBottom()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM TOPRIGHT TO LEFTBOTTOM
		int strlength = Helper::StringLenght(userEnteredWords[count]);
		int k = 0;
		if((strlength-1) + startOfWordR >= rows || startOfWordC - (strlength-1) < 0)
		{
			return false;
		}
		
		for(int i = startOfWordR, j = startOfWordC; i < startOfWordR+strlength, j > startOfWordC - strlength; i++, j--)
		{
			if(i < rows && i >= 0 && j < columns && j >= 0)
			{
				if(Grid[i][j] != userEnteredWords[count][k])
				{
					return false;
				}
				k++;
			}
		}
		
		return true;
	}

	bool TowardsLeftTop()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM RIGHTBOTTOM TO LEFTTOP
		int strlength = Helper::StringLenght(userEnteredWords[count]);
		int k = 0;
		if(startOfWordR - (strlength-1) < 0 || startOfWordC - (strlength-1) < 0)
		{
			return false;
		}
		
		
		for(int i = startOfWordR, j = startOfWordC; i > (startOfWordR - strlength), j > (startOfWordC - strlength); i--, j--)
		{
			if(i < rows && i >= 0 && j < columns && j >= 0)
			{
				if(Grid[i][j] != userEnteredWords[count][k])
				{
					return false;
				}
				k++;
			}
		}
		
		return true;
	}

	bool TowardsRightTop()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM LEFTBOTTOM TO RIGHTTOP
		int strlength = Helper::StringLenght(userEnteredWords[count]);
		int k = 0;
		if(startOfWordR - (strlength-1) < 0 || (strlength-1) + startOfWordC >= columns)
		{
			return false;
		}
		
		
		for(int i = startOfWordR, j = startOfWordC; i > (startOfWordR - strlength), j < (strlength-1) + startOfWordC; i--, j++)
		{
			if(i < rows && i >= 0 && j < columns && j >= 0)
			{
				if(Grid[i][j] != userEnteredWords[count][k])
				{
					return false;
				}
				k++;
			}
		}
		
		return true;
	}

	bool Vertical()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM TOP TO BOTTOM
		int k = 0;
		int strlength = Helper::StringLenght(userEnteredWords[count]);
		if((strlength -1) + startOfWordR >= rows)
		{
			return false;
		}
		for(int i = startOfWordR; i < startOfWordR+strlength; i++)
			{
				if(i < rows  && i >= 0)
				{
					if(Grid[i][startOfWordC] != userEnteredWords[count][k])
					{
						return false;
					}
					k++;
				}
			}
		
		return true;
	}

	bool ReverseVertical()
	{
		// CHECKING THE FEASIBILITY OF WORD FORMATION FROM BOTTOM TO TOP
		int k = 0;
		int strlength = Helper::StringLenght(userEnteredWords[count]);
		if(startOfWordR - (strlength - 1) < 0)
		{
			return false;
		}
		for(int i = startOfWordR; i > startOfWordR - strlength; i--)
			{
				if(i < rows  && i >= 0)
				{
					if(Grid[i][startOfWordC] != userEnteredWords[count][k])
					{
						return false;
					}
					k++;
				}
			}
		
		return true;
	}

	void Searching(char outputfile[40])
	{
		// SEARCHING THE WORDS USER ENTERED ON THE WORD PUZZLE
		ofstream OUT(outputfile);
		cout << endl << endl;
		while(count != noOfWords)
		{
			bool horizontal = false;
			bool reversehorizontal = false;
			bool towarsdsrightbottom = false;
			bool towardslefttop = false;
			bool towarsdsleftbottom =  false;
			bool towardsrighttop = false;
			bool vertical = false;
			bool reversevertical =  false;
			cout << userEnteredWords[count] << " : ";
			for(int i = 0; i < rows; i++)
			{
				for(int j = 0; j < columns; j++)
				{
					startOfWordR = i;
					startOfWordC = j;

					if(userEnteredWords[count][0] == Grid[i][j])
					{

						if(TowardsRightBottom())
						{
							endOfWordR = startOfWordR + Helper::StringLenght(userEnteredWords[count]);
							endOfWordC = startOfWordC + Helper::StringLenght(userEnteredWords[count]);
							cout << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR - 1 << "," << endOfWordC -1 << "}" << endl;
							OUT << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR - 1 << "," << endOfWordC -1 << "}" << endl;
							towarsdsrightbottom = true;
							break;
						}

						if(TowardsLeftBottom())
						{
							endOfWordR = startOfWordR + Helper::StringLenght(userEnteredWords[count]);
							endOfWordC = startOfWordC - Helper::StringLenght(userEnteredWords[count]);
							cout << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR - 1 << "," << endOfWordC + 1 << "}" << endl;
							OUT << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR - 1 << "," << endOfWordC + 1 << "}" << endl;
							towarsdsleftbottom = true;
							break;
						}

						if(TowardsLeftTop())
						{
							endOfWordR = startOfWordR - Helper::StringLenght(userEnteredWords[count]);
							endOfWordC = startOfWordC - Helper::StringLenght(userEnteredWords[count]);
							cout << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR + 1 << "," << endOfWordC + 1 << "}" << endl;
							OUT << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR + 1 << "," << endOfWordC + 1 << "}" << endl;
							towardslefttop = true;
							break;
						}

						if(TowardsRightTop())
						{
							endOfWordR = startOfWordR - Helper::StringLenght(userEnteredWords[count]);
							endOfWordC = startOfWordC + Helper::StringLenght(userEnteredWords[count]);
							cout << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR + 1 << "," << endOfWordC - 1 << "}" << endl;
							OUT << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR + 1 << "," << endOfWordC - 1 << "}" << endl;
							towardsrighttop = true;
							break;
						}



						if(Horizontal())
						{
							endOfWordR = startOfWordR;
							endOfWordC = startOfWordC + Helper::StringLenght(userEnteredWords[count]);
							cout << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR << "," << endOfWordC - 1 << "}" << endl;
							OUT << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR << "," << endOfWordC - 1 << "}" << endl;
							horizontal = true;
							break;
						}

						if(ReverseHorizontal())
						{
							endOfWordR = startOfWordR;
							endOfWordC = startOfWordC - Helper::StringLenght(userEnteredWords[count]);
							cout << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR << "," << endOfWordC + 1 << "}" << endl;
							OUT << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR << "," << endOfWordC + 1 << "}" << endl;
							reversehorizontal = true;
							break;
						}

						if(Vertical())
						{
							endOfWordR = startOfWordR + Helper::StringLenght(userEnteredWords[count]);
							endOfWordC = startOfWordC;
							cout << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR - 1 << "," << endOfWordC << "}" << endl;
							OUT << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR - 1 << "," << endOfWordC << "}" << endl;
							vertical = true;
							break;
						}

						if(ReverseVertical())
						{
							endOfWordR = startOfWordR - Helper::StringLenght(userEnteredWords[count]);
							endOfWordC = startOfWordC;
							cout << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR + 1 << "," << endOfWordC << "}" << endl;
							OUT << "{" << startOfWordR << "," <<startOfWordC << "} {" << endOfWordR + 1 << "," << endOfWordC << "}" << endl;
							reversevertical = true;
							break;
						}


					}
				}
			}
			if(horizontal == false && reversehorizontal == false && 
			    towarsdsrightbottom == false && towardslefttop == false &&
				towarsdsleftbottom == false && towardsrighttop == false &&
				vertical == false && reversevertical == false)
			{
				cout << "Not Found" << endl;
			}
			count++;
		}
	}

	void SearchingWordsFromGrid(char gridfileName[40], char wordsfileName[40], char outputfileName[40])
	{
		Input(wordsfileName);
		copyDataFromFile(gridfileName);
		Searching(outputfileName);
	}


};


void main()
{
	while(1)
	{
		system("CLS");
		char option;
		cout << "------------------------------------------------- HOMESCREEN -------------------------------------------------\n\n";
		cout << "-------------------------------------------------- OPTIONS -------------------------------------------------\n\n";
		cout << "Press C to Create a Word Puzzle Grid\n";
		cout << "Press S to Search words in the Word Puzzle Grid\n";
		cout << "Press Q to Quit\n\n";
		cout << "-------------------------------------------------- OPTIONS -------------------------------------------------\n\n";
		cout << "Please Select the Option: ";
		cin >> option;
		if(option != 'C' && option != 'S' && option != 'Q') // Any other key would return the user to to the home screan
		{
			continue;
		}
		cout << "\n\n";
		cout << "------------------------------------------------- HOMESCREEN -------------------------------------------------\n\n";

		if (option == 'C') // If user wants to create a grid
		{
				system("CLS");
				char inputfileName[40]; // for words input from file
				char outputfileName[40]; // For grid generation
				int totalWords = 0, rows = 0, cols = 0;

				cout << "--------------------------------------- CREATION OF WORD SEARCH PUZZLE ---------------------------------------\n\n";
				WordPuzzleCreation GridMaking; // object created for Puzzle creation

				cout << "Please Enter the Input File where the Words are stored: ";
				cin.ignore();
				cin.getline(inputfileName, 40); // inputting the the whole file name into inputfileName

				cout << "Please Enter the Number of Words in the file: ";
				cin >> totalWords;  // Entering total no:of words in file

				cout << "Please Enter the Number of Rows You Want in the Grid: ";
				cin >> rows; //Entering total rows user wants for grid

				cout << "Please Enter the Number of Columns You Want in the Grid: ";
				cin >> cols; //Entering total columns user wants for grid

				cout << "Please Enter the Output File where the Words are stored: ";
				cin.ignore();
				cin.getline(outputfileName, 40); // inputting the the whole file name into outputfileName

				cout << endl;
				GridMaking.CreatingWordSearchPuzzle(inputfileName, outputfileName, rows, cols, totalWords); // Running program for creation of Puzzle

				cout << endl;

				system("pause");
		}

		if(option == 'S') // If user wants to search words in the grid
		{
				system("CLS");
				char gridfileName[40]; // for inputting gridfileName
				char wordsfileName[40]; // for inputting words file Name
				char outputfileName[40]; // for outputting the coordinates of words

				cout << "------------------------------------ SEARCHING WORDS IN WORD SEARCH PUZZLE ------------------------------------\n\n";
				
				WordPuzzuleSearch WordIdentifier;

				cout << "Please Enter the Input File where the Grid are stored: ";
				cin.ignore();
				cin.getline(gridfileName, 40); // To store the name of the grid file 

				cout << "Please Enter the Input File where the Words are stored: ";
				//cin.ignore();
				cin.getline(wordsfileName, 40); //To store  the name of  the words file

				cout << "Please Enter the Output File where the Words are stored: ";
				//cin.ignore();
				cin.getline(outputfileName, 40); // To store the name of the output file


				WordIdentifier.SearchingWordsFromGrid(gridfileName, wordsfileName, outputfileName); // Running program for searchingg words in Puzzle

				cout << endl;

				system("pause");
		}

		if(option == 'Q')
		{
			break; // Exit from the console window
		}


	}

}









