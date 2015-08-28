///Version 2
///Created by David Lubling-Kolbow
///For CS5403 FALL 2014 at NYU Polytechnic University
///Taught by Professor Joseph Vaisman
///
///Final Project:
///Problem Statement:
///Implement a solution to the eight queens problem. It should work for 4,5, and 8 queens
///Documentation:
///I wanted to solve this problem without referencing the literature. So, starting with the representation used in class, I decided to implement a brute-force algorithm that checked every possible ordering of n elements.
///To check if a given combo was correct or not, I just checked if every two digits that were one apart in spacing were not one apart in number, if every two digits that were two apart in spacing were not two apart in number, and so on.
///However, I quickly noticed that brute force algorithm was not very efficient, since it checks all combos starting with 12, for instance, even though a working combo cannot possibly start with two queens diagonal from each other.
///So, I briefly looked at the literature and implemented a backtracking method. Determining each possible loop case was the difficult part.
///
///The biggest issue I ran into, during implentation was the trimming of excess solutions. Since my representation of solutions was done using int arrays instead of vectors, it was not easy to check if two solutions were 90 degree rotations of each other. If this implementation was done using vectors, it would have been much easier to apply a matrix transform operation on the coordinates of each solution, since reflection and rotation have easy matrix representations.
///Originally I did not want to actually generate all the 7 possible transforms of a solution. Instead I tried in place element comparison checking, but it was only easy to check reflections this way. So, for this final version, I generated each transformation and checked it.


///Implementation(assuming input form is multiple integers seperated by spaces):
///
#include <iostream>   //needed for console output
#include <iomanip>    //needed for chesboard printing using setw
#include <cstdlib>    //needed for atoi
#include <algorithm>  //needed for sort, next_permutation
#include <cassert>    //needed for assertions
#include <queue>      //data structure to store multiple results

using std::cout;
using std::endl;
using std::next_permutation;
using std::setw;
using std::queue;

typedef unsigned int uint;

//class definition
class QueenResults{
  public:
  
  const static uint MAX_SIZE = 12;
  
  //constructor and destructor
  QueenResults() = default;
  QueenResults(uint i_size);
  ~QueenResults();
  
  //printing function
  void printResult();
  
  //solves by generating all (size)! permutations and checking if is result
  void BruteForce();
  //solves by using standard backtracking technique
  void BackTrack();
  
  //trims solutions
  void TrimSols();
  
  protected:
  //check if up to specificied row, and if is result overall
  bool isResult(uint* testArr, uint stopCol) const;
  bool isResult(uint* testArr) const{return isResult(testArr, size);}
  
  //checks if two input solutions are unique
  bool isUnique(uint* first, uint* last) const;
  
  //Rotates a solution
  void rotateClockwise(uint* colCoords) const;
  
  //reflects a solution across vertical
  void reflectVertical(uint* colCoords) const;
  
  //sorts a set of coordinates by rowCoords
  void bubbleSortAsc(uint* rowCoords, uint* colCoords) const;
  
  //standard swap function for uints
  void swap(uint &A, uint &B) const;
  
  //queue containing results
  queue<uint*> results;
  
  //size of chessboard
  uint size=0;
  
  //number of nonunique results found
  uint numResults=0;
  
  //trimming applied
  bool trimmed = false;
};

int main(int argc, char* argv[])
{
  //initialize array and its size variables.
  uint size;
  int* intArr;
  int result;
  bool trim;
  if(argc == 2)
  {
    //convert to integer
    size = atoi(argv[1]);
    trim = true;
  }
  else if(argc == 3)
  {
    //convert to integer
    size = atoi(argv[1]);
    trim = atoi(argv[2]);
  }
  //if not valid input format
  else
  {
    //print usages and examples
    cout << endl << "Invalid Usage:" << endl;
    cout << "Usage: " << argv[0] << " " <<"<size>" << "[trim]" << endl;
    cout << "Example: " << argv[0] << " " <<"4" << endl;
    return -1;
  }
  
  //initialize array
  QueenResults queens {size};
  
  //print initial array
  cout << endl << "Determining possible solutions to a " << size << "x" << size << " chessboard:" << endl;
  
  //get solutions, trim, and print
  queens.BackTrack();
  if(trim)
    queens.TrimSols();
  queens.printResult();
  return 0;
  
}


