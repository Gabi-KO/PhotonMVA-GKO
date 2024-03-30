
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <utility>
#include <set>

#include "TTree.h"
#include "TFile.h"
#include "TChain.h"

typedef std::map<std::string, std::pair<std::string, std::string> > strIdxMap ;
typedef std::map<std::string, std::pair<std::vector<float>*,std::vector<float>*>> vecIdxMap;

class TTreeInterface{
	public:
	
	TFile* _f{};
	std::vector<std::string> _filelist{};
	TTree* _ttree{0};
	TChain* _tchain{0};
	std::string _filename{};
	std::string _treename{};
	bool _isVersion_geq14 = false;
	std::set<std::string> _idxSet{};
	
	//index chasing var
	strIdxMap _idxMaps{};//stores name associations, keep names around for debug and printing
	vecIdxMap _branchMaps{}; //auto load datastructures with same key mapping
	
	
	
	//constructors to auto load trees/files from strings
	TTreeInterface(std::string fileName, std::string treeName, bool isNewVersion);
	TTreeInterface(std::vector<std::string> fileList, std::string treeName, bool isNewVersion );
	
	//do index chasing for quantities in newer versions
	//input idx branchname associated to idx of parent array, output label is csv header for this mapping
	void MapIdx( std::string idxBranchName, std::string targetBranch, std::string outLabel );
	float RetrieveMapValue( std::string mapkey, int dim_idx);
	bool CheckUniqueMap( std::string idxBranchName);
	std::vector<float>* GetBranchAlreadyAssigned( std::string idxBranchName );
	
	//make a flattened csv
	void CreateFlattenedCSV( std::vector<std::string> branchList, std::string csvname);
	
		
};
TTreeInterface::TTreeInterface(std::string fileName, std::string treeName, bool isNewVersion){
	_filename = fileName;
	_treename = treeName;
	_f = TFile::Open(_filename.c_str());
	std::cout<<"Loading "<<_treename<<" from "<< _filename<<"\n";
	_ttree = (TTree*)_f->Get(_treename.c_str());
	std::cout<<"Processing newer ntuple = "<< isNewVersion <<"\n";
	_isVersion_geq14 = isNewVersion;
}
TTreeInterface::TTreeInterface(std::vector<std::string> fileList, std::string treeName, bool isNewVersion){
	_filelist = fileList;
	_treename = treeName;
	_tchain = new TChain(_treename.c_str());
	std::cout<<"Loading "<<_treename<<" from "<<_filelist.size()<<" files \n";
	for( int i=0; i<_filelist.size(); i++){
		_tchain->AddFile(_filelist[i].c_str());
	}
	_ttree = _tchain;
	std::cout<<"Processing newer ntuple = "<< isNewVersion <<"\n";
	_isVersion_geq14 = isNewVersion;
}
bool TTreeInterface::CheckUniqueMap( std::string idxBranchName){
	//can only register the idx array to 1 object, so we need point to the original if it exists already
	bool branchExists=false;
		for (auto& idxstr : _idxSet) {
     
		  if( idxstr == idxBranchName ){
		  	branchExists = true;
		  }	   
		}
	std::cout<<"Checking if branch "<< idxBranchName <<" address is already assigned: branchExists="<<branchExists<<"\n";
	return branchExists;
	
}
std::vector<float>* TTreeInterface::GetBranchAlreadyAssigned( std::string idxBranchName ){
	for(strIdxMap::iterator iter = _idxMaps.begin(); iter != _idxMaps.end(); ++iter)
		{
			std::string key = iter->first;
		  if( _idxMaps[ key].first == idxBranchName ){
		  	return _branchMaps[key].first;
		  }
		}
	return 0;

}
void TTreeInterface::MapIdx( std::string idxBranchName, std::string targetBranch, std::string outLabel ){
	_idxMaps[ outLabel ] = std::make_pair( idxBranchName, targetBranch );
	std::vector<float>* v1 =0;
	std::vector<float>* v2 =0;
	_branchMaps[ outLabel ] = std::make_pair( v1, v2 );//init pointers to 0
	
	std::cout<<"setting idx chase branch address of: "<< outLabel <<"\n";
	if( !CheckUniqueMap( idxBranchName ) ){
		_ttree->SetBranchAddress(idxBranchName.c_str(),&(_branchMaps[outLabel].first) );
	}else{
		_branchMaps[ outLabel ].first = GetBranchAlreadyAssigned( idxBranchName );
	}
    _ttree->SetBranchAddress(targetBranch.c_str(), &(_branchMaps[outLabel].second) );
    
    _idxSet.insert( idxBranchName );
}
float TTreeInterface::RetrieveMapValue(std::string mapkey, int dim_idx){
	
	float val{};
	float idx{};
	if( (_branchMaps[mapkey].first)->size()==0 || (_branchMaps[mapkey].second)->size()==0){
		return -999;
	}
	
	if( (_branchMaps[mapkey].first)->at(dim_idx) < 0 ){
		return -1;
	}
	else{
		idx = (_branchMaps[mapkey].first)->at(dim_idx);
		val = (_branchMaps[mapkey].second)->at(idx);
		return val;
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
	
	//do gen idx chasing in new version of ntuple
	if(_isVersion_geq14){//if we have a newer version load the necessary gen information

		
		//idx mapping abstraction
		for(strIdxMap::iterator iter = _idxMaps.begin(); iter != _idxMaps.end(); ++iter)
		{
		  std::string key =  iter->first;
		  std::cout<<"found mapping: "<<key<<" "<<_idxMaps[key].first<<":"<<_idxMaps[key].second<<"\n"; 
		  //print headers
		  ocsv<< " "<<key<<" ";
		}

		
	}//end new version check

	
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
			//if newever versions extract the values from the idx mapping
			if(_isVersion_geq14){
				for(strIdxMap::iterator iter = _idxMaps.begin(); iter != _idxMaps.end(); ++iter)
				{
					ocsv<< RetrieveMapValue( iter->first, j) << " ";
				}
			}
			
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






