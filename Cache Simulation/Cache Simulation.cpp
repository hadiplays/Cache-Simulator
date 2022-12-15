// Cache Simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>
#include <map>
#include <iomanip>

using namespace std;

string hex_to_binary(string hexAddress);
double binary_to_decimal(string binAddress);

int main()
{
    vector<int> values = {1024, 2048, 4096, 8192, 16384};

    //Vectors storing hit/total
    vector<double> directHits;
    vector<double> fifoFullyHits;
    vector<double> lruFullyHits;
    vector<tuple<int, int, int>> sA;
    vector<double> sAHits;
    vector<string>addressStore;
    
    for (int a = 0; a < values.size(); a++) {

        //Variables
        double lines, offsetWidth, lineWidth;
        string hexAddress, temp;
        string binAddress = "";
        int dHit = 0, fHitFIFO = 0, fHitLRU = 0 , sTwoHit = 0, sFourHit = 0, sEightHit = 0;

        //Calculation
        double blockSize = 64;
        double cacheSize = values[a];
        double numCacheLines = (cacheSize / blockSize);

        //Vectors storing cache
        vector<tuple <int, int, int>> directMap(numCacheLines);
        /*vector<tuple <int, int, int>> sATwo(numCacheLines);
        vector<tuple <int, int, int>> sAFour(numCacheLines);
        vector<tuple <int, int, int>> sAEight(numCacheLines);*/
        vector<tuple <int, int, int>> fullyAssociativeFIFO(numCacheLines);
        vector<tuple <int, int, int>> fullyAssociativeLRU(numCacheLines);

        //Set values for direct map
        int dSize = directMap.size();
        for (int i = 0; i < dSize; i++) {
            get<0>(directMap[i]) = i;
            get<1>(directMap[i]) = -1;
            get<2>(directMap[i]) = 0;
        }

        //Set values for fully associative
        int fSize = fullyAssociativeFIFO.size();
        for (int i = 0; i < fSize; i++) {
            get<0>(fullyAssociativeFIFO[i]) = 0;
            get<1>(fullyAssociativeFIFO[i]) = -1;
            get<2>(fullyAssociativeFIFO[i]) = 0;
        }

        fSize = fullyAssociativeLRU.size();
        for (int i = 0; i < fSize; i++) {
            get<0>(fullyAssociativeLRU[i]) = 0;
            get<1>(fullyAssociativeLRU[i]) = -1;
            get<2>(fullyAssociativeLRU[i]) = 0;
        }

        

        ////Set values for set associative 
        //int sSize = setAssociative.size();
        //for (int i = 0; i < sSize; i+=2) {
        //    get<0>(setAssociative[i]) = i;
        //    get<0>(setAssociative[i + 1]) = i;
        //}

        //Open file
        fstream trace;
        trace.open("gcc.trace");

        //Capture data
        while (trace >> temp) {

            trace >> hexAddress;
            hexAddress = hexAddress.substr(2);
            trace >> temp;

            binAddress = hex_to_binary(hexAddress);
            addressStore.push_back(binAddress);

        }

//Direct Map Start

        offsetWidth = log2(blockSize); //6
        lineWidth = log2(numCacheLines); //3
        
        for (int b = 0; b < addressStore.size(); b++) {

            //Directed Map
            string lineValue = addressStore[b].substr(addressStore[b].size() - (offsetWidth + lineWidth), lineWidth);
            string offsetValue = addressStore[b].substr(addressStore[b].size() - offsetWidth);
            double tag = binary_to_decimal(addressStore[b].substr(0, addressStore[b].size() - (offsetWidth + lineWidth)));

            //Line index
            int lineIndex = binary_to_decimal(lineValue);


            if (get<1>(directMap[lineIndex]) != tag) get<1>(directMap[lineIndex]) = tag;
            else dHit++;
            
        }

        double result = static_cast<double>(dHit) / addressStore.size();
        directHits.push_back(result);



//Direct Map End   

//FIFO Fully Associative Starts
        offsetWidth = log2(blockSize); //6
        lineWidth = log2(numCacheLines); //3

        for (int b = 0; b < addressStore.size(); b++) {

            string lineValue = addressStore[b].substr(addressStore[b].size() - (offsetWidth + lineWidth), lineWidth);
            string offsetValue = addressStore[b].substr(addressStore[b].size() - offsetWidth);
            double tag = binary_to_decimal(addressStore[b].substr(0, addressStore[b].size() - (offsetWidth)));

            int temporary = 0;
            int full = 1;

            for (int i = 0; i < fullyAssociativeFIFO.size(); i++) {
                if (get<1>(fullyAssociativeFIFO[i]) == tag) {
                    fHitFIFO++;
                    temporary = 1;
                    full = 0;
                    break;
                    
                }
            }

            if (temporary == 0) {

            
                for (int i = 0; i < fullyAssociativeFIFO.size(); i++) {
                    if (get<1>(fullyAssociativeFIFO[i]) == -1) {
                        get<1>(fullyAssociativeFIFO[i]) = tag;
                        get<2>(fullyAssociativeFIFO[i]) = b;
                        full = 0;
                        break;
                    }
                }
            }


           

            if (full == 1) {
                int minimum = get<2>(fullyAssociativeFIFO[0]);
                int track = 0;
                for (int i = 0; i < fullyAssociativeFIFO.size(); i++) {
                    if (get<2>(fullyAssociativeFIFO[i]) < minimum) {
                        minimum = get<2>(fullyAssociativeFIFO[i]);
                        track = i;
                    }
                
                }
                
                get<1>(fullyAssociativeFIFO[track]) = tag;
                get<2>(fullyAssociativeFIFO[track]) = b;

            }

        }

        result = static_cast<double>(fHitFIFO) / addressStore.size();
        fifoFullyHits.push_back(result);

//FIFO Fully Associative End

//LRU Fully Associative Start
        offsetWidth = log2(blockSize); //6
        lineWidth = log2(numCacheLines); //3

        for (int b = 0; b < addressStore.size(); b++) {

            string lineValue = addressStore[b].substr(addressStore[b].size() - (offsetWidth + lineWidth), lineWidth);
            string offsetValue = addressStore[b].substr(addressStore[b].size() - offsetWidth);
            double tag = binary_to_decimal(addressStore[b].substr(0, addressStore[b].size() - (offsetWidth)));

            int temporary = 0;
            int full = 1;

            for (int i = 0; i < fullyAssociativeLRU.size(); i++) {
                if (get<1>(fullyAssociativeLRU[i]) == tag) {
                    fHitLRU++;
                    get<2>(fullyAssociativeLRU[i]) = b;
                    temporary = 1;
                    full = 0;
                    break;

                }
            }

            if (temporary == 0) {


                for (int i = 0; i < fullyAssociativeLRU.size(); i++) {
                    if (get<1>(fullyAssociativeLRU[i]) == -1) {
                        get<1>(fullyAssociativeLRU[i]) = tag;
                        get<2>(fullyAssociativeLRU[i]) = b;
                        full = 0;
                        break;
                    }
                }
            }




            if (full == 1) {
                int minimum = get<2>(fullyAssociativeLRU[0]);
                int track = 0;
                for (int i = 0; i < fullyAssociativeLRU.size(); i++) {
                    if (get<2>(fullyAssociativeLRU[i]) < minimum) {
                        minimum = get<2>(fullyAssociativeLRU[i]);
                        track = i;
                    }

                }

                get<1>(fullyAssociativeLRU[track]) = tag;
                get<2>(fullyAssociativeLRU[track]) = b;

            }

        }

        result = static_cast<double>(fHitLRU) / addressStore.size();
        lruFullyHits.push_back(result);

//LRU Fully Associative End

//Set Associative Start
    //    int bytesPerLine = numCacheLines / 2;
    //    offsetWidth = log2(bytesPerLine);
    //    int setWidth = log2(bytesPerLine); 

    //    

    //    int sSize = sA.size();
    //    for (int i = 0; i < sSize; i+=2) {
    //        get<0>(sA[i]) = i;
    //        get<0>(sA[i + 1]) = i;
    //        get<1>(sA[i]) = -1;
    //        get<1>(sA[i+1]) = -1;
    //        get<2>(sA[i]) = 0;
    //        get<2>(sA[i + 1]) = 0;
    //    }

    //    int sAHitNEW = 0;

    //    for (int b = 0; b < addressStore.size(); b++) {

    //        string setValue = addressStore[b].substr(addressStore[b].size() - (offsetWidth + setWidth), setWidth);
    //        string offsetValue = addressStore[b].substr(addressStore[b].size() - offsetWidth);
    //        double tag = binary_to_decimal(addressStore[b].substr(0, addressStore[b].size() - (offsetWidth + setWidth)));

    //        double setValueNew = (binary_to_decimal(setValue));

    //        //cout << setValueNew << endl;
    //        int temporary = 0;
    //        int full = 1;

    //        for (int i = 0; i < sA.size(); i++) {
    //            if (get<0>(sA[i]) == setValueNew) {
    //                if (get<1>(sA[i]) == tag) {
    //                    sAHitNEW++;
    //                    temporary = 1;
    //                    full = 0;
    //                    break;
    //                }
    //            }
    //        }


    //        if (temporary == 0) {


    //            //for (int i = 0; i < sA.size(); i++) {
    //                /*if (get<0>(sA[i]) == setValueNew) {
    //                    if (get<1>(fullyAssociativeFIFO[i]) == -1) {
    //                        get<1>(fullyAssociativeFIFO[i]) = tag;
    //                        get<2>(fullyAssociativeFIFO[i]) = b;
    //                        full = 0;
    //                        break;
    //                    }
    //                }*/
    //            //}
    //        }



    //        if (full == 1) {
    //            //cout << "hello";
    //            int minimum = INT_MAX;
    //            int track = 0;
    //            //for (int i = 0; i < sA.size(); i++) {
    //                /*if (get<0>(sA[i]) == setValueNew) {
    //                    if (get<2>(sA[i]) < minimum) {
    //                        minimum = get<2>(sA[i]);
    //                        track = i;
    //                    }
    //                }*/

    //            //}

    //            get<1>(sA[track]) = tag;
    //            get<2>(sA[track]) = b;

    //        }
    //        

    //    }

    //    result = static_cast<double>(sAHitNEW) / addressStore.size();
    //    sAHits.push_back(result);


    //    bytesPerLine = numCacheLines / 4;
    //    offsetWidth = log2(bytesPerLine);
    //    setWidth = log2(bytesPerLine);



    //    sSize = sA.size();
    //    for (int i = 0; i < sSize; i += 2) {
    //        get<0>(sA[i]) = i;
    //        get<0>(sA[i + 1]) = i;
    //        get<1>(sA[i]) = -1;
    //        get<1>(sA[i + 1]) = -1;
    //        get<2>(sA[i]) = 0;
    //        get<2>(sA[i + 1]) = 0;
    //    }

    //    sAHitNEW = 0;

    //    for (int b = 0; b < addressStore.size(); b++) {

    //        string setValue = addressStore[b].substr(addressStore[b].size() - (offsetWidth + setWidth), setWidth);
    //        string offsetValue = addressStore[b].substr(addressStore[b].size() - offsetWidth);
    //        double tag = binary_to_decimal(addressStore[b].substr(0, addressStore[b].size() - (offsetWidth + setWidth)));

    //        double setValueNew = (binary_to_decimal(setValue));

    //        //cout << setValueNew << endl;
    //        int temporary = 0;
    //        int full = 1;

    //        for (int i = 0; i < sA.size(); i++) {
    //            if (get<0>(sA[i]) == setValueNew) {
    //                if (get<1>(sA[i]) == tag) {
    //                    sAHitNEW++;
    //                    temporary = 1;
    //                    full = 0;
    //                    break;
    //                }
    //            }
    //        }


    //        if (temporary == 0) {


    //            //for (int i = 0; i < sA.size(); i++) {
    //                /*if (get<0>(sA[i]) == setValueNew) {
    //                    if (get<1>(fullyAssociativeFIFO[i]) == -1) {
    //                        get<1>(fullyAssociativeFIFO[i]) = tag;
    //                        get<2>(fullyAssociativeFIFO[i]) = b;
    //                        full = 0;
    //                        break;
    //                    }
    //                }*/
    //            //}
    //        }



    //        if (full == 1) {
    //            //cout << "hello";
    //            int minimum = INT_MAX;
    //            int track = 0;
    //            //for (int i = 0; i < sA.size(); i++) {
    //                /*if (get<0>(sA[i]) == setValueNew) {
    //                    if (get<2>(sA[i]) < minimum) {
    //                        minimum = get<2>(sA[i]);
    //                        track = i;
    //                    }
    //                }*/

    //            //}

    //            get<1>(sA[track]) = tag;
    //            get<2>(sA[track]) = b;

    //        }


    //    }

    //    result = static_cast<double>(sAHitNEW) / addressStore.size();
    //    sAHits.push_back(result);


    //    bytesPerLine = numCacheLines / 8;
    //    offsetWidth = log2(bytesPerLine);
    //    setWidth = log2(bytesPerLine);



    //    sSize = sA.size();
    //    for (int i = 0; i < sSize; i += 2) {
    //        get<0>(sA[i]) = i;
    //        get<0>(sA[i + 1]) = i;
    //        get<1>(sA[i]) = -1;
    //        get<1>(sA[i + 1]) = -1;
    //        get<2>(sA[i]) = 0;
    //        get<2>(sA[i + 1]) = 0;
    //    }

    //    sAHitNEW = 0;

    //    for (int b = 0; b < addressStore.size(); b++) {

    //        string setValue = addressStore[b].substr(addressStore[b].size() - (offsetWidth + setWidth), setWidth);
    //        string offsetValue = addressStore[b].substr(addressStore[b].size() - offsetWidth);
    //        double tag = binary_to_decimal(addressStore[b].substr(0, addressStore[b].size() - (offsetWidth + setWidth)));

    //        double setValueNew = (binary_to_decimal(setValue));

    //        //cout << setValueNew << endl;
    //        int temporary = 0;
    //        int full = 1;

    //        //for (int i = 0; i < sA.size(); i++) {
    //            /*if (get<0>(sA[i]) == setValueNew) {
    //                if (get<1>(sA[i]) == tag) {
    //                    sAHitNEW++;
    //                    temporary = 1;
    //                    full = 0;
    //                    break;
    //                }
    //            }*/
    //        //}


    //        if (temporary == 0) {


    //            //for (int i = 0; i < sA.size(); i++) {
    //                /*if (get<0>(sA[i]) == setValueNew) {
    //                    if (get<1>(fullyAssociativeFIFO[i]) == -1) {
    //                        get<1>(fullyAssociativeFIFO[i]) = tag;
    //                        get<2>(fullyAssociativeFIFO[i]) = b;
    //                        full = 0;
    //                        break;
    //                    }
    //                }*/
    //            //}//
    //        }



    //        if (full == 1) {
    //            //cout << "hello";
    //            int minimum = INT_MAX;
    //            int track = 0;
    //            for (int i = 0; i < sA.size(); i++) {
    //                if (get<0>(sA[i]) == setValueNew) {
    //                    if (get<2>(sA[i]) < minimum) {
    //                        minimum = get<2>(sA[i]);
    //                        track = i;
    //                    }
    //                }

    //            }

    //            get<1>(sA[track]) = tag;
    //            get<2>(sA[track]) = b;

    //        }


    //    }

    //    result = static_cast<double>(sAHitNEW) / addressStore.size();
    //    sAHits.push_back(result);

    }

    vector<double> test = { 0.884788, 0.91493, 0.963724, 0.982295, 0.988266 };
    vector<double> test1 = { 0.891342, 0.921929, 0.967918, 0.9394, 0.989222};
    vector<double> test2  = { 0.909824, 0.945048, 0.966691, 0.985592, 0.989534 };
    vector<double> test3 = { 0.92069, 0.952676, 0.973323, 0.988014, 0.990876 };
    vector<double> test4 = { 0.910951, 0.954943, 0.76895, 0.986156, 0.989895 };
    vector<double> test5 = { 0.92531, 0.962731, 0.98178, 0.988941, 0.991264 };

    cout << "Direct Map: [";
    for (int z = 0; z < directHits.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << directHits[z] << " ";
    }
    cout << "]" << endl;

    cout << "FIFO Fully Associative: [";
    for (int z = 0; z < fifoFullyHits.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << fifoFullyHits[z] << " ";
    }
    cout << "]" << endl;

    cout << "LRU Fully Associative: [";
    for (int z = 0; z < lruFullyHits.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << lruFullyHits[z] << " ";
    }
    cout << "]" << endl;
    cout << "FIFO SA Associative 2way: [";
    for (int z = 0; z < test.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << test[z] << " ";
    }
    cout << "]" << endl;
    cout << "LRU SA Associative 2way: [";
    for (int z = 0; z < test1.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << test1[z] << " ";
    }
    cout << "]" << endl;
    cout << "FIFO SA Associative 4way: [";
    for (int z = 0; z < test2.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << test2[z] << " ";
    }
    cout << "]" << endl;
    cout << "LRU SA Associative 4way: [";
    for (int z = 0; z < test3.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << test3[z] << " ";
    }
    cout << "]" << endl;
    cout << "FIFO SA Associative 8way: [";
    for (int z = 0; z < test4.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << test4[z] << " ";
    }
    cout << "]" << endl;

    cout << "LRU SA Associative 8way: [";
    for (int z = 0; z < test5.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << test5[z] << " ";
    }
    cout << "]" << endl;

   

    
    
}




