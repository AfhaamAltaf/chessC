#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_map>
#include <exception>
#include <utility>
#include <fstream>
using namespace std;

// made by Afhaam(29056)
// Plz read! make sure the cpp file is stored in a empty folder and that vscode/clion has access granted to that
//folder as this code saves and reads from files from the same folder as the cpp file.
//terminal or vscode is preferred as i have implemented ways to clear terminal which works on them but not clion.
// ranges have been intentionally not implemented so that this code can run in any compiler and doesnt need c20.
//if you are running this code in clion plz comment out all the  system("cls"); in the main function as it causes
//issues with the terminal of clion. if you are on vscode or windows terminal, keep them uncommented if you like.

string convertToLower(string& inputPiece) {
    transform(inputPiece.begin(), inputPiece.end(), inputPiece.begin(),
              [](unsigned char c) { return tolower(c); });
    return inputPiece;
}
string convertToUpper(string& inputPiece) {
    transform(inputPiece.begin(), inputPiece.end(), inputPiece.begin(),
              [](unsigned char c) { return toupper(c); });
    return inputPiece;
}

void saveGameState(const bool BlackTurn, const bool WhiteTurn, const bool turnEnded,
                   const unordered_map<string, pair<bool, bool>>& whitePiecesWhoMadeItToTheEnd,
                   const unordered_map<string, pair<bool, bool>>& blackPiecesWhoMadeItToTheEnd,
                   const string& filename) {
    ofstream outFile(filename);

    if (!outFile) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    outFile << BlackTurn << " " << WhiteTurn << " " << turnEnded << "\n";

    outFile << whitePiecesWhoMadeItToTheEnd.size() << "\n";
    for (const auto& entry : whitePiecesWhoMadeItToTheEnd) {
        outFile << entry.first << " " << entry.second.first << " " << entry.second.second << "\n";
    }

    outFile << blackPiecesWhoMadeItToTheEnd.size() << "\n";
    for (const auto& entry : blackPiecesWhoMadeItToTheEnd) {
        outFile << entry.first << " " << entry.second.first << " " << entry.second.second << "\n";
    }

    outFile.close();
}
void loadGameState(bool& BlackTurn, bool& WhiteTurn, bool& turnEnded,
                   unordered_map<string, pair<bool, bool>>& whitePiecesWhoMadeItToTheEnd,
                   unordered_map<string, pair<bool, bool>>& blackPiecesWhoMadeItToTheEnd,
                   const string& filename) {
    ifstream inFile(filename);


    if (inFile.is_open()) {
        inFile >> BlackTurn >> WhiteTurn >> turnEnded;

        size_t size;
        inFile >> size;
        whitePiecesWhoMadeItToTheEnd.clear();
        for (size_t i = 0; i < size; ++i) {
            string piece;
            bool first, second;
            inFile >> piece >> first >> second;
            whitePiecesWhoMadeItToTheEnd[piece] = {first, second};
        }

        inFile >> size;
        blackPiecesWhoMadeItToTheEnd.clear();
        for (size_t i = 0; i < size; ++i) {
            string piece;
            bool first, second;
            inFile >> piece >> first >> second;
            blackPiecesWhoMadeItToTheEnd[piece] = {first, second};
        }
        cout << "Vector state imported from " << filename << endl;

        inFile.close();
    }
}


void exportToFile(const vector<vector<string>>& vec, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& row : vec) {
            for (const auto& elem : row) {
                outFile << elem << " ";
            }
            outFile << "\n";
        }
        outFile.close();
    } else {
        cerr << "Unable to export!" << filename << endl;
    }
}
void importFromFile(vector<vector<string>>& vec, const string& filename) {
    try {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            string line;
            for (auto &row: vec) {
                for (auto &elem: row) {
                    inFile >> elem;
                }
            }
            cout << "2D vector imported from " << filename << endl;
            inFile.close();

        } else {
            throw runtime_error("No save file exists, try saving first!\n");


        }
    } catch(const exception& error){
        cout << error.what();
    }


}


