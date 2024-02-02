

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


