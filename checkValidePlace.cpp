
// X OOX
bool isCapture(bool myColor, bool nextTreeEmptyOrFull[3], bool nextTreePieces[3])
{
    if (nextTreeEmptyOrFull[0] && nextTreeEmptyOrFull[1] && nextTreeEmptyOrFull[2])
        if (nextTreePieces[0] != myColor && nextTreePieces[1] != myColor && nextTreePieces[2] == myColor)
            return true;
    return false;
}

// .O O OX | XO O O.
bool isAnThreeMidNotSplitReverse(bool myColor, bool EmptyOrFullLeft[2], bool EmptyOrFullRight[2], bool colorOfPiecesLeft[2], bool colorOfPiecesRight[2])
{
    if (EmptyOrFullLeft[1] && EmptyOrFullRight[0] && colorOfPiecesLeft[1] == myColor && colorOfPiecesRight[0] == myColor)
        if ((EmptyOrFullLeft[0] && !EmptyOrFullRight[1] && colorOfPiecesLeft[0] != myColor) || (!EmptyOrFullLeft[0] && EmptyOrFullRight[1] && colorOfPiecesRight[1] != myColor))
            return true;
    return false;
}

// .O O O.
bool isAnThreeMidNotSplit(bool myColor, bool EmptyOrFullLeft[2], bool EmptyOrFullRight[2], bool colorOfPiecesLeft[2], bool colorOfPiecesRight[2])
{
    if (EmptyOrFullLeft[1] && EmptyOrFullRight[0] && colorOfPiecesLeft[1] == myColor && colorOfPiecesRight[0] == myColor && !EmptyOrFullLeft[0] && !EmptyOrFullRight[1])
        return true;
    return false;
}

// . O OOX | X O OO.
bool isAnThreeSideNotSplitReverse(bool myColor, bool EmptyOrFullLeft[1], bool EmptyOrFullRight[3], bool colorOfPiecesLeft[1], bool colorOfPiecesRight[3])
{
    if (EmptyOrFullRight[0] && EmptyOrFullRight[1] && colorOfPiecesRight[0] == myColor && colorOfPiecesRight[1] == myColor)
        if ((EmptyOrFullLeft[0] && !EmptyOrFullRight[2] && colorOfPiecesLeft[0] != myColor) ||
            (!EmptyOrFullLeft[0] && EmptyOrFullRight[2] && colorOfPiecesRight[2] != myColor))
            return true;
    return false;
}

// . O OO.
bool isAnThreeSideNotSplit(bool myColor, bool EmptyOrFullLeft[1], bool EmptyOrFullRight[3], bool colorOfPiecesLeft[1], bool colorOfPiecesRight[3])
{
    if (!EmptyOrFullLeft[0] && EmptyOrFullRight[1] && EmptyOrFullRight[0] && !EmptyOrFullRight[2] && colorOfPiecesRight[0] == myColor && colorOfPiecesRight[1] == myColor)
        return true;
    return false;
}

// X O .OO. | . O .OOX
bool isAnThreeSideOneSplitReverse(bool myColor, bool EmptyOrFullLeft[1], bool EmptyOrFullRight[4], bool colorOfPiecesLeft[1], bool colorOfPiecesRight[4])
{
    // if (EmptyOrFullRight[0] && EmptyOrFullRight[1] && colorOfPiecesRight[0] == myColor && colorOfPiecesRight[1] == myColor)
    //     if ((EmptyOrFullLeft[0] && !EmptyOrFullRight[2] && colorOfPiecesLeft[0] != myColor) ||
    //         (!EmptyOrFullLeft[0] && EmptyOrFullRight[2] && colorOfPiecesRight[2] != myColor))
    //         return true;
    // return false;
}
