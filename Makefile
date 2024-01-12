


all: TTreeInterface.h TTreeToCSV.C
	g++ -o csv.x TTreeToCSV.C `root-config --cflags --glibs`
     
clean:
	rm csv.x
