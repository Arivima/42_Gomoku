#include "checkValidePalce.hpp"
#include <iostream>
using namespace std;

void isValideFunctionBool(bool expectedResult, bool myResult, string message)
{
    cout << message << " : ";
    if (expectedResult == myResult)
    {
        cout << "\033[1;32m" << "valide" << "\033[0m"
             << endl;
    }
    else
    {
        cout << "\033[1;31m" << "error" << "\033[0m"
             << endl;
        cout << "expectedResult : " << expectedResult
             << endl;
        cout << "myResult : " << myResult << endl;
    }
}

string listToString(bool *listToPars, int size)
{
    string res = "[";
    for (int i = 0; i < size; i++)
    {
        if (i)
        {
            res += ",";
        }
        res += listToPars[i] ? "1" : "0";
    }
    return res + "]";
}

void testFunctionIsCature()
{
    bool nextPlaceIsPieceHere[3];
    bool nextPlaceWhatColor[3];
    for (int i = 0; i < (1 << 3); i++)
    {
        for (int x = 0; x < 3; x++)
        {
            nextPlaceIsPieceHere[x] = i & 1 << x ? true : false;
        }
        for (int j = 0; j < (1 << 3); j++)
        {
            for (int y = 0; y < 3; y++)
            {
                nextPlaceWhatColor[y] = j & 1 << y ? true : false;
            }
            bool myRes;
            bool expected;
            myRes = isCapture(0, nextPlaceIsPieceHere, nextPlaceWhatColor);
            expected = (nextPlaceIsPieceHere[0] && nextPlaceIsPieceHere[1] && nextPlaceIsPieceHere[2] && nextPlaceWhatColor[0] && nextPlaceWhatColor[1] && !nextPlaceWhatColor[2]);
            isValideFunctionBool(expected, myRes, "isCapture(0," + listToString(nextPlaceIsPieceHere, 3) + listToString(nextPlaceWhatColor, 3) + ")");
            expected = (nextPlaceIsPieceHere[0] && nextPlaceIsPieceHere[1] && nextPlaceIsPieceHere[2] && !nextPlaceWhatColor[0] && !nextPlaceWhatColor[1] && nextPlaceWhatColor[2]);
            myRes = isCapture(1, nextPlaceIsPieceHere, nextPlaceWhatColor);
            isValideFunctionBool(expected, myRes, "isCapture(1," + listToString(nextPlaceIsPieceHere, 3) + listToString(nextPlaceWhatColor, 3) + ")");
        }
    }
}

void testFunctionIsAnThreeMidNotSplitReverse()
{
    bool EmptyOrFullLeft[2];
    bool EmptyOrFullRight[2];
    bool colorOfPiecesLeft[2];
    bool colorOfPiecesRight[2];
    for (int i = 0; i < (1 << 2); i++)
    {
        for (int x = 0; x < 2; x++)
        {
            EmptyOrFullLeft[x] = i & 1 << x ? true : false;
        }
        for (int j = 0; j < (1 << 2); j++)
        {
            for (int x = 0; x < 2; x++)
            {
                EmptyOrFullRight[x] = j & 1 << x ? true : false;
            }
            for (int k = 0; k < (1 << 2); k++)
            {
                for (int x = 0; x < 2; x++)
                {
                    colorOfPiecesLeft[x] = k & 1 << x ? true : false;
                }
                for (int l = 0; l < (1 << 2); l++)
                {
                    for (int x = 0; x < 2; x++)
                    {
                        colorOfPiecesRight[x] = l & 1 << x ? true : false;
                    }
                    bool myRes;
                    bool expected;
                    myRes = isAnThreeMidNotSplitReverse(0, EmptyOrFullLeft, EmptyOrFullRight, colorOfPiecesLeft, colorOfPiecesRight);
                    expected = ((EmptyOrFullLeft[0] && EmptyOrFullLeft[1] && EmptyOrFullRight[0] && !EmptyOrFullRight[1] && colorOfPiecesLeft[0] == 1 && colorOfPiecesLeft[1] == 0 && colorOfPiecesRight[0] == 0) ||
                                (!EmptyOrFullLeft[0] && EmptyOrFullLeft[1] && EmptyOrFullRight[0] && EmptyOrFullRight[1] && colorOfPiecesLeft[1] == 0 && colorOfPiecesRight[0] == 0 && colorOfPiecesRight[1] == 1));
                    isValideFunctionBool(expected, myRes, "isAnThreeMidNotSplitReverse(0," + listToString(EmptyOrFullLeft, 2) + listToString(EmptyOrFullRight, 2) + listToString(colorOfPiecesLeft, 2) + listToString(colorOfPiecesRight, 2) + ")");
                    expected = ((EmptyOrFullLeft[0] && EmptyOrFullLeft[1] && EmptyOrFullRight[0] && !EmptyOrFullRight[1] && colorOfPiecesLeft[0] == 0 && colorOfPiecesLeft[1] == 1 && colorOfPiecesRight[0] == 1) ||
                                (!EmptyOrFullLeft[0] && EmptyOrFullLeft[1] && EmptyOrFullRight[0] && EmptyOrFullRight[1] && colorOfPiecesLeft[1] == 1 && colorOfPiecesRight[0] == 1 && colorOfPiecesRight[1] == 0));
                    myRes = isAnThreeMidNotSplitReverse(1, EmptyOrFullLeft, EmptyOrFullRight, colorOfPiecesLeft, colorOfPiecesRight);
                    isValideFunctionBool(expected, myRes, "isAnThreeMidNotSplitReverse(1," + listToString(EmptyOrFullLeft, 2) + listToString(EmptyOrFullRight, 2) + listToString(colorOfPiecesLeft, 2) + listToString(colorOfPiecesRight, 2) + ")");
                }
            }
        }
    }
}

