

# LLP - MVA Projects

## Exercise 1 - Label sampling/Training and testing split

Data preprocessing and balancing the amount of data for each label is a very important step training a NN model. If the distribution of each label are uneven, the model will learn about the statistical imbalance and would over-emphasize learned features from the over represented label in the dataset. To correct for this, we will randomly sample even amounts of each label

#### Part 1) Label Sampling
Inputs $\rightarrow$ (1) the base photon numpy matrix (2) a list of labels to sample
Output $\rightarrow$  a numpy array which is a subset of (1) with evenly sampled labels from (2)

Pseudo-code
```
def labelSamplingFunction( inputDataFrame,  inputLabelList):
  (1) Determine the total number photons
  (2) Determine the number (N) of photons per label
    - The number of photons per label should be the label with the least photons
  (4) Shuffle the inputDataFrame for easy random sampling of photons
  (5) Create a empty newDataFrame to hold sampled photons

  for each photon in inputDataFrame:
    Select N photons from each label
    Add photons to a newDataFrame
  
  return newDataFrame

```

**Validate your function by testing the output**
  - If you tabulate the number of photons for each label, is it what you expect?
  - Do edge cases work, say if we only have 1 label in the `inputLabelList`?

#### Part 2) Training and testing split

Currently the model is trained on the full sample, which isn't normal practice - the model will likely be overtrained.  Typically, we split our dataset into a training and testing set. A large fraction of the dataset is used to train the model and a smaller piece is set aside to test the model after training. The test dataset validates our model by demonstrating that the model can generalize to data it hasn't seen before.

1) Use existing tools in sklearn to divide the output from Part 1) into a testing and training dataset
    -  https://scikit-learn.org/stable/modules/generated/sklearn.model_selection.train_test_split.html
    - A good splitting fraction to start with: $10\%$ testing fraction
2) In `model.py` at the `model.fit(...)` step, input the features and labels from your *training* dataset
3) In `model.py` at the `model.evaluate(...)` step, input the features and labels from you *testing* dataset

**Validate your results by comparing outputs**
  - What are the model results if you have uneven labels ?
  - What are the model results if try different testing and training splitting? 50/50? 60/40?




***


## Exercise 2 - Model Building

All three parts of this exercise can and should be iterated on: selection of features, optimization of the network architecture, and validation of the model accuracy by assessing the systematic spread in classification via sampling of training data

#### Part 1) Feature Selection

To create a competitive model we first need establish a benchmark by reproducing current results and then later try to improve on those results. The first model we should build should involve the following pre-processing/pre-selection cuts

![alt text](https://github.com/Jphsx/PhotonMVA/blob/master/doc/Photon_Preselection.png?raw=true)


The branches from the input ROOT file that correspond to the preselection criteria are:
```
H/E : Photon_hadTowOverEM
R9 : Photon_r9
sigma_ieie : Photon_SigmaIEtaIEta
ECAL iso: Photon_ecalRHSumEtConeDR04
HCAL iso: Photon_hcalTowerSumEtConeDR04
Tracker iso: Photon_trkSumPtSolidConeDR04
```

There are also 4 additional parameters that should be kept in the csv file, including the labels
```
Photon_smaj 
Photon_smin
Photon_pt
Photon_genLlpId
```

**Modify `TTreeToCSV.C` to use only these branches**

The pT branch will be used to compute the pt dependent cuts on isolations and then should be dropped before training

The default pT cut is 10 GeV which should be increased to match the 70//40 GeV leading/subleading requirments. 

To match the leading/subleading photon selection we need to restrict each event to 1 or 2 photons. This can be done by dropping the third (or more) photons which have the same event ID.

The pseudocode would be something like the following:
We can do a slower more intuitive loop, or optimize with slicing/masking later



First, the selection of photons based on multiplicity
```
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
```

For applying preselection cuts use the 2017 criteria. 
```
def ApplyPreselection(inputDataFrame):

	newDataFrame = a new empty frame to load with photons that pass cuts
	
	for each photon in inputDataFrame:
	
		compute the pt based isolation requirements
		check each criteria from preselection
		if the photon passes all criteria save the photon on newDataFrame

	return newDataFrame
```
	 

If you want to try slicing, the sytnax would be something like this:

```
listOfPhotonPt #the pt column of full matrix
inputDataFrame # the numpy full matrix
inputDataFrame = inputDataFrame[ listOfPhotonPt >= 40 ]
```

After all of the preselection is completed, drop the pT column and create the evenly sampled testing/training split for input into the DNN (same as Exercise 1)



#### Part 2) Network Architecture

The base architecture and training methods should also be reproduced:

![alt text](https://github.com/Jphsx/PhotonMVA/blob/master/doc/DNN_architecture.png?raw=true)

Since our current implementation is a small statsitical subset which doesn't include the full MC background we don't dont need to train as long

	- For testing used a small number of epochs (~10)
	- Implement with up to 100 epochs and a batch size up to 1000
	- Experiment with nodes per layer e.g. 10,20,40
	- Experiment with number of layers e.g. 3,5,10
	

** How does your model perform? What differences can you find by varying architecture/epochs/batch size? **




