#include <gtest/gtest.h>
#include "board.hpp"

class BoardTest : public ::testing::Test {
protected:
    GomokuBoard board;
};

TEST_F(BoardTest, InitialBoardIsEmpty) {
    for (int y = 0; y < GomokuBoard::BOARD_SIZE; ++y) {
        for (int x = 0; x < GomokuBoard::BOARD_SIZE; ++x) {
            EXPECT_EQ(board.getCell(x, y), Cell::EMPTY) << "Cell (" << x << "," << y << ") should be empty";
        }
    }
}

TEST_F(BoardTest, InitialState) {
    EXPECT_EQ(board.getCurrentPlayer(), Player::BLACK);
    EXPECT_EQ(board.getBlackCaptures(), 0);
    EXPECT_EQ(board.getWhiteCaptures(), 0);
    EXPECT_EQ(board.getMoveCount(), 0);
    EXPECT_FALSE(board.isFull());
}

TEST_F(BoardTest, PlaceStoneBlack) {
    EXPECT_TRUE(board.placeStone(9, 9, Player::BLACK));
    EXPECT_EQ(board.getCell(9, 9), Cell::BLACK);
    EXPECT_EQ(board.getMoveCount(), 1);
}

TEST_F(BoardTest, PlaceStoneWhite) {
    board.placeStone(9, 9, Player::BLACK);
    EXPECT_TRUE(board.placeStone(9, 10, Player::WHITE));
    EXPECT_EQ(board.getCell(9, 10), Cell::WHITE);
    EXPECT_EQ(board.getMoveCount(), 2);
}

TEST_F(BoardTest, CannotPlaceOnOccupiedCell) {
    board.placeStone(9, 9, Player::BLACK);
    EXPECT_FALSE(board.placeStone(9, 9, Player::WHITE));
    EXPECT_EQ(board.getMoveCount(), 1);
}

TEST_F(BoardTest, CannotPlaceOutsideBoard) {
    EXPECT_FALSE(board.placeStone(-1, 9, Player::BLACK));
    EXPECT_FALSE(board.placeStone(9, -1, Player::BLACK));
    EXPECT_FALSE(board.placeStone(19, 9, Player::BLACK));
    EXPECT_FALSE(board.placeStone(9, 19, Player::BLACK));
    EXPECT_EQ(board.getMoveCount(), 0);
}

TEST_F(BoardTest, PlaceStoneViaPosition) {
    Position pos{9, 9};
    EXPECT_TRUE(board.placeStone(pos, Player::BLACK));
    EXPECT_EQ(board.getCell(pos), Cell::BLACK);
}

TEST_F(BoardTest, RemoveStone) {
    board.placeStone(9, 9, Player::BLACK);
    EXPECT_TRUE(board.removeStone(9, 9));
    EXPECT_EQ(board.getCell(9, 9), Cell::EMPTY);
    EXPECT_EQ(board.getMoveCount(), 0);
}

TEST_F(BoardTest, CannotRemoveEmptyCell) {
    EXPECT_FALSE(board.removeStone(9, 9));
    EXPECT_EQ(board.getMoveCount(), 0);
}

TEST_F(BoardTest, ClearBoard) {
    board.placeStone(9, 9, Player::BLACK);
    board.placeStone(9, 10, Player::WHITE);
    board.addCapture(Player::BLACK, 3);

    board.clear();

    EXPECT_EQ(board.getCell(9, 9), Cell::EMPTY);
    EXPECT_EQ(board.getCell(9, 10), Cell::EMPTY);
    EXPECT_EQ(board.getCurrentPlayer(), Player::BLACK);
    EXPECT_EQ(board.getBlackCaptures(), 0);
    EXPECT_EQ(board.getMoveCount(), 0);
}

TEST_F(BoardTest, SwitchPlayer) {
    EXPECT_EQ(board.getCurrentPlayer(), Player::BLACK);
    board.switchPlayer();
    EXPECT_EQ(board.getCurrentPlayer(), Player::WHITE);
    board.switchPlayer();
    EXPECT_EQ(board.getCurrentPlayer(), Player::BLACK);
}

TEST_F(BoardTest, AddCaptures) {
    board.addCapture(Player::BLACK, 3);
    EXPECT_EQ(board.getBlackCaptures(), 3);
    board.addCapture(Player::BLACK, 2);
    EXPECT_EQ(board.getBlackCaptures(), 5);
}

TEST_F(BoardTest, SetCaptures) {
    board.addCapture(Player::BLACK, 10);
    board.setCaptures(Player::BLACK, 5);
    EXPECT_EQ(board.getBlackCaptures(), 5);
}

TEST_F(BoardTest, IsFull) {
    GomokuBoard smallBoard;
    for (int y = 0; y < GomokuBoard::BOARD_SIZE; ++y) {
        for (int x = 0; x < GomokuBoard::BOARD_SIZE; ++x) {
            Player p = ((x + y) % 2 == 0) ? Player::BLACK : Player::WHITE;
            smallBoard.placeStone(x, y, p);
        }
    }
    EXPECT_TRUE(smallBoard.isFull());
}

TEST_F(BoardTest, GetCellOutOfBounds) {
    EXPECT_EQ(board.getCell(-1, 9), Cell::EMPTY);
    EXPECT_EQ(board.getCell(9, -1), Cell::EMPTY);
    EXPECT_EQ(board.getCell(19, 9), Cell::EMPTY);
    EXPECT_EQ(board.getCell(9, 19), Cell::EMPTY);
}