void testFunctionIsAnThreeMidNotSplit()
{
    bool EmptyOrFullLeft[2];
    bool EmptyOrFullRight[2];
    bool colorOfPiecesLeft[2];
    bool colorOfPiecesRight[2];
    for (int i = 0; i < (1 << 2); i++)
    {
        for (int x = 0; x < 2; x++)
        {
            EmptyOrFullLeft[x] = i & 1 << x ? true : false;
        }
        for (int j = 0; j < (1 << 2); j++)
        {
            for (int x = 0; x < 2; x++)
            {
                EmptyOrFullRight[x] = j & 1 << x ? true : false;
            }
            for (int k = 0; k < (1 << 2); k++)
            {
                for (int x = 0; x < 2; x++)
                {
                    colorOfPiecesLeft[x] = k & 1 << x ? true : false;
                }
                for (int l = 0; l < (1 << 2); l++)
                {
                    for (int x = 0; x < 2; x++)
                    {
                        colorOfPiecesRight[x] = l & 1 << x ? true : false;
                    }
                    bool myRes;
                    bool expected;
                    myRes = isAnThreeMidNotSplit(0, EmptyOrFullLeft, EmptyOrFullRight, colorOfPiecesLeft, colorOfPiecesRight);
                    expected = (!EmptyOrFullLeft[0] && EmptyOrFullLeft[1] && EmptyOrFullRight[0] && !EmptyOrFullRight[1] && colorOfPiecesLeft[1] == 0 && colorOfPiecesRight[0] == 0);
                    isValideFunctionBool(expected, myRes, "isAnThreeMidNotSplit(0," + listToString(EmptyOrFullLeft, 2) + listToString(EmptyOrFullRight, 2) + listToString(colorOfPiecesLeft, 2) + listToString(colorOfPiecesRight, 2) + ")");
                    expected = ((!EmptyOrFullLeft[0] && EmptyOrFullLeft[1] && EmptyOrFullRight[0] && !EmptyOrFullRight[1] && colorOfPiecesLeft[1] == 1 && colorOfPiecesRight[0] == 1));
                    myRes = isAnThreeMidNotSplit(1, EmptyOrFullLeft, EmptyOrFullRight, colorOfPiecesLeft, colorOfPiecesRight);
                    isValideFunctionBool(expected, myRes, "isAnThreeMidNotSplit(1," + listToString(EmptyOrFullLeft, 2) + listToString(EmptyOrFullRight, 2) + listToString(colorOfPiecesLeft, 2) + listToString(colorOfPiecesRight, 2) + ")");
                }
            }
        }
    }
}