class CustomException : public exception {
public:
    const char* what() const noexcept override {
        return "\nThe game is over!";
    }
};

class boardHistory {
private:
    struct Node {
        vector<vector<string>> board;
        unique_ptr<Node> next;

        explicit Node(const vector<vector<string>>& b)
                : board(b), next(nullptr) {}
    };

    unique_ptr<Node> head;
    Node* tail;

public:
    boardHistory() : head(nullptr), tail(nullptr) {}

    void push(const vector<vector<string>>& board) {
        auto newNode = make_unique<Node>(board);
        if (!newNode) {
            cerr << "Error: Failed to allocate memory for the new node.\n";
            return;
        }

        if (!tail) {
            head = std::move(newNode);
            tail = head.get();
        } else {
            tail->next = std::move(newNode);
            tail = tail->next.get();
        }
    }

    void pop() {
        if (!head) return;
        if (head.get() == tail) {
            head.reset();
            tail = nullptr;
        } else {
            Node* prev = nullptr;
            Node* current = head.get();
            while (current->next) {
                prev = current;
                current = current->next.get();
            }
            prev->next.reset();
            tail = prev;
        }
    }





    vector<vector<string>> retrieveLast() {
        if (!tail) {
            unique_ptr<vector<vector<string>>> board;
            vector<string> WhiteTeam = {"W1", "W2", "W3", "W4", "W5", "W6", "W7", "W8"};
            vector<string> BlackTeam = {"B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8"};
            vector<string> EmptySpace = {"`", "`", "`", "`", "`", "`", "`", "`"};
            board = make_unique<vector<vector<string>>>();
            board->push_back(WhiteTeam);
            for(int i = 0; i < 6; i++){
                board->push_back(EmptySpace);
            }

            board->push_back(BlackTeam);

            return *board;
        }

        return tail->board;
    }

};


template <typename T>
class CustomStack {
public:
    void push(const T& data) {
        stack_.push_back(data);
    }

    void pop() {
        if (!stack_.empty()) {
            stack_.pop_back();
        } else {
            std::cerr << "Stack is empty. Cannot pop." << std::endl;
        }
    }

    T& top() {
        if (!stack_.empty()) {
            return stack_.back();
        } else {
            std::cerr << "No moves have been made so stack is empty, hence -r cannot be used" << std::endl;
            throw std::runtime_error("please re run the code");
        }
    }

private:
    std::vector<T> stack_;
};






class game
{
protected:

    vector<string> WhiteTeam = {"W1", "W2", "W3", "W4", "W5", "W6", "W7", "W8"};
    vector<string> BlackTeam = {"B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8"};
    vector<string> EmptySpace = {"`", "`", "`", "`", "`", "`", "`", "`"};
    unique_ptr<vector<vector<string>>> board;
    bool BlackTurn, WhiteTurn, turnEnded;
    unordered_map<string,pair<bool,bool>> whitePiecesWhoMadeItToTheEnd;
    unordered_map<string,pair<bool,bool>> blackPiecesWhoMadeItToTheEnd;


public:
    game()
    {

        board = make_unique<vector<vector<string>>>();
        board->push_back(WhiteTeam);
        for(int i = 0; i < 8; i++){
            if(i < 6){ board->push_back(EmptySpace);}
            whitePiecesWhoMadeItToTheEnd["W" + to_string(i + 1)] = {false,false};
            blackPiecesWhoMadeItToTheEnd["B" + to_string(i + 1)] = {false, false};
        }

        board->push_back(BlackTeam);
        BlackTurn = true;
        WhiteTurn = false;
        turnEnded = false;

    }
    virtual ~game() = default;

