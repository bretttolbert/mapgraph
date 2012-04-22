Usage:

-m <mode>                Set mode. Posible values are "game" and "bfs".
-s <start>                Specify starting node (optional, defaults to random).
-g <goal>                Specify goal node (optional, defaults to random).
-af <file> <fmt>        Set adjacency file and adjacency file format.
                        Possible formats:
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