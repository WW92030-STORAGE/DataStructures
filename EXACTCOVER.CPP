#include <vector>
#include <iostream>
#include <set>
#include <climits>

void disp(std::set<int>& s) {
    std::cout << "[";
    for (auto i : s) std::cout << i << " ";
    std::cout << "]\n";
}

void vdisp(std::vector<int>& v) {
    std::cout << "[";
    for (auto i : v) std::cout << i << " ";
    std::cout << "]\n";
}

void matdisp(std::vector<std::vector<bool>>& mat, std::set<int>& rows, std::set<int>& cols) {
    for (int i = 0; i < mat.size(); i++) {
        for (int j = 0; j < mat[i].size(); j++) {
            if (rows.find(i) != rows.end()) std::cout << "X";
            else if (cols.find(j) != cols.end()) std::cout << "X";
            else std::cout << mat[i][j];
        }
        std::cout << std::endl;
    }
}

const bool DEBUG = false;

// matrix / eliminated rows / eliminated cols / temporary std::vector for temp use
bool search(std::vector<std::vector<bool>>& mat, std::set<int>& rows, std::set<int>& cols, std::set<int>& solution) {
    // std::cout << "SEARCHING...\n";
    // matdisp(mat, rows, cols);
    
    
    if (cols.size() == mat[0].size()) { // If the matrix has no unused columns then return true
        // std::cout << "SOLUTION FOUND\n";
        disp(solution); // PRINT OUT SOLUTION
        return true;
    }
    
    int col = -1;
    int mincount = INT_MAX;
    int rowcount = 0;
    
    for (int c = 0; c < mat[0].size(); c++) { // Choose a column with ones
        if (cols.find(c) == cols.end()) { // The column must be unused
            rowcount = 0;
            for (int row = 0; row < mat.size(); row++) {
                if (rows.find(row) != rows.end()) continue; // Rows must be unused to be considered
                if (mat[row][c]) rowcount++;
            }
            if (rowcount < mincount) {
                col = c;
                mincount = rowcount;
            }
        } // use a non deleted col
    }
    
    if (col < 0) { // No nonzero column
        // std::cout << "BAD BAD BAD\n";
        return false;
    }
    
    // std::cout << "COLUMN " << col << std::endl;
    
    std::vector<int> temp;
    
    for (int i = 0; i < mat.size(); i++) {
        if (rows.find(i) != rows.end()) continue; // do not use deleted rows
        if (mat[i][col]) temp.push_back(i);
    }
    
    // std::cout << "CHOICES IN COLUMN - ";
    // for (auto i : temp) std::cout << i << " ";
    // std::cout << std::endl;
    
    std::set<int> ar;
    std::set<int> ac;
    
    bool retval = false;
    
    for (auto row : temp) {
        
    // std::cout << "ROW " << row << std::endl;
    
    solution.insert(row);
    
    for (int c = 0; c < mat[row].size(); c++) {
        if (cols.find(c) != cols.end()) continue;
        if (!mat[row][c]) continue; // select columns in row with value 1
        for (int r = 0; r < mat.size(); r++) {
            if (rows.find(r) != rows.end()) continue;
            if (!mat[r][c]) continue; // select rows in the column with value 1
            rows.insert(r);
            ar.insert(r);
        }
        cols.insert(c);
        ac.insert(c);
    }
    
    // std::cout << "ROWS ";
    // disp(rows);
    // std::cout << "COLS ";
    // disp(cols);
    
    retval = retval | search(mat, rows, cols, solution);
    if (retval) return retval;
    
    /*
    std::cout << "END RECURSIVE SEARCH\n";
    std::cout << "ADDING BACK ROWS ";
    disp(ar);
    std::cout << "ADDING BACK COLS ";
    disp(ac);
    disp(rows);
    disp(cols);
    */
    
    
    for (auto r : ar) {
        if (rows.find(r) != rows.end()) rows.erase(rows.find(r));
    }
    for (auto c : ac) {
        if (cols.find(c) != cols.end()) cols.erase(cols.find(c));
    }
    
    solution.erase(row);
    
    // disp(rows);
    // disp(cols);
    
    }
    
    return retval;
}

int main()
{
    std::vector<std::vector<bool>> mat(6, std::vector<bool>(7, false)); // Example from GeeksForGeeks
    mat[0][0] = mat[0][3] = mat[0][6] = true;
    mat[1][0] = mat[1][3] = true;
    mat[2][3] = mat[2][4] = mat[2][6] = true;
    mat[3][2] = mat[3][4] = mat[3][5] = true;
    mat[4][1] = mat[4][2] = mat[4][5] = mat[4][6] = true;
    mat[5][1] = mat[5][6] = true;
    
    std::set<int> r;
    std::set<int> c;
    std::set<int> solution;
    
    matdisp(mat, r, c);
    
    std::cout << search(mat, r, c, solution); // rows 1 3 5

    return 0;
}
