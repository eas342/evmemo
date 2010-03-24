#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;

//Test if a file exists
bool exists(string filename) {
  ifstream fin;
  // We must use the string class's c_str() function to convert
  // The C++ string into an old C string (character array).
  // This is because the open() function expects a character array
  fin.open (filename.c_str());
  if (fin.fail()) return false;
  fin.close();
  return true;
}

// Count the number of cards
int countn() {

  int ncards=0;
  bool doneall=false;
  string filecheck;
  stringstream ss;

  //  nold = numlines("flashcards/stats.stat");

  while((doneall == false))
    {
           ++ncards;
	   // look for directory (flashcards, externals, etc)
	   ss << ncards << ".card";
	   filecheck = ss.str();
	   //	   cout << "ss is " << ss.str() ;
      if(exists(filecheck))
	{
	  //	  cout << "I found " << filecheck << endl;
	  // Now reset
	  ss.seekp(0); // reset the position
	  ss.str(""); // reset the string
	}
      else
	break;

    }

  return ncards - 1;

}

int numlines(string filen)
{
  int n=0;
  ifstream numfile;
  string junk;

  numfile.open(filen.c_str() );
  //  cout << "End of file = " << numfile.eof() << endl;
  while(!numfile.eof())
    {
      getline(numfile,junk);

	++n;
    }
  --n;
  return n;

  numfile.close();

}

void createc(string filen, int start)
{
  string data1, data2, data3;
  ifstream sourcef;
  ofstream cardf;
  stringstream ss;

  sourcef.open(filen.c_str() );
  //  cardf.open(
  while(!sourcef.eof())
    {
      getline(sourcef,data1);
      getline(sourcef,data2);
      getline(sourcef,data3);
      ss << start << ".card";

      // make a card
      string newcard = ss.str();
      cardf.open(newcard.c_str() );
      cardf << data1 << endl;
      cardf << data2 << endl;
      cardf << data3 << endl;

      cardf.close();
      
      // Reset the stringstream
      ss.seekp(0); // reset the position
      ss.str(""); // reset the string
      
      ++start;
    }
}


int main()
{
  int ncards, nnew;
  string sourcef, junk;

  ncards = countn()+1;

  cout << "********************************************************" << endl;
  cout << "Make sure you've backed up your cards before proceeding!" << endl;
  cin >> junk;
  cout << "Make sure the last line of your file is the last item of a card" << endl;
  cin >> junk;
  cout << "Please enter the name of your sourcefile" << endl;
  cin >> sourcef;
  nnew = (numlines(sourcef.c_str())+1) / 3;

  cout << "OK. Beginning with card " << ncards << endl;
  cout << "I will add " << nnew << " new cards." << endl;
  
  createc(sourcef.c_str(), ncards);
  
  return 0;
}
