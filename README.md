# mapgraph

## Interactive SVG maps of the World, United States and US counties. 

## Demo

- Visualizing population growth of U.S. Counties at the county level
    - [2010 US Counties Population](https://bretttolbert.com/assets/projects/mapgraph/mapgraph.php?map=us-counties&mode=graph&dataset=2010-quick-facts&dataitem=POP010210)
    - [2024 US Counties Population](https://bretttolbert.com/assets/projects/mapgraph/mapgraph.php?map=us-counties&mode=graph&dataset=2024-census-pop-est&dataitem=2024-population-est)
- Gini indexes for all countries (high number means greater inequality)
    - [2025 World Gini indexes](https://bretttolbert.com/assets/projects/mapgraph/mapgraph.php?map=world&mode=graph&dataset=2025-WDI-poverty&dataitem=SI.POV.GINI)
- Visualizing changes in adolecesent fertility rates around the world
    - [2010 World Adolecesent fertility rates](https://bretttolbert.com/assets/projects/mapgraph/mapgraph.php?map=world&mode=graph&dataset=2012-WDI-social-dev&dataitem=SP.ADO.TFRT)
    - [2024 World Adolecesent fertility rates](https://bretttolbert.com/assets/projects/mapgraph/mapgraph.php?map=world&mode=graph&dataset=2025-WDI-social-dev&dataitem=SP.ADO.TFRT)

## Features

- Displays node name on mouse hover. 
- Opens Wikipdia page on mouse click. 
- Visualization of US census data. Sandbox for graph traversal algorithms (each county is a node)

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

# Screenhots

![2024_US_Counties_Population_Est](/doc/screenshots/2024_US_Counties_Population_Est.png)
![old_thumbnail](/doc/screenshots/old_thumbnail.png)
![2013_Alabama_Population](/doc/screenshots/2013_Alabama_Population.png)
![2024_Alabama_Est_Population](/doc/screenshots/2024_Alabama_Est_Population.png)