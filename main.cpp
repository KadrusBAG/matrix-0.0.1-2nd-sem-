#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class matrix_t{
private:
	int ** data;
	unsigned int rows;
	unsigned int collumns;
public:
	matrix_t() {
		data=nullptr;
		rows=0;
		collumns=0;
	}

	matrix_t add(matrix_t & other) const {
		if (this->rows==other.rows && this->collumns==other.collumns){
			for(int i=0; i<this->rows; ++i){
				for(int j= 0; j<this->collumns; ++j){
					other.data[i][j]=this->data[i][j]+other.data[i][j];
				}
			}
		}
		else{
			error();
		}
		return other;
	}

	matrix_t sub(matrix_t & other) const {
		if(this->rows==other.rows && this->collumns==other.collumns){
			for(int i=0; i<this->rows; ++i){
				for(int j=0; j<this->collumns; ++j){
					other.data[i][j]=this->data[i][j]-other.data[i][j];
				}
			}
		}
		else{
			error();
		}
		return other;
	}

	matrix_t mul(matrix_t & other) const {
		matrix_t result;
		if(this->collumns==other.rows){
			result.rows=this->rows;
			result.collumns=other.collumns;
			result.data=new int *[this->rows];
			for(int i=0; i<this->rows; ++i){
				result.data[i]=new int[other.collumns];
			}
			for(int i=0; i<this->rows; ++i){
				for(int j=0; j<other.collumns; ++j){
					int sum=0;
					for(int k=0; k<other.rows; ++k) {
						sum+=this->data[i][k]*other.data[k][j];
					}
					result.data[i][j] = sum;
				}
			}
		}
		else{
			error();
		}
		result.~matrix_t();
		return result;
	}

	matrix_t trans(matrix_t & other) const {
		other.rows=this->collumns;
		other.collumns=this->rows;
		other.data=new int *[other.rows];
		for(int i=0; i<this->collumns; ++i){
			other.data[i]=new int [other.collumns];
		}
		for(int i=0; i<other.rows; ++i){
			for(int j=0; j<other.collumns; ++j){
				other.data[i][j]=this->data[j][i];
			}
		}
		return other;
	}

	std::ifstream & read(std::ifstream & stream, std::string fileName){
		stream.open(fileName.c_str());
		char symbol;
		if(stream.is_open() && stream>>rows && stream>>symbol && symbol==',' && stream>>collumns){
			data=new int *[rows];
			for(int i=0; i<rows; ++i){
				data[i]=new int[collumns];
				for(int j=0; j<collumns; ++j){
					stream>>data[i][j];
				}
			}
		}
		else{
			error();
		}
		return stream;
	}

	std::ofstream & write(std::ofstream & stream, std::string fileName) const {
        stream.open(fileName.c_str());
		if(stream.is_open()){
			for(int i=0; i<this->rows; ++i){
				for(int j = 0; j<this->collumns; ++j){
					stream<<this->data[i][j]<<'\t';
					if(j==this->collumns-1){
						stream<<'\n';
					}
				}
			}
		}
		else{
			error();
		}
		return stream;
	}

	void error() const {
		std::cout << "An error has occured while reading input data\n";
	}

	~matrix_t(){
    }
};

int main(){
	matrix_t A;
    string line;
	getline(cin, line);
	istringstream stroka(line);
	string fileNameA;	
	ifstream streamA;	
	stroka>>fileNameA;
	char op;
	if((A.read(streamA, fileNameA)) && (stroka>>op)){
		if(op=='+' || op=='-' || op=='*'){
			matrix_t B;
			matrix_t R;
			string fileNameB, fileNameR="result.txt";
			ifstream streamB;
			stroka>>fileNameB;
			ofstream streamR;
			if(B.read(streamB, fileNameB)){
				if(op=='+'){
					R=A.add(B);
					R.write(streamR, fileNameR);
				}
				else if(op=='-') {
					R=A.sub(B);
					R.write(streamR, fileNameR);
				}
				else if(op=='*'){
					R=A.mul(B);
					R.write(streamR, fileNameR);
				}
			}
			else{
				B.error();
			}
			streamB.close();
			streamR.close();
		}
		else if(op=='T'){
			matrix_t R;
			string fileNameR="result.txt";
			ofstream streamR;
			R=A.trans(R);
			R.write(streamR, fileNameR);
			streamR.close();
		}
		else{
			A.error();
		}
	}
	else {
		A.error();
	}
	streamA.close();
    cin.get();
    return 0;
}