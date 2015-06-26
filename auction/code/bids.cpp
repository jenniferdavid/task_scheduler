#include <string>
#include <fstream>

using namespace std;

//
//  Bids Received During Auction
//

#include "bids.h"


int nBids;

const int MaxBids = 1000;

// Names of bidders
std::string bidBidder[MaxBids];

// Bid Amounts
double bidAmounts[MaxBids];

// Names of items
std::string bidItems[MaxBids];

// Time at which bid was placed
int bidHours[MaxBids];
int bidMinutes[MaxBids];
int bidSeconds[MaxBids];




/**
 * Read all bids from the indicated file
 */
void readBids (std::string fileName)
{
    ifstream in (fileName.c_str());
    in >> nBids;
    for (int i = 0; i < nBids; ++i)
    {
        char c;
        in >> bidBidder[i] >> bidAmounts[i];
        in >> bidHours[i] >> c >> bidMinutes[i] >> c >> bidSeconds[i];
        string word, line;
        in >> word; // First word of itemName
        getline (in, line); // rest of item name
        bidItems[i] = word + line;
    }
}
