import sys
f=file(sys.argv[1])
for line in f:
	line = line.replace('\t',' ')
	print line.replace('\n',' '),