void testFunctionIsAnThreeSideNotSplitReverse()
{
    bool EmptyOrFullLeft[1];
    bool EmptyOrFullRight[3];
    bool colorOfPiecesLeft[1];
    bool colorOfPiecesRight[3];
    for (int i = 0; i < (1 << 1); i++)
    {
        for (int x = 0; x < 1; x++)
        {
            EmptyOrFullLeft[x] = i & 1 << x ? true : false;
        }
        for (int j = 0; j < (1 << 3); j++)
        {
            for (int x = 0; x < 3; x++)
            {
                EmptyOrFullRight[x] = j & 1 << x ? true : false;
            }
            for (int k = 0; k < (1 << 1); k++)
            {
                for (int x = 0; x < 1; x++)
                {
                    colorOfPiecesLeft[x] = k & 1 << x ? true : false;
                }
                for (int l = 0; l < (1 << 3); l++)
                {
                    for (int x = 0; x < 3; x++)
                    {
                        colorOfPiecesRight[x] = l & 1 << x ? true : false;
                    }
                    bool myRes;
                    bool expected;
                    myRes = isAnThreeSideNotSplitReverse(0, EmptyOrFullLeft, EmptyOrFullRight, colorOfPiecesLeft, colorOfPiecesRight);
                    expected = ((!EmptyOrFullLeft[0] && EmptyOrFullRight[0] && EmptyOrFullRight[1] && EmptyOrFullRight[2] && colorOfPiecesRight[0] == 0 && colorOfPiecesRight[1] == 0 && colorOfPiecesRight[2] == 1) ||
                                (EmptyOrFullLeft[0] && EmptyOrFullRight[0] && EmptyOrFullRight[1] && !EmptyOrFullRight[2] && colorOfPiecesRight[0] == 0 && colorOfPiecesRight[1] == 0 && colorOfPiecesLeft[0] == 1));
                    isValideFunctionBool(expected, myRes, "isAnThreeSideNotSplitReverse(0," + listToString(EmptyOrFullLeft, 1) + listToString(EmptyOrFullRight, 3) + listToString(colorOfPiecesLeft, 1) + listToString(colorOfPiecesRight, 3) + ")");
                    expected = ((!EmptyOrFullLeft[0] && EmptyOrFullRight[0] && EmptyOrFullRight[1] && EmptyOrFullRight[2] && colorOfPiecesRight[0] == 1 && colorOfPiecesRight[1] == 1 && colorOfPiecesRight[2] == 0) ||
                                (EmptyOrFullLeft[0] && EmptyOrFullRight[0] && EmptyOrFullRight[1] && !EmptyOrFullRight[2] && colorOfPiecesRight[0] == 1 && colorOfPiecesRight[1] == 1 && colorOfPiecesLeft[0] == 0));
                    myRes = isAnThreeSideNotSplitReverse(1, EmptyOrFullLeft, EmptyOrFullRight, colorOfPiecesLeft, colorOfPiecesRight);
                    isValideFunctionBool(expected, myRes, "isAnThreeSideNotSplitReverse(1," + listToString(EmptyOrFullLeft, 1) + listToString(EmptyOrFullRight, 3) + listToString(colorOfPiecesLeft, 1) + listToString(colorOfPiecesRight, 3) + ")");
                }
            }
        }
    }
}

void testFunctionIsAnThreeSideNotSplit()
{
    bool EmptyOrFullLeft[1];
    bool EmptyOrFullRight[3];
    bool colorOfPiecesLeft[1];
    bool colorOfPiecesRight[3];
    for (int i = 0; i < (1 << 1); i++)
    {
        for (int x = 0; x < 1; x++)
        {
            EmptyOrFullLeft[x] = i & 1 << x ? true : false;
        }
        for (int j = 0; j < (1 << 3); j++)
        {
            for (int x = 0; x < 3; x++)
            {
                EmptyOrFullRight[x] = j & 1 << x ? true : false;
            }
            for (int k = 0; k < (1 << 1); k++)
            {
                for (int x = 0; x < 1; x++)
                {
                    colorOfPiecesLeft[x] = k & 1 << x ? true : false;
                }
                for (int l = 0; l < (1 << 3); l++)
                {
                    for (int x = 0; x < 3; x++)
                    {
                        colorOfPiecesRight[x] = l & 1 << x ? true : false;
                    }
                    bool myRes;
                    bool expected;
                    myRes = isAnThreeSideNotSplit(0, EmptyOrFullLeft, EmptyOrFullRight, colorOfPiecesLeft, colorOfPiecesRight);
                    expected = ((!EmptyOrFullLeft[0] && EmptyOrFullRight[0] && EmptyOrFullRight[1] && !EmptyOrFullRight[2] && colorOfPiecesRight[0] == 0 && colorOfPiecesRight[1] == 0));
                    isValideFunctionBool(expected, myRes, "isAnThreeSideNotSplit(0," + listToString(EmptyOrFullLeft, 1) + listToString(EmptyOrFullRight, 3) + listToString(colorOfPiecesLeft, 1) + listToString(colorOfPiecesRight, 3) + ")");
                    expected = ((!EmptyOrFullLeft[0] && EmptyOrFullRight[0] && EmptyOrFullRight[1] && !EmptyOrFullRight[2] && colorOfPiecesRight[0] == 1 && colorOfPiecesRight[1] == 1));
                    myRes = isAnThreeSideNotSplit(1, EmptyOrFullLeft, EmptyOrFullRight, colorOfPiecesLeft, colorOfPiecesRight);
                    isValideFunctionBool(expected, myRes, "isAnThreeSideNotSplit(1," + listToString(EmptyOrFullLeft, 1) + listToString(EmptyOrFullRight, 3) + listToString(colorOfPiecesLeft, 1) + listToString(colorOfPiecesRight, 3) + ")");
                }
            }
        }
    }
}

int main()
{
    cout << "Test function : " << "isCapture(bool myColor, bool nextTreeEmptyOrFull[3], bool nextTreePieces[3])" << endl;
    testFunctionIsCature();
    testFunctionIsAnThreeMidNotSplitReverse();
    testFunctionIsAnThreeMidNotSplit();
    testFunctionIsAnThreeSideNotSplitReverse();
    testFunctionIsAnThreeSideNotSplit();
    return 1;
}