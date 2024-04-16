
#include "TTreeInterface.h"



int main(){

	//small test file
	//std::string fname = "InputFile/GMSB_AOD_v13_GMSB_L-350TeV_Ctau-200cm_AODSIM_RunIIFall17DRPremix_output99.root";
	std::string fname = "InputFile/kucmsntuple_GMSB_AOD_v14_GMSB_L-200TeV_Ctau-200cm_AODSIM_RunIIFall17DRPremix.root";
	
	//bigger file with newer ntuple version
	
	std::string tname = "tree/llpgtree";
	std::string flistname = "InputFileLists/test.list";
	
	///loading a single file///
	//TTreeInterface TI(fname,tname,false);//load small older ntuple with false
	TTreeInterface TI(fname,tname,true);//load new big file with true (auto load labels)
	//TI._ttree->Print();
	
	
	///example code to load tchain///
	//std::vector<std::string> fileList{};
	//fileList = ReadList(flistname);
	//TTreeInterface TI2(fileList, tname);
	//TI2._tchain->Print();
	

    //this branch list would use the older version, manually put in the label branch "photon_genllpid"
	//std::vector<std::string> branchList{"Photon_genLlpId", "Photon_pt", "Photon_pz", "Photon_eta", "Photon_salp", "Photon_smaj", "Photon_smin", "Photon_trkSumPtSolidConeDR04", "Photon_trkSumPtHollowConeDR04"  };
	
	//this branch list assumes a newever version, it will automatically generate the labels, do not include the brance (it doesn't exist)
	//std::vector<std::string> branchList{ "Photon_pt", "Photon_pz", "Photon_eta", "Photon_salp", "Photon_smaj", "Photon_smin", "Photon_trkSumPtSolidConeDR04", "Photon_trkSumPtHollowConeDR04"  };
	std::vector<std::string> branchList{ "Photon_pt", "Photon_pz", "Photon_eta", "Photon_trkSumPtSolidConeDR04", "Photon_trkSumPtHollowConeDR04"  };
	//abstracted idx chasing, provide mapping (idxbranch:targetbranch, output header name)
	TI.MapIdx("Photon_genIdx" , "Gen_susId", "Photon_genLlpId" );
	TI.MapIdx("Photon_scIndex" , "SuperCluster_salp", "Photon_salp" );
	TI.MapIdx("Photon_scIndex" , "SuperCluster_smaj", "Photon_smaj" );
	TI.MapIdx("Photon_scIndex" , "SuperCluster_smin", "Photon_smin" );
	
	std::string csvname = "InputCSV/test_FullabstractionMap_new_v14.csv";
	TI.CreateFlattenedCSV( branchList,csvname );
	//TI2.CreateFlattenedCSV( branchList, "InputCSV/test2.csv");

	return(0);
}
