#include <iostream>
#include <fstream>

#include "items.h"

//
//  Items up for auction
//

using namespace std;

int nItems;

extern const int MaxItems = 100;

// Names of items
std::string itemNames[MaxItems];

// Reserve price (minimum acceptable price)
double itemReservedPrice[MaxItems];

// Time at which auction ends for each item
int itemEndHours[MaxItems];
int itemEndMinutes[MaxItems];
int itemEndSeconds[MaxItems];


/**
 * Read one item from the indicated file
 */
void readItem (istream& in, int itemNum)
{
  in >> itemReservedPrice[itemNum];
  in >> itemEndHours[itemNum];
  char c;
  in >> c; // Ignore the ':' between hours and minutes
  in >> itemEndMinutes[itemNum];
  in >> c; // Ignore the ':' between minutes and seconds
  in >> itemEndSeconds[itemNum];

  // Reading the item name.
  in >> c; // Skips blanks and reads first character of the name
  string line;
  getline (in, line); // Read the rest of the line
  itemNames[itemNum] = string(1,c) + line;
}


/**
 * Read all items from the indicated file
 */
void readItems (std::string fileName)
{
  ifstream in (fileName.c_str());
  in >> nItems;
  for (int i = 0; i < nItems; ++i)
    readItem (in, i);
}

