
#include "TTreeInterface.h"



int main(){

	std::string fname = "InputFile/GMSB_AOD_v13_GMSB_L-350TeV_Ctau-200cm_AODSIM_RunIIFall17DRPremix_output99.root";
	std::string tname = "tree/llpgtree";
	std::string flistname = "InputFileLists/test.list";
	
	///loading a single file///
	TTreeInterface TI(fname,tname);
	TI._ttree->Print();
	//TI._ttree->Draw("Met_pt");
	
	///example code to load tchain///
	/*
	std::vector<std::string> fileList{};
	fileList = ReadList(flistname);
	TTreeInterface TI2(fileList, tname);
	TI2._tchain->Print();
	TI2._tchain->Scan("ECALRecHit_energy");
	*/
	
	std::vector<std::string> branchList{"Photon_pt", "Photon_pz"};
	std::string csvname = "InputCSV/test.csv";
	TI.CreateFlattenedCSV( branchList,csvname );
	
	return(0);
}
