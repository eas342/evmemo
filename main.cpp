#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>
#include <math.h>
#include <time.h>

using namespace std;

//Finds the number of lines in the file filen
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

int countn(string type) {

  int ncards=0;
  bool doneall=false;
  string filecheck;
  stringstream ss;

  //  nold = numlines("flashcards/stats.stat");

  while((doneall == false))
    {
           ++ncards;
	   // look for directory (flashcards, externals, etc)
	   ss << type << "/" << ncards << ".card";
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

// Rewrites the stat and index files with one line changed
void restat(string type,int ncard,double hours, float index)
{
  int totn;
  ifstream statfile, indexfile;
  string statfilen, indexfilen;
  stringstream ss, ss2,ss3,ss4;

  //  cout << "At line 86" << endl;

  // Open the files
  ss << type << "/stats.stat";
  statfilen = ss.str();
  totn=numlines(statfilen.c_str());   //find the total number
  statfile.open(statfilen.c_str());
  ss2 << type << "/index.stat";
  indexfilen= ss2.str();
  indexfile.open(indexfilen.c_str());

  double dates[totn+1];
  float indices[totn+1];

  //  cout << "ncard= " << ncard << endl;

  //Copy the stats and indices
  for(int i=1;i<=totn;++i)
    {
      statfile >> dates[i];
      //      cout << "Read this date:  " << dates[i] << endl;
      indexfile >> indices[i];
      
    }

  //Modify the date and index as given by the arguments
  dates[ncard] = hours;
  indices[ncard] = index;

  //Close the files
  statfile.close();
  indexfile.close();

  //Rewrite the files
  ofstream wstatfile,windexfile;
  wstatfile.open(statfilen.c_str());
  windexfile.open(indexfilen.c_str());

  // Allow for rewriting dates that are new cards
  if(ncard > totn)
    totn = ncard;
    
  for(int i=1;i<=totn;++i)
    {
      wstatfile << dates[i] << endl;
      windexfile << indices[i] << endl;
    }
  
  //Close the files
  wstatfile.close();
  windexfile.close();

}

bool checkdate(string type,int ncard)
{
  // Find the date when scheduled for review
  string filen;
  double dsched;
  ifstream statfile;
  stringstream ss;
  ss << type << "/stats.stat";
  filen=ss.str();
  statfile.open(filen.c_str());
  for(int i=1;i<=ncard;++i)
    statfile >> dsched;
  statfile.close();

  //Find today's date, in hours
  time_t hours;
  hours = time (NULL)/3600;
 
//  cout << "hours= " << hours << endl;
//  cout << "dsched= " << dsched << endl;

  if(hours >= dsched)
    return false; // False means it needs review
  else
    return true; // true means you don't need to review yet

}

void show(string input)
{
  int i;
  string s2,s3;
  //  cout << input << endl << endl;
  i= input.find("cmd:");
  if(i !=string::npos)
    {
      //      cout << "I found a command at position " << i << endl;
//      cout << "Will execute this:" << endl;
      s2.assign(input,i+4,input.size());
      //      cout << s2 << endl;
      system(s2.c_str());
      system("open /Applications/Utilities/Terminal.app");
      s3.assign(input,0,i);
      cout << s3 << endl; //Show everything before "cmd:"
    }
  else
    cout << input << endl << endl; // just show it if it's not a command
  
}

bool showcard(string type, int ncard, bool &quit,float startint)
{

  ifstream cardread;
  string question,answer,reversible,  filenc, filend, junk;
  string eval;
  bool prevfile, misunderstood = true;
  float randfac; // a random factor to space out the reviews
  float index;
  double next, previous,dsched, factor; // dates for review, etc
  double difference;
  string statn,indexfn;

  stringstream ss, ss2;
  //open the flashcard
  ss <<  type << "/" << ncard << ".card";
  filenc = ss.str();
  cardread.open(filenc.c_str());

  //Check to see if there is a previous date file
  ss2 <<  type << "/" << ncard << ".date";
  filend = ss2.str();
  prevfile = exists(filend.c_str());

  getline(cardread,question);
  getline(cardread,answer);
  getline(cardread,reversible);
  cardread.close();

  // If it's a reversible card, lets make give answer, then question
  // with a probability of 1/2

  if(rand() % 2 == 1 && reversible == "r")
    {
      system("clear");   // Clear screen
      cout << "             Answer:   " << endl << endl;
      //      cout << answer << endl << endl;
      show(answer);
      cin >> junk;
      
      cout << endl << endl << endl << endl << endl;
      cout << "             Question:  " << endl << endl;
      show(question);
      cout << endl;


    }
  else
    {
      system("clear");   // Clear screen
      cout << "             Question:  " << endl;
      show(question);
      cout << endl;
      //      cout << question << endl << endl << endl; // Question
      cin >> junk; // Wait for keyboard entry
      
      cout << endl << endl << endl << endl << endl;
      cout << "             Answer:   " << endl << endl;
      
      //      cout << answer << endl << endl; // Answer
      show(answer);

    }

  

  // Now reset
  ss.seekp(0); // reset the position
  ss.str(""); // reset the string

  //Make a random factor in the range 1.0 to 1.2
  randfac = (((rand() % 20))/ 100.0) + 1.0;


   //Find the time since January 1, 1970, in hours
   time_t hours;
   hours = time (NULL)/3600;


   if(prevfile == true)
     {
       // Get previous review date & scheduled date & index
       ifstream latestd,sched,indexf;

       stringstream ss5;
       
       ss <<  type << "/stats.stat";
       statn = ss.str();
       ss5 << type << "/index.stat";
       indexfn = ss5.str();
       indexf.open(indexfn.c_str());
       latestd.open(filend.c_str());
       sched.open(statn.c_str());
       

       while(!latestd.eof())
         latestd >> previous;
       latestd.close();
       
       for(int i=1;i<=ncard;++i)
         indexf >> index;
       indexf.close();
       
       for(int i=1;i<=ncard;++i)
         sched >> dsched;
       sched.close();
     }



while(true)    //Continue asking for a response until the user gives you one
  {

    cout << "Here are the options: (r) to review this card (n) no review (q) to quit" << endl;
    cout << "(k) to kill card (remove from database) (m) move later (e) edit card" << endl;
    cout << "and (s) to skip." << endl;
    cin >> eval;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore all characters
    //    cin.ignore(150,'\n');    this was a quick fix to the 
    //cin followed by getline(cin problem)
    if((eval =="q") || (eval == "Q"))
    {
      quit = true;
      // Make the scheduled review to today if it's a new card, just in case
      if(prevfile == false)
	restat(type,ncard,hours,1.0);
      break;
    }
    if((eval == "s") || eval == "S") // skip
      {
	quit=false; // same as quit, except that you don't quit the program
	// Make the scheduled review to today if it's a new card, just in case
	if(prevfile == false)
	  restat(type,ncard,hours,1.0);
	return true; // pretend it's learned
	break;
	
      }
    else if(eval == "k" || eval == "K")
      {
	//	quit = false;
	
	string check;

	while(true)
	  {

	    cout << "Are you sure you never want to review this card again? (y/n)" << endl;
	    cin >> check;
	
	    if(check == "Yes" || check == "y" || check == "Y" || check == "yes")
	      {
		restat(type,ncard,999999,index);
		return true; // say it's learned so it doesn't review
		break;
		
	      }
	    else if(check == "No" || check == "N" || check == "n" || check == "no")
	      {
		// Here you continue on
		cout << "We'll return to the review options, then." << endl;
		misunderstood = false;
		break;
	      }
	    else
		cout << endl << "I didn't understand your response." << endl << endl;
	  }

      }
    else if(eval == "e" || eval == "E")
      {
	string nquest, newans, reversibility, check;
	bool bail=false;
	while(true)
	  {
	    cout << "Editing card ...................." << endl;
	    cout << "Old Question: " << question << endl;
	    cout << "Old Answer: " << answer << endl;
	    cout << "Old Reversibility: " << reversible << endl;
	    cout << "New Question: ";
	    getline(cin,nquest);
	    cout << "New Answer: ";
	    getline(cin,newans);
	    cout << "New Reversibility: ";
	    getline(cin, reversibility);
	    cout << endl << "Your question, answer & reversibility: " << endl;
	    cout << nquest << endl << newans << endl << reversibility << endl << endl;
	    cout << "OK?" << endl;
	    cin >> check;
	    if(check == "Yes" || check == "y" || check == "Y" || check == "yes")
	      break;
	    else if(check == "No" || check == "N" || check == "n" || check == "no")
	      {
		bail = true;
		misunderstood = false;
		break;
	      }
	    else
	      {
		cout << endl << "I didn't understand your response." << endl << endl;
	      }
	    
	  }
	if(bail == false)
	  {
	    misunderstood = false;
	    cout << "rewriting card file .............." << endl;
	    ofstream cardwrite;
	    cardwrite.open(filenc.c_str());
	    cardwrite << nquest << endl;
	    cardwrite << newans << endl;
	    cardwrite << reversibility << endl;
	    cardwrite.close();
	  }
	// No matter what, we go back to the review options to let the user decide what to do
      }
    else if(eval == "r" || eval == "R")
      {
	quit = false;
	
	// Make the scheduled review to today if it's a new card
//, just so it doesn't record nonsense
	if(prevfile == false)
	  restat(type,ncard,hours,1.0);
	else
	  {
	    //If it's a card we've already seen and we got it wrong, let's increase its index
	    // We'll also reset its next review as if it were just learned
	    next = hours+startint*randfac*randfac;
	    //startint is the initial interval, I started the program with 24 hours
	    // I think we should lower the index so it exponentially approaches 1/1.7
	    restat(type,ncard,next,(index+1));
	    
	    cout << "Since you forgot, We'll review in " << (next-hours) << " hours." << endl;
	    // record today's date, so that the program knows refers to ~24 hour interval
	    ofstream reviewd;
	    reviewd.open(filend.c_str(),ios::app);
	    
	    reviewd << hours << endl;
	    reviewd.close();
	    
	  }
	return false; // to record that it isn't learned
	break;
      }
    else if(eval == "m" || eval == "M")
      {
	float moveto;
	string check;


	cout << "So you know this one well?" << endl;


	cout << "By how much would you like to delay?" << endl;
	if(prevfile == true) // for cards already reviewed
	  {
	    // Calculate what the delay would be
	    factor = 1 + 0.7 * pow(2.718,-1.0*index/20.0);
	    difference = (dsched - previous)*factor*randfac/24.0;
	    cout << "Found a previous!" << endl;
	  }
	else
	  {
	    difference = startint*randfac*randfac/24.0;
	  }
	
	cout << "The item would be reviewed in ";
	cout << difference << " days." << endl << endl;
	cout << "How many days do you wish to review in?" << endl << endl;
	cin >> check;
	stringstream ss6(check);
	ss6 >> moveto;
	
	if(moveto >= 1.0)
	  {
	    cout << "Okie-Doke. Will review in " << check << " days" << endl;
	    restat(type,ncard,hours+(moveto*24.0),index);
	    // Save today's date
	    // record today's date
	    ofstream reviewd;
	    reviewd.open(filend.c_str(),ios::app);
	    
	    //  cout << "The time in hours is " << hours << endl << endl;
	    // save the time last reviewed
	    reviewd << hours << endl;
	    reviewd.close();

	    return true; // say it's learned so it doesn't review
	    break;
	    
	  }
	else
	  {
	    // Here you continue on
	    cout << "I didn't understand your response" << endl;
	    cout << "We'll return to the review options." << endl;

	  }




      }
    else if(eval == "n" || eval == "N")
      {
	
	
	//Now that it's learned, let's choose a date for the next review    
	//The default will be ~30 hours for newly learned
	if(prevfile == false)
	  {
	    next = hours+startint*randfac*randfac; // throw in the randonmess squared the first time
	    //startint is the initial interval, I started the program with 24 hours	    
	    //	cout << "randfac= " << randfac << endl;
	    cout << "OK. We'll review in " << (next-hours) << " hours." << endl;
	    restat(type,ncard,next,1.0);
	    
	  }
	else
	  {
	    
	    // Increase increment by a factor between 1 and 1.7
	    factor = 1 + 0.7 * pow(2.718,-1.0*index/20.0);
	    // throw in some randomness to spread out the cards
	    next = (dsched - previous)*factor*randfac + dsched;
	    cout << "This was card number " << ncard << endl;
	    cout << "dsched= " << dsched << endl;
	    cout << "previous= " << previous << endl;
	    cout << "factor= " << factor << endl;
	    cout << "hours= " << dsched << endl;
	    cout << "next= " << next << endl;
	    cout << "(dsched - previous)*factor= " << (dsched - previous)*factor << endl;
	    cout << "OK. We'll review this one in ";
	    difference = (next - hours);
	    if(difference <= 48.0)
	      {
	    cout << difference;
	    cout << " hours." << endl;
	      }
	    else
	      {
		cout << difference/24.;
		cout << " days." << endl;
	      }
	    restat(type,ncard,next,index);
	    
	    
	    
	  }
	// record today's date
	ofstream reviewd;
	reviewd.open(filend.c_str(),ios::app);
	
	//  cout << "The time in hours is " << hours << endl << endl;
	// save the time last reviewed
	reviewd << hours << endl;
	reviewd.close();
	
	return true;     // to record that the item is learned
	break;
      }
   
    cout << endl;
    if(misunderstood == true)
      cout << "I did not understand your response." << endl << endl;
    
  }
} 
 
void learn(string type, bool &quit,float startint)
{
  int newc, newe,  nflash, oldc, ncards, nlearn=0;
  quit=false;
  string junk, statfilen;
  stringstream ss7, ss8;

  // Count the number of new cards
  ss7 << type << "/stats.stat";
  statfilen = ss7.str();
  oldc = numlines(statfilen.c_str());
  ncards = countn(type);
  newc = ncards  - oldc;
  string newci;
  int newcint;
  if(newc > 0)
    {
      //      cout << "There are " << countn(type) << type << "." << endl;
      cout << "I find " << newc << " new " << type << "." << endl;

      while(true)
	{
	  cout << "How many would you like to learn?" << endl;
	  cin >> newci;
	  ss8 << newci;
	  ss8 >> newcint;;
	  cout << "You typed " << newci << endl;
	  cout << "I'm converting that to " << newcint << endl;
	  if(newcint < 0 || newcint > newc)
	    cout << "That's an invalid number." << endl;
	  else
	    {
	      newc = newcint;
	      break;
	    }
	  newci=""; newcint=0;
	  ss8.seekp(0);
	  ss8.str("");
	}
    }

  // figure out which indices to learn
  // begin at: oldc+1;
  // end at:  oldc + newc;

  // make an array to record whether they knew or not. Initialize to all being known
  bool knownc[ncards+1];
  for(int i=0; i<=ncards+1;++i)
    knownc[i]=true;

  // Learn the newest cards
  for(int i=oldc+1; i<=oldc+newc ;++i)
    {
      knownc[i] = showcard(type,i,quit,startint);

      if(quit == true)
	break;
    }

  int nsched=0;
  // Review the cards that are scheduled for review
  // Denote them as unlearned
  for(int i=1;i<=oldc;++i)
    {
      bool fakeunk; // fake unknown
      fakeunk = checkdate(type,i); //false should flag that it needs review
      if(fakeunk == false)
	{
	++nsched;
	knownc[i] = false; // pretend you don't know
	}
    }

  cout << "There are " << nsched << " previous " << type;
  cout << " cards scheduled for review today." << endl;

  // Count the number of learned cards
  for(int i=1;i<=ncards;++i)
    {
      if(knownc[i] == true)
	++nlearn;
    }

  // Review any cards that are unknown
  if(quit == false &&  nlearn != ncards)
    {
	  // Review the ones you didn't know
	  //  system("clear);
	  cout << endl << endl << "OK. Now we'll review.";
	  cout << endl << endl;
	  cin >> junk;
	  // our index
	  int j=1;
  
	  //nlearn allows us to break the loop once all cards are learned
	  nlearn = 0; //reset it to zero
	while(true)
	    {
	      if(knownc[j] == false)
		knownc[j] = showcard(type,j,quit,startint);
	      else
		++nlearn;
	      if(quit == true)
		break;
	      if(nlearn >= ncards)
		break;
	      if(j >= ncards)
		{
		j=1;
		nlearn=0;
		}
	      else
		++j;
	    }		
    }

  //  drills = showcard("flashcards",newc);
  if(quit == false)
    cout << "You're done with " << type << " for the day." << endl;

}

int main()
{
  bool quit;
  string junk;

  system("clear");
  
  cout << "*******************" << endl;
  cout << "Welcome to eV memo!" << endl;
  cout << "*******************" << endl << endl ;

  srand(time(0));
  system("open ~/Music/iTunes/\"iTunes Media\"/Music/\"Ohio Division Of Wildlife\"/\"Bird Songs - Common Birds Of Ohio\"/\"48 Hermil Thrush.mp3\"");
  system("open /Applications/Utilities/Terminal.app");
  cout << "Please prepare iTunes for Bird Songs, etc." << endl;
  cin >> junk;


  learn("bird_songs",quit,24.0); //review 24 days after first
  if(quit == false)
    learn("flashcards",quit,24.0); // begin reviewing 24 hours after learning
  if(quit == false)
    learn("quotes_n_lit",quit,24.0); //review 24 days after first
  if(quit == false)
    learn("astroph",quit,24.0); //review 24 days after first
  if(quit == false)
    learn("readings",quit,24.0); //review 24 days after first
  if(quit == false)
    learn("geography",quit,24.0); //review 24 days after first
  if(quit == false)
    learn("history",quit,24.0); //review 24 days after first
  if(quit == false)
    learn("classes10/statmech",quit,24.0*2.0); //review 2 days after first
  if(quit == false)
    learn("classes10/cosmology",quit,24.0*2.0); //review 2 days after first
  if(quit == false)
    learn("classes10/compphys",quit,24.0*2.0); //review 2 days after first
  if(quit == false)
    learn("classes10/astrophproc",quit,24.0*2.0); //review 2 days after first



  cout << "Goodbye!" << endl;

  return 0;  
}
