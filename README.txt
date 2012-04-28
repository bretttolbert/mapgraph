Compiling from source:

	git clone git@github.com:bretttolbert/graphgame.git
	cd graphgame
	git clone git://github.com/leethomason/tinyxml2.git
	make

Usage:

-m <mode>               Set mode. Posible include:
    game        Navigation game
    bfs         Breadth first search
    tsp         Traveling salesman problem
        Find a path to visit all 31 counties named "Washington" starting
        from and returning to Washington County, AL. 
        In a traditional TSP, the distance between each Washington
        would be given. In this case, we must use another algorithm
        to find the shortest path between each Washington and then use
        that path length as the distance for the TSP algorithm.
    test        Test stub
-s <start>              Specify starting node (optional, defaults to random).
-g <goal>               Specify goal node (optional, defaults to random).
-w                      Show warnings (useful for validating adjacency file).
-lf <file>              Specify log file (defaults to "output.txt")
-al preset <preset>     Use preset adjacency list. Presets include:
                          us-states - The 48 continental states of the USA
                          us-counties - 3000+ county or county equivalents 
-al <file> <fmt>        Use custom adjacency list file. Must specify adjacency 
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

    -m game -af 48US.txt slcsv

    -m game -af US_county_adjacency_2010.txt mltdusc

    -m bfs -s "Madison County, AL" -g "San Diego County, CA" -af US_county_adjacency_2010.txt mltdusc

Ideas: 
    Different goal - have the user "walk" the graph, attempting to traverse all 48 states with as few moves as possible.

