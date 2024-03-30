
        	
import numpy as np
import sys
import matplotlib.pyplot as plt

np.set_printoptions(threshold=sys.maxsize)
np.set_printoptions(suppress=True, linewidth=100000)

csvFile = './InputCSV/test_FullabstractionMap_new_v14.csv'


base_dataset = np.genfromtxt(csvFile, dtype=float, delimiter=' ', names=True, skip_header=0, missing_values={'nan':np.NaN})
print("Loaded csv into special tuple, shape=",np.shape(base_dataset))
print("Features: ",base_dataset.dtype.names)


print("Converting 1D tuple to 2D array")
dataset = [list(tup) for tup in base_dataset]
dataset = np.array(dataset)
#print(dataset)
print("Shape=",np.shape(dataset))


print("Dropping NaN rows")
dataset = dataset[~np.isnan(dataset).any(axis=1), :]
#print(dataset)
print("Shape=",np.shape(dataset))

print(dataset[:10])

#select labels
print("Selecting Photons 22")
signal_dataset = dataset[dataset[:, 1] == 22., :]
print(signal_dataset[:10])

print("Selecting BKG not 22")
bkg_dataset = dataset[dataset[:, 1] != 22., :]
print(bkg_dataset[:10])

#plot normalized disgtributions of salpha for signal and background
print("Getting salp values")
signal_salp = signal_dataset[:,2]
bkg_salp = bkg_dataset[:,2]

print(signal_salp[:10])
print(bkg_salp[:10])


#plot the normalized 1-D distributions of salp (normalize with density=True)
fig,ax = plt.subplots() # Instantiate figure and axes object
ax.hist(signal_salp, label="Signal_salp", histtype="step", density=True, bins=50) # Plot histogram of nums1
ax.hist(bkg_salp, label="Bkg_salp", histtype="step", density=True, bins=50) # Plot histogram of nums2
plt.legend()
#either show it to screen or save it to pdf
#plt.show()
plt.savefig('salp.png')

#try the same with smin
sig_smin = signal_dataset[:,4]
bkg_smin = bkg_dataset[:,4]
fig, ax = plt.subplots()
ax.hist(sig_smin, label="Signal_smin", histtype="step", density=True, bins=50) # Plot histogram
ax.hist(bkg_smin, label="Bkg_smin", histtype="step", density=True, bins=50) # Plot histogram of
plt.legend()
#plt.show()
plt.savefig('smin.png')

#plot 2-D distribution of salp and smin, do it twice, once for sig and once for bg
fig, ax = plt.subplots()
ax.hist2d(signal_salp, sig_smin, bins=(np.arange(-1.5, 1.5, 0.1), np.arange(0, 0.5, 0.02)),cmap=plt.cm.jet)
#plt.show()
plt.savefig('2d_sig_salp_smin.png')

fig, ax = plt.subplots()
ax.hist2d(bkg_salp, bkg_smin, bins=(np.arange(-1.5, 1.5, 0.1), np.arange(0, 0.5, 0.02)),cmap=plt.cm.jet)
#plt.show()
plt.savefig('2d_bkg_salp_smin.png')








