import csv
import codecs
import simplejson as json
import sys

"""
A script for converting subsets of WDI_Data.csv (146 MB) to mapgraph JSON dataset format for use with the "world" SVG map
WDI_Data.csv retreived 2013-03-03 from http://data.worldbank.org/data-catalog/world-development-indicators
Usage: convert_WDI_Data.csv.py > poverty.json
"""

wdi_data_csv_path = '../untracked/WDI_csv/WDI_Data.csv' #path to WDI_Data.csv
world_map_data_file_path = '../maps/world/map.json' #path to map.json for the "world" map, used for converting alpha-3 country codes to ISO 3166-1 numeric

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
        'id': 'agriculture-and-rural-dev',
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
            }
        ]
    },
    {
        "id": "aid-effectiveness",
        "name": "Aid Effectiveness",
        "indicators": [
            {
                "Indicator Code": "EN.ATM.CO2E.PC",
                "Indicator Name": "CO2 emissions (metric tons per capita)"
            },
            {
                "Indicator Code": "DT.ODA.ODAT.PC.ZS",
                "Indicator Name": "Net ODA received per capita (current US$)"
            },
            {
                "Indicator Code": "SP.DYN.CONU.ZS",
                "Indicator Name": "Contraceptive prevalence (% of women ages 15-49)"
            },
            {
                "Indicator Code": "DT.ODA.ALLD.CD",
                "Indicator Name": "Net official development assistance and official aid received (current US$)"
            },
            {
                "Indicator Code": "SH.STA.ACSN",
                "Indicator Name": "Improved sanitation facilities (% of population with access)"
            },
            {
                "Indicator Code": "DT.ODA.ODAT.CD",
                "Indicator Name": "Net official development assistance received (current US$)"
            },
            {
                "Indicator Code": "SH.TBS.INCD",
                "Indicator Name": "Incidence of tuberculosis (per 100,000 people)"
            },
            {
                "Indicator Code": "SP.POP.TOTL.FE.ZS",
                "Indicator Name": "Population, female (% of total)"
            },
            {
                "Indicator Code": "SI.DST.FRST.20",
                "Indicator Name": "Income share held by lowest 20%"
            },
            {
                "Indicator Code": "SH.STA.ANVC.ZS",
                "Indicator Name": "Pregnant women receiving prenatal care (%)"
            },
            {
                "Indicator Code": "SP.DYN.LE00.FE.IN",
                "Indicator Name": "Life expectancy at birth, female (years)"
            },
            {
                "Indicator Code": "SH.DYN.AIDS.ZS",
                "Indicator Name": "Prevalence of HIV, total (% of population ages 15-49)"
            },
            {
                "Indicator Code": "SP.DYN.LE00.MA.IN",
                "Indicator Name": "Life expectancy at birth, male (years)"
            },
            {
                "Indicator Code": "SE.PRM.CMPT.ZS",
                "Indicator Name": "Primary completion rate, total (% of relevant age group)"
            },
            {
                "Indicator Code": "SH.STA.MALN.ZS",
                "Indicator Name": "Malnutrition prevalence, weight for age (% of children under 5)"
            },
            {
                "Indicator Code": "SG.GEN.PARL.ZS",
                "Indicator Name": "Proportion of seats held by women in national parliaments (%)"
            },
            {
                "Indicator Code": "SH.STA.MMRT",
                "Indicator Name": "Maternal mortality ratio (modeled estimate, per 100,000 live births)"
            },
            {
                "Indicator Code": "SE.ENR.PRSC.FM.ZS",
                "Indicator Name": "Ratio of girls to boys in primary and secondary education (%)"
            },
            {
                "Indicator Code": "IT.CEL.SETS.P2",
                "Indicator Name": "Mobile cellular subscriptions (per 100 people)"
            },
            {
                "Indicator Code": "SL.EMP.INSV.FE.ZS",
                "Indicator Name": "Share of women employed in the nonagricultural sector (% of total nonagricultural employment)"
            },
            {
                "Indicator Code": "SH.DYN.MORT",
                "Indicator Name": "Mortality rate, under-5 (per 1,000 live births)"
            },
            {
                "Indicator Code": "SP.MTR.1519.ZS",
                "Indicator Name": "Teenage mothers (% of women ages 15-19 who have had children or are currently pregnant)"
            },
            {
                "Indicator Code": "SM.POP.NETM",
                "Indicator Name": "Net migration"
            },
            {
                "Indicator Code": "SL.EMP.VULN.ZS",
                "Indicator Name": "Vulnerable employment, total (% of total employment)"
            },
            {
                "Indicator Code": "DT.ODA.ODAT.GN.ZS",
                "Indicator Name": "Net ODA received (% of GNI)"
            }
        ]
    },
    {
        "id": "climate-change",
        "name": "Climate Change",
        "indicators": [
            {
                "Indicator Code": "EG.ELC.ACCS.ZS",
                "Indicator Name": "Access to electricity (% of population)"
            },
            {
                "Indicator Code": "IE.PPI.ENGY.CD",
                "Indicator Name": "Investment in energy with private participation (current US$)"
            },
            {
                "Indicator Code": "AG.LND.IRIG.AG.ZS",
                "Indicator Name": "Agricultural irrigated land (% of total agricultural land)"
            },
            {
                "Indicator Code": "IE.PPI.TELE.CD",
                "Indicator Name": "Investment in telecoms with private participation (current US$)"
            },
            {
                "Indicator Code": "AG.LND.AGRI.ZS",
                "Indicator Name": "Agricultural land (% of land area)"
            },
            {
                "Indicator Code": "IE.PPI.TRAN.CD",
                "Indicator Name": "Investment in transport with private participation (current US$)"
            },
            {
                "Indicator Code": "NV.AGR.TOTL.ZS",
                "Indicator Name": "Agriculture, value added (% of GDP)"
            },
            {
                "Indicator Code": "IE.PPI.WATR.CD",
                "Indicator Name": "Investment in water and sanitation with private participation (current US$)"
            },
            {
                "Indicator Code": "ER.H2O.FWTL.K3",
                "Indicator Name": "Annual freshwater withdrawals, total (billion cubic meters)"
            },
            {
                "Indicator Code": "AG.LND.EL5M.ZS",
                "Indicator Name": "Land area where elevation is below 5 meters (% of total land area)"
            },
            {
                "Indicator Code": "AG.YLD.CREL.KG",
                "Indicator Name": "Cereal yield (kg per hectare)"
            },
            {
                "Indicator Code": "SH.STA.MALN.ZS",
                "Indicator Name": "Malnutrition prevalence, weight for age (% of children under 5)"
            },
            {
                "Indicator Code": "EN.ATM.CO2E.KT",
                "Indicator Name": "CO2 emissions (kt)"
            },
            {
                "Indicator Code": "EN.ATM.METH.KT.CE",
                "Indicator Name": "Methane emissions (kt of CO2 equivalent)"
            },
            {
                "Indicator Code": "EN.ATM.CO2E.PC",
                "Indicator Name": "CO2 emissions (metric tons per capita)"
            },
            {
                "Indicator Code": "SH.DYN.MORT",
                "Indicator Name": "Mortality rate, under-5 (per 1,000 live births)"
            },
            {
                "Indicator Code": "IQ.CPA.PUBS.XQ",
                "Indicator Name": "CPIA public sector management and institutions cluster average (1=low to 6=high)"
            },
            {
                "Indicator Code": "EN.ATM.NOXE.KT.CE",
                "Indicator Name": "Nitrous oxide emissions (thousand metric tons of CO2 equivalent)"
            },
            {
                "Indicator Code": "IC.BUS.EASE.XQ",
                "Indicator Name": "Ease of doing business index (1=most business-friendly regulations)"
            },
            {
                "Indicator Code": "SH.MLR.INCD",
                "Indicator Name": "Notified cases of malaria (per 100,000 people)"
            },
            {
                "Indicator Code": "EG.USE.ELEC.KH.PC",
                "Indicator Name": "Electric power consumption (kWh per capita)"
            },
            {
                "Indicator Code": "EN.ATM.GHGO.KT.CE",
                "Indicator Name": "Other greenhouse gas emissions, HFC, PFC and SF6 (thousand metric tons of CO2 equivalent)"
            },
            {
                "Indicator Code": "EG.USE.PCAP.KG.OE",
                "Indicator Name": "Energy use (kg of oil equivalent per capita)"
            },
            {
                "Indicator Code": "SP.POP.GROW",
                "Indicator Name": "Population growth (annual %)"
            },
            {
                "Indicator Code": "EG.USE.COMM.KT.OE",
                "Indicator Name": "Energy use (kt of oil equivalent)"
            },
            {
                "Indicator Code": "EN.URB.MCTY.TL.ZS",
                "Indicator Name": "Population in urban agglomerations of more than 1 million (% of total population)"
            },
            {
                "Indicator Code": "BX.KLT.DINV.CD.WD",
                "Indicator Name": "Foreign direct investment, net inflows (BoP, current US$)"
            },
            {
                "Indicator Code": "EN.POP.EL5M.ZS",
                "Indicator Name": "Population living in areas where elevation is below 5 meters (% of total population)"
            },
            {
                "Indicator Code": "AG.LND.FRST.ZS",
                "Indicator Name": "Forest area (% of land area)"
            },
            {
                "Indicator Code": "SP.POP.TOTL",
                "Indicator Name": "Population, total"
            },
            {
                "Indicator Code": "AG.LND.FRST.K2",
                "Indicator Name": "Forest area (sq. km)"
            },
            {
                "Indicator Code": "SI.POV.DDAY",
                "Indicator Name": "Poverty headcount ratio at $1.25 a day (PPP) (% of population)"
            },
            {
                "Indicator Code": "NY.GDP.MKTP.CD",
                "Indicator Name": "GDP (current US$)"
            },
            {
                "Indicator Code": "SE.PRM.CMPT.ZS",
                "Indicator Name": "Primary completion rate, total (% of relevant age group)"
            },
            {
                "Indicator Code": "NY.GNP.PCAP.CD",
                "Indicator Name": "GNI per capita, Atlas method (current US$)"
            },
            {
                "Indicator Code": "SE.ENR.PRSC.FM.ZS",
                "Indicator Name": "Ratio of girls to boys in primary and secondary education (%)"
            },
            {
                "Indicator Code": "SH.STA.ACSN",
                "Indicator Name": "Improved sanitation facilities (% of population with access)"
            },
            {
                "Indicator Code": "IS.ROD.PAVE.ZS",
                "Indicator Name": "Roads, paved (% of total roads)"
            },
            {
                "Indicator Code": "SH.H2O.SAFE.RU.ZS",
                "Indicator Name": "Improved water source, rural (% of rural population with access)"
            },
            {
                "Indicator Code": "SP.URB.TOTL",
                "Indicator Name": "Urban population"
            },
            {
                "Indicator Code": "SH.H2O.SAFE.UR.ZS",
                "Indicator Name": "Improved water source, urban (% of urban population with access)"
            },
            {
                "Indicator Code": "SP.URB.TOTL.IN.ZS",
                "Indicator Name": "Urban population (% of total)"
            }
        ]
    },
    {
        "id": "eco-policy-and-ext-debt",
        "name": "Economic Policy & External Debt",
        "indicators": [
            {
                "Indicator Code": "NV.AGR.TOTL.ZS",
                "Indicator Name": "Agriculture, value added (% of GDP)"
            },
            {
                "Indicator Code": "DT.DOD.MWBG.CD",
                "Indicator Name": "IBRD loans and IDA credits (DOD, current US$)"
            },
            {
                "Indicator Code": "GC.BAL.CASH.GD.ZS",
                "Indicator Name": "Cash surplus/deficit (% of GDP)"
            },
            {
                "Indicator Code": "NE.IMP.GNFS.ZS",
                "Indicator Name": "Imports of goods and services (% of GDP)"
            },
            {
                "Indicator Code": "GC.DOD.TOTL.GD.ZS",
                "Indicator Name": "Central government debt, total (% of GDP)"
            },
            {
                "Indicator Code": "NV.IND.TOTL.ZS",
                "Indicator Name": "Industry, value added (% of GDP)"
            },
            {
                "Indicator Code": "BN.CAB.XOKA.CD",
                "Indicator Name": "Current account balance (BoP, current US$)"
            },
            {
                "Indicator Code": "FP.CPI.TOTL.ZG",
                "Indicator Name": "Inflation, consumer prices (annual %)"
            },
            {
                "Indicator Code": "NE.EXP.GNFS.ZS",
                "Indicator Name": "Exports of goods and services (% of GDP)"
            },
            {
                "Indicator Code": "NY.GDP.DEFL.KD.ZG",
                "Indicator Name": "Inflation, GDP deflator (annual %)"
            },
            {
                "Indicator Code": "DT.DOD.DPNG.CD",
                "Indicator Name": "External debt stocks, private nonguaranteed (PNG) (DOD, current US$)"
            },
            {
                "Indicator Code": "DT.ODA.ODAT.GN.ZS",
                "Indicator Name": "Net ODA received (% of GNI)"
            },
            {
                "Indicator Code": "DT.DOD.DPPG.CD",
                "Indicator Name": "External debt stocks, public and publicly guaranteed (PPG) (DOD, current US$)"
            },
            {
                "Indicator Code": "DT.ODA.ODAT.PC.ZS",
                "Indicator Name": "Net ODA received per capita (current US$)"
            },
            {
                "Indicator Code": "DT.DOD.DSTC.CD",
                "Indicator Name": "External debt stocks, short-term (DOD, current US$)"
            },
            {
                "Indicator Code": "DT.ODA.ALLD.CD",
                "Indicator Name": "Net official development assistance and official aid received (current US$)"
            },
            {
                "Indicator Code": "DT.DOD.DECT.CD",
                "Indicator Name": "External debt stocks, total (DOD, current US$)"
            },
            {
                "Indicator Code": "DT.ODA.ODAT.CD",
                "Indicator Name": "Net official development assistance received (current US$)"
            },
            {
                "Indicator Code": "BX.KLT.DINV.CD.WD",
                "Indicator Name": "Foreign direct investment, net inflows (BoP, current US$)"
            },
            {
                "Indicator Code": "BX.TRF.PWKR.CD.DT",
                "Indicator Name": "Personal transfers and compensation of employees, received (current US$)"
            },
            {
                "Indicator Code": "NY.GDP.MKTP.CD",
                "Indicator Name": "GDP (current US$)"
            },
            {
                "Indicator Code": "BX.PEF.TOTL.CD.WD",
                "Indicator Name": "Portfolio equity, net inflows (BoP, current US$)"
            },
            {
                "Indicator Code": "NY.GDP.MKTP.KD.ZG",
                "Indicator Name": "GDP growth (annual %)"
            },
            {
                "Indicator Code": "GC.REV.XGRT.GD.ZS",
                "Indicator Name": "Revenue, excluding grants (% of GDP)"
            },
            {
                "Indicator Code": "NY.GDP.PCAP.CD",
                "Indicator Name": "GDP per capita (current US$)"
            },
            {
                "Indicator Code": "BM.GSR.ROYL.CD",
                "Indicator Name": "Royalty and license fees, payments (BoP, current US$)"
            },
            {
                "Indicator Code": "NY.GNP.ATLS.CD",
                "Indicator Name": "GNI, Atlas method (current US$)"
            },
            {
                "Indicator Code": "BX.GSR.ROYL.CD",
                "Indicator Name": "Royalty and license fees, receipts (BoP, current US$)"
            },
            {
                "Indicator Code": "NY.GNP.PCAP.CD",
                "Indicator Name": "GNI per capita, Atlas method (current US$)"
            },
            {
                "Indicator Code": "NV.SRV.TETC.ZS",
                "Indicator Name": "Services, etc., value added (% of GDP)"
            },
            {
                "Indicator Code": "NY.GNP.PCAP.PP.CD",
                "Indicator Name": "GNI per capita, PPP (current international $)"
            },
            {
                "Indicator Code": "DT.TDS.DECT.EX.ZS",
                "Indicator Name": "Total debt service (% of exports of goods, services and income)"
            },
            {
                "Indicator Code": "NY.GNP.MKTP.PP.CD",
                "Indicator Name": "GNI, PPP (current international $)"
            },
            {
                "Indicator Code": "FI.RES.TOTL.CD",
                "Indicator Name": "Total reserves (includes gold, current US$)"
            },
            {
                "Indicator Code": "NE.GDI.TOTL.ZS",
                "Indicator Name": "Gross capital formation (% of GDP)"
            },
            {
                "Indicator Code": "BG.GSR.NFSV.GD.ZS",
                "Indicator Name": "Trade in services (% of GDP)"
            },
            {
                "Indicator Code": "NY.GNS.ICTR.ZS",
                "Indicator Name": "Gross savings (% of GDP)"
            },
            {
                "Indicator Code": "DT.DOD.DIMF.CD",
                "Indicator Name": "Use of IMF credit (DOD, current US$)"
            }
        ]
    },
    {
        "id": "edcuation",
        "name": "Education",
        "indicators": [
            {
                "Indicator Code": "SE.PRM.UNER.FE",
                "Indicator Name": "Children out of school, primary, female"
            },
            {
                "Indicator Code": "SE.SEC.PROG.FE.ZS",
                "Indicator Name": "Progression to secondary school, female (%)"
            },
            {
                "Indicator Code": "SE.PRM.UNER.MA",
                "Indicator Name": "Children out of school, primary, male"
            },
            {
                "Indicator Code": "SE.SEC.PROG.MA.ZS",
                "Indicator Name": "Progression to secondary school, male (%)"
            },
            {
                "Indicator Code": "SE.XPD.PRIM.PC.ZS",
                "Indicator Name": "Expenditure per student, primary (% of GDP per capita)"
            },
            {
                "Indicator Code": "SE.XPD.TOTL.GD.ZS",
                "Indicator Name": "Public spending on education, total (% of GDP)"
            },
            {
                "Indicator Code": "SE.XPD.SECO.PC.ZS",
                "Indicator Name": "Expenditure per student, secondary (% of GDP per capita)"
            },
            {
                "Indicator Code": "SE.XPD.TOTL.GB.ZS",
                "Indicator Name": "Public spending on education, total (% of government expenditure)"
            },
            {
                "Indicator Code": "SE.XPD.TERT.PC.ZS",
                "Indicator Name": "Expenditure per student, tertiary (% of GDP per capita)"
            },
            {
                "Indicator Code": "SE.PRM.ENRL.TC.ZS",
                "Indicator Name": "Pupil-teacher ratio, primary"
            },
            {
                "Indicator Code": "SE.PRM.GINT.FE.ZS",
                "Indicator Name": "Gross intake rate in grade 1, female (% of relevant age group)"
            },
            {
                "Indicator Code": "SE.ENR.PRIM.FM.ZS",
                "Indicator Name": "Ratio of female to male primary enrollment (%)"
            },
            {
                "Indicator Code": "SE.PRM.GINT.MA.ZS",
                "Indicator Name": "Gross intake rate in grade 1, male (% of relevant age group)"
            },
            {
                "Indicator Code": "SE.ENR.SECO.FM.ZS",
                "Indicator Name": "Ratio of female to male secondary enrollment (%)"
            },
            {
                "Indicator Code": "SL.TLF.TOTL.IN",
                "Indicator Name": "Labor force, total"
            },
            {
                "Indicator Code": "SE.ENR.TERT.FM.ZS",
                "Indicator Name": "Ratio of female to male tertiary enrollment (%)"
            },
            {
                "Indicator Code": "SE.ADT.LITR.ZS",
                "Indicator Name": "Literacy rate, adult total (% of people ages 15 and above)"
            },
            {
                "Indicator Code": "SE.ENR.PRSC.FM.ZS",
                "Indicator Name": "Ratio of girls to boys in primary and secondary education (%)"
            },
            {
                "Indicator Code": "SE.ADT.1524.LT.FE.ZS",
                "Indicator Name": "Literacy rate, youth female (% of females ages 15-24)"
            },
            {
                "Indicator Code": "SE.PRM.REPT.FE.ZS",
                "Indicator Name": "Repeaters, primary, female (% of female enrollment)"
            },
            {
                "Indicator Code": "SE.ADT.1524.LT.MA.ZS",
                "Indicator Name": "Literacy rate, youth male (% of males ages 15-24)"
            },
            {
                "Indicator Code": "SE.PRM.REPT.MA.ZS",
                "Indicator Name": "Repeaters, primary, male (% of male enrollment)"
            },
            {
                "Indicator Code": "SE.ADT.1524.LT.ZS",
                "Indicator Name": "Literacy rate, youth total (% of people ages 15-24)"
            },
            {
                "Indicator Code": "SE.PRE.ENRR",
                "Indicator Name": "School enrollment, preprimary (% gross)"
            },
            {
                "Indicator Code": "SH.DYN.MORT",
                "Indicator Name": "Mortality rate, under-5 (per 1,000 live births)"
            },
            {
                "Indicator Code": "SE.PRM.ENRR",
                "Indicator Name": "School enrollment, primary (% gross)"
            },
            {
                "Indicator Code": "SE.PRM.PRSL.FE.ZS",
                "Indicator Name": "Persistence to last grade of primary, female (% of cohort)"
            },
            {
                "Indicator Code": "SE.PRM.NENR",
                "Indicator Name": "School enrollment, primary (% net)"
            },
            {
                "Indicator Code": "SE.PRM.PRSL.MA.ZS",
                "Indicator Name": "Persistence to last grade of primary, male (% of cohort)"
            },
            {
                "Indicator Code": "SE.SEC.ENRR",
                "Indicator Name": "School enrollment, secondary (% gross)"
            },
            {
                "Indicator Code": "SP.POP.0014.TO.ZS",
                "Indicator Name": "Population ages 0-14 (% of total)"
            },
            {
                "Indicator Code": "SE.SEC.NENR",
                "Indicator Name": "School enrollment, secondary (% net)"
            },
            {
                "Indicator Code": "SP.POP.1564.TO.ZS",
                "Indicator Name": "Population ages 15-64 (% of total)"
            },
            {
                "Indicator Code": "SE.TER.ENRR",
                "Indicator Name": "School enrollment, tertiary (% gross)"
            },
            {
                "Indicator Code": "SH.DYN.AIDS.ZS",
                "Indicator Name": "Prevalence of HIV, total (% of population ages 15-49)"
            },
            {
                "Indicator Code": "SE.PRM.TCAQ.ZS",
                "Indicator Name": "Trained teachers in primary education (% of total teachers)"
            },
            {
                "Indicator Code": "SE.PRM.CMPT.FE.ZS",
                "Indicator Name": "Primary completion rate, female (% of relevant age group)"
            },
            {
                "Indicator Code": "SL.UEM.TOTL.FE.ZS",
                "Indicator Name": "Unemployment, female (% of female labor force)"
            },
            {
                "Indicator Code": "SE.PRM.CMPT.MA.ZS",
                "Indicator Name": "Primary completion rate, male (% of relevant age group)"
            },
            {
                "Indicator Code": "SL.UEM.TOTL.MA.ZS",
                "Indicator Name": "Unemployment, male (% of male labor force)"
            },
            {
                "Indicator Code": "SE.PRM.CMPT.ZS",
                "Indicator Name": "Primary completion rate, total (% of relevant age group)"
            },
            {
                "Indicator Code": "SL.UEM.TOTL.ZS",
                "Indicator Name": "Unemployment, total (% of total labor force)"
            }
        ]
    },
    {
        "id": "energy-and-mining",
        "name": "Energy & Mining",
        "indicators": [
            {
                "Indicator Code": "EG.USE.COMM.CL.ZS",
                "Indicator Name": "Alternative and nuclear energy (% of total energy use)"
            },
            {
                "Indicator Code": "EG.USE.COMM.KT.OE",
                "Indicator Name": "Energy use (kt of oil equivalent)"
            },
            {
                "Indicator Code": "EG.USE.CRNW.ZS",
                "Indicator Name": "Combustible renewables and waste (% of total energy)"
            },
            {
                "Indicator Code": "EG.USE.COMM.FO.ZS",
                "Indicator Name": "Fossil fuel energy consumption (% of total)"
            },
            {
                "Indicator Code": "EG.IMP.CONS.ZS",
                "Indicator Name": "Energy imports, net (% of energy use)"
            },
            {
                "Indicator Code": "EG.GDP.PUSE.KO.PP.KD",
                "Indicator Name": "GDP per unit of energy use (constant 2005 PPP $ per kg of oil equivalent)"
            },
            {
                "Indicator Code": "EG.EGY.PROD.KT.OE",
                "Indicator Name": "Energy production (kt of oil equivalent)"
            },
            {
                "Indicator Code": "EP.PMP.DESL.CD",
                "Indicator Name": "Pump price for diesel fuel (US$ per liter)"
            },
            {
                "Indicator Code": "EG.USE.PCAP.KG.OE",
                "Indicator Name": "Energy use (kg of oil equivalent per capita)"
            },
            {
                "Indicator Code": "EP.PMP.SGAS.CD",
                "Indicator Name": "Pump price for gasoline (US$ per liter)"
            }
        ]
    },
    {
        "id": "environment",
        "name": "Environment",
        "indicators": [
            {
                "Indicator Code": "EG.ELC.ACCS.ZS",
                "Indicator Name": "Access to electricity (% of population)"
            },
            {
                "Indicator Code": "EE.BOD.TOTL.KG",
                "Indicator Name": "Organic water pollutant (BOD) emissions (kg per day)"
            },
            {
                "Indicator Code": "EN.ATM.METH.AG.ZS",
                "Indicator Name": "Agricultural methane emissions (% of total)"
            },
            {
                "Indicator Code": "EE.BOD.WRKR.KG",
                "Indicator Name": "Organic water pollutant (BOD) emissions (kg per day per worker)"
            },
            {
                "Indicator Code": "EN.ATM.NOXE.AG.ZS",
                "Indicator Name": "Agricultural nitrous oxide emissions (% of total)"
            },
            {
                "Indicator Code": "EN.ATM.GHGO.KT.CE",
                "Indicator Name": "Other greenhouse gas emissions, HFC, PFC and SF6 (thousand metric tons of CO2 equivalent)"
            },
            {
                "Indicator Code": "EN.ATM.CO2E.KT",
                "Indicator Name": "CO2 emissions (kt)"
            },
            {
                "Indicator Code": "EN.HPT.THRD.NO",
                "Indicator Name": "Plant species (higher), threatened"
            },
            {
                "Indicator Code": "EN.ATM.CO2E.PC",
                "Indicator Name": "CO2 emissions (metric tons per capita)"
            },
            {
                "Indicator Code": "EN.POP.EL5M.ZS",
                "Indicator Name": "Population living in areas where elevation is below 5 meters (% of total population)"
            },
            {
                "Indicator Code": "EN.FSH.THRD.NO",
                "Indicator Name": "Fish species, threatened"
            },
            {
                "Indicator Code": "EE.BOD.CHEM.ZS",
                "Indicator Name": "Water pollution, chemical industry (% of total BOD emissions)"
            },
            {
                "Indicator Code": "AG.LND.FRST.ZS",
                "Indicator Name": "Forest area (% of land area)"
            },
            {
                "Indicator Code": "EE.BOD.CGLS.ZS",
                "Indicator Name": "Water pollution, clay and glass industry (% of total BOD emissions)"
            },
            {
                "Indicator Code": "AG.LND.FRST.K2",
                "Indicator Name": "Forest area (sq. km)"
            },
            {
                "Indicator Code": "EE.BOD.FOOD.ZS",
                "Indicator Name": "Water pollution, food industry (% of total BOD emissions)"
            },
            {
                "Indicator Code": "ER.BDV.TOTL.XQ",
                "Indicator Name": "GEF benefits index for biodiversity (0 = no biodiversity potential to 100 = maximum)"
            },
            {
                "Indicator Code": "EE.BOD.MTAL.ZS",
                "Indicator Name": "Water pollution, metal industry (% of total BOD emissions)"
            },
            {
                "Indicator Code": "AG.LND.EL5M.ZS",
                "Indicator Name": "Land area where elevation is below 5 meters (% of total land area)"
            },
            {
                "Indicator Code": "EE.BOD.OTHR.ZS",
                "Indicator Name": "Water pollution, other industry (% of total BOD emissions)"
            },
            {
                "Indicator Code": "EN.MAM.THRD.NO",
                "Indicator Name": "Mammal species, threatened"
            },
            {
                "Indicator Code": "EE.BOD.PAPR.ZS",
                "Indicator Name": "Water pollution, paper and pulp industry (% of total BOD emissions)"
            },
            {
                "Indicator Code": "ER.MRN.PTMR.ZS",
                "Indicator Name": "Marine protected areas (% of territorial waters)"
            },
            {
                "Indicator Code": "EE.BOD.TXTL.ZS",
                "Indicator Name": "Water pollution, textile industry (% of total BOD emissions)"
            },
            {
                "Indicator Code": "EN.ATM.METH.KT.CE",
                "Indicator Name": "Methane emissions (kt of CO2 equivalent)"
            },
            {
                "Indicator Code": "EE.BOD.WOOD.ZS",
                "Indicator Name": "Water pollution, wood industry (% of total BOD emissions)"
            },
            {
                "Indicator Code": "EN.ATM.NOXE.KT.CE",
                "Indicator Name": "Nitrous oxide emissions (thousand metric tons of CO2 equivalent)"
            }
        ]
    },
    {
        "id": "financial-sector",
        "name": "Financial Sector",
        "indicators": [
            {
                "Indicator Code": "FB.BNK.CAPA.ZS",
                "Indicator Name": "Bank capital to assets ratio (%)"
            },
            {
                "Indicator Code": "FM.LBL.MQMY.ZG",
                "Indicator Name": "Money and quasi money growth (annual %)"
            },
            {
                "Indicator Code": "FB.AST.NPER.ZS",
                "Indicator Name": "Bank nonperforming loans to total gross loans (%)"
            },
            {
                "Indicator Code": "SM.POP.NETM",
                "Indicator Name": "Net migration"
            },
            {
                "Indicator Code": "FM.AST.CGOV.ZG.M3",
                "Indicator Name": "Claims on central government (annual growth as % of broad money)"
            },
            {
                "Indicator Code": "BX.TRF.PWKR.CD.DT",
                "Indicator Name": "Personal transfers and compensation of employees, received (current US$)"
            },
            {
                "Indicator Code": "FM.AST.DOMO.ZG.M3",
                "Indicator Name": "Claims on other sectors of the domestic economy (annual growth as % of broad money)"
            },
            {
                "Indicator Code": "BX.PEF.TOTL.CD.WD",
                "Indicator Name": "Portfolio equity, net inflows (BoP, current US$)"
            },
            {
                "Indicator Code": "IC.CRD.INFO.XQ",
                "Indicator Name": "Credit depth of information index (0=low to 6=high)"
            },
            {
                "Indicator Code": "IC.CRD.PRVT.ZS",
                "Indicator Name": "Private credit bureau coverage (% of adults)"
            },
            {
                "Indicator Code": "FR.INR.DPST",
                "Indicator Name": "Deposit interest rate (%)"
            },
            {
                "Indicator Code": "IC.CRD.PUBL.ZS",
                "Indicator Name": "Public credit registry coverage (% of adults)"
            },
            {
                "Indicator Code": "FS.AST.DOMS.GD.ZS",
                "Indicator Name": "Domestic credit provided by banking sector (% of GDP)"
            },
            {
                "Indicator Code": "FR.INR.RINR",
                "Indicator Name": "Real interest rate (%)"
            },
            {
                "Indicator Code": "BX.KLT.DINV.CD.WD",
                "Indicator Name": "Foreign direct investment, net inflows (BoP, current US$)"
            },
            {
                "Indicator Code": "FR.INR.RISK",
                "Indicator Name": "Risk premium on lending (lending rate minus treasury bill rate, %)"
            },
            {
                "Indicator Code": "FR.INR.LNDP",
                "Indicator Name": "Interest rate spread (lending rate minus deposit rate, %)"
            },
            {
                "Indicator Code": "CM.MKT.INDX.ZG",
                "Indicator Name": "S&P Global Equity Indices (annual % change)"
            },
            {
                "Indicator Code": "SM.POP.TOTL",
                "Indicator Name": "International migrant stock, total"
            },
            {
                "Indicator Code": "CM.MKT.TRAD.GD.ZS",
                "Indicator Name": "Stocks traded, total value (% of GDP)"
            },
            {
                "Indicator Code": "FR.INR.LEND",
                "Indicator Name": "Lending interest rate (%)"
            },
            {
                "Indicator Code": "CM.MKT.TRNR",
                "Indicator Name": "Stocks traded, turnover ratio (%)"
            },
            {
                "Indicator Code": "CM.MKT.LDOM.NO",
                "Indicator Name": "Listed domestic companies, total"
            },
            {
                "Indicator Code": "IC.LGL.CRED.XQ",
                "Indicator Name": "Strength of legal rights index (0=weak to 10=strong)"
            },
            {
                "Indicator Code": "CM.MKT.LCAP.CD",
                "Indicator Name": "Market capitalization of listed companies (current US$)"
            },
            {
                "Indicator Code": "FI.RES.TOTL.CD",
                "Indicator Name": "Total reserves (includes gold, current US$)"
            },
            {
                "Indicator Code": "CM.MKT.LCAP.GD.ZS",
                "Indicator Name": "Market capitalization of listed companies (% of GDP)"
            }
        ]
    },
    {
        "id": "gender",
        "name": "Gender",
        "indicators": [
            {
                "Indicator Code": "SP.ADO.TFRT",
                "Indicator Name": "Adolescent fertility rate (births per 1,000 women ages 15-19)"
            },
            {
                "Indicator Code": "SL.UEM.LTRM.FE.ZS",
                "Indicator Name": "Long-term unemployment, female (% of female unemployment)"
            },
            {
                "Indicator Code": "SH.STA.BRTC.ZS",
                "Indicator Name": "Births attended by skilled health staff (% of total)"
            },
            {
                "Indicator Code": "SL.UEM.LTRM.MA.ZS",
                "Indicator Name": "Long-term unemployment, male (% of male unemployment)"
            },
            {
                "Indicator Code": "SL.TLF.0714.FE.ZS",
                "Indicator Name": "Children in employment, female (% of female children ages 7-14)"
            },
            {
                "Indicator Code": "SH.STA.MMRT",
                "Indicator Name": "Maternal mortality ratio (modeled estimate, per 100,000 live births)"
            },
            {
                "Indicator Code": "SL.TLF.0714.MA.ZS",
                "Indicator Name": "Children in employment, male (% of male children ages 7-14)"
            },
            {
                "Indicator Code": "SE.PRM.PRSL.FE.ZS",
                "Indicator Name": "Persistence to last grade of primary, female (% of cohort)"
            },
            {
                "Indicator Code": "SL.TLF.0714.SW.FE.ZS",
                "Indicator Name": "Children in employment, study and work, female (% of female children in employment, ages 7-14)"
            },
            {
                "Indicator Code": "SE.PRM.PRSL.MA.ZS",
                "Indicator Name": "Persistence to last grade of primary, male (% of cohort)"
            },
            {
                "Indicator Code": "SL.TLF.0714.SW.MA.ZS",
                "Indicator Name": "Children in employment, study and work, male (% of male children in employment, ages 7-14)"
            },
            {
                "Indicator Code": "SH.STA.ANVC.ZS",
                "Indicator Name": "Pregnant women receiving prenatal care (%)"
            },
            {
                "Indicator Code": "SL.TLF.0714.WK.FE.ZS",
                "Indicator Name": "Children in employment, work only, female (% of female children in employment, ages 7-14)"
            },
            {
                "Indicator Code": "SH.HIV.1524.FE.ZS",
                "Indicator Name": "Prevalence of HIV, female (% ages 15-24)"
            },
            {
                "Indicator Code": "SL.TLF.0714.WK.MA.ZS",
                "Indicator Name": "Children in employment, work only, male (% of male children in employment, ages 7-14)"
            },
            {
                "Indicator Code": "SH.HIV.1524.MA.ZS",
                "Indicator Name": "Prevalence of HIV, male (% ages 15-24)"
            },
            {
                "Indicator Code": "SE.PRM.UNER.FE",
                "Indicator Name": "Children out of school, primary, female"
            },
            {
                "Indicator Code": "SE.PRM.CMPT.FE.ZS",
                "Indicator Name": "Primary completion rate, female (% of relevant age group)"
            },
            {
                "Indicator Code": "SE.PRM.UNER.MA",
                "Indicator Name": "Children out of school, primary, male"
            },
            {
                "Indicator Code": "SE.PRM.CMPT.MA.ZS",
                "Indicator Name": "Primary completion rate, male (% of relevant age group)"
            },
            {
                "Indicator Code": "SP.DYN.CONU.ZS",
                "Indicator Name": "Contraceptive prevalence (% of women ages 15-49)"
            },
            {
                "Indicator Code": "SE.SEC.PROG.FE.ZS",
                "Indicator Name": "Progression to secondary school, female (%)"
            },
            {
                "Indicator Code": "SL.AGR.EMPL.FE.ZS",
                "Indicator Name": "Employees, agriculture, female (% of female employment)"
            },
            {
                "Indicator Code": "SE.SEC.PROG.MA.ZS",
                "Indicator Name": "Progression to secondary school, male (%)"
            },
            {
                "Indicator Code": "SL.AGR.EMPL.MA.ZS",
                "Indicator Name": "Employees, agriculture, male (% of male employment)"
            },
            {
                "Indicator Code": "SG.GEN.PARL.ZS",
                "Indicator Name": "Proportion of seats held by women in national parliaments (%)"
            },
            {
                "Indicator Code": "SL.IND.EMPL.FE.ZS",
                "Indicator Name": "Employees, industry, female (% of female employment)"
            },
            {
                "Indicator Code": "SE.ENR.PRIM.FM.ZS",
                "Indicator Name": "Ratio of female to male primary enrollment (%)"
            },
            {
                "Indicator Code": "SL.IND.EMPL.MA.ZS",
                "Indicator Name": "Employees, industry, male (% of male employment)"
            },
            {
                "Indicator Code": "SE.ENR.SECO.FM.ZS",
                "Indicator Name": "Ratio of female to male secondary enrollment (%)"
            },
            {
                "Indicator Code": "SL.SRV.EMPL.FE.ZS",
                "Indicator Name": "Employees, services, female (% of female employment)"
            },
            {
                "Indicator Code": "SE.ENR.TERT.FM.ZS",
                "Indicator Name": "Ratio of female to male tertiary enrollment (%)"
            },
            {
                "Indicator Code": "SL.SRV.EMPL.MA.ZS",
                "Indicator Name": "Employees, services, male (% of male employment)"
            },
            {
                "Indicator Code": "SE.ENR.PRSC.FM.ZS",
                "Indicator Name": "Ratio of girls to boys in primary and secondary education (%)"
            },
            {
                "Indicator Code": "SP.DYN.TFRT.IN",
                "Indicator Name": "Fertility rate, total (births per woman)"
            },
            {
                "Indicator Code": "SE.PRM.REPT.FE.ZS",
                "Indicator Name": "Repeaters, primary, female (% of female enrollment)"
            },
            {
                "Indicator Code": "SE.PRM.GINT.FE.ZS",
                "Indicator Name": "Gross intake rate in grade 1, female (% of relevant age group)"
            },
            {
                "Indicator Code": "SE.PRM.REPT.MA.ZS",
                "Indicator Name": "Repeaters, primary, male (% of male enrollment)"
            },
            {
                "Indicator Code": "SE.PRM.GINT.MA.ZS",
                "Indicator Name": "Gross intake rate in grade 1, male (% of relevant age group)"
            },
            {
                "Indicator Code": "SL.EMP.INSV.FE.ZS",
                "Indicator Name": "Share of women employed in the nonagricultural sector (% of total nonagricultural employment)"
            },
            {
                "Indicator Code": "SL.TLF.CACT.FE.ZS",
                "Indicator Name": "Labor participation rate, female (% of female population ages 15+)"
            },
            {
                "Indicator Code": "SP.MTR.1519.ZS",
                "Indicator Name": "Teenage mothers (% of women ages 15-19 who have had children or are currently pregnant)"
            },
            {
                "Indicator Code": "SL.TLF.CACT.MA.ZS",
                "Indicator Name": "Labor participation rate, male (% of male population ages 15+)"
            },
            {
                "Indicator Code": "SL.UEM.TOTL.FE.ZS",
                "Indicator Name": "Unemployment, female (% of female labor force)"
            },
            {
                "Indicator Code": "SL.TLF.CACT.ZS",
                "Indicator Name": "Labor participation rate, total (% of total population ages 15+)"
            },
            {
                "Indicator Code": "SL.UEM.TOTL.MA.ZS",
                "Indicator Name": "Unemployment, male (% of male labor force)"
            },
            {
                "Indicator Code": "SP.DYN.LE00.FE.IN",
                "Indicator Name": "Life expectancy at birth, female (years)"
            },
            {
                "Indicator Code": "SL.UEM.1524.FE.ZS",
                "Indicator Name": "Unemployment, youth female (% of female labor force ages 15-24)"
            },
            {
                "Indicator Code": "SP.DYN.LE00.MA.IN",
                "Indicator Name": "Life expectancy at birth, male (years)"
            },
            {
                "Indicator Code": "SL.UEM.1524.MA.ZS",
                "Indicator Name": "Unemployment, youth male (% of male labor force ages 15-24)"
            },
            {
                "Indicator Code": "SE.ADT.1524.LT.FE.ZS",
                "Indicator Name": "Literacy rate, youth female (% of females ages 15-24)"
            },
            {
                "Indicator Code": "SL.EMP.VULN.FE.ZS",
                "Indicator Name": "Vulnerable employment, female (% of female employment)"
            },
            {
                "Indicator Code": "SE.ADT.1524.LT.MA.ZS",
                "Indicator Name": "Literacy rate, youth male (% of males ages 15-24)"
            },
            {
                "Indicator Code": "SL.EMP.VULN.MA.ZS",
                "Indicator Name": "Vulnerable employment, male (% of male employment)"
            }
        ]
    },
    {
        "id": "health",
        "name": "Health",
        "indicators": [
            {
                "Indicator Code": "SP.ADO.TFRT",
                "Indicator Name": "Adolescent fertility rate (births per 1,000 women ages 15-19)"
            },
            {
                "Indicator Code": "SH.STA.MALN.ZS",
                "Indicator Name": "Malnutrition prevalence, weight for age (% of children under 5)"
            },
            {
                "Indicator Code": "SP.DYN.CBRT.IN",
                "Indicator Name": "Birth rate, crude (per 1,000 people)"
            },
            {
                "Indicator Code": "SH.STA.MMRT",
                "Indicator Name": "Maternal mortality ratio (modeled estimate, per 100,000 live births)"
            },
            {
                "Indicator Code": "SH.STA.BRTC.ZS",
                "Indicator Name": "Births attended by skilled health staff (% of total)"
            },
            {
                "Indicator Code": "SP.DYN.IMRT.IN",
                "Indicator Name": "Mortality rate, infant (per 1,000 live births)"
            },
            {
                "Indicator Code": "SP.DYN.CONU.ZS",
                "Indicator Name": "Contraceptive prevalence (% of women ages 15-49)"
            },
            {
                "Indicator Code": "SH.DYN.MORT",
                "Indicator Name": "Mortality rate, under-5 (per 1,000 live births)"
            },
            {
                "Indicator Code": "SP.DYN.CDRT.IN",
                "Indicator Name": "Death rate, crude (per 1,000 people)"
            },
            {
                "Indicator Code": "SH.MLR.INCD",
                "Indicator Name": "Notified cases of malaria (per 100,000 people)"
            },
            {
                "Indicator Code": "SP.DYN.TFRT.IN",
                "Indicator Name": "Fertility rate, total (births per woman)"
            },
            {
                "Indicator Code": "SH.XPD.OOPC.ZS",
                "Indicator Name": "Out-of-pocket health expenditure (% of private expenditure on health)"
            },
            {
                "Indicator Code": "SH.XPD.PCAP",
                "Indicator Name": "Health expenditure per capita (current US$)"
            },
            {
                "Indicator Code": "SP.POP.0014.TO.ZS",
                "Indicator Name": "Population ages 0-14 (% of total)"
            },
            {
                "Indicator Code": "SH.XPD.PUBL",
                "Indicator Name": "Health expenditure, public (% of total health expenditure)"
            },
            {
                "Indicator Code": "SP.POP.1564.TO.ZS",
                "Indicator Name": "Population ages 15-64 (% of total)"
            },
            {
                "Indicator Code": "SH.XPD.TOTL.ZS",
                "Indicator Name": "Health expenditure, total (% of GDP)"
            },
            {
                "Indicator Code": "SP.POP.65UP.TO.ZS",
                "Indicator Name": "Population ages 65 and above (% of total)"
            },
            {
                "Indicator Code": "SH.IMM.IDPT",
                "Indicator Name": "Immunization, DPT (% of children ages 12-23 months)"
            },
            {
                "Indicator Code": "SP.POP.TOTL.FE.ZS",
                "Indicator Name": "Population, female (% of total)"
            },
            {
                "Indicator Code": "SH.IMM.MEAS",
                "Indicator Name": "Immunization, measles (% of children ages 12-23 months)"
            },
            {
                "Indicator Code": "SP.POP.GROW",
                "Indicator Name": "Population growth (annual %)"
            },
            {
                "Indicator Code": "SH.STA.ACSN",
                "Indicator Name": "Improved sanitation facilities (% of population with access)"
            },
            {
                "Indicator Code": "SP.POP.TOTL",
                "Indicator Name": "Population, total"
            },
            {
                "Indicator Code": "SH.STA.ACSN.UR",
                "Indicator Name": "Improved sanitation facilities, urban (% of urban population with access)"
            },
            {
                "Indicator Code": "SH.STA.ANVC.ZS",
                "Indicator Name": "Pregnant women receiving prenatal care (%)"
            },
            {
                "Indicator Code": "SH.TBS.INCD",
                "Indicator Name": "Incidence of tuberculosis (per 100,000 people)"
            },
            {
                "Indicator Code": "SH.HIV.1524.FE.ZS",
                "Indicator Name": "Prevalence of HIV, female (% ages 15-24)"
            },
            {
                "Indicator Code": "SP.DYN.LE00.FE.IN",
                "Indicator Name": "Life expectancy at birth, female (years)"
            },
            {
                "Indicator Code": "SH.HIV.1524.MA.ZS",
                "Indicator Name": "Prevalence of HIV, male (% ages 15-24)"
            },
            {
                "Indicator Code": "SP.DYN.LE00.MA.IN",
                "Indicator Name": "Life expectancy at birth, male (years)"
            },
            {
                "Indicator Code": "SH.DYN.AIDS.ZS",
                "Indicator Name": "Prevalence of HIV, total (% of population ages 15-49)"
            },
            {
                "Indicator Code": "SP.DYN.LE00.IN",
                "Indicator Name": "Life expectancy at birth, total (years)"
            },
            {
                "Indicator Code": "SP.MTR.1519.ZS",
                "Indicator Name": "Teenage mothers (% of women ages 15-19 who have had children or are currently pregnant)"
            },
            {
                "Indicator Code": "SH.STA.STNT.ZS",
                "Indicator Name": "Malnutrition prevalence, height for age (% of children under 5)"
            },
            {
                "Indicator Code": "SP.UWT.TFRT",
                "Indicator Name": "Unmet need for contraception (% of married women ages 15-49)"
            }
        ]
    },
    {
        "id": "infrastructure",
        "name": "Infrastructure",
        "indicators": [
            {
                "Indicator Code": "IS.AIR.DPRT",
                "Indicator Name": "Air transport, registered carrier departures worldwide"
            },
            {
                "Indicator Code": "IT.NET.USER.P2",
                "Indicator Name": "Internet users (per 100 people)"
            },
            {
                "Indicator Code": "ER.H2O.FWAG.ZS",
                "Indicator Name": "Annual freshwater withdrawals, agriculture (% of total freshwater withdrawal)"
            },
            {
                "Indicator Code": "ER.H2O.FWDM.ZS",
                "Indicator Name": "Annual freshwater withdrawals, domestic (% of total freshwater withdrawal)"
            },
            {
                "Indicator Code": "IT.CEL.SETS.P2",
                "Indicator Name": "Mobile cellular subscriptions (per 100 people)"
            },
            {
                "Indicator Code": "ER.H2O.FWIN.ZS",
                "Indicator Name": "Annual freshwater withdrawals, industry (% of total freshwater withdrawal)"
            },
            {
                "Indicator Code": "IS.VEH.NVEH.P3",
                "Indicator Name": "Motor vehicles (per 1,000 people)"
            },
            {
                "Indicator Code": "ER.H2O.FWTL.K3",
                "Indicator Name": "Annual freshwater withdrawals, total (billion cubic meters)"
            },
            {
                "Indicator Code": "IS.VEH.PCAR.P3",
                "Indicator Name": "Passenger cars (per 1,000 people)"
            },
            {
                "Indicator Code": "IS.SHP.GOOD.TU",
                "Indicator Name": "Container port traffic (TEU: 20 foot equivalent units)"
            },
            {
                "Indicator Code": "IS.RRS.TOTL.KM",
                "Indicator Name": "Rail lines (total route-km)"
            },
            {
                "Indicator Code": "IT.PRT.NEWS.P3",
                "Indicator Name": "Daily newspapers (per 1,000 people)"
            },
            {
                "Indicator Code": "ER.H2O.INTR.PC",
                "Indicator Name": "Renewable internal freshwater resources per capita (cubic meters)"
            },
            {
                "Indicator Code": "EG.USE.ELEC.KH.PC",
                "Indicator Name": "Electric power consumption (kWh per capita)"
            },
            {
                "Indicator Code": "ER.H2O.INTR.K3",
                "Indicator Name": "Renewable internal freshwater resources, total (billion cubic meters)"
            },
            {
                "Indicator Code": "IT.NET.BBND.P2",
                "Indicator Name": "Fixed broadband Internet subscribers (per 100 people)"
            },
            {
                "Indicator Code": "IS.ROD.DESL.PC",
                "Indicator Name": "Road sector diesel fuel consumption per capita (kg of oil equivalent)"
            },
            {
                "Indicator Code": "TX.VAL.ICTG.ZS.UN",
                "Indicator Name": "ICT goods exports (% of total goods exports)"
            },
            {
                "Indicator Code": "IS.ROD.ENGY.ZS",
                "Indicator Name": "Road sector energy consumption (% of total energy consumption)"
            },
            {
                "Indicator Code": "TM.VAL.ICTG.ZS.UN",
                "Indicator Name": "ICT goods imports (% total goods imports)"
            },
            {
                "Indicator Code": "IS.ROD.SGAS.PC",
                "Indicator Name": "Road sector gasoline fuel consumption per capita (kg of oil equivalent)"
            },
            {
                "Indicator Code": "BX.GSR.CCIS.ZS",
                "Indicator Name": "ICT service exports (% of service exports, BoP)"
            },
            {
                "Indicator Code": "IS.ROD.PAVE.ZS",
                "Indicator Name": "Roads, paved (% of total roads)"
            },
            {
                "Indicator Code": "SH.H2O.SAFE.RU.ZS",
                "Indicator Name": "Improved water source, rural (% of rural population with access)"
            },
            {
                "Indicator Code": "IT.NET.SECR.P6",
                "Indicator Name": "Secure Internet servers (per 1 million people)"
            },
            {
                "Indicator Code": "SH.H2O.SAFE.UR.ZS",
                "Indicator Name": "Improved water source, urban (% of urban population with access)"
            },
            {
                "Indicator Code": "IS.VEH.ROAD.K1",
                "Indicator Name": "Vehicles (per km of road)"
            }
        ]
    },
    {
        "id": "labor-and-social-protection",
        "name": "Labor & Social Protection",
        "indicators": [
            {
                "Indicator Code": "SL.AGR.EMPL.FE.ZS",
                "Indicator Name": "Employees, agriculture, female (% of female employment)"
            },
            {
                "Indicator Code": "SL.TLF.CACT.ZS",
                "Indicator Name": "Labor participation rate, total (% of total population ages 15+)"
            },
            {
                "Indicator Code": "SL.AGR.EMPL.MA.ZS",
                "Indicator Name": "Employees, agriculture, male (% of male employment)"
            },
            {
                "Indicator Code": "SL.UEM.LTRM.FE.ZS",
                "Indicator Name": "Long-term unemployment, female (% of female unemployment)"
            },
            {
                "Indicator Code": "SL.IND.EMPL.FE.ZS",
                "Indicator Name": "Employees, industry, female (% of female employment)"
            },
            {
                "Indicator Code": "SL.UEM.LTRM.MA.ZS",
                "Indicator Name": "Long-term unemployment, male (% of male unemployment)"
            },
            {
                "Indicator Code": "SL.IND.EMPL.MA.ZS",
                "Indicator Name": "Employees, industry, male (% of male employment)"
            },
            {
                "Indicator Code": "SL.UEM.LTRM.ZS",
                "Indicator Name": "Long-term unemployment (% of total unemployment)"
            },
            {
                "Indicator Code": "SL.SRV.EMPL.FE.ZS",
                "Indicator Name": "Employees, services, female (% of female employment)"
            },
            {
                "Indicator Code": "SL.UEM.TOTL.FE.ZS",
                "Indicator Name": "Unemployment, female (% of female labor force)"
            },
            {
                "Indicator Code": "SL.SRV.EMPL.MA.ZS",
                "Indicator Name": "Employees, services, male (% of male employment)"
            },
            {
                "Indicator Code": "SL.UEM.TOTL.MA.ZS",
                "Indicator Name": "Unemployment, male (% of male labor force)"
            },
            {
                "Indicator Code": "SL.AGR.EMPL.ZS",
                "Indicator Name": "Employment in agriculture (% of total employment)"
            },
            {
                "Indicator Code": "SL.UEM.TOTL.ZS",
                "Indicator Name": "Unemployment, total (% of total labor force)"
            },
            {
                "Indicator Code": "SL.EMP.TOTL.SP.ZS",
                "Indicator Name": "Employment to population ratio, 15+, total (%)"
            },
            {
                "Indicator Code": "SL.UEM.1524.FE.ZS",
                "Indicator Name": "Unemployment, youth female (% of female labor force ages 15-24)"
            },
            {
                "Indicator Code": "SL.GDP.PCAP.EM.KD",
                "Indicator Name": "GDP per person employed (constant 1990 PPP $)"
            },
            {
                "Indicator Code": "SL.UEM.1524.MA.ZS",
                "Indicator Name": "Unemployment, youth male (% of male labor force ages 15-24)"
            },
            {
                "Indicator Code": "SL.TLF.TOTL.IN",
                "Indicator Name": "Labor force, total"
            },
            {
                "Indicator Code": "SL.EMP.VULN.FE.ZS",
                "Indicator Name": "Vulnerable employment, female (% of female employment)"
            },
            {
                "Indicator Code": "SL.TLF.CACT.FE.ZS",
                "Indicator Name": "Labor participation rate, female (% of female population ages 15+)"
            },
            {
                "Indicator Code": "SL.EMP.VULN.MA.ZS",
                "Indicator Name": "Vulnerable employment, male (% of male employment)"
            },
            {
                "Indicator Code": "SL.TLF.CACT.MA.ZS",
                "Indicator Name": "Labor participation rate, male (% of male population ages 15+)"
            },
            {
                "Indicator Code": "SL.EMP.VULN.ZS",
                "Indicator Name": "Vulnerable employment, total (% of total employment)"
            }
        ]
    },
    {
        'id': 'poverty',
        'name': 'poverty',
        'indicators': [
            {
                'Indicator Name': 'Income share held by highest 10%',
                'Indicator Code': 'SI.DST.10TH.10'
            },
            {
                'Indicator Name': 'Income share held by highest 20%',
                'Indicator Code': 'SI.DST.05TH.20'
            },
            {
                'Indicator Name': 'Income share held by lowest 10%',
                'Indicator Code': 'SI.DST.FRST.10'
            },
            {
                'Indicator Name': 'Income share held by lowest 20%',
                'Indicator Code': 'SI.DST.FRST.20'
            },
            {
                'Indicator Name': 'Income share held by second 20%',
                'Indicator Code': 'SI.DST.02ND.20'
            },
            {
                'Indicator Name': 'Income share held by third 20%',
                'Indicator Code': 'SI.DST.03RD.20'
            },
            {
                'Indicator Name': 'Income share held by fourth 20%',
                'Indicator Code': 'SI.DST.04TH.20'
            },
            {
                'Indicator Name': 'Poverty gap at national poverty line (%)',
                'Indicator Code': 'SI.POV.NAGP'
            },
            {
                'Indicator Name': 'Poverty gap at rural poverty line (%)',
                'Indicator Code': 'SI.POV.RUGP'
            },
            {
                'Indicator Name': 'Poverty gap at urban poverty line (%)',
                'Indicator Code': 'SI.POV.URGP'
            },
            {
                'Indicator Name': 'Poverty headcount ratio at $1.25 a day (PPP) (% of population)',
                'Indicator Code': 'SI.POV.DDAY'
            },
            {
                'Indicator Name': 'Poverty headcount ratio at $2 a day (PPP) (% of population)',
                'Indicator Code': 'SI.POV.2DAY'
            },
            {
                'Indicator Name': 'Poverty headcount ratio at national poverty line (% of population)',
                'Indicator Code': 'SI.POV.NAHC'
            },
            {
                'Indicator Name': 'Poverty headcount ratio at rural poverty line (% of rural population)',
                'Indicator Code': 'SI.POV.RUHC'
            },
            {
                'Indicator Name': 'Poverty headcount ratio at urban poverty line (% of urban population)',
                'Indicator Code': 'SI.POV.URHC'
            },
            {
                'Indicator Name': 'Poverty gap at $1.25 a day (PPP) (%)',
                'Indicator Code': 'SI.POV.GAPS'
            },
            {
                'Indicator Name': 'Poverty gap at $2 a day (PPP) (%)',
                'Indicator Code': 'SI.POV.GAP2'
            }
        ]
    },
    {
        "id": "private-sector",
        "name": "Private Sector",
        "indicators": [
            {
                "Indicator Code": "IC.TAX.METG",
                "Indicator Name": "Average number of times firms spent in meetings with tax officials"
            },
            {
                "Indicator Code": "IC.FRM.ISOC.ZS",
                "Indicator Name": "ISO certification ownership (% of firms)"
            },
            {
                "Indicator Code": "IC.EXP.DOCS",
                "Indicator Name": "Documents to export (number)"
            },
            {
                "Indicator Code": "LP.EXP.DURS.MD",
                "Indicator Name": "Lead time to export, median case (days)"
            },
            {
                "Indicator Code": "IC.IMP.DOCS",
                "Indicator Name": "Documents to import (number)"
            },
            {
                "Indicator Code": "LP.IMP.DURS.MD",
                "Indicator Name": "Lead time to import, median case (days)"
            },
            {
                "Indicator Code": "FS.AST.PRVT.GD.ZS",
                "Indicator Name": "Domestic credit to private sector (% of GDP)"
            },
            {
                "Indicator Code": "LP.LPI.OVRL.XQ",
                "Indicator Name": "Logistics performance index: Overall (1=low to 5=high)"
            },
            {
                "Indicator Code": "IC.BUS.EASE.XQ",
                "Indicator Name": "Ease of doing business index (1=most business-friendly regulations)"
            },
            {
                "Indicator Code": "TG.VAL.TOTL.GD.ZS",
                "Indicator Name": "Merchandise trade (% of GDP)"
            },
            {
                "Indicator Code": "TX.VAL.MRCH.XD.WD",
                "Indicator Name": "Export value index (2000 = 100)"
            },
            {
                "Indicator Code": "TT.PRI.MRCH.XD.WD",
                "Indicator Name": "Net barter terms of trade index (2000 = 100)"
            },
            {
                "Indicator Code": "TX.QTY.MRCH.XD.WD",
                "Indicator Name": "Export volume index (2000 = 100)"
            },
            {
                "Indicator Code": "IC.BUS.NREG",
                "Indicator Name": "New businesses registered (number)"
            },
            {
                "Indicator Code": "IC.FRM.BNKS.ZS",
                "Indicator Name": "Firms using banks to finance investment (% of firms)"
            },
            {
                "Indicator Code": "IC.REG.PROC",
                "Indicator Name": "Start-up procedures to register a business (number)"
            },
            {
                "Indicator Code": "TM.VAL.MRCH.XD.WD",
                "Indicator Name": "Import value index (2000 = 100)"
            },
            {
                "Indicator Code": "IC.TAX.PAYM",
                "Indicator Name": "Tax payments (number)"
            },
            {
                "Indicator Code": "TM.QTY.MRCH.XD.WD",
                "Indicator Name": "Import volume index (2000 = 100)"
            },
            {
                "Indicator Code": "IC.REG.DURS",
                "Indicator Name": "Time required to start a business (days)"
            },
            {
                "Indicator Code": "IC.FRM.CORR.ZS",
                "Indicator Name": "Informal payments to public officials (% of firms)"
            },
            {
                "Indicator Code": "IC.TAX.DURS",
                "Indicator Name": "Time to prepare and pay taxes (hours)"
            },
            {
                "Indicator Code": "IE.PPI.ENGY.CD",
                "Indicator Name": "Investment in energy with private participation (current US$)"
            },
            {
                "Indicator Code": "IC.ISV.DURS",
                "Indicator Name": "Time to resolve insolvency (years)"
            },
            {
                "Indicator Code": "IE.PPI.TELE.CD",
                "Indicator Name": "Investment in telecoms with private participation (current US$)"
            },
            {
                "Indicator Code": "IC.TAX.TOTL.CP.ZS",
                "Indicator Name": "Total tax rate (% of commercial profits)"
            },
            {
                "Indicator Code": "IE.PPI.TRAN.CD",
                "Indicator Name": "Investment in transport with private participation (current US$)"
            },
            {
                "Indicator Code": "BG.GSR.NFSV.GD.ZS",
                "Indicator Name": "Trade in services (% of GDP)"
            },
            {
                "Indicator Code": "IE.PPI.WATR.CD",
                "Indicator Name": "Investment in water and sanitation with private participation (current US$)"
            },
            {
                "Indicator Code": "IC.FRM.OUTG.ZS",
                "Indicator Name": "Value lost due to electrical outages (% of sales)"
            }
        ]
    },
    {
        "id": "public-sector",
        "name": "Public Sector",
        "indicators": [
            {
                "Indicator Code": "GC.BAL.CASH.GD.ZS",
                "Indicator Name": "Cash surplus/deficit (% of GDP)"
            },
            {
                "Indicator Code": "MS.MIL.XPND.ZS",
                "Indicator Name": "Military expenditure (% of central government expenditure)"
            },
            {
                "Indicator Code": "GC.DOD.TOTL.GD.ZS",
                "Indicator Name": "Central government debt, total (% of GDP)"
            },
            {
                "Indicator Code": "MS.MIL.XPND.GD.ZS",
                "Indicator Name": "Military expenditure (% of GDP)"
            },
            {
                "Indicator Code": "IQ.CPA.ECON.XQ",
                "Indicator Name": "CPIA economic management cluster average (1=low to 6=high)"
            },
            {
                "Indicator Code": "SG.GEN.PARL.ZS",
                "Indicator Name": "Proportion of seats held by women in national parliaments (%)"
            },
            {
                "Indicator Code": "IQ.CPA.SOCI.XQ",
                "Indicator Name": "CPIA policies for social inclusion/equity cluster average (1=low to 6=high)"
            },
            {
                "Indicator Code": "GC.REV.XGRT.GD.ZS",
                "Indicator Name": "Revenue, excluding grants (% of GDP)"
            },
            {
                "Indicator Code": "IQ.CPA.PUBS.XQ",
                "Indicator Name": "CPIA public sector management and institutions cluster average (1=low to 6=high)"
            },
            {
                "Indicator Code": "IC.TAX.PAYM",
                "Indicator Name": "Tax payments (number)"
            },
            {
                "Indicator Code": "IQ.CPA.STRC.XQ",
                "Indicator Name": "CPIA structural policies cluster average (1=low to 6=high)"
            },
            {
                "Indicator Code": "GC.TAX.TOTL.GD.ZS",
                "Indicator Name": "Tax revenue (% of GDP)"
            },
            {
                "Indicator Code": "GC.XPN.TOTL.GD.ZS",
                "Indicator Name": "Expense (% of GDP)"
            },
            {
                "Indicator Code": "IC.TAX.DURS",
                "Indicator Name": "Time to prepare and pay taxes (hours)"
            },
            {
                "Indicator Code": "IQ.CPA.IRAI.XQ",
                "Indicator Name": "IDA resource allocation index (1=low to 6=high)"
            },
            {
                "Indicator Code": "IC.TAX.TOTL.CP.ZS",
                "Indicator Name": "Total tax rate (% of commercial profits)"
            }
        ]
    },
    {
        "id": "science-and-tech",
        "name": "Science & Technology",
        "indicators": [
            {
                "Indicator Code": "TX.VAL.TECH.CD",
                "Indicator Name": "High-technology exports (current US$)"
            },
            {
                "Indicator Code": "BM.GSR.ROYL.CD",
                "Indicator Name": "Royalty and license fees, payments (BoP, current US$)"
            },
            {
                "Indicator Code": "TX.VAL.TECH.MF.ZS",
                "Indicator Name": "High-technology exports (% of manufactured exports)"
            },
            {
                "Indicator Code": "BX.GSR.ROYL.CD",
                "Indicator Name": "Royalty and license fees, receipts (BoP, current US$)"
            },
            {
                "Indicator Code": "IP.PAT.NRES",
                "Indicator Name": "Patent applications, nonresidents"
            },
            {
                "Indicator Code": "IP.JRN.ARTC.SC",
                "Indicator Name": "Scientific and technical journal articles"
            },
            {
                "Indicator Code": "IP.PAT.RESD",
                "Indicator Name": "Patent applications, residents"
            },
            {
                "Indicator Code": "SP.POP.TECH.RD.P6",
                "Indicator Name": "Technicians in R&D (per million people)"
            },
            {
                "Indicator Code": "GB.XPD.RSDV.GD.ZS",
                "Indicator Name": "Research and development expenditure (% of GDP)"
            },
            {
                "Indicator Code": "IP.TMK.NRES",
                "Indicator Name": "Trademark applications, direct nonresident"
            },
            {
                "Indicator Code": "SP.POP.SCIE.RD.P6",
                "Indicator Name": "Researchers in R&D (per million people)"
            },
            {
                "Indicator Code": "IP.TMK.RESD",
                "Indicator Name": "Trademark applications, direct resident"
            }
        ]
    },
    {
        "id": "social-dev",
        "name": "Social Development",
        "indicators": [
            {
                "Indicator Code": "SP.ADO.TFRT",
                "Indicator Name": "Adolescent fertility rate (births per 1,000 women ages 15-19)"
            },
            {
                "Indicator Code": "SH.HIV.1524.MA.ZS",
                "Indicator Name": "Prevalence of HIV, male (% ages 15-24)"
            },
            {
                "Indicator Code": "SL.TLF.0714.FE.ZS",
                "Indicator Name": "Children in employment, female (% of female children ages 7-14)"
            },
            {
                "Indicator Code": "SG.GEN.PARL.ZS",
                "Indicator Name": "Proportion of seats held by women in national parliaments (%)"
            },
            {
                "Indicator Code": "SL.TLF.0714.MA.ZS",
                "Indicator Name": "Children in employment, male (% of male children ages 7-14)"
            },
            {
                "Indicator Code": "SE.ENR.PRIM.FM.ZS",
                "Indicator Name": "Ratio of female to male primary enrollment (%)"
            },
            {
                "Indicator Code": "SL.TLF.0714.SW.FE.ZS",
                "Indicator Name": "Children in employment, study and work, female (% of female children in employment, ages 7-14)"
            },
            {
                "Indicator Code": "SE.ENR.SECO.FM.ZS",
                "Indicator Name": "Ratio of female to male secondary enrollment (%)"
            },
            {
                "Indicator Code": "SL.TLF.0714.SW.MA.ZS",
                "Indicator Name": "Children in employment, study and work, male (% of male children in employment, ages 7-14)"
            },
            {
                "Indicator Code": "SE.ENR.TERT.FM.ZS",
                "Indicator Name": "Ratio of female to male tertiary enrollment (%)"
            },
            {
                "Indicator Code": "SL.TLF.0714.ZS",
                "Indicator Name": "Children in employment, total (% of children ages 7-14)"
            },
            {
                "Indicator Code": "SE.ENR.PRSC.FM.ZS",
                "Indicator Name": "Ratio of girls to boys in primary and secondary education (%)"
            },
            {
                "Indicator Code": "SL.TLF.0714.WK.FE.ZS",
                "Indicator Name": "Children in employment, work only, female (% of female children in employment, ages 7-14)"
            },
            {
                "Indicator Code": "SM.POP.REFG",
                "Indicator Name": "Refugee population by country or territory of asylum"
            },
            {
                "Indicator Code": "SL.TLF.0714.WK.MA.ZS",
                "Indicator Name": "Children in employment, work only, male (% of male children in employment, ages 7-14)"
            },
            {
                "Indicator Code": "SM.POP.REFG.OR",
                "Indicator Name": "Refugee population by country or territory of origin"
            },
            {
                "Indicator Code": "SL.TLF.CACT.FE.ZS",
                "Indicator Name": "Labor participation rate, female (% of female population ages 15+)"
            },
            {
                "Indicator Code": "SL.EMP.INSV.FE.ZS",
                "Indicator Name": "Share of women employed in the nonagricultural sector (% of total nonagricultural employment)"
            },
            {
                "Indicator Code": "SL.TLF.CACT.MA.ZS",
                "Indicator Name": "Labor participation rate, male (% of male population ages 15+)"
            },
            {
                "Indicator Code": "SL.UEM.TOTL.FE.ZS",
                "Indicator Name": "Unemployment, female (% of female labor force)"
            },
            {
                "Indicator Code": "SP.DYN.LE00.FE.IN",
                "Indicator Name": "Life expectancy at birth, female (years)"
            },
            {
                "Indicator Code": "SL.UEM.TOTL.MA.ZS",
                "Indicator Name": "Unemployment, male (% of male labor force)"
            },
            {
                "Indicator Code": "SP.DYN.LE00.MA.IN",
                "Indicator Name": "Life expectancy at birth, male (years)"
            },
            {
                "Indicator Code": "SL.EMP.VULN.FE.ZS",
                "Indicator Name": "Vulnerable employment, female (% of female employment)"
            },
            {
                "Indicator Code": "SH.HIV.1524.FE.ZS",
                "Indicator Name": "Prevalence of HIV, female (% ages 15-24)"
            },
            {
                "Indicator Code": "SL.EMP.VULN.MA.ZS",
                "Indicator Name": "Vulnerable employment, male (% of male employment)"
            }
        ]
    },
    {
        "id": "urban-dev",
        "name": "Urban Development",
        "indicators": [
            {
                "Indicator Code": "SH.STA.ACSN.UR",
                "Indicator Name": "Improved sanitation facilities, urban (% of urban population with access)"
            },
            {
                "Indicator Code": "EP.PMP.DESL.CD",
                "Indicator Name": "Pump price for diesel fuel (US$ per liter)"
            },
            {
                "Indicator Code": "SH.H2O.SAFE.UR.ZS",
                "Indicator Name": "Improved water source, urban (% of urban population with access)"
            },
            {
                "Indicator Code": "EP.PMP.SGAS.CD",
                "Indicator Name": "Pump price for gasoline (US$ per liter)"
            },
            {
                "Indicator Code": "IS.VEH.NVEH.P3",
                "Indicator Name": "Motor vehicles (per 1,000 people)"
            },
            {
                "Indicator Code": "IS.ROD.DESL.PC",
                "Indicator Name": "Road sector diesel fuel consumption per capita (kg of oil equivalent)"
            },
            {
                "Indicator Code": "IS.VEH.PCAR.P3",
                "Indicator Name": "Passenger cars (per 1,000 people)"
            },
            {
                "Indicator Code": "IS.ROD.ENGY.ZS",
                "Indicator Name": "Road sector energy consumption (% of total energy consumption)"
            },
            {
                "Indicator Code": "EN.ATM.PM10.MC.M3",
                "Indicator Name": "PM10, country level (micrograms per cubic meter)"
            },
            {
                "Indicator Code": "IS.ROD.SGAS.PC",
                "Indicator Name": "Road sector gasoline fuel consumption per capita (kg of oil equivalent)"
            },
            {
                "Indicator Code": "EN.URB.LCTY.UR.ZS",
                "Indicator Name": "Population in the largest city (% of urban population)"
            },
            {
                "Indicator Code": "SP.URB.TOTL",
                "Indicator Name": "Urban population"
            },
            {
                "Indicator Code": "EN.URB.MCTY.TL.ZS",
                "Indicator Name": "Population in urban agglomerations of more than 1 million (% of total population)"
            },
            {
                "Indicator Code": "SP.URB.TOTL.IN.ZS",
                "Indicator Name": "Urban population (% of total)"
            },
            {
                "Indicator Code": "SI.POV.URGP",
                "Indicator Name": "Poverty gap at urban poverty line (%)"
            },
            {
                "Indicator Code": "IS.VEH.ROAD.K1",
                "Indicator Name": "Vehicles (per km of road)"
            },
            {
                "Indicator Code": "SI.POV.URHC",
                "Indicator Name": "Poverty headcount ratio at urban poverty line (% of urban population)"
            }
        ]
    }
]

