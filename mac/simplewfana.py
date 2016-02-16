# To run script type python path/simplewfana.py nnbar-rawdigit-file numi-truth-file numi-rawdigit-file

import sys,os

if len(sys.argv) < 2:
	msg  = '\n'
	msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
	msg += '\n'
	sys.stderr.write(msg)
	sys.exit(1)

from ROOT import gSystem
#gSystem.Load("libSimpleWFAna")
from ROOT import larlite as fmwk

print('Enter a cut (1=hit num, 2=TDC std, 3=ADC amp, 4=integrated WF, 5=TDC iqr):')
while True:
	try:
		option = int(raw_input());
		break
	except(option not in [1,2,3,4,5]):
		print('Must be an integer between 1 and 5')

# Loop over different tolerances changing file names each time
for x in range (0,6):
	if x == 3:
		T = 4
		fname = "HitWire-nnbar4.root"
		fname1 = "HitWire-numi4.root"
		tname = "nnbar4.txt"
		tname1 = "numi4.txt"
	if x == 1:
		T = 6
		fname = "HitWire-nnbar6.root"
		fname1 = "HitWire-numi6.root"
		tname = "nnbar6.txt"
		tname1 = "numi6.txt"
	if x == 2:
		T = 8
		fname = "HitWire-nnbar8.root"
		fname1 = "HitWire-numi8.root"
		tname = "nnbar8.txt"
		tname1 = "numi8.txt"
	if x == 0:
		T = 10
		fname = "HitWire-nnbar10.root"
		fname1 = "HitWire-numi10.root"
		tname = "nnbar10.txt"
		tname1 = "numi10.txt"
	if x == 4:
		T = 12
		fname = "HitWire-nnbar12.root"
		fname1 = "HitWire-numi12.root"
		tname = "nnbar12.txt"
		tname1 = "numi12.txt"
	if x == 5:
		T = 14
		fname = "HitWire-nnbar14.root"
		fname1 = "HitWire-numi14.root"
		tname = "nnbar14.txt"
		tname1 = "numi14.txt"

	# Create ana_processor instance
	my_proc = fmwk.ana_processor()
	
	# add input files - nnbar rawdigit file
	my_proc.add_input_file(sys.argv[1])
	
	# Specify IO mode
	my_proc.set_io_mode(fmwk.storage_manager.kREAD)
	
	ana_unit = fmwk.SimpleWFAna(T,fname,tname,option)
	
	my_proc.add_process(ana_unit)
	
	print
	print  "Finished configuring ana_processor. Start event loop!"
	print
	
	# Let's run it.
	my_proc.run()
	# Get cut limits from nnbar file
	t_min = ana_unit.GetTmin()
	t_max = ana_unit.GetTmax()
	u_min = ana_unit.GetUmin()
	u_max = ana_unit.GetUmax()
	v_min = ana_unit.GetVmin()
	v_max = ana_unit.GetVmax()
	y_min = ana_unit.GetYmin()
	y_max = ana_unit.GetYmax()
	# done!
	print 
	print "Finished running ana_processor event loop!"
	print

	# Create ana_processor instance
	my_proc1 = fmwk.ana_processor()

	# add input files - numi truth file
	my_proc1.add_input_file(sys.argv[2])

	# Specify IO mode
	my_proc1.set_io_mode(fmwk.storage_manager.kREAD)

	ana_unit1 = fmwk.SimpleWFAna(T,"delete.root","delete.txt",option)

	my_proc1.add_process(ana_unit1)

	print
	print  "Finished configuring ana_processor. Start event loop!"
	print

	# Let's run it.
	my_proc1.run()
	# Get interaction types
	typ = ana_unit1.GetType()
	# Get neutrino energies
	qsq = ana_unit1.GetQsq()
	# done!
	print
	print "Finished running ana_processor event loop!"
	print
	
	# Create ana_processor instance
	my_proc2 = fmwk.ana_processor()
	
	# add input files
	my_proc2.add_input_file(sys.argv[3])
	
	# Specify IO mode
	my_proc2.set_io_mode(fmwk.storage_manager.kREAD)
	
	ana_unit2 = fmwk.SimpleWFAna(T,fname1,tname1,option)
	
	# Set cut limits, interaction types and neutrino energies
	ana_unit2.SetTmin(t_min)
	ana_unit2.SetTmax(t_max)
	ana_unit2.SetUmin(u_min)
	ana_unit2.SetUmax(u_max)
	ana_unit2.SetVmin(v_min)
	ana_unit2.SetVmax(v_max)
	ana_unit2.SetYmin(y_min)
	ana_unit2.SetYmax(y_max)
	ana_unit2.SetType(typ)
	ana_unit2.SetQsq(qsq)
	
	my_proc2.add_process(ana_unit2)
	
	print
	print  "Finished configuring ana_processor. Start event loop!"
	print
	
	# Let's run it.
	my_proc2.run()
	
	# done!
	print
	print "Finished running ana_processor event loop!"
	print
	
sys.exit(0)