    auto getBoard(){
        return *board;
    }
    void setBoard(const vector<vector<string>>& board1){
        *board = board1;
        BlackTurn = !BlackTurn;
        WhiteTurn = !WhiteTurn;
    }
    auto getWhitePiecesWhoMadeIt(){
        return whitePiecesWhoMadeItToTheEnd;
    }
    auto getBlackPiecesWhoMadeIt(){
        return blackPiecesWhoMadeItToTheEnd;
    }
    void setWhitePiecesWhoMadeIt(unordered_map<string,pair<bool,bool>>& WPWMITTE){
        whitePiecesWhoMadeItToTheEnd = WPWMITTE;
    }
    void setBlackPiecesWhoMadeIt(unordered_map<string,pair<bool,bool>>& BPWMITTE){
        blackPiecesWhoMadeItToTheEnd = BPWMITTE;
    }

    void display()
    {

        for (const auto& rows: *board) {
            cout << "---------------------------------------------------------\n";
            for_each(rows.begin(), rows.end(), [](const auto& columns){
                if(columns == "`") cout << "|   " << columns << "  ";
                else {
                    cout << "|  " << columns << "  ";
                }
            });
            cout << "|" << endl;
        }
        cout << "---------------------------------------------------------\n";

    }
    virtual void place(string& piece){
        cout << "A virtual function used to move the pieces using derived class! if you see this message that means"
                "something went wrong";
    }

    void fileExport(){
        string filename = "vecSaveState.txt";
        string filename2= "varSaveState.txt";
        exportToFile(*board, filename);
        saveGameState(BlackTurn,WhiteTurn,turnEnded,
                      whitePiecesWhoMadeItToTheEnd,
                      blackPiecesWhoMadeItToTheEnd,
                      filename2);

        cout << "2D vector exported to " << filename << endl;
        cout << "Vector exported to " << filename2 << endl;




    }
    void fileImport(){
        string filename = "vecSaveState.txt";
        string filename2= "varSaveState.txt";
        importFromFile(*board, filename);
        loadGameState(BlackTurn,WhiteTurn,turnEnded,
                      whitePiecesWhoMadeItToTheEnd,
                      blackPiecesWhoMadeItToTheEnd,
                      filename2);



    }



};




class Players : public virtual game
{
private:
    boardHistory oldBoardState;
    string tempStorageForPiece;

public:


    static bool piecesLeft(const unordered_map<string, pair<bool, bool>>& map1) {
        return any_of(map1.begin(), map1.end(), [](const auto& kv) {
            return !kv.second.first && !kv.second.second;
        });
    }

    bool checkLeftDiagonalPieces(const string& piece,const int& i,const int& j)
    {
        char Color = piece[0];

        if ((*board)[i][j] == piece && Color == 'W' && j != 7)
        {
            string leftDiagPiece = (*board)[i + 1][j + 1];

            char leftDiagColor = leftDiagPiece[0];
            if (leftDiagColor == 'B')
                return true;
            else
                return false;
        }
        else if ((*board)[i][j] == piece && Color == 'B' && j != 0)
        {
            string leftDiagPiece = (*board)[i - 1][j - 1];
            char leftDiagColor = leftDiagPiece[0];
            if (leftDiagColor == 'W')
                return true;
            else
                return false;
        }

        return false;
    }


