#include <iostream>

using namespace std;

class SparseRow {
protected:
    int row;   // Row#
    int col;   // Column#
    int value; // We will assume that all our values will be integers

public:
    // Constructor with default values
    SparseRow(int r = -1, int c = -1, int v = 0){
        row = r;
        col = c;
        value = v;
    }

    // Overload the << operator
    friend ostream& operator<<(ostream& os, const SparseRow& sr) {
        os << sr.row << ", " << sr.col << ", " << sr.value;
        return os;
    }

    // Other methods (getters/setters)
    int getRow() { return row; }
    int getCol() { return col; }
    int getValue() { return value; }

    void setRow(int r) { row = r; }
    void setCol(int c) { col = c; }
    void setValue(int v) { value = v; }
};

class SparseMatrix {
protected:
    int noRows;           // Number of rows of the original matrix
    int noCols;           // Number of columns of the original matrix
    int commonValue;      // Default common value
    int noNonSparseValues;// Number of non-default values
    int value;
    SparseRow* myMatrix;  // Array of SparseRow

public:
    // Default constructor
    SparseMatrix(){
        noRows = 0;
        noCols = 0;
        commonValue = 0;
        noNonSparseValues = 0;
        myMatrix = nullptr;
    }

    // Constructor with parameters
    SparseMatrix(int n, int m, int cv, int noNSV) {
        noRows = n;
        noCols = m;
        commonValue = cv;
        noNonSparseValues = noNSV;
        myMatrix = new SparseRow[noNonSparseValues];

        int NSIndex = 0;
        for(int i = 0; i < noRows; i++){
            for (int j = 0; j < noCols; j++){
                cin >> value;
                if (value != commonValue){
                    myMatrix[NSIndex] = SparseRow(i,j,value);
                    NSIndex++;
                }
            }
        }
    }

    // Destructor
    ~SparseMatrix() {
        delete[] myMatrix;
    }

    // Overload << operator for SparseMatrix
    friend ostream& operator<<(ostream& os, const SparseMatrix& sm) {
        for (int i = 0; i < sm.noNonSparseValues; i++) {
            os << sm.myMatrix[i] << endl;  // Using overloaded SparseRow <<
        }
        return os;
    }

    // Matrix transpose method
    SparseMatrix* Transpose() {
        SparseMatrix* transposed = new SparseMatrix(noCols, noRows, commonValue, noNonSparseValues);
        for (int i = 0; i < noNonSparseValues; i++) {
            int newRow = myMatrix[i].getCol();
            int newCol = myMatrix[i].getRow();
            int value = myMatrix[i].getValue();
            transposed->myMatrix[i] = SparseRow(newRow, newCol, value);
        }
        return transposed;
    }

    void displayMatrix() {
        // Display matrix in normal format (implementation needed)
        int index = 0;

        for(int i = 0; i < noRows; i++){
            for(int j = 0; j < noCols; j++){
                if (index < noNonSparseValues && myMatrix[index].getRow() == i &&
                myMatrix[index].getCol() == j) {
                    cout << myMatrix[index].getValue() << " ";
                    index++;
                } else {
                // Print the common value
                cout << commonValue << " ";
            }
            }
            cout << endl;
        }
    }

    SparseMatrix* Addition(SparseMatrix& M) {
         // Check if the two matrices have the same dimensions
    if (noRows != M.noRows || noCols != M.noCols) {
        cout << "Matrices cannot be added due to dimension mismatch!" << endl;
        return nullptr;
    }

    // Step 1: Create a new matrix to store the result
    SparseMatrix* result = new SparseMatrix(noRows, noCols, commonValue + M.commonValue, 0); // Temp noNonSparseValues will be set later

    int resultNonSparseCount = 0;

    // Step 2: Traverse both matrices and add corresponding values
    int currentIndexA = 0, currentIndexB = 0;
    for (int i = 0; i < noRows; i++) {
        for (int j = 0; j < noCols; j++) {
            int valueA = (currentIndexA < noNonSparseValues && myMatrix[currentIndexA].getRow() == i && myMatrix[currentIndexA].getCol() == j) ? myMatrix[currentIndexA++].getValue() : commonValue;
            int valueB = (currentIndexB < M.noNonSparseValues && M.myMatrix[currentIndexB].getRow() == i && M.myMatrix[currentIndexB].getCol() == j) ? M.myMatrix[currentIndexB++].getValue() : M.commonValue;

            int sum = valueA + valueB;

            // If the sum is not the common value, store it in the result matrix
            if (sum != (commonValue + M.commonValue)) {
                result->myMatrix[resultNonSparseCount] = SparseRow(i, j, sum);
                resultNonSparseCount++;
            }
        }
    }

    // Update the number of non-sparse values in the result matrix
    result->noNonSparseValues = resultNonSparseCount;

    // Step 3: Return the result matrix
    return result;
    }

    SparseMatrix* Multiply(SparseMatrix& M) {
        // Placeholder logic for multiplication (implementation needed)
        return this;
    }
};

int main() {
    int n, m, cv, noNSV;
    SparseMatrix* temp;

    // Input for first matrix
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);

    // Input for second matrix
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);

    //Display first matrix in sparse format
    cout << "First one in sparse matrix format" << endl;
    cout << *firstOne << endl;

    // Transpose of the first matrix
    cout << "After Transpose" << endl;
    SparseMatrix* transposed = firstOne->Transpose();
    cout << *transposed << endl;

    // Display first matrix
    cout << "First one in matrix format" << endl;
    firstOne->displayMatrix();

    //Display second matrix in sparse format
    cout << "Second one in sparse matrix format" << endl;
    cout << *secondOne << endl;

    //Transpose of the second matrix
    cout << "After Transpose" << endl;
    SparseMatrix* secondTransposed = secondOne->Transpose();
    cout << *secondTransposed << endl;

    // Display second matrix
    cout << "Second one in matrix format" << endl;
    secondOne->displayMatrix();

    // Matrix addition result
    cout << "Matrix Addition Result" << endl;
    temp = firstOne->Addition(*secondOne);
    cout << *temp;
    temp->displayMatrix();

    // Matrix multiplication result
    cout << "Matrix Multiplication Result" << endl;
    temp = firstOne->Multiply(*secondOne);
    cout << *temp;
    temp->displayMatrix();

    // Clean up
    delete firstOne;
    delete secondOne;
    delete transposed;
    delete temp;

    return 0;
}