//
// Created by Daniel Byomujuni on 10/9/23.
//

#include "CSVGateway.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>

#define csvFile "mangaUpdates.csv"
#define csvTempFile "mangaUpdatesTemp.csv"

CSVGateway::CSVGateway(std::string folder) {
    this->workingFolder = folder;
}

CSVGateway::CSVGateway() {
    this->workingFolder = std::string(getenv("HOME")) + "/Documents/";
}

void CSVGateway::setRecord(std::string MangaName, std::string MangaID, int32_t VolumesOwned) {

    if (this->doesRecordExist(MangaName)) {
        //std::cout << "e" << std::endl;
        this->updateRecord(MangaName, MangaID, VolumesOwned);
        return;
    }
    //std::cout << "n" << std::endl;
    this->appendRecord(MangaName, MangaID, VolumesOwned);
}

void CSVGateway::appendRecord(std::string MangaName, std::string MangaID, int32_t VolumesOwned) {
    // file pointer
    std::fstream fstream;
    fstream.open(this->workingFolder + csvFile, std::fstream::out | std::fstream::app);
    fstream << MangaName << "," << MangaID << "," << VolumesOwned << std::endl;

    fstream.close();
}

void CSVGateway::updateRecord(std::string MangaName, std::string MangaID, int32_t VolumesOwned) {
// File pointer
    std::fstream fin, fout;

    // Open an existing record
    fin.open(this->workingFolder + csvFile, std::fstream::in);

    // Create a new file to store updated data
    fout.open(this->workingFolder + csvTempFile, std::fstream::out);

    std::string line, word;
    std::vector<std::string> row;
    while (!fin.eof()) {

        row.clear();
        getline(fin, line);
        //std::cout << line << std::endl;
        std::stringstream s(line);

        while (std::getline(s, word, ',')) {
            //std::cout << word << std::endl;
            row.push_back(word);
        }

        std::vector<std::string>::iterator ptr;

        std::stringstream outString;
        for (ptr = row.begin(); ptr < row.end(); ptr++) {
            //std::cout << *ptr << std::endl;
            if (*ptr == MangaName) {
                outString << MangaName << "," << MangaID << "," << VolumesOwned << ",";
                break;
            }
            outString << *ptr + ',';
        }
        std::string str = outString.str();
        if (str == "\000") {
            break;
        }
        str.pop_back();
        fout << str << "\n";
        std::cout << std::endl;

        if (fin.eof()) {
            break;
        }
    }
    fin.close();
    fout.close();

    // removing the existing file
    std::remove((this->workingFolder + csvFile).c_str());
    // renaming the updated file with the existing file name
    std::rename((this->workingFolder + csvTempFile).c_str(), (this->workingFolder + csvFile).c_str());

}


bool CSVGateway::doesRecordExist(std::string MangaName) {
    return MangaName == this->getRecord(MangaName).title;
}

Manga CSVGateway::getRecord(std::string MangaName) {
    for (Manga mg: this->allRecords()) {
        if (mg.title == MangaName) {
            return mg;
        }
    }
    Manga nullManga;
    nullManga.title = "undefined";
    nullManga.id = "undefined";
    nullManga.volumes = 0;

    return nullManga;
};

std::vector<Manga> CSVGateway::allRecords() {
    std::vector<Manga> mangas;

    // File pointer
    std::fstream fin;

    // Open an existing file
    fin.open(this->workingFolder + csvFile, std::fstream::in);
    // Read the Data from the file
    // as String Vector
    std::vector <std::string> row;
    std::string line, word, temp;

    while (fin >> temp) {
        row.clear();

        // read an entire row and
        // store it in a string variable 'line'
        //std::getline(fin, line);
        //std::cout << line << std::endl;
        // used for breaking words
        std::stringstream s(temp);

        // read every column data of a row and
        // store it in a string variable, 'word'
        while (getline(s, word, ',')) {

            // add all the column data
            // of a row to a vector
            row.push_back(word);
        }
        if (row.size() != 3) {
            break;
        }

        Manga newManga;
        newManga.title = row.at(0);
        newManga.id = row.at(1);
        newManga.volumes = stoi(row.at(2));

        mangas.push_back(newManga);
    }

    //std::cout << mangas.size() << std::endl;
    return mangas;
};