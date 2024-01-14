import numpy as np
import sys
from sklearn.preprocessing import normalize
np.set_printoptions(threshold=sys.maxsize)
np.set_printoptions(suppress=True, linewidth=100000)

csvFile = './InputCSV/test.csv'


base_dataset = np.genfromtxt(csvFile, dtype=float, delimiter=' ', names=True, skip_header=0, missing_values={'nan':np.NaN})
print("Loaded csv into special tuple, shape=",np.shape(base_dataset))
print("Features: ",base_dataset.dtype.names)

#slice for debugging
#base_dataset = base_dataset[:25] # note that genfrom txt generates a '1D' dictionary
#print(base_dataset)
#print(np.shape(base_dataset))


print("Converting 1D tuple to 2D array")
dataset = [list(tup) for tup in base_dataset]
dataset = np.array(dataset)
#print(dataset)
print("Shape=",np.shape(dataset))


print("Dropping NaN rows")
dataset = dataset[~np.isnan(dataset).any(axis=1), :]
#print(dataset)
print("Shape=",np.shape(dataset))


print("Veto photons with pt < 10 GeV")
dataset = dataset[dataset[:, 2] > 10., :]
#print(dataset)
print("Shape=",np.shape(dataset))

print("Removing evitd, splitting labels and feature matrix")
labels = dataset[:,1]
features = dataset[:,2:]
#print(labels)
#print(features)
print("Label Shape=",np.shape(labels))
print("Feature Shape=", np.shape(features))

print("Normalizing Features")
features = normalize(features, axis=0, norm='max')
#print(features)
print("Feature Shape=",np.shape(features))

print("Evaluating Labels")
#print(labels)
labelset = set(labels)
ndata = np.shape(dataset)[0]
#print("ndata=",ndata)
print("There are",len(labelset)," labels")
print("LabelSet:",labelset)
for l in labelset:
	lsubset = labels[ labels[:] == l ]
	freq = float(np.shape(lsubset)[0])
	freq = freq/ndata * 100.
	print(l, "label frequency ",freq,"%")
	
print("Redefine labels to simple binary case, 0=22 and 1=everything else")
relabels = [ 0 if x == 22 else 1 for x in labels ]
#print(labels)
#print(relabels)

print("Restacking features and labels for sampling")
relabels = np.array([relabels])
features = np.array(features)
#print(relabels.T)
#print(features)
dataset = np.hstack((relabels.T,features))
#print(dataset)
print("Shape=", np.shape(dataset))

#enough pre-processing now, could improve with even sampling and shuffling, maybe even limit photon contribution per event

