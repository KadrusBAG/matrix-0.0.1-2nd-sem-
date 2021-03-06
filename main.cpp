#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class matrix_t
{
private:
    int** data;
    unsigned int rows;
    unsigned int collumns;

public:
    matrix_t()
    {
        data = nullptr;
        rows = 0;
        collumns = 0;
    }

    matrix_t add(matrix_t& other) const
    {
        if (this->rows == other.rows && this->collumns == other.collumns)
        {
            for (int i = 0; i < this->rows; ++i)
            {
                for (int j = 0; j < this->collumns; ++j)
                {
                    other.data[i][j] = this->data[i][j] + other.data[i][j];
                }
            }
        }
        else
        {
            error();
        }
        return other;
    }

    matrix_t sub(matrix_t& other) const
    {
        if (this->rows == other.rows && this->collumns == other.collumns)
        {
            for (int i = 0; i < this->rows; ++i)
            {
                for (int j = 0; j < this->collumns; ++j)
                {
                    other.data[i][j] = this->data[i][j] - other.data[i][j];
                }
            }
        }
        else
        {
            error();
        }
        return other;
    }

    matrix_t mul(matrix_t& other) const
    {
        matrix_t result;
        if (this->collumns == other.rows)
        {
            result.rows = this->rows;
            result.collumns = other.collumns;
            result.data = new int*[this->rows];
            for (int i = 0; i < this->rows; ++i)
            {
                result.data[i] = new int[other.collumns];
            }
            for (int i = 0; i < this->rows; ++i)
            {
                for (int j = 0; j < other.collumns; ++j)
                {
                    int sum = 0;
                    for (int k = 0; k < other.rows; ++k)
                    {
                        sum += this->data[i][k] * other.data[k][j];
                    }
                    result.data[i][j] = sum;
                }
            }
        }
        else
        {
            error();
        }
        return result;
    }

    matrix_t trans(matrix_t& other) const
    {
        other.rows = this->collumns;
        other.collumns = this->rows;
        other.data = new int*[other.rows];
        for (int i = 0; i < this->collumns; ++i)
        {
            other.data[i] = new int[other.collumns];
        }
        for (int i = 0; i < other.rows; ++i)
        {
            for (int j = 0; j < other.collumns; ++j)
            {
                other.data[i][j] = this->data[j][i];
            }
        }
        return other;
    }
    
    matrix_t(const matrix_t & matrix) {
		this->rows = matrix.rows;
		this->collumns = matrix.collumns;
		this->data = new int *[this->rows];
		for (unsigned int i = 0; i < this->rows; ++i) {
			data[i] = new int[this->collumns];
			for (unsigned int j = 0; j < this->collumns; ++j) {
				this->data[i][j] = matrix.data[i][j];
			}
		}
	}
	
	matrix_t & operator=(const matrix_t & matrix) {
		for (unsigned int i = 0; i < rows; ++i) {
			delete[] this->data[i];
		}
		delete[] this->data;
		this->rows = matrix.rows;
		this->collumns = matrix.collumns;
		this->data = new int * [rows];
		for (unsigned int i = 0; i < this->rows; ++i) {
			this->data[i] = new int[this->collumns];
			for (unsigned int j = 0; j < this->collumns; ++j) {
				this->data[i][j] = matrix.data[i][j];
			}
		}			
	}
    
    

    std::ifstream& read(std::ifstream& stream, std::string fileName)
    {
        stream.open(fileName.c_str());
        char symbol;
        if (stream.is_open() && stream >> rows && stream >> symbol && symbol == ','
            && stream >> collumns)
        {
            data = new int*[rows];
            for (int i = 0; i < rows; ++i)
            {
                data[i] = new int[collumns];
                for (int j = 0; j < collumns; ++j)
                {
                    stream >> data[i][j];
                }
            }
        }
        else
        {
            error();
        }
        return stream;
    }

    std::ostream& write(std::ostream& stream)
    {
        for (unsigned int i = 0; i < this->rows; i++)
        {
            for (unsigned int j = 0; j < this->collumns; j++)
            {
                cout << this->data[i][j] << ' ';
            }
            cout << endl;
        }
        return stream;
    }

    void error() const
    {
        std::cout << "An error has occured while reading input data\n";
    }

    ~matrix_t()
    {
        for (unsigned int i = 0; i < this->rows; i++) {
		delete [] this->data[i];
	    }
	    delete []this->data;
    }
};

int main()
{
    matrix_t A;
    string line;
    getline(cin, line);
    istringstream stroka(line);
    string fileNameA;
    ifstream streamA;
    stroka >> fileNameA;
    char op;
    if ((A.read(streamA, fileNameA)) && (stroka >> op))
    {
        if (op == '+' || op == '-' || op == '*')
        {
            matrix_t B;
            matrix_t R;
            string fileNameB;
            ifstream streamB;
            stroka >> fileNameB;
            if (B.read(streamB, fileNameB))
            {
                if (op == '+')
                {
                    R = A.add(B);
                    R.write(cout);
                }
                else if (op == '-')
                {
                    R = A.sub(B);
                    R.write(cout);
                }
                else if (op == '*')
                {
                    R = A.mul(B);
                    R.write(cout);
                }
            }
            else
            {
                B.error();
            }
            streamB.close();
        }
        else if (op == 'T')
        {
            matrix_t R;
            R = A.trans(R);
            R.write(cout);
        }
        else
        {
            A.error();
        }
    }
    else
    {
        A.error();
    }
    streamA.close();
    cin.get();
    return 0;
}
