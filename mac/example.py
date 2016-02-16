import sys
from ROOT import gSystem
gSystem.Load("libShowerTrigger_ShowerTrig")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing ShowerTrig..."

sys.exit(0)

