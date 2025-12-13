# mapgraph

## Interactive SVG maps of the World, United States and US counties. 

## Demo

Live demo: [bretttolbert.com/assets/projects/mapgraph/mapgraph.php?map=us-counties&mode=graph&dataset=2024-census-pop-est](https://bretttolbert.com/assets/projects/mapgraph/mapgraph.php?map=us-counties&mode=graph&dataset=2024-census-pop-est)

## Features

- Displays node name on mouse hover. 
- Opens Wikipdia page on mouse click. 
- Visualization of US census data. Sandbox for graph traversal algorithms (each county is a node)

![Screenshot](thumbnail.png?raw=true "Screenshot")


## datasets

* map: us-counties
    * dataset: 2024-census-pop-est
        * Desc: U.S. Census Bureau, 2024 populuation est, counties
        * Source File: co-est2024-alldata.csv
        * Source URL: https://www2.census.gov/programs-surveys/popest/datasets/2020-2024/counties/totals/
    * dataset: 2010-quick-facts
        * 2010 census us-counties quickfacts 
* map: us-states
    * dataset: 2020-census-pop-CD118
        * Desc: 2020 census us-states population data (CD118)
        * Desc: U.S. Census Bureau, 2020 Census 118th Congressional District Summary File (CD118)
        * Source File: DECENNIALCD1182020.P1_2025-12-12T192416/DECENNIALCD1182020.P1-Data.csv
        * URL: https://data.census.gov/table/DECENNIALCD1182020.P1
    * dataset: quick-facts-2010
        * Desc: 2010 census us-states quickfacts 
    * dataset: Tornado data
* map: world
    * datasets: 2012 World Development Indicators
