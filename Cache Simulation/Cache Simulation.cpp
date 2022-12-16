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
    vector<double> fifoSetTwoHits;
    vector<double> lruSetTwoHits;
    vector<double> testing;
    vector<double> fifoSetFourHits;
    vector<double> fifoSetEightHits;
    vector<double> lruSetFourHits;
    vector<double> lruSetEightHits;
    vector<string>addressStore;
    
    for (int a = 0; a < values.size(); a++) {

        //Variables
        double lines, offsetWidth, lineWidth;
        string hexAddress, temp;
        string binAddress = "";
        int dHit = 0, fHitFIFO = 0, fHitLRU = 0, sTwoHitFIFO = 0, fifoSFourHit = 0, fifoSEightHit = 0, sTwoHitLRU = 0, lruSFourHit = 0, lruSEightHit = 0, testingNum = 0;

        //Calculation
        double blockSize = 64;
        double cacheSize = values[a];
        double numCacheLines = (cacheSize / blockSize);

        //Vectors storing cache
        vector<tuple <int, int, int>> directMap(numCacheLines);
        vector<tuple <int, int, int>> sATwoFIFO(numCacheLines);
        vector<tuple <int, int, int>> setFourFIFO(numCacheLines);
        vector<tuple <int, int, int>> setEightFIFO(numCacheLines);
        vector<tuple <int, int, int>> sATwoLRU(numCacheLines);
        vector<tuple <int, int, int>> setFourLRU(numCacheLines);
        vector<tuple <int, int, int>> setEightLRU(numCacheLines);
        vector<tuple <int, int, int>> fullyAssociativeFIFO(numCacheLines);
        vector<tuple <int, int, int>> fullyAssociativeLRU(numCacheLines);
        vector<tuple <int, int, int>> testingCache(numCacheLines);


        //Set values for direct map
        int dSize = directMap.size();
        for (int i = 0; i < dSize; i++) {
            get<0>(directMap[i]) = i;
            get<1>(directMap[i]) = -1;
            get<2>(directMap[i]) = 0;
        }

        //Set values for fully associative FIFO
        int fSize = fullyAssociativeFIFO.size();
        for (int i = 0; i < fSize; i++) {
            get<0>(fullyAssociativeFIFO[i]) = 0;
            get<1>(fullyAssociativeFIFO[i]) = -1;
            get<2>(fullyAssociativeFIFO[i]) = 0;
        }

        //Set values for fully associative LRU
        fSize = fullyAssociativeLRU.size();
        for (int i = 0; i < fSize; i++) {
            get<0>(fullyAssociativeLRU[i]) = 0;
            get<1>(fullyAssociativeLRU[i]) = -1;
            get<2>(fullyAssociativeLRU[i]) = 0;
        }

        //Set values for set associative FIFO 2 way
        int sSize = sATwoFIFO.size();
        int counter = 0;
        for (int i = 0; i < sSize; i+=2) {
            
            get<0>(sATwoFIFO[i]) = counter;
            get<0>(sATwoFIFO[i + 1]) = counter;
            get<1>(sATwoFIFO[i]) = -1;
            get<1>(sATwoFIFO[i + 1]) = -1;
            //get<2>(sATwoFIFO[i]) = 0;
            //get<2>(sATwoFIFO[i]) = 0;

            counter++;
        }
        //Set values for set associative LRU 2 way
        sSize = testingCache.size();
        counter = 0;
        for (int i = 0; i < sSize; i += 2) {

            get<0>(testingCache[i]) = counter;
            get<0>(testingCache[i + 1]) = counter;
            get<1>(testingCache[i]) = -1;
            get<1>(testingCache[i + 1]) = -1;
            //get<2>(sATwoLRU[i]) = 0;
            //get<2>(sATwoLRU[i]) = 0;

            counter++;
        }

        sSize = setFourFIFO.size();
        counter = 0;
        for (int i = 0; i < sSize; i += 4) {

            get<0>(setFourFIFO[i]) = counter;
            get<0>(setFourFIFO[i + 1]) = counter;
            get<0>(setFourFIFO[i + 2]) = counter;
            get<0>(setFourFIFO[i + 3]) = counter;
            get<1>(setFourFIFO[i]) = -1;
            get<1>(setFourFIFO[i + 1]) = -1;
            get<1>(setFourFIFO[i + 2]) = -1;
            get<1>(setFourFIFO[i + 3]) = -1;
            //get<2>(sATwoLRU[i]) = 0;
            //get<2>(sATwoLRU[i]) = 0;

            counter++;
        }

        sSize = setFourLRU.size();
        counter = 0;
        for (int i = 0; i < sSize; i += 4) {

            get<0>(setFourLRU[i]) = counter;
            get<0>(setFourLRU[i + 1]) = counter;
            get<0>(setFourLRU[i + 2]) = counter;
            get<0>(setFourLRU[i + 3]) = counter;
            get<1>(setFourLRU[i]) = -1;
            get<1>(setFourLRU[i + 1]) = -1;
            get<1>(setFourLRU[i + 2]) = -1;
            get<1>(setFourLRU[i + 3]) = -1;
            //get<2>(sATwoLRU[i]) = 0;
            //get<2>(sATwoLRU[i]) = 0;

            counter++;
        }

        sSize = setEightFIFO.size();
        counter = 0;
        for (int i = 0; i < sSize; i += 8) {

            get<0>(setEightFIFO[i]) = counter;
            get<0>(setEightFIFO[i + 1]) = counter;
            get<0>(setEightFIFO[i + 2]) = counter;
            get<0>(setEightFIFO[i + 3]) = counter;
            get<0>(setEightFIFO[i + 4]) = counter;
            get<0>(setEightFIFO[i + 5]) = counter;
            get<0>(setEightFIFO[i + 6]) = counter;
            get<0>(setEightFIFO[i + 7]) = counter;
            get<1>(setEightFIFO[i]) = -1;
            get<1>(setEightFIFO[i + 1]) = -1;
            get<1>(setEightFIFO[i + 2]) = -1;
            get<1>(setEightFIFO[i + 3]) = -1;
            get<1>(setEightFIFO[i + 4]) = -1;
            get<1>(setEightFIFO[i + 5]) = -1;
            get<1>(setEightFIFO[i + 6]) = -1;
            get<1>(setEightFIFO[i + 7]) = -1;

            //get<2>(sATwoLRU[i]) = 0;
            //get<2>(sATwoLRU[i]) = 0;

            counter++;
        }

        sSize = setEightLRU.size();
        counter = 0;
        for (int i = 0; i < sSize; i += 8) {

            get<0>(setEightLRU[i]) = counter;
            get<0>(setEightLRU[i + 1]) = counter;
            get<0>(setEightLRU[i + 2]) = counter;
            get<0>(setEightLRU[i + 3]) = counter;
            get<0>(setEightLRU[i + 4]) = counter;
            get<0>(setEightLRU[i + 5]) = counter;
            get<0>(setEightLRU[i + 6]) = counter;
            get<0>(setEightLRU[i + 7]) = counter;
            get<1>(setEightLRU[i]) = -1;
            get<1>(setEightLRU[i + 1]) = -1;
            get<1>(setEightLRU[i + 2]) = -1;
            get<1>(setEightLRU[i + 3]) = -1;
            get<1>(setEightLRU[i + 4]) = -1;
            get<1>(setEightLRU[i + 5]) = -1;
            get<1>(setEightLRU[i + 6]) = -1;
            get<1>(setEightLRU[i + 7]) = -1;
            //get<2>(sATwoLRU[i]) = 0;
            //get<2>(sATwoLRU[i]) = 0;

            counter++;
        }
        /*for (int i = 0; i < sSize; i++) {
            cout << get<0>(sATwoFIFO[i]) << " " << get<1>(sATwoFIFO[i]) << " " << get<2>(sATwoFIFO[i]) << endl;
        }*/


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

//FIFO Set Associative Start

      //Two
        int bytesPerLine = numCacheLines / 2;
        offsetWidth = log2(blockSize);
        int setWidth = log2(bytesPerLine); 
        //cout << setWidth;

        for (int b = 0; b < addressStore.size(); b++) {

            string setValue = addressStore[b].substr(addressStore[b].size() - (offsetWidth + setWidth), setWidth);
            string offsetValue = addressStore[b].substr(addressStore[b].size() - offsetWidth);
            double tag = binary_to_decimal(addressStore[b].substr(0, addressStore[b].size() - (offsetWidth + setWidth)));

            //cout << tag << setValue << offsetValue << endl;
            double setValueNew = (binary_to_decimal(setValue));

            int temporary = 0;
            int full = 1;

            for (int i = 0; i < sATwoFIFO.size(); i++) {
                if (get<0>(sATwoFIFO[i]) == setValueNew) {
                    if (get<1>(sATwoFIFO[i]) == tag) {
                        sTwoHitFIFO++;
                        temporary = 1;
                        full = 0;
                        break;
                    }
                }
            }


            if (temporary == 0) {

                for (int i = 0; i < sATwoFIFO.size(); i++) {
                    if (get<0>(sATwoFIFO[i]) == setValueNew) {
                        if (get<1>(sATwoFIFO[i]) == -1) {
                            get<1>(sATwoFIFO[i]) = tag;
                            get<2>(sATwoFIFO[i]) = b;
                            full = 0;
                            break;
                        }
                    }
                }
            }



            if (full == 1) {
                //cout << "hello";
                int minimum = get<2>(sATwoFIFO[0]);
                int track = 0;
                for (int i = 0; i < sATwoFIFO.size(); i++) {
                    if (get<0>(sATwoFIFO[i]) == setValueNew) {
                        if (get<2>(sATwoFIFO[i]) < minimum) {
                            minimum = get<2>(sATwoFIFO[i]);
                            track = i;
                        }
                    }

                }

                get<1>(sATwoFIFO[track]) = tag;
                get<2>(sATwoFIFO[track]) = b;

            }
            


        }
        result = static_cast<double>(sTwoHitFIFO) / addressStore.size();
        fifoSetTwoHits.push_back(result);

      //Four
        bytesPerLine = numCacheLines / 4;
        offsetWidth = log2(blockSize);
        setWidth = log2(bytesPerLine);
        //cout << setWidth;

        for (int b = 0; b < addressStore.size(); b++) {

            string setValue = addressStore[b].substr(addressStore[b].size() - (offsetWidth + setWidth), setWidth);
            string offsetValue = addressStore[b].substr(addressStore[b].size() - offsetWidth);
            double tag = binary_to_decimal(addressStore[b].substr(0, addressStore[b].size() - (offsetWidth + setWidth)));

            //cout << tag << setValue << offsetValue << endl;
            double setValueNew = (binary_to_decimal(setValue));

            int temporary = 0;
            int full = 1;

            for (int i = 0; i < setFourFIFO.size(); i++) {
                if (get<0>(setFourFIFO[i]) == setValueNew) {
                    if (get<1>(setFourFIFO[i]) == tag) {
                        fifoSFourHit++;
                        temporary = 1;
                        full = 0;
                        break;
                    }
                }
            }


            if (temporary == 0) {

                for (int i = 0; i < setFourFIFO.size(); i++) {
                    if (get<0>(setFourFIFO[i]) == setValueNew) {
                        if (get<1>(setFourFIFO[i]) == -1) {
                            get<1>(setFourFIFO[i]) = tag;
                            get<2>(setFourFIFO[i]) = b;
                            full = 0;
                            break;
                        }
                    }
                }
            }



            if (full == 1) {
                //cout << "hello";
                int minimum = get<2>(setFourFIFO[0]);
                int track = 0;
                for (int i = 0; i < setFourFIFO.size(); i++) {
                    if (get<0>(setFourFIFO[i]) == setValueNew) {
                        if (get<2>(setFourFIFO[i]) < minimum) {
                            minimum = get<2>(setFourFIFO[i]);
                            track = i;
                        }
                    }

                }

                get<1>(setFourFIFO[track]) = tag;
                get<2>(setFourFIFO[track]) = b;

            }



        }
        result = static_cast<double>(fifoSFourHit) / addressStore.size();
        fifoSetFourHits.push_back(result);


      //Eight
        bytesPerLine = numCacheLines / 8;
        offsetWidth = log2(blockSize);
        setWidth = log2(bytesPerLine);
        //cout << setWidth;

        for (int b = 0; b < addressStore.size(); b++) {

            string setValue = addressStore[b].substr(addressStore[b].size() - (offsetWidth + setWidth), setWidth);
            string offsetValue = addressStore[b].substr(addressStore[b].size() - offsetWidth);
            double tag = binary_to_decimal(addressStore[b].substr(0, addressStore[b].size() - (offsetWidth + setWidth)));

            //cout << tag << setValue << offsetValue << endl;
            double setValueNew = (binary_to_decimal(setValue));

            int temporary = 0;
            int full = 1;

            for (int i = 0; i < setEightFIFO.size(); i++) {
                if (get<0>(setEightFIFO[i]) == setValueNew) {
                    if (get<1>(setEightFIFO[i]) == tag) {
                        fifoSEightHit++;
                        temporary = 1;
                        full = 0;
                        break;
                    }
                }
            }


            if (temporary == 0) {

                for (int i = 0; i < setEightFIFO.size(); i++) {
                    if (get<0>(setEightFIFO[i]) == setValueNew) {
                        if (get<1>(setEightFIFO[i]) == -1) {
                            get<1>(setEightFIFO[i]) = tag;
                            get<2>(setEightFIFO[i]) = b;
                            full = 0;
                            break;
                        }
                    }
                }
            }



            if (full == 1) {
                //cout << "hello";
                int minimum = get<2>(setEightFIFO[0]);
                int track = 0;
                for (int i = 0; i < setEightFIFO.size(); i++) {
                    if (get<0>(setEightFIFO[i]) == setValueNew) {
                        if (get<2>(setEightFIFO[i]) < minimum) {
                            minimum = get<2>(setEightFIFO[i]);
                            track = i;
                        }
                    }

                }

                get<1>(setEightFIFO[track]) = tag;
                get<2>(setEightFIFO[track]) = b;

            }



        }
        result = static_cast<double>(fifoSEightHit) / addressStore.size();
        fifoSetEightHits.push_back(result);

//FIFO Set Associative End

//LRU Set Associative Start

    //Two
        bytesPerLine = numCacheLines / 2;
        offsetWidth = log2(blockSize);
        setWidth = log2(bytesPerLine);
        //cout << setWidth;

        for (int b = 0; b < addressStore.size(); b++) {

            string setValue = addressStore[b].substr(addressStore[b].size() - (offsetWidth + setWidth), setWidth);
            string offsetValue = addressStore[b].substr(addressStore[b].size() - offsetWidth);
            double tag = binary_to_decimal(addressStore[b].substr(0, addressStore[b].size() - (offsetWidth + setWidth)));

            //cout << tag << setValue << offsetValue << endl;
            double setValueNew = (binary_to_decimal(setValue));

            int temporary = 0;
            int full = 1;

            for (int i = 0; i < testingCache.size(); i++) {
                if (get<0>(testingCache[i]) == setValueNew) {
                    if (get<1>(testingCache[i]) == tag) {
                        testingNum++;
                        get<2>(testingCache[i]) = b;
                        temporary = 1;
                        full = 0;
                        break;
                    }
                }
            }


            if (temporary == 0) {

                for (int i = 0; i < testingCache.size(); i++) {
                    if (get<0>(testingCache[i]) == setValueNew) {
                        if (get<1>(testingCache[i]) == -1) {
                            get<1>(testingCache[i]) = tag;
                            get<2>(testingCache[i]) = b;
                            full = 0;
                            break;
                        }
                    }
                }
            }



            if (full == 1) {
                //cout << "hello";
                int minimum = get<2>(testingCache[0]);
                int track = 0;
                for (int i = 0; i < testingCache.size(); i++) {
                    if (get<0>(testingCache[i]) == setValueNew) {
                        if (get<2>(testingCache[i]) < minimum) {
                            minimum = get<2>(testingCache[i]);
                            track = i;
                        }
                    }

                }

                get<1>(testingCache[track]) = tag;
                get<2>(testingCache[track]) = b;

            }



        }
        result = static_cast<double>(testingNum) / addressStore.size();
        testing.push_back(result);

    //Four
        bytesPerLine = numCacheLines / 4;
        offsetWidth = log2(blockSize);
        setWidth = log2(bytesPerLine);
        //cout << setWidth;

        for (int b = 0; b < addressStore.size(); b++) {

            string setValue = addressStore[b].substr(addressStore[b].size() - (offsetWidth + setWidth), setWidth);
            string offsetValue = addressStore[b].substr(addressStore[b].size() - offsetWidth);
            double tag = binary_to_decimal(addressStore[b].substr(0, addressStore[b].size() - (offsetWidth + setWidth)));

            //cout << tag << setValue << offsetValue << endl;
            double setValueNew = (binary_to_decimal(setValue));

            int temporary = 0;
            int full = 1;

            for (int i = 0; i < setFourLRU.size(); i++) {
                if (get<0>(setFourLRU[i]) == setValueNew) {
                    if (get<1>(setFourLRU[i]) == tag) {
                        lruSFourHit++;
                        get<2>(setFourLRU[i]) = b;
                        temporary = 1;
                        full = 0;
                        break;
                    }
                }
            }


            if (temporary == 0) {

                for (int i = 0; i < setFourLRU.size(); i++) {
                    if (get<0>(setFourLRU[i]) == setValueNew) {
                        if (get<1>(setFourLRU[i]) == -1) {
                            get<1>(setFourLRU[i]) = tag;
                            get<2>(setFourLRU[i]) = b;
                            full = 0;
                            break;
                        }
                    }
                }
            }



            if (full == 1) {
                //cout << "hello";
                int minimum = get<2>(setFourLRU[0]);
                int track = 0;
                for (int i = 0; i < setFourLRU.size(); i++) {
                    if (get<0>(setFourLRU[i]) == setValueNew) {
                        if (get<2>(setFourLRU[i]) < minimum) {
                            minimum = get<2>(setFourLRU[i]);
                            track = i;
                        }
                    }

                }

                get<1>(setFourLRU[track]) = tag;
                get<2>(setFourLRU[track]) = b;

            }



        }
        result = static_cast<double>(lruSFourHit) / addressStore.size();
        lruSetFourHits.push_back(result);


    //Eight
        bytesPerLine = numCacheLines / 8;
        offsetWidth = log2(blockSize);
        setWidth = log2(bytesPerLine);
        //cout << setWidth;

        for (int b = 0; b < addressStore.size(); b++) {

            string setValue = addressStore[b].substr(addressStore[b].size() - (offsetWidth + setWidth), setWidth);
            string offsetValue = addressStore[b].substr(addressStore[b].size() - offsetWidth);
            double tag = binary_to_decimal(addressStore[b].substr(0, addressStore[b].size() - (offsetWidth + setWidth)));

            //cout << tag << setValue << offsetValue << endl;
            double setValueNew = (binary_to_decimal(setValue));

            int temporary = 0;
            int full = 1;

            for (int i = 0; i < setEightLRU.size(); i++) {
                if (get<0>(setEightLRU[i]) == setValueNew) {
                    if (get<1>(setEightLRU[i]) == tag) {
                        lruSEightHit++;
                        get<2>(setEightLRU[i]) = b;
                        temporary = 1;
                        full = 0;
                        break;
                    }
                }
            }


            if (temporary == 0) {

                for (int i = 0; i < setEightLRU.size(); i++) {
                    if (get<0>(setEightLRU[i]) == setValueNew) {
                        if (get<1>(setEightLRU[i]) == -1) {
                            get<1>(setEightLRU[i]) = tag;
                            get<2>(setEightLRU[i]) = b;
                            full = 0;
                            break;
                        }
                    }
                }
            }



            if (full == 1) {
                //cout << "hello";
                int minimum = get<2>(setEightLRU[0]);
                int track = 0;
                for (int i = 0; i < setEightLRU.size(); i++) {
                    if (get<0>(setEightLRU[i]) == setValueNew) {
                        if (get<2>(setEightLRU[i]) < minimum) {
                            minimum = get<2>(setEightLRU[i]);
                            track = i;
                        }
                    }

                }

                get<1>(setEightLRU[track]) = tag;
                get<2>(setEightLRU[track]) = b;

            }



        }
        result = static_cast<double>(lruSEightHit) / addressStore.size();
        lruSetEightHits.push_back(result);


//LRU Set Associative End

    }

    /*vector<double> test = { 0.884788, 0.91493, 0.963724, 0.982295, 0.988266 };
    vector<double> test1 = { 0.891342, 0.921929, 0.967918, 0.9394, 0.989222};
    vector<double> test2  = { 0.909824, 0.945048, 0.966691, 0.985592, 0.989534 };
    vector<double> test3 = { 0.92069, 0.952676, 0.973323, 0.988014, 0.990876 };
    vector<double> test4 = { 0.910951, 0.954943, 0.76895, 0.986156, 0.989895 };
    vector<double> test5 = { 0.92531, 0.962731, 0.98178, 0.988941, 0.991264 };*/

    cout << "Direct Map: [ ";
    for (int z = 0; z < directHits.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << directHits[z] << " ";
    }
    cout << "]" << endl;
    cout << "FIFO Fully Associative: [ ";
    for (int z = 0; z < fifoFullyHits.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << fifoFullyHits[z] << " ";
    }
    cout << "]" << endl;
    cout << "LRU Fully Associative: [ ";
    for (int z = 0; z < lruFullyHits.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << lruFullyHits[z] << " ";
    }
    cout << "]" << endl;
    cout << "FIFO SA Associative 2way: [ ";
    for (int z = 0; z < fifoSetTwoHits.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << fifoSetTwoHits[z] << " ";
    }
    cout << "]" << endl;
    cout << "LRU SA Associative 2way: [ ";
    for (int z = 0; z < testing.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << testing[z] << " ";
    }
    cout << "]" << endl;
    cout << "FIFO SA Associative 4way: [ ";
    for (int z = 0; z < fifoSetFourHits.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << fifoSetFourHits[z] << " ";
    }
    cout << "]" << endl;
    cout << "LRU SA Associative 4way: [ ";
    for (int z = 0; z < lruSetFourHits.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << lruSetFourHits[z] << " ";
    }
    cout << "]" << endl;
    cout << "FIFO SA Associative 8way: [ ";
    for (int z = 0; z < fifoSetEightHits.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << fifoSetEightHits[z] << " ";
    }
    cout << "]" << endl;
    cout << "LRU SA Associative 8way: [ ";
    for (int z = 0; z < lruSetEightHits.size(); z++) {
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        cout << lruSetEightHits[z] << " ";
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

