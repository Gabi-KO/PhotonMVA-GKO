
#include "TTreeInterface.h"



int main(){

	std::string fname = "InputFile/GMSB_AOD_v13_GMSB_L-350TeV_Ctau-200cm_AODSIM_RunIIFall17DRPremix_output99.root";
	std::string tname = "tree/llpgtree";
	std::string flistname = "InputFileLists/test.list";
	
	///loading a single file///
	TTreeInterface TI(fname,tname);
	//TI._ttree->Print();
	
	
	///example code to load tchain///
	//std::vector<std::string> fileList{};
	//fileList = ReadList(flistname);
	//TTreeInterface TI2(fileList, tname);
	//TI2._tchain->Print();
	

	std::vector<std::string> branchList{"Photon_genLlpId", "Photon_pt", "Photon_pz", "Photon_eta", "Photon_salp", "Photon_smaj", "Photon_smin", "Photon_trkSumPtSolidConeDR04", "Photon_trkSumPtHollowConeDR04"  };
	std::string csvname = "InputCSV/test.csv";
	TI.CreateFlattenedCSV( branchList,csvname );
	//TI2.CreateFlattenedCSV( branchList, "InputCSV/test2.csv");

	return(0);
}