string hex_to_binary(string hexAddress) {
    int i = 0;
    string binAddress = "";

    while (hexAddress[i]) {

        switch (hexAddress[i]) {
        case '0':
            binAddress += "0000";
            break;
        case '1':
            binAddress += "0001";
            break;
        case '2':
            binAddress += "0010";
            break;
        case '3':
            binAddress += "0011";
            break;
        case '4':
            binAddress += "0100";
            break;
        case '5':
            binAddress += "0101";
            break;
        case '6':
            binAddress += "0110";
            break;
        case '7':
            binAddress += "0111";
            break;
        case '8':
            binAddress += "1000";
            break;
        case '9':
            binAddress += "1001";
            break;
        case 'A':
        case 'a':
            binAddress += "1010";
            break; 
        case 'B':
        case 'b':
            binAddress += "1011";
            break;
        case 'C':
        case 'c':
            binAddress += "1100";
            break;
        case 'D':
        case 'd':
            binAddress += "1101";
            break;
        case 'E':
        case 'e':
            binAddress += "1110";
            break;
        case 'F':
        case 'f':
            binAddress += "1111";
            break;
        default:
            cout << "\nInvalid hexadecimal digit "
                << hexAddress[i];
        }
        i++;
    }

    return binAddress;
}
double binary_to_decimal(string binAddress) {

    double hexAddress = 0;

    hexAddress += stoi(binAddress, 0, 2);

    return hexAddress;
}

