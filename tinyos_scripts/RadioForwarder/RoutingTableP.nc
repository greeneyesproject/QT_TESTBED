/**
 *  Source file for module RoutingTableP, a component that provides
 *  an implementation of interface RoutingTable
 *
 *  @author Alessandro Redondi
 */
#include "RadioSystem.h"

module RoutingTableP
{
    provides
    {
        interface RoutingTable;
    }

}

implementation
{
  routing_table_entry_t routing_table[TABLE_SIZE];
  
  command void RoutingTable.init(){
    
    routing_table[0].dst_addr = 0;
    routing_table[0].next_hop = 0;
    
    routing_table[1].dst_addr = 1;
    routing_table[1].next_hop = 2;
  }
  
  command am_addr_t RoutingTable.getNextHop(am_addr_t dst_addr){
    uint8_t iCont;
    for(iCont=0; iCont<TABLE_SIZE;iCont++){
      if(routing_table[iCont].dst_addr == dst_addr)
	return routing_table[iCont].next_hop;
    }
    return -1;
  }
  
}
