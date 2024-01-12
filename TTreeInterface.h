
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "TTree.h"
#include "TFile.h"
#include "TChain.h"

class TTreeInterface{
	public:
	
	TFile* _f{};
	std::vector<std::string> _filelist{};
	TTree* _ttree{0};
	TChain* _tchain{0};
	std::string _filename{};
	std::string _treename{};
	
	//constructors to auto load trees/files from strings
	TTreeInterface(std::string fileName, std::string treeName);
	TTreeInterface(std::vector<std::string> fileList, std::string treeName);
	
	//make a flattened csv
	void CreateFlattenedCSV( std::vector<std::string> branchList, std::string csvname);
	
		
};
TTreeInterface::TTreeInterface(std::string fileName, std::string treeName){
	_filename = fileName;
	_treename = treeName;
	_f = TFile::Open(_filename.c_str());
	std::cout<<"Loading "<<_treename<<" from "<< _filename<<"\n";
	_ttree = (TTree*)_f->Get(_treename.c_str());
}
TTreeInterface::TTreeInterface(std::vector<std::string> fileList, std::string treeName){
	_filelist = fileList;
	_treename = treeName;
	_tchain = new TChain(_treename.c_str());
	std::cout<<"Loading "<<_treename<<" from "<<_filelist.size()<<" files \n";
	for( int i=0; i<_filelist.size(); i++){
		_tchain->AddFile(_filelist[i].c_str());
	}
}
void TTreeInterface::CreateFlattenedCSV( std::vector<std::string> branchList, std::string csvname ){
	// loop over selected branches
	// expect vector of primitive types and flatten
	// also record a relative event ID
	
	//prepare space delimited csv file output
	std::ofstream ocsv;
  	ocsv.open(csvname);
  	ocsv << "evtid ";
  //myfile.close();
	
	//create variables for ttree to load into
	std::vector< std::vector<float>* > branchVec(branchList.size(), 0);
	for(int i=0; i<branchList.size(); i++){

		//set dynamic addresses
		std::cout<<"setting branch address of: "<< branchList[i]<<"\n";
		_ttree->SetBranchAddress(branchList[i].c_str(),&branchVec[i]);
		
		//write the csv headers
		ocsv << branchList[i] << " ";
	}
	ocsv << "\n";
	
	Long64_t nentries = _ttree->GetEntries();
	std::cout<<"Looping over "<<nentries<<" events\n";
	int dim;
	for(Long64_t i=0; i< nentries; i++){
		_ttree->GetEntry(i);
			
		
		//how many objects to flatten? grab the first one on the list
		dim = (branchVec[0])->size();
		//debug print
		//std::cout<<"found vector of dimension "<<dim<<"\n";
		//unroll the vector
		for(int j=0; j<dim; j++){
			ocsv<<i<<" ";
			for(int k=0; k<branchVec.size(); k++){
				//debug print
				//std::cout<<branchList[k]<<" "<<branchVec[k]->at(j)<<" \n";
				//write branch quantities and evtid
				ocsv<<branchVec[k]->at(j)<<" "; 
			}
			ocsv<<"\n";
		}
			
	}
	ocsv.close();
}


/*///////////////////////
Generic CSV global methods
////////////////////////*/
std::vector<std::string> ReadList( std::string fileList){
	std::vector<std::string> filevec{};
	std::ifstream f; 
	f.open(fileList);
	std::string ifile{};
	if( f.is_open()){
		while( f >> ifile ) { 
			std::cout << "adding item: "<<ifile<<"\n";
			filevec.push_back(ifile);
		}
		f.close();
	}		
	else{ std::cout<< "filelist not found\n"; }
	return filevec;
}






