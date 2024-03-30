
# LLP - Photon MVA 

## Workflow
```mermaid
graph LR
  TTree --> CSV --> Preprocessing --> DNN
```

### ROOT interface
- `TTreeInterface.h`  
  - Helper class/ and helper functions that read and manage `TTree*` and `TChain*` 
- `TTreeToCSV.C`
  - Compiled macro which uses `TTreeInterface.h` and creates space delimited CSV files
    - Loads a `vector<string>` of specified branch names -- these branches are associated with the CSV headers
    - Loops over the `TTree*` and flattens each event into individual photons
    - Writes each photon to an output CSV file, records a relative event number for posterity

- **Note on Ntuple version**
	- v13 is flat, labels and certain values can be directly accessed
	- for v14+ ntuples are not flat, labels and other values need are index mapped to other collections. This mapping is automatic if the proper labels are provided in the MapIdx() function

### Data Preprocessing
- `DataPreprocessing.py`
  - python script to analyze csv and prepare input for MVA model
    - Reads CSV into numpy array
    - Drops NaNs, drops photons which don't pass kinematic cuts (veto pT<10), removes event id
    - Splits dataset into label and feature matrix
      - Analyzes and simplifies labels into binary classification
      - Normalizes each colummn of the feature matrix (via max)
      - Restacks labels and features 

### Model training
- `model.py`
  - Tensorflow based DNN that imports the numpy array from preprocessing step
    - Resplits dataset to prepare model inputs
    - Constructs a basic DNN with a few nodes and layers
    - Fits the model and evaluates its accuracy on the dataset

#### Running the workflow
1) Compile the CSV tools with `make`
2) Run `./csv.x` on your input ROOT file or input list of ROOT files
3) Launch preprocessing and model fitting with `python model.py`

#### Setting up the environment
1) Need ROOT, Anaconda, and newer python ~3.8
2) Make a conda environment to hold Tensorflow, install tensorflow and other tools

`conda create -n tflow tensorflow`
`conda activate tflow`
`conda install keras`
`conda install conda-forge::scikit-learn`

3) sklearn is used for matrix normalization and keras is the tensorflow API/interface, you may also need numpy `conda install numpy`




