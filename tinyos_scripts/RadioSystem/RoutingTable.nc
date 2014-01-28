/**
 *  Source file for declaration of interface RoutingTable.
 *  Defines commands for the management of the routing table 
 *
 *  @author Luca Pietro Borsani
 *  @author Alessandro Redondi
 */

#include "AM.h"
interface RoutingTable
{
  command void init();
  
  command am_addr_t getNextHop(am_addr_t dst_addr);
}
