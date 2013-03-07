import json
import codecs

"""
Another throwaway conversion script.
You can use this script as a template to convert CSV or tab-delimited files to mapgraph JSON dataset format
for use with the us-states SVG map
"""

columns = {
    'State': 0,
    'Average Number of Tornadoes per Year': 1,
    'Average Number of Deaths per Year': 2, 
    'Average Number of Tornadoes Per 10,000 Sq Mi': 3
}

map_data_file_path = '../maps/us-states/map.json' #path to map.json for the "us-states" map, used for converting state name to FIPS numeric ID

with codecs.open(map_data_file_path, 'r', encoding='utf-8') as mapfile:
    mapdata = json.loads(mapfile.read())
    data = { 'nodeIdSource': 'FIPS', 'metadata': [{'Data_Item':'FIPS','Item_Description':'FIPS state code'}], 'data': [] }
    data['name'] = 'U.S. Reported Tornadoes and Average Number of Deaths per Year 1961-1990'
    data['sourceUrl'] = 'http://www.erh.noaa.gov/cae/svrwx/tornadobystate.htm'
    data['metadata'].append({'Data_Item':'avg-tornadoes-per-yr','Item_Description':'Average Number of Tornadoes per Year'})
    data['metadata'].append({'Data_Item':'avg-deaths-per-yr','Item_Description':'Average Number of Deaths per Year'})
    data['metadata'].append({'Data_Item':'avg-tornadoes-per-10000sqmi','Item_Description':'Average Number of Tornadoes Per 10,000 Sq Mi'})
    with open('tornadoes.txt') as f:
        first_line = True
        for line in f.readlines():
            if first_line:
                first_line = False
            else:
                tokens = [i.strip() for i in line.split('\t')]
                state_name = tokens[columns['State']]
                #convert state name to US state FIPS code
                found = False
                for node in mapdata['nodes']:
                    if node['s'] == state_name:
                        state_fips = node['id']
                        found = True
                        break
                if not found:
                    print 'Error: failed to find FIPS for state {0}'.format(state_name)
                else:
                    record = {'FIPS': state_fips}
                    record['avg-tornadoes-per-yr'] = float(tokens[columns['Average Number of Tornadoes per Year']])
                    record['avg-deaths-per-yr'] = float(tokens[columns['Average Number of Deaths per Year']])
                    record['avg-tornadoes-per-10000sqmi'] = float(tokens[columns['Average Number of Tornadoes Per 10,000 Sq Mi']])
                    data['data'].append(record)
    with open('../datasets/us-states/tornadoes.json','w') as json_out_file:
        json_out_file.write(json.dumps(data, sort_keys=False, indent=4, separators=(',',': ')))
    with open('../datasets/us-states/tornadoes.min.json','w') as json_min_out_file:
        json_min_out_file.write(json.dumps(data, separators=(',',':')))