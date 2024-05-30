import numpy as np
import sys
from sklearn.preprocessing import normalize
np.set_printoptions(threshold=sys.maxsize)
np.set_printoptions(suppress=True, linewidth=100000)

csvFile = './InputCSV/test_FullabstractionMap_new_v14.csv'


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


#print("Veto photons with pt < 10 GeV")
#dataset = dataset[dataset[:, 2] > 10., :] #this moved columns, need to update this part
#print(dataset)
#print("Shape=",np.shape(dataset))

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
#print(dataset)



def GetLeadingSubLeading(inputDataFrame):
	
	newDataFrame = a new empty frame to load with photons that pass cuts
	
	for each photon in inputDataFrame:	
		
		check the event ID 
			if the event ID is new, save the new ID, start a counter
			
			if the event ID is the same as the last one, increment a counter
			
			if the counter ==1 and pT > 70 save the photon
			
			if the counter ==2 and pT > 40 save the photon
			
			if the counter > 2 do not save the photon				 
	
	return newDataFrame



def ApplyPreselection(inputDataFrame):

	newDataFrame = a new empty frame to load with photons that pass cuts
	
	for each photon in inputDataFrame:
	
		compute the pt based isolation requirements
		check each criteria from preselection
		if the photon passes all criteria save the photon on newDataFrame

	return newDataFrame




listOfPhotonPt #the pt column of full matrix
inputDataFrame # the numpy full matrix
inputDataFrame = inputDataFrame[ listOfPhotonPt >= 40 ]




#enough pre-processing now, could improve with even sampling and shuffling, maybe even limit photon contribution per event

def labelSamplingFunction(inputDataFrame, inputLabelSet):
  #print(inputDataFrame)
  photontuple = np.shape(inputDataFrame) #(1) Determine the total number photons
  #print(photontuple)
  #labelarray = inputDataFrame[:,0]
  #labelarray = inputLabelSet
  #print('label array:', labelarray)
  #labelarrayset = set(labelarray)
  relabelset = [ 0 if x == 22 else 1 for x in labels ] #binary time
  relabelset = np.array(relabelset)
  #print('relabelset:', relabelset)
  relabelset = [element for element in relabelset if 1 != element] #remove 1s, this is all zero
  #print('relabelset:', relabelset)
  N = len(relabelset) #(2) Determine the number (N) of photons per label
  #print('Number of Photons per Label:', N)
    #- The number of photons per label should be the label with the least photons
  #print('inputDataFrame:', inputDataFrame)
  signalcount = 0
  for row in inputDataFrame:
    
    if row[0] == 0:
      signalcount += 1

  np.random.shuffle(inputDataFrame) #(4) Shuffle the inputDataFrame for easy random sampling of photons
  newDataFrame = [] #(5) Create a empty newDataFrame to hold sampled photons 

  #print('inputDataFrame:', inputDataFrame)
  #print('newDataFrame:', newDataFrame)

  backgroundcount = 0
  
  for p in inputDataFrame:
    #print(p[0])
    if p[0] == 0:
      newDataFrame.append(p)
    else:
      if backgroundcount < signalcount:
        newDataFrame.append(p)
        backgroundcount += 1
    #Select N photons from each label
    #Add photons to a newDataFrame

  #print(Nphotons)
  print('LABEL SAMPLING STUFF')
  print('Signal Count:', signalcount)
  print('Background Count:', backgroundcount)
  print('N:', N)
  print('photontuple shape:', photontuple)
  print('label set shape:', np.shape(relabelset))
  print('inputDataFrame shape:', np.shape(inputDataFrame))
  print('newDataFrame shape:', np.shape(newDataFrame))

  #print(newDataFrame)
  
  
  
  return newDataFrame


#test = labelSamplingFunction(dataset)

#print(test)

#print(photondataset)


