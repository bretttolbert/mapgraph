import csv
import codecs
import simplejson as json
import sys

"""
A script for converting subsets of WDI_Data.csv (146 MB) to mapgraph JSON dataset format for use with the "world" SVG map
WDI_Data.csv retreived 2013-03-03 from http://data.worldbank.org/data-catalog/world-development-indicators
Usage: convert_WDI_Data.csv.py > poverty.json
"""

wdi_data_csv_path = 'WDI_Data.csv' #path to WDI_Data.csv
world_map_data_file_path = 'map.json' #path to map.json for the "world" map, used for converting alpha-3 country codes to ISO 3166-1 numeric

# WDI_Data.csv columns:
columns = {
    'Country Name': 0,
    'Country Code': 1,
    'Indicator Name': 2,
    'Indicator Code': 3,
    '1960': 4,
    '1961': 5,
    '1962': 6,
    '1963': 7,
    '1964': 8,
    '1965': 9,
    '1966': 10,
    '1967': 11,
    '1968': 12,
    '1969': 13,
    '1970': 14,
    '1971': 15,
    '1972': 16,
    '1973': 17,
    '1974': 18,
    '1975': 19,
    '1976': 20,
    '1977': 21,
    '1978': 22,
    '1979': 23,
    '1980': 24,
    '1981': 25,
    '1982': 26,
    '1983': 27,
    '1984': 28,
    '1985': 29,
    '1986': 30,
    '1987': 31,
    '1988': 32,
    '1989': 33,
    '1990': 34,
    '1991': 35,
    '1992': 36,
    '1993': 37,
    '1994': 38,
    '1995': 39,
    '1996': 40,
    '1997': 41,
    '1998': 42,
    '1999': 43,
    '2000': 44,
    '2001': 45,
    '2002': 46,
    '2003': 47,
    '2004': 48,
    '2005': 49,
    '2006': 50,
    '2007': 51,
    '2008': 52,
    '2009': 53,
    '2010': 54,
    '2011': 55,
    '2012': 56
}

#indicator groups, per http://data.worldbank.org/indicator
datasets = [
    {
        'id': 'agr-rur-dev',
        'name': 'Agriculture & Rural Development',
        'indicators': [
            {
                'Indicator Name': 'Agricultural irrigated land (% of total agricultural land)',
                'Indicator Code': 'AG.LND.IRIG.AG.ZS',
            },
            {
                'Indicator Name': 'Agricultural land (% of land area)',
                'Indicator Code': 'AG.LND.AGRI.ZS'
            },
            {
                'Indicator Name': 'Agricultural machinery, tractors per 100 sq. km of arable land',
                'Indicator Code': 'AG.LND.TRAC.ZS'
            },
            {
                'Indicator Name': 'Agriculture, value added (% of GDP)',
                'Indicator Code': 'NV.AGR.TOTL.ZS'
            },
            {
                'Indicator Name': 'Agriculture value added per worker (constant 2000 US$)',
                'Indicator Code': 'EA.PRD.AGRI.KD'
            },
            {
                'Indicator Name': 'Arable land (hectares per person)',
                'Indicator Code': 'AG.LND.ARBL.HA.PC'
            },
            {
                'Indicator Name': 'Arable land (% of land area)',
                'Indicator Code': 'AG.LND.ARBL.ZS'
            },
            {
                'Indicator Name': 'Cereal yield (kg per hectare)',
                'Indicator Code': 'AG.YLD.CREL.KG'
            },
            {
                'Indicator Name': 'Crop production index (2004-2006 = 100)',
                'Indicator Code': 'AG.PRD.CROP.XD'
            },
            {
                'Indicator Name': 'Employment in agriculture (% of total employment)',
                'Indicator Code': 'SL.AGR.EMPL.ZS'
            },
            {
                'Indicator Name': 'Fertilizer consumption (kilograms per hectare of arable land)',
                'Indicator Code': 'AG.CON.FERT'
            },
            {
                'Indicator Name': 'Food production index (2004-2006 = 100)',
                'Indicator Code': 'AG.PRD.FOOD.XD'
            },
            {
                'Indicator Name': 'Forest area (% of land area)',
                'Indicator Code': 'AG.LND.FRST.ZS'
            },
            {
                'Indicator Name': 'Forest area (sq. km)',
                'Indicator Code': 'AG.LND.FRST.K2'
            },
            {
                'Indicator Name': 'Improved water source, rural (% of rural population with access)',
                'Indicator Code': 'SH.H2O.SAFE.RU.ZS'
            },
            {
                'Indicator Name': 'Land area (sq. km)',
                'Indicator Code': 'AG.LND.TOTL.K2'
            },
            {
                'Indicator Name': 'Land under cereal production (hectares)',
                'Indicator Code': 'AG.LND.CREL.HA'
            },
            {
                'Indicator Name': 'Livestock production index (2004-2006 = 100)',
                'Indicator Code': 'AG.PRD.LVSK.XD'
            },
            {
                'Indicator Name': 'Permanent cropland (% of land area)',
                'Indicator Code': 'AG.LND.CROP.ZS'
            },
            {
                'Indicator Name': 'Poverty gap at rural poverty line (%)',
                'Indicator Code': 'SI.POV.RUGP'
            },
            {
                'Indicator Name': 'Poverty headcount ratio at rural poverty line (% of rural population)',
                'Indicator Code': 'SI.POV.RUHC'
            },
            {
                'Indicator Name': 'Rural population',
                'Indicator Code': 'SI.POV.RUHC'
            },
            {
                'Indicator Name': 'Rural population (% of total population)',
                'Indicator Code': 'SP.RUR.TOTL.ZS'
            },
        ]
    }
]

