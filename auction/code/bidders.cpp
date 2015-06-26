#include <string>
#include <fstream>
#include <iostream>
//
//  Bidders Registered for auction
//

#include "bidders.h"

using namespace std;


int nBidders;

const int MaxBidders = 100;

// Names of bidders
std::string bidderNames[MaxBidders];

// Account balances
double bidderBalances[MaxBidders];


/**
 * Read all bidders from the indicated file
 */
void readBidders (std::string fileName)
{
    ifstream in (fileName.c_str());
    in >> nBidders;
    for (int i = 0; i < nBidders; ++i)
    {
        in >> bidderNames[i] >> bidderBalances[i];
    }
}


/**
 * Find the index of the bidder with the given name. If no such bidder exists,
 * return nBidders.
 */
 int findBidder (std::string name)
 {
     int found = nBidders;
     for (int i = 0; i < nBidders && found == nBidders; ++i)
     {
         if (name == bidderNames[i])
            found = i;
     }
     return found;
 }
