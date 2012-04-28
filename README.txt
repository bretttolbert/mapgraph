Compiling from source:

	git clone git@github.com:bretttolbert/graphgame.git
	cd graphgame
	git clone git://github.com/leethomason/tinyxml2.git
	make

Usage:

-m <mode>               Set mode. Modes include:
    game                
        Navigation game
                        Required arguments: -af
                        Optional arugments: -n1, -n2
                        -n1 and -n2 may be used to specify start
                        and goal nodes (respectively). If omitted,
                        a random node will be chosen.
                        
    bfs-demo            
        Breadth first search demonstration
                        Required arguments: -af
                        Optional arugments: -n1, -n2
                        -n1 and -n2 may be used to specify start
                        and goal nodes (respectively). If omitted,
                        a random node will be chosen.
    tsp-demo            
        Traveling salesman problem demonstration
            Finds a path to visit all 31 counties named "Washington" starting
            from and returning to Washington County, AL. 
            In a traditional TSP, the distance between each Washington
            would be given. In this case, we must use another algorithm
            to find the shortest path between each Washington and then use
            that path length as the distance for the TSP algorithm.
                        Required arguments: none
    bipartite-demo  
        Demonstration of algorithm to test bipartiteness of a graph.
                        Required arguments: -af
    neighbors           
        Displays the neighbors of a specified node
                        Required arguments: -n1, -af
    test                
        Test stub

-n1 <node>              Specify first node
-n2 <node>              Specify second node
-w                      Show warnings (useful for validating adjacency file).
-af preset <preset>     Use preset adjacency list. Presets include:
                          us-states - The 48 continental states of the USA
                          us-counties - 3000+ county or county equivalents 
-af <file> <fmt>        Use custom adjacency list file. Must specify adjacency 
                        file and adjacency file format. Possible formats:
   slc            Single Line CSV
     Example file: 48US.txt
     One line per node with tokens seperated by commas.
     First token is the node. The rest of the tokens are its neighbors.
   mltdusc        Multi Line Tab Delimited US Census
     Example file: US_county_adjacency_2010.txt Originally from 
     http://www.census.gov/geo/www/geoareas/county_adjacency.html
     but it was missing entries for several counties. We've added the missing 
     entries for all counties in the continental US using adjacency info from 
     Wikipedia. Each node entry spans multiple lines. Each line consists of 
     the following columns: 
         1 County Name        
            2010 State and County Name
         2 County GEOID     
            2010 State and County FIPS Codes
         3 Neighbor Name    
            2010 State and County name of each neighboring county or county equivalent
         4 Neighbor GEOID    
            2010 State and County FIPS Codes of each neighboring county or county equivalent
    Lines in which columns 1 and 2 are not blank denote the start of an entry.
    In the US census file, every county has itself listed as a neighboring county.

Examples:

    -m game -af preset us-states -w

    -m game -n1 "Madison County, AL" -n2 "Covington County, AL" -af preset us-counties -w

    -m bfs-demo -af preset us-states -w

    -m bfs-demo -n1 "Madison County, AL" -n2 "San Diego County, CA" -af preset us-counties -w

    -m bipartite-demo -af preset us-counties -w

    -m neighbors -n1 "AL" -af preset us-states

    -m neighbors -n1 "Madison County, AL" -af preset us-counties -w



