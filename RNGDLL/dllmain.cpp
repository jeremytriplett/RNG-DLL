// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <string>
#include <iostream> 
#include <fstream> 
#include <string.h> 
#include <stack>
#include <vector>
#using <System.Data.dll>
#using <System.dll>



using namespace std;
using namespace Oracle::ManagedDataAccess::Client;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" {

    __declspec(dllexport) int GetRandomNumber(int min, int max, std::stack<std::string> randBits)
    {

        int range = max - min;

        int numBitsToTake = (int)floor(log2((double)range) + 1);

        int value;

        do
        {
            std::string bitsString = "";

            for (int i = 0; i < numBitsToTake; i++)
            {
                bitsString += randBits.top();
                randBits.pop();
            }

            value = std::stoi(bitsString, 0, 2);

        } while (value > range);

        return (value + min);

    }

    __declspec(dllexport) stack<string> GetRandomBitsStack(string fileName)
    {
        stack<string>randBits;

        std::ifstream input(fileName, std::ios::binary);

        std::vector<char> bytes(
            (std::istreambuf_iterator<char>(input)),
            (std::istreambuf_iterator<char>()));

        bytes.erase(bytes.begin(), bytes.begin() + 44);

        std::vector<bool> bits;

        for (char byte : bytes) {
            char temp = byte;
            string s = "";
            while (temp > 0) {
                s += temp % 2;
                temp /= 2;
            }
            for (int i = s.length() - 1; i >= 0; i--) {
                bits.push_back(s[i]);
            }
        }


        int position = 0;

        for (char bit : bits)
        {

            if (position % 16 == 0)
            {
                randBits.push(bit == false ? "0" : "1");
            }

            position++;
        }

        return randBits;
    }
}




