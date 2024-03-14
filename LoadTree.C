#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> // for std::setw and std::setfill
#include <string>
#include "TFile.h"
#include "TTree.h"

using namespace std;

int LoadTree() {
    // Create a ROOT file
    TFile *file = new TFile("GEMdata.root", "RECREATE");
    if (!file || file->IsZombie()) {
        cerr << "Error: Unable to create ROOT file!" << endl;
        return 1;
    }
    cout<<"Root File created."<<endl;
    // Create a ROOT tree
    TTree *tree = new TTree("data_tree", "Data Tree");

    // Define variables to hold data
    int channel;
    int time;
    double current;
    int NumFile; 

    // Set branches in the tree
    tree->Branch("channel", &channel, "channel/I");
    tree->Branch("time", &time, "time/I");
    tree->Branch("current", &current, "current/D");
    tree->Branch("NumFile", &NumFile, "NumFile/I"); // Use char array for fileNum
    cout<<"Root Tree initialized."<<endl;
    // Loop over all files Test_0000.dat, Test_0001.dat, etc.
    const int numFiles = 201; // Highest file num+1 for *0000.dat
    for (int i = 0; i < numFiles;i++) {
        // Construct the file name with leading zeros
        ostringstream filename;
        filename << "GEMsample/Test_" << setw(4) << setfill('0') << i << ".dat";

        // Open the data file
        ifstream infile(filename.str());
        if (!infile) {
            cerr << "Error: Unable to open the file " << filename.str() << endl;
            continue; // Move to the next file if this one cannot be opened
        }
        cout<<"Opened file "<<filename.str()<<endl;
        // Set the file number

	for (int headerline = 0; headerline<9; headerline++){
	  infile.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	int numRows = 256;
	int numCols = 512;

        // Read data from file and fill the tree
        string line;
        //int timeIndex=0;
	int channelIndex=1;

	while(getline(infile,line)){
	  istringstream iss(line);
	  
	  //Loop over all time indices
	  for(int timeIndex=0; timeIndex<numCols; timeIndex++){

	    string data;
	    if(getline(iss, data, '\t')){
	      size_t commaPos = data.find(",");
	      if(commaPos != string::npos){
		string integerPart = data.substr(0,commaPos);
		string decimalPart = data.substr(commaPos+1);
		current= stod(integerPart+'.'+decimalPart);
		channel = channelIndex;
		time = timeIndex;
		NumFile = i;

		cout<<time<<" "<<channel<<" "<<NumFile<<" "<<current<<endl;

		tree->Fill();
		
	      }
	    }
	  }
	  
	  
	  
	  channelIndex++;
	}

        infile.close();
	cout<<"File closed "<<NumFile<<endl;
    }

    // Write the tree to the file
    cout<<"Saving Tree "<<endl;
    tree->Write();
    cout<<"Tree Saved "<<endl;
    // Close the file
    file->Close();
    cout<<"root file closed "<<endl;

    // Clean up
    //delete file;
    //delete tree;

    // Inform the user
    cout << "Data has been saved to GEMdata.root successfully!" << endl;

    return 0;
}