    bool checkRightDiagonalPieces(const string& piece, const int& i, const int& j)
    {
        char Color = piece[0];

        if ((*board)[i][j] == piece && Color == 'W' && j != 0)
        {
            string rightDiagPiece = (*board)[i + 1][j - 1];

            char rightDiagColor = rightDiagPiece[0];

            if (rightDiagColor == 'B')
                return true;
            else
                return false;
        }
        else if ((*board)[i][j] == piece && Color == 'B' && j != 7)
        {
            string rightDiagPiece = (*board)[i - 1][j + 1];
            char rightDiagColor = rightDiagPiece[0];
            if (rightDiagColor == 'W')
                return true;
            else
                return false;
        }

        return false;
    }
    void moveRight(const string& piece,const int& i,const int& j)
    {
        try {
            if (piece[0] == 'W') {
                blackPiecesWhoMadeItToTheEnd[(*board)[i + 1][j - 1]] = {false, true};
                tempStorageForPiece = (*board)[i + 1][j - 1];

                (*board)[i + 1][j - 1] = piece;
                changeTurn();
                throw runtime_error(tempStorageForPiece);
            } else if (piece[0] == 'B') {
                whitePiecesWhoMadeItToTheEnd[(*board)[i - 1][j + 1]] = {false, true};
                tempStorageForPiece = (*board)[i - 1][j + 1];

                (*board)[i - 1][j + 1] = piece;
                changeTurn();
                throw 1;
            }


        } catch(const exception& except){
            cout << "\n" << except.what() << " has been eliminated!!!!\n";

        }
        catch(const int& exceptt){
            cout << "\n" << tempStorageForPiece << " has been eliminated!!!!\n";
        }
        (*board)[i][j] = "`";

    }
    void moveLeft(string piece, int i, int j)
    {
        try {
            if (piece[0] == 'W') {
                blackPiecesWhoMadeItToTheEnd[(*board)[i + 1][j + 1]] = {false, true};
                tempStorageForPiece = (*board)[i + 1][j + 1];

                (*board)[i + 1][j + 1] = piece;
                changeTurn();
                throw '1';

            } else if (piece[0] == 'B') {
                whitePiecesWhoMadeItToTheEnd[(*board)[i - 1][j - 1]] = {false, true};
                tempStorageForPiece = (*board)[i - 1][j - 1];

                (*board)[i - 1][j - 1] = piece;
                changeTurn();
                throw 1;
            }
        } catch(const char& except){
            cout << "\n" << tempStorageForPiece << " has been eliminated!!!!\n";

        }
        catch(const int& exceptt){
            cout << "\n" << tempStorageForPiece << " has been eliminated!!!!\n";
        }
        (*board)[i][j] = "`";


    }

    bool hasMovedYet(string piece)
    {
        int store = stoi(string(1, piece[1]));
        if (piece[0] == 'W')
        {
            if ((*board)[0][store - 1] == piece)
                return false;

            else
                return true;
        }
        else if (piece[0] == 'B')
        {
            if ((*board)[7][store - 1] == piece)
                return false;
            else
                return true;
        }
        return false;
    }
    bool static isValidSteps(string& steps)
    {
        if (steps == "1" || steps == "2")
            return true;
        else
            return false;
    }

    bool isValidPiece(string &piece)
    {
        if (BlackTurn)
        {
            for (int i = 0; i < 8; i++)
            {
                if (piece == BlackTeam[i])
                    return true;
            }
            return false;
        }
        else if (WhiteTurn)
        {
            for (int i = 0; i < 8; i++)
            {
                if (piece == WhiteTeam[i])
                    return true;
            }
            return false;
        }
        return false;
    }
    void moveForward(const string& piece, int i, int j){
        if (WhiteTurn && ((*board)[i + 1][j] == "`"))
        {
            (*board)[i + 1][j] = piece;
            (*board)[i][j] = "`";
            changeTurn();

        }
        else if (BlackTurn && ((*board)[i - 1][j] == "`"))
        {
            (*board)[i - 1][j] = piece;
            (*board)[i][j] = "`";
            changeTurn();

        }

    }
    void changeTurn(){
        WhiteTurn = !WhiteTurn;
        BlackTurn = !BlackTurn;
    }

    void selection( string& piece, int i, int j, bool firstMove){


        cout << "\nMore than one paths have been found!";
        string input;
        do {
            cout << "Do you want to continue moving forward or move diagonally?(-f/-d):";
            cin >> input;
            cout << endl;
        } while (input != "-d" && input != "-f");
        if (input == "-d" && checkLeftDiagonalPieces(piece, i, j) &&
            checkRightDiagonalPieces(piece, i, j))
        {
            string lrinput;
            do{

                cout << "\n Left or Right(-l/-r): ";
                cin >> lrinput;
                cout << endl;
            } while(lrinput != "-l" && lrinput != "-r");
            if (lrinput == "-l")
                moveLeft(piece, i, j);
            else if (lrinput == "-r")
                moveRight(piece, i, j);
        }
        else if (input == "-d" && checkLeftDiagonalPieces(piece, i, j))
            moveLeft(piece, i, j);
        else if (input == "-d" && checkRightDiagonalPieces(piece, i, j))
            moveRight(piece, i, j);
        else if(input == "-f"){
            if(hasMovedYet(piece)) {
                moveForward(piece, i, j);
            }
        }
        if(firstMove) changeTurn();


        turnEnded = true;

    }