QueenResults::QueenResults(uint i_size)
{
  assert(i_size <= MAX_SIZE);
  size = i_size;
}

QueenResults::~QueenResults()
{
  uint* temp;
  
  //empties queue and deletes each int array.
  while(!results.empty())
  {
    temp = results.front();
    results.pop();
    delete [] temp;
  } 
}
  
void QueenResults::printResult()
{
  if(results.size() == 0)
  {
    cout << endl << "No solutions were found..." << endl;
    return;
  }
  
  uint* result;
  uint counter = 1; 
  cout << endl << numResults << " solution(s) were found to this problem." << endl;
  if(trimmed)
    cout << results.size() <<" solution(s) remain after trimming." << endl << endl;
  cout << endl;
  
  for(counter=1;counter<=results.size();counter++){
    result = results.front();
    cout << "Chessboard " << counter << ":" << endl;
    for(uint i=0;i<size;i++)
      cout << result[i]+1 << " ";
    cout << endl;
    
    for(uint j=0;j<2*size+1;j++)
      cout << "-";
    cout <<endl;
    for(uint i=0;i<size;i++)
    {
      for(uint j=0;j<size;j++)
      {
        if(result[i]==j)
          cout << "|Q";
        else
          cout << "| ";
      }
      cout << "|" << endl;
        
      for(uint j=0;j<2*size+1;j++)
        cout << "-";
      cout << endl;
    }
    cout << endl;
    results.pop();
    results.push(result);
  }
  return;
}

bool QueenResults::isResult(uint* testArr, uint stopCol) const
{

  //for each possible row difference, check if column difference is same or if pieces are same
  for(uint offset = 1;offset<stopCol;offset++)
  {
    for(uint i = 0; i<stopCol-offset; i++)
      if(abs(testArr[i]-testArr[i+offset])==offset or abs(testArr[i]-testArr[i+offset])==0)
        return false;
  }
  return true;
}
  
void QueenResults::BruteForce()
{
  uint* test = new uint[size];
  uint* input;
  uint counter;
  
  for(uint i=0;i<size;i++)
    test[i]=i;
  
  do {
    if(isResult(test))
    {
      input = new uint[size];
      for(uint i=0;i<size;i++)
        input[i] = test[i];
      results.push(input);
    }
  counter++;
  } while(next_permutation(test,test+size) );
  
  delete [] test; test = nullptr; input = nullptr;
  
  numResults = results.size();
  return;
}

void QueenResults::BackTrack()
{

  char dummy;

  int layer = 0;
  uint* test = new uint[size];
  uint* input = nullptr;
  bool endReached;
  for(uint i=0;i<size;i++)
  {
    test[i]=0;
  } 
  
  while(true){
    endReached = false;
    //if solution, record it and backtrack
    if(isResult(test,layer+1) and layer == size-1)
    {
      input = new uint[size];
      for(uint i=0;i<size;i++)
        input[i] = test[i];
      results.push(input);
      numResults++;
      endReached = true;
    }
    
    //if what we have is a solution so far, step further in and reset counter
    else if(isResult(test,layer+1) and layer < size-1)
    {
      layer++;
      test[layer]=0;
    }
    
    //if what we have is not a solution and this digit and not at end yet, increment it
    else if(!isResult(test,layer+1) and test[layer] < size-1)
    {
      test[layer]++;
    }
    //else backtrack
    else
    {
      endReached = true;
    }
    
    //for backtracking, step back a layer and continue for all layers that are at the last digit, and then increment the digit.
    //break the loop if we have already reached the last combination
    if(endReached)
    {
      do{
        layer--;
      }
      while(layer >= 0 and test[layer]==size-1);
      
      if(layer==-1)
        break;
      
      test[layer]++;
    }
  }

  delete [] test; test = nullptr; input = nullptr;
  return;
}

