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
    if (noRows != M.noRows || noCols != M.noCols) {
        cout << "Matrix addition is not possible" << endl;
        return nullptr;
    }

    // result matrix to store new matrix
    SparseMatrix* result = new SparseMatrix(noRows, noCols, commonValue, 0);

    // Array to store sum in sparse format
    SparseRow* tempMatrix = new SparseRow[noRows * noCols];
    int resultNonSparseCount = 0;

    // Loop through each cell of the matrix
    for (int i = 0; i < noRows; i++) {
        for (int j = 0; j < noCols; j++) {
            int valueA = commonValue;
            int valueB = M.commonValue;

            // Find value in current matrix
            for (int k = 0; k < noNonSparseValues; k++) {
                if (myMatrix[k].getRow() == i && myMatrix[k].getCol() == j) {
                    valueA = myMatrix[k].getValue(); // Get value from the first matrix
                    break;
                }
            }

            // Find value in matrix M
            for (int k = 0; k < M.noNonSparseValues; k++) {
                if (M.myMatrix[k].getRow() == i && M.myMatrix[k].getCol() == j) {
                    valueB = M.myMatrix[k].getValue(); // Get value from the second matrix
                    break;
                }
            }

            int sum = valueA + valueB;

            // Only store non-sparse values
            if (sum != commonValue) {
                tempMatrix[resultNonSparseCount] = SparseRow(i, j, sum);
                resultNonSparseCount++;
            }
        }
    }

    // Set the non-sparse values count and transfer results to the result matrix
    result->noNonSparseValues = resultNonSparseCount;
    delete[] result->myMatrix;  // Delete old empty matrix
    result->myMatrix = new SparseRow[resultNonSparseCount];
    for (int i = 0; i < resultNonSparseCount; i++) {
        result->myMatrix[i] = tempMatrix[i];
    }

    delete[] tempMatrix; // Free temp storage
    return result;
}

    SparseMatrix* Multiply(SparseMatrix& M) {
        if (noRows != M.noCols){
            cout << "Matrix multiplication is not possible" << endl;
            return nullptr;
        }
        // Create a result matrix with dimensions (noRows x M.noCols)
    SparseMatrix* result = new SparseMatrix(noRows, M.noCols, commonValue, 0); // We don't know noNonSparseValues yet

    // Temporary array to store the non-sparse values of the result matrix
    int nonSparseCount = 0;
    SparseRow* tempArray = new SparseRow[noRows * M.noCols];

    // Perform matrix multiplication
    for (int i = 0; i < noRows; i++) {
        for (int j = 0; j < M.noCols; j++) {
            int sum = 0;

            // Compute the dot product of row i from first matrix and column j from second matrix
            for (int k = 0; k < noCols; k++) {
                int valueA = commonValue;
                int valueB = M.commonValue;

                // Find value in matrix A (row i, column k)
                for (int a = 0; a < noNonSparseValues; a++) {
                    if (myMatrix[a].getRow() == i && myMatrix[a].getCol() == k) {
                        valueA = myMatrix[a].getValue();
                        break;
                    }
                }

                // Find value in matrix B (row k, column j)
                for (int b = 0; b < M.noNonSparseValues; b++) {
                    if (M.myMatrix[b].getRow() == k && M.myMatrix[b].getCol() == j) {
                        valueB = M.myMatrix[b].getValue();
                        break;
                    }
                }

                // Multiply the two values and add to the sum
                sum += valueA * valueB;
            }

            // If the result is not the common value, store it
            if (sum != commonValue) {
                tempArray[nonSparseCount] = SparseRow(i, j, sum);
                nonSparseCount++;
            }
        }
    }

    // Create a new SparseMatrix with the computed non-sparse values
    result->noNonSparseValues = nonSparseCount;
    result->myMatrix = new SparseRow[nonSparseCount];
    for (int i = 0; i < nonSparseCount; i++) {
        result->myMatrix[i] = tempArray[i];
    }

    // Clean up temporary storage
    delete[] tempArray;

    return result;
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
    cout << *firstOne;

    // Transpose of the first matrix
    cout << "After Transpose" << endl;
    SparseMatrix* transposed = firstOne->Transpose();
    cout << *transposed;

    // Display first matrix
    cout << "First one in matrix format" << endl;
    firstOne->displayMatrix();

    //Display second matrix in sparse format
    cout << "Second one in sparse matrix format" << endl;
    cout << *secondOne;

    //Transpose of the second matrix
    cout << "After Transpose" << endl;
    SparseMatrix* secondTransposed = secondOne->Transpose();
    cout << *secondTransposed;

    // Display second matrix
    cout << "Second one in matrix format" << endl;
    secondOne->displayMatrix();

    // Matrix addition result
    cout << "Matrix Addition Result" << endl;
    temp = firstOne->Addition(*secondOne);
    temp->displayMatrix();

    // Matrix multiplication result
    cout << "Matrix Multiplication Result" << endl;
    temp = firstOne->Multiply(*secondOne);
    temp->displayMatrix();

    // Clean up
    delete firstOne;
    delete secondOne;
    delete transposed;
    delete temp;

    return 0;
}