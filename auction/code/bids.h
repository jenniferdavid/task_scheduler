#ifndef BIDS_H
#define BIDS_H

#include <string>

//
//  Bids Received During Auction
//


extern int nBids;

extern const int MaxBids;

// Names of bidders
extern std::string bidBidder[];

// Bid Amounts
extern double bidAmounts[];

// Names of items
extern std::string bidItems[];

// Time at which bid was placed
extern int bidHours[];
extern int bidMinutes[];
extern int bidSeconds[];




/**
 * Read all bids from the indicated file
 */
void readBids (std::string fileName);



#endif