void QueenResults::TrimSols()
{
  if(trimmed)
    return;
  uint loopCount = 0;
  uint *checker, *checkee;
  
  //loop through list until we have used everything as checkers
  while(loopCount<results.size())
  {
    results.push(nullptr);
    //choose checker item to compare other guys to.
    for(uint i=0;i<loopCount+1;i++)
    {
      checker = results.front();
      results.pop();
      results.push(checker);
    }
    //compare checker with everything else(stop condition is placeholder reached) and remove if not unique
    checkee = results.front();
    while(checkee)
    {
      results.pop();
      if(!isUnique(checker,checkee))
        delete [] checkee;
      else
        results.push(checkee);
      checkee = results.front();
    }
    //remove placeholder
    results.pop();
    loopCount++;
  }
  trimmed = true;
}

bool QueenResults::isUnique(uint* first, uint* last) const
{
  uint* temp = new uint[size];
  bool isSame;
  
  for(uint i = 0;i<size;i++)
    temp[i] = first[i];
  
  //check all 3 rotations
  for(uint count=0;count<3;count++)
  {
    rotateClockwise(temp); isSame=true;
    for(uint i = 0;i<size;i++)
    {
      isSame = isSame and (temp[i]==last[i]);
      if(!isSame)
        break;
    }
    if(isSame)
    {
      delete [] temp; return false;
    }
  }
  
  //check reflection
  reflectVertical(temp); isSame = true;
  for(uint i = 0;i<size;i++)
  {
    isSame = isSame and (temp[i]==last[i]);
    if(!isSame)
      break;
  }
  if(isSame)
  {
    delete [] temp; return false;
  }
  
  //check all 3 rotations
  for(uint count=0;count<3;count++)
  {
    rotateClockwise(temp); isSame=true;
    for(uint i = 0;i<size;i++)
    {
      isSame = isSame and (temp[i]==last[i]);
      if(!isSame)
        break;
    }
    if(isSame)
    {
      delete [] temp; return false;
    }
  }
  
  delete [] temp; return true;
}

void QueenResults::rotateClockwise(uint* colCoords) const
{
  uint* rowCoords = new uint[size];
  for(uint i = 0;i<size;i++)
  {
    rowCoords[i] = size-1-colCoords[i];
    colCoords[i] = i;
  }
  bubbleSortAsc(rowCoords, colCoords);
  delete [] rowCoords;
}

void QueenResults::reflectVertical(uint* colCoords) const
{
  for(uint i = 0;i<size;i++)
  {
    colCoords[i]=size-1-colCoords[i];
  }
}

void QueenResults::bubbleSortAsc(uint* rowCoords, uint* colCoords) const
{
  //declare sort checking variable
  bool isSorted;
  
  //initialize number of runs counter to save time/computation power to check if already sorted items are in order.
  uint numRuns = 0;
  
  //do the following at least once, until the array is sorted:
  do
  {
    //reinitialize sort checker variable
    isSorted = true;
    
    //loop through the array, skipping the last element and all sorted elements
    for(uint i = 0; i<size-numRuns-1; i++)
    {
      //if things are not in order,
      if(rowCoords[i]>rowCoords[i+1])
      {
        //flag the array as not sorted
        isSorted = false;
        
        //fix the mis-sort by swapping
        swap(rowCoords[i],rowCoords[i+1]);
        swap(colCoords[i],colCoords[i+1]);
        
        //cout << "Swapped " << i << ":" << intArr[i] << " " << i+1 << ":" << intArr[i+1] << endl;
      }
    }
    numRuns++;
  } while(!isSorted);
}

void QueenResults::swap(uint &A, uint &B) const
{
  //assign A to temp
  int temp = A;
  
  //assign B to A
  A=B;
  
  //assign temp to B
  B=temp;
}
