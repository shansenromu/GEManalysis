#include <iostream>
#include <fstream>
#include <sstream>
#include "TFile.h"
#include "TTree.h"

using namespace std;

int analyzeGEM() {
    // Open the data file
    ifstream infile("GEMsample/Test_0000.dat");
    if (!infile) {
        cerr << "Error: Unable to open the file!" << endl;
        return 1;
    }

    // Create a ROOT file
    TFile *file = new TFile("GEMdata.root", "RECREATE");
    if (!file || file->IsZombie()) {
        cerr << "Error: Unable to create ROOT file!" << endl;
        return 1;
    }

    // Create a ROOT tree
    TTree *tree = new TTree("data_tree", "Data Tree");

    // Define variables to hold data
    int channel;
    int time;
    float value;

    // Set branches in the tree
    tree->Branch("channel", &channel, "channel/I");
    tree->Branch("time", &time, "time/I");
    tree->Branch("value", &value, "value/F");

    // Read data from file and fill the tree
    string line;
    int row = 0;
    while (getline(infile, line)) {
        istringstream iss(line);
        int col = 0;
        string token;
        while (getline(iss, token, ' ')) {
            size_t comma_pos = token.find(",");
            if (comma_pos != string::npos) {
                channel = row;
                time = col;
                value = stof(token.substr(0, comma_pos) + "." + token.substr(comma_pos + 1));
                tree->Fill();
                col++;
            }
        }
        row++;
    }

    // Write the tree to the file
    tree->Write();

    // Close the file
    file->Close();

    // Clean up
    delete file;

    // Inform the user
    cout << "Data has been saved to GEMdata.root successfully!" << endl;

    return 0;
}
