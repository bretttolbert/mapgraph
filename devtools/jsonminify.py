import sys
import simplejson as json
import codecs
if len(sys.argv) != 2:
    print 'Usage: jsonminify.py infile > outfile'
else:
    fname = sys.argv[1]
    with codecs.open(fname, 'r', encoding='utf-8') as f:
        data = json.loads(f.read())
        print json.dumps(data, separators=(',',':'))