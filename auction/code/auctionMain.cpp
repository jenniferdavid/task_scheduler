#include <iostream>
#include <string>

using namespace std;

#include "items.h"
#include "bidders.h"
#include "bids.h"
#include "times.h"


/**
 * Determine the winner of the auction for item #i.
 * Announce the winner and remove money from winner's account.
 */
void resolveAuction (int itemNumber);


int main (int argc, char** argv)
{
  if (argc != 4)
    {
      cerr << "Usage: " << argv[0] << " itemsFile biddersFile bidsFile" << endl;
      return -1;
    }

  readItems (argv[1]);
  readBidders (argv[2]);
  readBids (argv[3]);

  for (int i = 0; i < nItems; ++i)
    {
      resolveAuction(i);
    }
  return 0;
}


/**
 * Determine the winner of the auction for item #i.
 * Announce the winner and remove money from winner's account.
 */
void resolveAuction (int itemNum)
{
  string itemName = itemNames[itemNum];
  int itemH = itemEndHours[itemNum];
  int itemM = itemEndMinutes[itemNum];
  int itemS = itemEndSeconds[itemNum];
  double itemReserve = itemReservedPrice[itemNum];

  double highestBidSoFar = 0.0;
  string winningBidderSoFar;
  for (int bidNum = 0; bidNum < nBids; ++bidNum)
    {
      if (noLaterThan(bidHours[bidNum], bidMinutes[bidNum], bidSeconds[bidNum],
                  itemH, itemM, itemS))
        {
          if (bidItems[bidNum] == itemName
            && bidAmounts[bidNum] > highestBidSoFar
            && bidAmounts[bidNum] > itemReserve
            )
            {
              int bidderNum = findBidder(bidBidder[bidNum]);
              // Can this bidder afford it?
              if (bidAmounts[bidNum] <= bidderBalances[bidderNum])
                {
                  highestBidSoFar = bidAmounts[bidNum];
                  winningBidderSoFar = bidBidder[bidNum];
                }
            }
        }
    }

    // If highestBidSoFar is non-zero, we have a winner
    if (highestBidSoFar > 0.0)
      {
        int bidderNum = findBidder(winningBidderSoFar);
        cout << itemNames[itemNum]
             << " won by " << winningBidderSoFar
             << " for " << highestBidSoFar << endl;
        bidderBalances[bidderNum] -= highestBidSoFar;
      }
    else
      {
        cout << itemNames[itemNum]
             << " reserve not met"
             << endl;
      }
}