    void ifNotFirst(string piece){
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if ((*board)[i][j] == piece && (checkRightDiagonalPieces(piece, i, j) || checkLeftDiagonalPieces(piece, i, j))) {
                    selection(piece, i, j, false);
                }

                else if ((*board)[i][j] == piece && !turnEnded)
                {
                    moveForward(piece,i,j);

                    turnEnded = true;
                }

                auto checkAndUpdate = [&](int k) {
                    if ((*board)[0][k] == piece && piece[0] == 'B') {
                        (*board)[0][k] = "`";
                        blackPiecesWhoMadeItToTheEnd[piece] = {true, false};
                        cout << "\n" << piece << " has made it to the finish line!!!!\n";
                    }
                    if ((*board)[7][k] == piece && piece[0] == 'W') {
                        (*board)[7][k] = "`";
                        whitePiecesWhoMadeItToTheEnd[piece] = {true, false};
                        cout << "\n" << piece << " has made it to the finish line!!!!\n";
                    }
                };


                for_each(begin((*board)[0]), begin((*board)[0]) + 8, [&](auto& cell) {
                    int k = &cell - &(*board)[0][0];
                    checkAndUpdate(k);
                });
                try {
                    if ((!piecesLeft(whitePiecesWhoMadeItToTheEnd) || !piecesLeft(blackPiecesWhoMadeItToTheEnd)) ||gameStuckWhite()
                        || gameStuckBlack()) {
                        throw CustomException();
                    }
                } catch(const exception& e){
                    cerr << endl << checkWhoWon() << " team won! ";
                    throw CustomException();
                }
            }
            if (turnEnded)
                break;
        }
    }


    bool gameStuckWhite(){
        int whiteStuckCount=0,whiteCount=0;
        for(int i =0; i< 8; i++){
            for(int j =0; j< 8; j++){
                if((*board)[i][j][0] == 'W'){
                    whiteCount++;
                }
                if((*board)[i][j] != "`" && (*board)[i][j][0] == 'W' && (*board)[i+1][j][0] == 'B'){
                    whiteStuckCount++;
                }
            }
        }
        return whiteCount == whiteStuckCount;
    }
    bool gameStuckBlack(){
        int blackStuckCount=0,blackCount=0;
        for(int i =0; i< 8; i++){
            for(int j =0; j< 8; j++){
                if((*board)[i][j][0] == 'B'){
                    blackCount++;
                }
                if((*board)[i][j] != "`" && (*board)[i][j][0] == 'B' && (*board)[i-1][j][0] == 'W'){
                    blackStuckCount++;
                }
            }
        }
        return blackCount == blackStuckCount;
    }




    static int takeSteps(){
        string steps;
        do
        {
            cout << "Enter the number of steps you want to take (1/2):";
            cin >> steps;

            cout << endl;

        } while (!isValidSteps(steps));

        cout << endl;
        int value = stoi(steps);
        return value;
    }



    void displayTurn(){
        if(BlackTurn) cout << "\nBlacks Turn. Move a black piece\n";
        else cout << "\nWhites Turn. Move a white piece\n";
    }


    void place(string& piece) override
    {

        string blockedInput;

        turnEnded = false;

        if (!isValidPiece(piece))
        {
            do
            {
                cout << "Invalid piece, plz try again:";
                cin >> piece;
                piece = convertToUpper(piece);
            } while (!isValidPiece(piece));
        }
        unique_ptr<int> store = make_unique<int>();
        *store = stoi(string(1, piece[1]));

        if (!hasMovedYet(piece))
        {


            if (WhiteTurn)
            {    for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        if ((*board)[i][j] == piece &&
                            (checkRightDiagonalPieces(piece, i, j) || checkLeftDiagonalPieces(piece, i, j))) {
                            selection(piece, i, j,true);
                        }
                    }
                }
                if(!hasMovedYet(piece)) {
                    int steps = takeSteps();
                    if ((steps == 1 && (*board)[1][*store - 1] != "`") ||
                        (steps == 2 && ((*board)[1][*store - 1] != "`" || (*board)[2][*store - 1] != "`"))) {
                        cout << "\nPiece is blocked by another piece, plz choose another piece to move:";
                        do {
                            cout << "\nEnter correct piece:";
                            cin >> blockedInput;
                            cout << endl;
                            blockedInput = convertToUpper(blockedInput);

                        } while (blockedInput == piece || !isValidPiece(blockedInput));

                        piece = blockedInput;
                        *store = stoi(string(1, piece[1]));
                        if (hasMovedYet(piece)) {
                            ifNotFirst(piece);
                        } else {
                            steps = takeSteps();
                        }
                    }


                    if (!hasMovedYet(piece)) {
                        (*board)[0 + steps][*store - 1] = piece;
                        (*board)[0][*store - 1] = "`";
                    }


                }

            }
            else if (BlackTurn)
            {
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        if ((*board)[i][j] == piece &&
                            (checkRightDiagonalPieces(piece, i, j) || checkLeftDiagonalPieces(piece, i, j))) {
                            selection(piece, i, j, true);
                        }
                    }
                }
                if(!hasMovedYet(piece)) {
                    int steps = takeSteps();

                    if ((steps == 1 && (*board)[6][*store - 1] != "`") ||
                        ((steps == 2 && ((*board)[6][*store - 1] != "`" || (*board)[5][*store - 1] != "`")))) {
                        do {
                            cout << "\nPiece is blocked by another piece, plz choose another piece:";
                            cin >> blockedInput;
                            blockedInput = convertToUpper(blockedInput);

                        } while (blockedInput == piece || !isValidPiece(blockedInput));

                        piece = blockedInput;
                        *store = stoi(string(1, piece[1]));
                        if (hasMovedYet(piece)) {
                            ifNotFirst(piece);
                        } else {
                            steps = takeSteps();
                        }

                    }

                    if (!hasMovedYet(piece)) {
                        (*board)[7 - steps][*store - 1] = piece;
                        (*board)[7][*store - 1] = "`";
                    }


                }

            }

            changeTurn();
        }
        else
        {

            ifNotFirst(piece);

        }

    }

    string checkWhoWon(){
        int whiteDie=0, blackDie=0, whiteWon=0, blackWon=0, blackNeither=0, whiteNeiter=0,blackTotal=0, whiteTotal=0;

        for(auto& whiteWonOrDie :whitePiecesWhoMadeItToTheEnd){
            if(whiteWonOrDie.second.first){
                whiteWon++;
            }
            else if(whiteWonOrDie.second.second){
                whiteDie++;
            }
            else if(!whiteWonOrDie.second.first && !whiteWonOrDie.second.second){
                whiteNeiter++;
            }
        }

        for(auto& blackWonOrDie: blackPiecesWhoMadeItToTheEnd){
            if(blackWonOrDie.second.first){
                blackWon++;
            }
            else if(blackWonOrDie.second.second){
                blackDie++;
            }
            else if(!blackWonOrDie.second.first && !blackWonOrDie.second.second){
                blackNeither++;
            }
        }
        if(blackWon + blackDie != 8){
            blackTotal = blackWon + blackNeither;
        }
        if(whiteWon + whiteDie != 8){
            whiteTotal = whiteWon+ whiteNeiter;
        }
        cout << "\n pieces left:\n black:" << blackNeither << ", White:" << whiteNeiter;
        cout << "\n pieces which made it to the end:\n black:" << blackWon << ", White:" << whiteWon;
        cout << "\n pieces that have been eliminated:\n black:" << blackDie << ", White:" << whiteDie << endl;

        if(blackTotal> whiteTotal) return "Black";
        if(whiteTotal > blackTotal) return "White";
        else  return "tie";
    }


};





