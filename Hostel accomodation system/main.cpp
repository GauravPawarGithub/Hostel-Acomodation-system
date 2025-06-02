#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Hostel {
private:
    string Name;
    int Rent;
    int Bed;
public:
    Hostel(string name, int rent, int bed) : Name(name), Rent(rent), Bed(bed) {}

    string getName() const { return Name; }
    int getRent() const { return Rent; }
    int getBed() const { return Bed; }

    // Reads the current bed count from the file and updates the object
    void updateBedFromFile() {
        ifstream in("Hostel.txt");
        string line;
        while (getline(in, line)) {
            if (line.find(Name) != string::npos) {
                stringstream ss(line);
                string temp;
                getline(ss, temp, ':'); // name
                getline(ss, temp, ':'); // rent
                getline(ss, temp);      // bed
                Bed = stoi(temp);
                break;
            }
        }
        in.close();
    }

    // Updates the bed count in the file and in the object
    bool reserve() {
        if (Bed <= 0) {
            cout << "\tSorry, Bed is not available!\n";
            return false;
        }

        ifstream in("Hostel.txt");
        if (!in) {
            cerr << "Error: Could not open Hostel.txt for reading.\n";
            return false;
        }
        ofstream out("Hostel_Temp.txt");
        if (!out) {
            cerr << "Error: Could not create Hostel_Temp.txt.\n";
            in.close();
            return false;
        }

        string line;
        bool updated = false;
        while (getline(in, line)) {
            if (line.find(Name) != string::npos && !updated) {
                // Update bed count
                int bedPos = line.find_last_of(':');
                if (bedPos != string::npos) {
                    int newBed = Bed - 1;
                    string newLine = line.substr(0, bedPos + 1) + " " + to_string(newBed);
                    out << newLine << endl;
                    Bed = newBed; // Update in-memory bed count
                    updated = true;
                    continue;
                }
            }
            out << line << endl;
        }
        in.close();
        out.close();

        // Replace original file with updated file
        if (remove("Hostel.txt") != 0 || rename("Hostel_Temp.txt", "Hostel.txt") != 0) {
            cerr << "Error updating Hostel.txt.\n";
            return false;
        } else {
            cout << "\tBed Reserved successfully!\n";
            return true;
        }
    }
};

class Student {
private:
    string Name, RollNo, Address;
public:
    Student() : Name(""), RollNo(""), Address("") {}

    void setName(const string& name) { Name = name; }
    void setRollNo(const string& rollno) { RollNo = rollno; }
    void setAddress(const string& address) { Address = address; }

    string getName() const { return Name; }
    string getRollNo() const { return RollNo; }
    string getAddress() const { return Address; }
};

int main() {
    Hostel h("3star", 5000, 2);
    Student s;

    // Save initial hostel data if file doesn't exist
    ifstream checkFile("Hostel.txt");
    if (!checkFile) {
        ofstream myfile("Hostel.txt");
        if (!myfile) {
            cerr << "Error: Could not open Hostel.txt for writing.\n";
            return 1;
        }
        myfile << h.getName() << " : " << h.getRent() << " : " << h.getBed() << endl;
        myfile.close();
        cout << "Hostel Data Saved\n";
    }
    checkFile.close();

    bool exit = false;
    while (!exit) {
        h.updateBedFromFile(); // Always get latest bed count from file

        cout << "\n\tWelcome to Hostel Accommodation System\n";
        cout << "\t*************************************\n";
        cout << "\tHostel: " << h.getName() << " | Rent: " << h.getRent() << " | Beds Available: " << h.getBed() << endl;
        cout << "\t1. Reserve A Bed.\n";
        cout << "\t2. Exit.\n";
        cout << "\tEnter Choice: ";
        int val;
        cin >> val;
        cin.ignore(); // Clear newline from input buffer

        if (val == 1) {
            string name, rollno, address;
            cout << "\tEnter Name of Student: ";
            getline(cin, name);
            cout << "\tEnter RollNo of Student: ";
            getline(cin, rollno);
            cout << "\tEnter Address of Student: ";
            getline(cin, address);

            s.setName(name);
            s.setRollNo(rollno);
            s.setAddress(address);

            if (h.reserve()) {
                // Save student data
                ofstream myfile2("Student.txt", ios::app);
                if (!myfile2) {
                    cerr << "Error: Could not open Student.txt for writing.\n";
                } else {
                    myfile2 << s.getName() << " : " << s.getRollNo() << " : " << s.getAddress() << endl;
                    myfile2.close();
                }
            }
        } else if (val == 2) {
            exit = true;
            cout << "Good Luck!\n";
        } else {
            cout << "\tInvalid choice!\n";
        }
    }
    return 0;
}
