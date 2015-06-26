#ifndef BIDDERS_H
#define BIDDERS_H

#include <string>

//
//  Bidders Registered for auction
//


extern int nBidders;

extern const int MaxBidders;

// Names of bidders
extern std::string bidderNames[];

// Account balances
extern double bidderBalances[];


/**
 * Read all bidders from the indicated file
 */
void readBidders (std::string fileName);


/**
 * Find the index of the bidder with the given name. If no such bidder exists,
 * return nBidders.
 */
 int findBidder (std::string name);



#endif