with codecs.open(world_map_data_file_path, 'r', encoding='utf-8') as mapfile:
    mapdata = json.loads(mapfile.read())
    for dataset in datasets:
        print 'generating dataset {0}'.format(dataset['id'])
        with open(wdi_data_csv_path,'rb') as csvfile:
            reader = csv.reader(csvfile)
            data = { 'nodeIdSource': 'id', 'metadata': [{'Data_Item':'id','Item_Description':'ISO 3166-1 numeric country code'}], 'data': [] }
            data['id'] = dataset['id']
            data['name'] = 'World Development Indicators - {0}'.format(dataset['name'])
            data['sourceUrl'] = 'http://data.worldbank.org/data-catalog/world-development-indicators'
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
                            data_node[data_item] = float(val)
                            if new_data_node:
                                data['data'].append(data_node)
                            data_count += 1
                        else:
                            no_data_count += 1
        print 'data_count={0}'.format(data_count)
        print 'no_data_count={0}'.format(no_data_count)
        with open('../datasets/world/{0}.json'.format(dataset['id']),'w') as json_out_file:
            json_out_file.write(json.dumps(data, sort_keys=False, indent=4, separators=(',',': ')))
        with open('../datasets/world/{0}.min.json'.format(dataset['id']),'w') as json_min_out_file:
            json_min_out_file.write(json.dumps(data, separators=(',',':')))
                