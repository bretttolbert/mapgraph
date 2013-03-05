import csv
import simplejson as json

"""
A throwaway script to generate the JSON of an indicator group from the text of an indicator group copied from http://data.worldbank.org/indicator
"""

wdi_data_csv_path = '../untracked/WDI_csv/WDI_Data.csv' #path to WDI_Data.csv

#first line the dataset id (lowercase, words separated with dashes), e.g. social-dev
#second line is the dataset name, e.g. Social Development
#rest of the lines are the Indicator Names, one per line
#script will automatically find the Indicator Code from the CSV

raw = """
urban-dev
Urban Development
Improved sanitation facilities, urban (% of urban population with access)
Pump price for diesel fuel (US$ per liter)
Improved water source, urban (% of urban population with access)
Pump price for gasoline (US$ per liter)
Motor vehicles (per 1,000 people)
Road sector diesel fuel consumption per capita (kg of oil equivalent)
Passenger cars (per 1,000 people)
Road sector energy consumption (% of total energy consumption)
PM10, country level (micrograms per cubic meter)
Road sector gasoline fuel consumption per capita (kg of oil equivalent)
Population in the largest city (% of urban population)
Urban population
Population in urban agglomerations of more than 1 million (% of total population)
Urban population (% of total)
Poverty gap at urban poverty line (%)
Vehicles (per km of road)
Poverty headcount ratio at urban poverty line (% of urban population)
"""

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

dataset = {'id':'','name':'','indicators':[]}
for i,line in enumerate(raw.strip().split('\n')):
    indicator_name = ''
    indicator_code = ''
    if i == 0:
        dataset['id'] = line.strip()
    elif i == 1:
        dataset['name'] = line.strip()
    elif line != '':
        indicator_name = line
        #find indicator code from indicator name
        with open(wdi_data_csv_path,'rb') as csvfile:
            reader = csv.reader(csvfile)
            first_row = True
            for row in reader:
                if first_row:
                    first_row = False
                else:
                    if row[columns['Indicator Name']] == indicator_name:
                        indicator_code = row[columns['Indicator Code']]
                        dataset['indicators'].append({'Indicator Name':indicator_name,'Indicator Code':indicator_code})
                        break
print json.dumps(dataset, sort_keys=False, indent=4, separators=(',',': '))