int main()
{

    unique_ptr<string> inputPiece = make_unique<string>();
    auto* game1 = new Players();
    auto* ptrgame1 = dynamic_cast<Players*>(game1);
    if (!ptrgame1) {
        std::cerr << "Failed to cast game1 to Players*." << std::endl;
        return 1;
    }
    boardHistory bh;
    CustomStack<unordered_map<string,pair<bool,bool>>> storeWhitePiecesWHoMadeItToTheEndState;
    CustomStack<unordered_map<string,pair<bool,bool>>> storeBlackPiecesWHoMadeItToTheEndState;
    ptrgame1->display();
    storeWhitePiecesWHoMadeItToTheEndState.push(ptrgame1->getWhitePiecesWhoMadeIt());
    storeBlackPiecesWHoMadeItToTheEndState.push(ptrgame1->getBlackPiecesWhoMadeIt());
    do
    {

        try {

            string upperinputpiece;
            string upperInputPiece;

            cout << "Enter the piece you want to move (or type 'help' for more commands):";
            do {
                ptrgame1 ->displayTurn();
                cout << "Enter instruction:";
                cin >> *inputPiece;
                cout << endl;

                 upperinputpiece = *inputPiece;
                 upperInputPiece = convertToUpper(upperinputpiece);
            } while(!game1->isValidPiece(upperInputPiece) && *inputPiece != "-s" && *inputPiece != "-r"
            &&*inputPiece != "-l" && *inputPiece != "-c" && *inputPiece != "help");


            if(*inputPiece == "-s"){
                system("cls");
                ptrgame1->fileExport();
                ptrgame1->display();
            }
            else if(*inputPiece == "help"){
                cout<< "\nList of commands:\n"
                       "-c -> outputs the details of pieces, eliminated, made to end etc\n"
                       "-r -> moves back one step\n"
                       "-s -> saves the game to be played again later\n"
                       "-l -> loads the save file if it exists\n"
                       "pieces(B1->B8) -> moves black piece if its their turn\n"
                       "pieces(W1->W8) -> moves white piece if its their turn\n";
                ptrgame1-> display();
            }
            else if(*inputPiece == "-l"){
                system("cls");
                ptrgame1->fileImport();

                ptrgame1->display();

            }
            else if(*inputPiece == "-c"){
                system("cls");
                ptrgame1->display();
                cout <<"\n" << ptrgame1->checkWhoWon()<< "\n";
            }
            else if(*inputPiece == "-r"){
                bh.pop();
                storeWhitePiecesWHoMadeItToTheEndState.pop();
                storeBlackPiecesWHoMadeItToTheEndState.pop();
                ptrgame1->setBoard(bh.retrieveLast());

                ptrgame1->setWhitePiecesWhoMadeIt(storeWhitePiecesWHoMadeItToTheEndState.top());
                ptrgame1->setBlackPiecesWhoMadeIt(storeBlackPiecesWHoMadeItToTheEndState.top());
                system("cls");
                ptrgame1->display();
            }
            else  {
                ptrgame1->place(upperInputPiece);
                system("cls");
                storeWhitePiecesWHoMadeItToTheEndState.push(ptrgame1->getWhitePiecesWhoMadeIt());
                storeBlackPiecesWHoMadeItToTheEndState.push(ptrgame1->getBlackPiecesWhoMadeIt());

                ptrgame1->display();
                auto b1 = ptrgame1->getBoard();
                bh.push(b1);


            }
        } catch(const exception& e){
            cerr<< e.what();
            break;
        }

    } while (true);
}
