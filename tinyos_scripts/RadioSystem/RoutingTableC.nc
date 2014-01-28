/**
 *  Source file for configuration RoutingTableC,
 *  a file that contain and provides the routing table information
 *
 *  @author Alessandro Redondi
 */

configuration RoutingTableC
{
    provides
    {
        interface RoutingTable;
    }
}

implementation
{
    components RoutingTableP;        

    RoutingTable = RoutingTableP;   
}