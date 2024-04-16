
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
import numpy as np

#Do data preprocessing:
#import and automatically run script
import DataPreprocessing as DP

# split into input (X) and output (y) variables
labels = DP.dataset[:,0]
features = DP.dataset[:,1:]
nlabels = len(set(labels))
nfeatures = np.shape(features)[1]
print("nlabels:",nlabels, "nfeatures",nfeatures)
#reshape 1D labels  for input
labels = np.array([labels])
labels = labels.T

print('LABEL SAMPLING STUFF')
print('N:', nlabels)
print('photontuple:', nfeatures)
#print('label set shape:', np.shape(relabelset))
#print('inputDataFrame shape:', np.shape(inputDataFrame))
#print('inputDataFrame:', inputDataFrame)


#print('LABELTEST', labels)
#print('DATA SET TEST', DP.dataset)
newdataframe = DP.labelSamplingFunction(DP.dataset, labels)
#newdataframe = DP.labelSamplingFunction(features, labels)

#print(labels)
#print(features)
# define the keras model
model = Sequential()
model.add(Dense(20, input_shape=(nfeatures,), activation='relu'))
model.add(Dense(20, activation='relu'))
model.add(Dense(3, activation='relu'))
model.add(Dense(nlabels-1, activation='softmax'))
#model.add(Dense(1, activation='sigmoid'))

# compile the keras model
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
# fit the keras model on the dataset
model.fit(features, labels, epochs=10, batch_size=5, validation_split=0.15, verbose=1, shuffle=True)
# evaluate the keras model
_, accuracy = model.evaluate(features, labels)
print('Accuracy: %.2f' % (accuracy*100))

#print('New Data Frame:', newdataframe)
