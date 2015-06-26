#ifndef ITEMS_H
#define ITEMS_H

#include <string>

//
//  Items up for auction
//


extern int nItems;

extern const int MaxItems;

// Names of items
extern std::string itemNames[];

// Reserve price (minimum acceptable price)
extern double itemReservedPrice[];

// Time at which auction ends for each item
extern int itemEndHours[];
extern int itemEndMinutes[];
extern int itemEndSeconds[];

/**
 * Read all items from the indicated file
 */
void readItems (std::string fileName);



#endif
