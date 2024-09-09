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

        SparseMatrix* result = new SparseMatrix(noRows, noCols, commonValue, 0);
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
                        valueA = myMatrix[k].getValue();
                        break;
                    }
                }

                // Find value in matrix M
                for (int k = 0; k < M.noNonSparseValues; k++) {
                    if (M.myMatrix[k].getRow() == i && M.myMatrix[k].getCol() == j) {
                        valueB = M.myMatrix[k].getValue();
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

        result->noNonSparseValues = resultNonSparseCount;
        delete[] result->myMatrix;
        result->myMatrix = new SparseRow[resultNonSparseCount];
        for (int i = 0; i < resultNonSparseCount; i++) {
            result->myMatrix[i] = tempMatrix[i];
        }

        delete[] tempMatrix;
        return result;
    }

    SparseMatrix* Multiply(SparseMatrix& M) {
        if (noCols != M.noRows) {
            cout << "Matrix multiplication is not possible" << endl;
            return nullptr;
        }

        SparseMatrix* result = new SparseMatrix(noRows, M.noCols, commonValue, 0);
        SparseRow* tempMatrix = new SparseRow[noRows * M.noCols];
        int resultNonSparseCount = 0;

        // Perform multiplication
        for (int i = 0; i < noRows; i++) {
            for (int j = 0; j < M.noCols; j++) {
                int sum = 0;
                for (int k = 0; k < noCols; k++) {
                    int valueA = commonValue;
                    int valueB = M.commonValue;

                    // Find value in matrix A
                    for (int a = 0; a < noNonSparseValues; a++) {
                        if (myMatrix[a].getRow() == i && myMatrix[a].getCol() == k) {
                            valueA = myMatrix[a].getValue();
                            break;
                        }
                    }

                    // Find value in matrix B
                    for (int b = 0; b < M.noNonSparseValues; b++) {
                        if (M.myMatrix[b].getRow() == k && M.myMatrix[b].getCol() == j) {
                            valueB = M.myMatrix[b].getValue();
                            break;
                        }
                    }

                    sum += valueA * valueB;
                }

                if (sum != commonValue) {
                    tempMatrix[resultNonSparseCount] = SparseRow(i, j, sum);
                    resultNonSparseCount++;
                }
            }
        }

        result->noNonSparseValues = resultNonSparseCount;
        delete[] result->myMatrix;
        result->myMatrix = new SparseRow[resultNonSparseCount];
        for (int i = 0; i < resultNonSparseCount; i++) {
            result->myMatrix[i] = tempMatrix[i];
        }

        delete[] tempMatrix;
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

    // Display first matrix in sparse format
    cout << "First one in sparse matrix format" << endl;
    cout << *firstOne;

    // Transpose of the first matrix
    cout << "After transpose" << endl;
    SparseMatrix* transposed = firstOne->Transpose();
    cout << *transposed;

    // Display first matrix in normal format
    cout << "First one in matrix format" << endl;
    firstOne->displayMatrix();

    // Display second matrix in sparse format
    cout << "Second one in sparse matrix format" << endl;
    cout << *secondOne;

    // Transpose of the second matrix
    cout << "After transpose" << endl;
    SparseMatrix* secondTransposed = secondOne->Transpose();
    cout << *secondTransposed;

    // Display second matrix in normal format
    cout << "Second one in matrix format" << endl;
    secondOne->displayMatrix();

    // Matrix addition result
    cout << "Matrix Addition Result" << endl;
    temp = firstOne->Addition(*secondOne);
    if (temp != nullptr) {
        temp->displayMatrix();
        delete temp;
    }

    // Matrix multiplication result
    cout << "Matrix Multiplication Result" << endl;
    temp = firstOne->Multiply(*secondOne);
    if (temp != nullptr) {
        temp->displayMatrix();
        delete temp;
    }

    // Clean up
    delete firstOne;
    delete secondOne;
    delete transposed;
    delete secondTransposed;

    return 0;
}


/*              LLM / chatGPT documentation and usage

    The first question for this project that I asked chatGPT about was "My cout statements in my main
     class are giving an error dued to my overwriting statements not being formatted correctly. Could
     you reformat them in a way that will not produced these errors and overwrite properly."

     When chatGPT gave me an adequate answer for this question it also gave me an early version of 
     of my sparseMatrix constructor in the form "SparseMatrix(int n, int m, int cv, int noNSV)
    : noRows(n), noCols(m), commonValue(cv), noNonSparseValues(noNSV)" I used this opportunity to learn
    the declaration method chatgpt was using by asking it what that line does but ended up rewriting my
    method in a way that was more understandable to me and that I was more comfortable with.

    This got me to a point where my code was able to run and produce a close result for the matrix editing
    methods and I came up with only 1 error which caused the first NSV to be skipped if there were 2 in 
    the same row. I asked chatGPT to check my logic and explained the error and it was able to explain
    what caused it and give me the information to edit my method and fix the error

    After this point all that was left was my addition and multiplication methods which would prove to be 
    the most difficult to comprehend for me. I was able to get the basic outline for both methods and 
    had a good base but was still producing a few errors and having problems with my output so I gave chatGPT
    the prompt "I have my methods for addition and multiplication but their output is not coming out correctly
    such as the addition method only outputing the first value and none of the others and the multiplication
    method only printing sparse values." 

    The solution that chatGPT came up with fixed my issues but was doing it in a weird way that was not
    in line with my main method or something I would write so I had to clarify a few times with it what
    I wanted it to write and to get something that I was happy with using for my project.

    After that was done my code was working correctly and matched the corresponding input / output files
    that were provided and runs smoothly on my device in VSCode. I asked chatGPT to do one final cleanup
    and remove any unnecessary code and finished up the project.

*/
/*                  debugging and testing plan 

The main bulk of my debugging was through trial and error and running my code at every point possible
throughout the development. Anytime I would make a major change to a class or method and had no large
compiler errors, I would run my code through the terminal and examine the output I got so I could see 
how my code was functioning and if it was doing what I expected.

Sometimes I would change a small piece of code and it would send an error ripple through my code and
cause a large headache where I would then have to backtrack and figure out what caused the issue. A few
times I would change a piece of code and completely unrelated parts in the main method would pop up with
error squiggles even though there was no real error present and the code would still compile with no issues.
This caused a big headache but once I realized the errors weren't real it made things go a lot quicker.

For issues that I did not understand I would ask chatGPT what the issues meant and ask it to explain to me
the issue and why it was happening so I could understand it better and try to fix it to the best of 
my ability.

At one point I was attempting to print out the results of the addition and multiplication methods within
the method themselves instead of the main methods which would result in something called a segmentation fault
where I was refrencing a null pointer. Once I understood how to do the printing in the main method and 
returned a matrix from the addition/multiplication methods I was no longer getting an error and my code
was fully working.

*/