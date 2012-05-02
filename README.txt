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
                        Required arguments: -af, -nn
                        Optional arguments: -n1
            Finds a path to visit the nodes specified by the -nn option
            starting from and returning to the first node specified.
            Optionally starting from and returning to the node specified 
            by the -n1 option (-n1 is useful if you are using a -nn preset, 
            such as -nn preset washingtons).
            In a traditional TSP, the distance between each target node
            would be given. In this case, we must use another algorithm (BFS)
            to find the shortest path between each target node and then use
            that path length as the distance for the TSP algorithm.

    greedy-coloring  
        Demonstration of greedy algorithm to attempt proper coloring of a graph
        using the colors specified with the -c argument.
                        Required arguments: -af, -c

    neighbors           
        Displays the neighbors of a specified node
                        Required arguments: -n1, -af
    test                
        Developer test stub

-af preset <preset>     Use preset adjacency list. Presets include:
                          us-states - The 48 continental states of the USA
                          us-counties - 3000+ county or county equivalents 
                        Both the us-states and us-counties presets support 
                        SVG output. Most modes will generate an SVG file
                        which by default is saved as output/output.svg

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
            2010 State and County name of each neighboring county 
            or county equivalent
         4 Neighbor GEOID    
            2010 State and County FIPS Codes of each neighboring 
            county or county equivalent
    Lines in which columns 1 and 2 are not blank denote the start of an entry.
    In the US census file, every county has itself listed as a neighboring county.

-n1 <node>              Specify first node. In bfs-demo mode, this option
                        is used to specify the starting node. In neighbors
                        mode, it is used to specify the node to show the
                        neighbors of.

-n2 <node>              Specify second node. In bfs-demo mode, this option
                        is used to specify the goal node. 

-nn <nodes>             Specify nodes to be used. This option is used by modes
                        which expect more than two nodes as input, such as the
                        traveling salesman problem demo. Nodes should be speci-
                        fied as a ';' delimited string. Node names do no have
                        to be exact. For example, the following command resovles
                        to "Washington County, AL", "San Diego County, CA" and
                        "Boulder County, CO":
                            -nn "washington, al;san diego;boulder co;"

-nn preset <preset>     Use a predefined set of nodes. Presets include:
                            washingtons - All 31 counties named "Washington"
                                To be used with the us-counties adjacency list.

-c <colors>             Specify colors to be used. All of the 147 colors defined in
                        the following table may be used with the exception of "black":
                        http://www.w3schools.com/html/html_colornames.asp
                        Colors should be specified as a ';' delimited string.

-c random <number>      Specify that the given number of colors should be used.
                        The colors will be generated randomly.

-w                      Show warnings (useful for validating adjacency file).

Examples:

Play the navigation game using the us-states adjacency file preset and
navigating from a random state to another random state:

    -m game -af preset us-states -w

Play the navigation game using the us-counties adjacency file preset and
navigating from a random county to another random county:

    -m game -n1 "Madison County, AL" -n2 "Covington County, AL" -af preset us-counties -w

Run the breadth first search demo using the us-state adajcency file preset
from a random starting node to a random goal node:

    -m bfs-demo -af preset us-states -w

Run the breadth first search demo using the us-state adajcency file preset
from the specified starting node to the specified goal node:

    -m bfs-demo -n1 "Madison County, AL" -n2 "San Diego County, CA" -af preset us-counties -w

Run the bipartite demo to test the bipartiteness of the specified graph:

    -m bipartite-demo -af preset us-counties -w

Show the neighbors (border states) of the specified node (Alabama):

    -m neighbors -n1 "AL" -af preset us-states -w

Show the neighbors (bordering counties) of the specified node (Madison County, AL):

    -m neighbors -n1 "Madison County, AL" -af preset us-counties -w

Run the traveling salesman problem to find a path traversing the specified nodes 
starting from and returning to "Washington County, AL":

    -m tsp-demo -nn "washington, al;san diego;boulder co;" -af preset us-counties -w

Run the traveling salesman problem demo using the "washingtons" nodes preset starting
from and returning to "Washington County, AL" (since it comes first alphabetically):

    -m tsp-demo -nn preset washingtons -af preset us-counties -w

Run the traveling salesman problem demo using the "washingtons" nodes preset starting
from and return to a randomly selected "Washington":

    -m tsp-demo -nn preset washingtons -n1 random -af preset us-counties -w

Run the traveling salesman problem demo using 10 randomly selected nodes:

    -m tsp-demo -nn random 10 -af preset us-counties -w