with open(wdi_data_csv_path,'rb') as csvfile:
    reader = csv.reader(csvfile)
    with codecs.open(world_map_data_file_path, 'r', encoding='utf-8') as mapfile:
        mapdata = json.loads(mapfile.read())
        for dataset in datasets:
            data = { 'nodeIdSource': 'id', 'metadata': [{'Data_Item':'id','Item_Description':'ISO 3166-1 numeric country code'}], 'data': [] }
            data_count = 0
            no_data_count = 0
            first_row = True
            for row in reader:
                if first_row:
                    first_row = False
                else:
                    data_item = row[columns['Indicator Code']]
                    item_description = row[columns['Indicator Name']]
                    #check if data item is in dataset
                    found = False
                    for indicator in dataset['indicators']:
                        if indicator['Indicator Code'] == data_item:
                            found = True
                            break
                    if found:
                        #check if this has been added to metadata
                        found = False
                        for node in data['metadata']:
                            if node['Data_Item'] == data_item:
                                found = True
                                break
                        if not found:
                            data['metadata'].append({'Data_Item':data_item,'Item_Description':item_description})
                        name = row[columns['Country Name']]
                        sid3 = row[columns['Country Code']]
                        # Skip codes that aren't represented in the SVG
                        # Caribbean small states
                        # Channel Islands
                        # East Asia and Pacific
                        # etc...
                        if sid3 in ('CSS','CHI','EAS','EAP','EMU','ECS','ECA','EUU','HPC','HIC','NOC','OEC','LCN','LAC','LDC','LMY','LIC','LMC','MEA','MNA','MIC','NAC','INX','OED','OSS','PSS','SST','SAS','SSF','SSA','UMC','WLD'):
                            continue
                        #convert world bank alpha-3 codes to ISO 3166-1 alpha-3
                        if sid3 == 'ADO':
                            sid3 = 'AND'
                        elif sid3 == 'ARB':
                            sid3 = 'ABW'
                        elif sid3 == 'ZAR':
                            sid3 = 'COD'
                        elif sid3 == 'IMY':
                            sid3 = 'IMN'
                        elif sid3 == 'KSV':
                            sid3 = 'UNK'
                        elif sid3 == 'ROM':
                            sid3 = 'ROU'
                        elif sid3 == 'TMP':
                            sid3 = 'TLS'
                        elif sid3 == 'WBG':
                            sid3 = 'PSE'
                        #find numeric id from alpha3
                        id = -1
                        for node in mapdata['nodes']:
                            if node['sid3'] == sid3:
                                id = node['id']
                        if id == -1:
                            print 'Error: failed to find numeric country code for alpha-3 {0} ({1})'.format(sid3,name)
                            sys.exit(1)
                        #determine if we've already created a data node with this id
                        new_data_node = True
                        data_node = {'id': id}
                        for node in data['data']:
                            if node['id'] == id:
                                new_data_node = False
                                data_node = node
                                break
                        year = 2012
                        #go backwards until a value is found (get the latest year which for which data is available)
                        val = ''
                        while val == '':
                            val = row[columns[str(year)]]
                            year -= 1
                            if year < 1960:
                                break
                        if val != '':
                            #print '{0}: {1}'.format(sid3, float(val))
                            data_node[data_item] = float(val)
                            if new_data_node:
                                data['data'].append(data_node)
                            data_count += 1
                        else:
                            #print 'null'
                            no_data_count += 1
            #print 'data_count={0}'.format(data_count)
            #print 'no_data_count={0}'.format(no_data_count)
            with open('{0}.json'.format(dataset['id']),'w') as json_out_file:
                json_out_file.write(json.dumps(data, sort_keys=False, indent=4, separators=(',',': ')))
            with open('{0}.min.json'.format(dataset['id']),'w') as json_min_out_file:
                json_min_out_file.write(json.dumps(data, separators=(',',':')))