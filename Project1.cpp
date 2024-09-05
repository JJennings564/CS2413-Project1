#include <iostream>

using namespace std;


int main () {
    int n, m, cv, noNSV;
    SparseMatrix* temp;

    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);
    //Write the Statements to read in the first matrix

    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);
    //Write the Statements to read in the second matrix
    cout << "First one in matrix format" << endl;
    (*firstOne).displayMatrix();

   // cout << "First one in sparse matrix format" << endl;
    cout << (*firstOne);

    cout << "Second one in matrix format" << endl;
    (*secondOne).displayMatrix();

    cout << "Second one in sparse matrix format" << endl;
    cout << (*secondOne);

    cout << "Transpose of the first one in matrix" << endl;
    cout << (*(*firstOne).Transpose());

    cout << "Matrix Addition Result" << endl;

    temp = (*(*firstOne).Addition(secondOne));
    cout << temp;
    (*temp).displayMatrix();

    cout << "Matrix Multiplication Result" << endl;

    temp = (*(*firstOne).Multiply(secondOne));
    cout << temp;
    (*temp).displayMatrix();
}

class SparseRow {
    protected:
    int row;   // Row#
    int col;   // Column#
    int value; // We will assume that all our values will be integers
public:
    SparseRow(int r = -1, int c = -1, int v = 0); // default constructor; row=-1;col=-1;value=0
    void display();                               // print Row#, Column#, value
    friend ostream &operator<<(ostream& os, const SparseRow& sr) {
        os << "Row: " << sr.row << ", Col: " << sr.col << ", Value: " << sr.value;
        return os;
    }

    void display() {
    cout << "Row: " << row << ", Col: " << col << ", Value: " << value << endl;
    }
    SparseRow(int r, int c, int v) {
    row = r;
    col = c;
    value = v;
    }

    // other methods that are necessary such as get and set
    int getRow() { return row; }
    int getCol() { return col; }
    int getValue() { return value; }

    void setRow(int r) { row = r; }
    void setCol(int c) { col = c; }
    void setValue(int v) { value = v; }
};

class SparseMatrix {
    protected:
        int noRows; //Number of rows of the original matrix
        int noCols; //Number of columns of the original matrix
        int commonValue; //read from input
        int noNonSparseValues;
        SparseRow* myMatrix; //Array
    public:
        SparseMatrix ();
        SparseMatrix (int n, int m, int cv, int noNSV){
            noRows = n;
            noCols = m;
            commonValue = cv;
            noNonSparseValues = noNSV;
        }
        SparseMatrix* Transpose (){
        for(int i = 0; i < noCols; i++){
            for(int j = 0; i < noRows; i++){

            }
        }
        } //Matrix Transpose
        SparseMatrix* Multiply (SparseMatrix& M);
        SparseMatrix* Addition (SparseMatrix& M);
        friend ostream &operator<<(ostream& os, const SparseRow);
        void displayMatrix (){



        } //Display the matrix in its original format
        //other methods that are necessary such as get and set
};