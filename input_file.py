import pyalps




parms = []
parms.append( { 
'MODEL_LIBRARY' : '/home/sierant/om_model_MPS/models.xml',
'LATTICE': 'open chain lattice', 
'MODEL': 'spinOm',
'L': 6,
'Jz': 0.15,
'dwa': 2.0,
'T': 1.0,
'V': 0.0,
'SWEEPS': 20,
'NUMBER_EIGENVALUES': 4,
'MAXSTATES': 100,
'MEASURE_AVERAGE[Magnetization]': 'Sz',
'MEASURE_AVERAGE[Exchange]': 'exchange',
'MEASURE_LOCAL[Local magnetization]': 'Sz',
'MEASURE_CORRELATIONS[diagonalspin correlations]': 'Sz',
'MEASURE_CORRELATIONS[off-diadonal spin correlations]': 'Sz:Sz',
} )

#write the input file and run the simulation
input_file = pyalps.writeInputFiles('results',parms)
res = pyalps.runApplication('mps_optim',input_file,writexml=True)

#load all measurements for all states
data = pyalps.loadEigenstateMeasurements(pyalps.getResultFiles(prefix='results'))






