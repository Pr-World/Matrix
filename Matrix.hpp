#include <iostream>
#include <vector>
#include<string>
#include <initializer_list>
#include <sstream>
using namespace std;

class Matrix
{
public:
	const unsigned int state_ok = 200;
	const unsigned int err_not_found = 404;
	const unsigned int err_forbidden = 403;
	// normal constructor
	Matrix(size_t rows, size_t cols):mRows(rows),mCols(cols),mData(rows * cols),error(200){
		for (size_t i = 0; i < rows*cols; ++i)
		{
			mData[i]=0;
		}
	}
	// constructor of double array
	Matrix(size_t rows, size_t cols, void* imtr):mRows(rows),mCols(cols),mData(rows*cols),
	error(200)
	{
		for (size_t i = 0; i < rows; ++i){
			for (size_t j = 0; j < cols; ++j){
				mData[i * mCols + j] = ((double*)(imtr))[i*mCols+j];
			}
		}
	}
	// constructor of vector matrix
	Matrix(vector<vector<double>>& dval):mRows(dval.size()),mCols(dval[0].size()),
	mData(mRows*mCols),error(200){
		for (size_t i = 0; i < mRows; ++i){
			for (size_t j = 0; j < mCols; ++j){
				mData[i * mCols + j] = dval[i][j];
			}
		}
	}
	// constructor using intializer_list Ex:- {{1,2},{3,4}}
	Matrix(initializer_list<initializer_list<double>> list){
		this->mRows = list.size();
		this->mCols = list.begin()->size();
		this->mData.resize(this->mCols*this->mRows);
		this->error = 200;
		size_t iji=0,ijj=0;
		for (const initializer_list<double>* i=list.begin(); i != list.end(); ++i)
		{
			ijj=0;
			for (const double* j=i->begin(); j != i->end(); ++j)
			{
				mData[iji*mCols+ijj]=(double)*j;
				ijj++;
			}
			iji++;
		}
	}
	// Destructor
	~Matrix(void){
		this->mData.resize(0,0);
		this->mCols=0;
		this->mRows=0;
	}

	void set_error(unsigned int err){
		this->error = err;
	}

	unsigned int get_error(void){
		unsigned int a = this->error;
		// 200 OK
		this->error = 200;
		// return the actual error!
		return a;
	}

	double& operator()(size_t i, size_t j){
		return mData[i * mCols + j];
	}

	double operator()(size_t i, size_t j) const{
		if(i>=mRows||j>=mCols){return 0;}
		return mData[i * mCols + j];
	}

	void set(size_t i, size_t j, double val){
		if(!(i<mRows&&j<mCols)){
			// 404 not found!
			this->set_error(404);
			return;
		}
		mData[i * mCols + j]=val;
	}

	string str(void){
		stringstream ss;
		// making the string itself [ i.e making up output! ]
		ss << "[" << endl;
		for (size_t i = 0; i < mRows; ++i){
			ss << "  ";
			for (size_t j = 0; j < mCols; ++j){
				ss << (*this)(i,j);
				if(j!=mCols-1)
					ss << "\t";
			}
			ss << endl;
		}
		ss << "]";
		return ss.str();
	}

	// easy convert to str! [ use ~varName / varName.str() ]
	string operator~(void){
		return this->str();
	}

	void print(){
		cout << ~(*this) << endl;
	}

	vector<double> operator[](size_t i){
		vector<double> a;
		for (size_t j = 0; j < mCols; ++j){
			a.push_back(mData[i*mCols+j]);
		}
		return a;
	}

	vector<vector<double>> toVector(){
		vector<vector<double>> a;
		for (size_t i = 0; i < mRows; ++i)
			a.push_back((*this)[i]);
		return a;
	}

	size_t row(){
		return mRows;
	}
	size_t col(){
		return mCols;
	}

	//-=-=-=-=-=-=-=-=- Matrix Arithmetic Start -=-=-=-=-=-=-
	Matrix operator=(Matrix m){
		Matrix a = Matrix(m.row(),m.col());
		for (size_t i = 0; i < m.row(); ++i)
		{
			for (size_t j = 0; j < m.col(); ++j)
			{
				a.set(i,j,m(i,j));
			}
		}
		return a;
	}

	bool operator==(Matrix m){
		bool tr = m.row()==this->mRows;
		tr &= m.col()==this->mCols;
		if(!tr){return tr;}
		// value check
		for (size_t i = 0; i < m.row(); ++i)
		{
			for (size_t j = 0; j < m.col(); ++j)
			{
				tr &= m(i,j)==(*this)(i,j);
				if(!tr){return tr;}
			}
		}
		return tr;
	}

	bool operator!(void){
		for (size_t i = 0; i < mRows*mCols; ++i)
		{
			if(mData[i]){return false;}
		}
		return true;
	}

	// determinant [ Currently only for 2 x 2 matrices ]
	double determinant(void){
		if(mRows!=mCols || mRows!=2){
			// 403 forbidden [ you can't do that if not a sqaure matrix and sadly not if
			// the matrix is not 2 x 2 ]
			this->set_error(403);
			return 0;
		}
		double d;
		d = (*this)(0,0)*(*this)(1,1);
		d -= (*this)(1,0)*(*this)(0,1);
		return d;
	}

	// adjacent matrix [ Currently only for 2 x 2 matrices ]
	Matrix adjacent(void){
		if(mRows!=mCols || mRows!=2){
			// 403 forbidden [ you can't do that if not a sqaure matrix and sadly not if
			// the matrix is not 2 x 2 ]
			Matrix a = Matrix(0,0);
			a.set_error(403);
			return a;
		}
		Matrix a = Matrix(this->mRows,this->mCols);
		a.set(0,0,(*this)(1,1));
		a.set(1,1,(*this)(0,0));
		a.set(0,1,-(*this)(0,1));
		a.set(1,0,-(*this)(1,0));
		return a;
	}

	// inverse matrix [ Currently only for 2 x 2 matrices ]
	Matrix inverse(void){
		if(mRows!=mCols || mRows!=2){
			// 403 forbidden [ you can't do that if not a sqaure matrix and sadly not if
			// the matrix is not 2 x 2 ]
			Matrix a = Matrix(0,0);
			a.set_error(403);
			return a;
		}
		Matrix adj = this->adjacent();
		double d = this->determinant();
		if(d==0){
			// 403 forbidden can't do that!
			Matrix a = Matrix(0,0);
			a.set_error(403);
			return a;
		}
		Matrix res = adj/d;
		return res;
	}

	// transpose matrix [ Any x Any ]
	Matrix transpose(void){
		Matrix a = Matrix(mCols,mRows);
		for (size_t i = 0; i < mCols; ++i)
		{
			for (size_t j = 0; j < mRows; ++j)
			{
				a.set(i,j,(*this)(j,i));
			}
		}
		return a;
	}

	// addition with a matrix
	Matrix operator+(Matrix m){
		Matrix a = Matrix(m.row(),m.col());
		if(this->mRows!=m.row()||this->mCols!=m.col()){
			// 403 forbidden
			Matrix a = Matrix(0,0);
			a.set_error(403);
			return a;
		}
		for (size_t i = 0; i < mRows; ++i)
		{
			for (size_t j = 0; j < mCols; ++j)
			{
				a.set(i,j,(*this)(i,j)+m(i,j));
			}
		}
		return a;
	}
	// addition with integer
	Matrix operator+(int val){
		Matrix a = Matrix(this->row(),this->col());
		for (size_t i = 0; i < mRows; ++i)
		{
			for (size_t j = 0; j < mCols; ++j)
			{
				a.set(i,j,(*this)(i,j)+val);
			}
		}
		return a;
	}
	// addition with double
	Matrix operator+(double val){
		Matrix a = Matrix(this->row(),this->col());
		for (size_t i = 0; i < mRows; ++i)
		{
			for (size_t j = 0; j < mCols; ++j)
			{
				a.set(i,j,(*this)(i,j)+val);
			}
		}
		return a;
	}

	// subtraction with matrix
	Matrix operator-(Matrix m){
		Matrix a = Matrix(m.row(),m.col());
		if(this->mRows!=m.row()||this->mCols!=m.col()){
			// 403 forbidden
			a.set_error(403);
			return a;
		}
		for (size_t i = 0; i < mRows; ++i)
		{
			for (size_t j = 0; j < mCols; ++j)
			{
				a.set(i,j,(*this)(i,j)-m(i,j));
			}
		}
		return a;
	}

	// subtraction with integer
	Matrix operator-(int val){
		Matrix a = Matrix(this->row(),this->col());
		for (size_t i = 0; i < mRows; ++i)
		{
			for (size_t j = 0; j < mCols; ++j)
			{
				a.set(i,j,(*this)(i,j)-val);
			}
		}
		return a;
	}

	// subtraction with double
	Matrix operator-(double val){
		Matrix a = Matrix(this->row(),this->col());
		for (size_t i = 0; i < mRows; ++i)
		{
			for (size_t j = 0; j < mCols; ++j)
			{
				a.set(i,j,(*this)(i,j)-val);
			}
		}
		return a;
	}

	// Multiplication with integer
	Matrix operator*(int val){
		Matrix a = Matrix(this->row(),this->col());
		for (size_t i = 0; i < mRows; ++i)
		{
			for (size_t j = 0; j < mCols; ++j)
			{
				a.set(i,j,(*this)(i,j)*val);
			}
		}
		return a;
	}

	// Multiplication with double
	Matrix operator*(double val){
		Matrix a = Matrix(this->row(),this->col());
		for (size_t i = 0; i < mRows; ++i)
		{
			for (size_t j = 0; j < mCols; ++j)
			{
				a.set(i,j,(*this)(i,j)*val);
			}
		}
		return a;
	}

	// Multiplication with matrix [ I had hard time during this \(o_O)/ ]
	Matrix operator*(Matrix m){
		Matrix a = Matrix(m.row(),m.col());
		if(mCols!=m.row()){
			// 403 forbidden
			a.set_error(403);
			return a;
		}
		double tmp;
		// actual Multiplication!
		for (size_t i = 0; i < mRows; ++i)
		{
			for (size_t j = 0; j < m.col(); ++j)
			{
				tmp = 0;
				for (size_t k = 0; k < mCols; ++k)
				{
					tmp += (*this)(i,k)*m(k,j);
				}
				a.set(i,j,tmp);
			}
		}
		return a;
	}

	// Division with integer
	Matrix operator/(int val){
		Matrix a = Matrix(this->row(),this->col());
		for (size_t i = 0; i < mRows; ++i)
		{
			for (size_t j = 0; j < mCols; ++j)
			{
				a.set(i,j,(*this)(i,j)/val);
			}
		}
		return a;
	}

	// Division with double
	Matrix operator/(double val){
		Matrix a = Matrix(this->row(),this->col());
		for (size_t i = 0; i < mRows; ++i)
		{
			for (size_t j = 0; j < mCols; ++j)
			{
				a.set(i,j,(*this)(i,j)/val);
			}
		}
		return a;
	}

	// division with matrix [ Currently only for 2x2 matrices! ]
	Matrix operator/(Matrix m){
		if(this->mRows!=m.row()||this->mCols!=m.col()||this->mRows!=2){
			Matrix a = Matrix(0,0);
			// 403 forbidden!
			a.set_error(403);
			return a;
		}
		Matrix a = m.inverse()*(*this);
		return a;
	}

	// += derivation from parent '+'
	void operator+=(int val){
		for (size_t i = 0; i < mRows; ++i){
			for (size_t j = 0; j < mCols; ++j){
				this->set(i,j,(*this)(i,j)+val);
			}
		}
	}
	void operator+=(double val){
		for (size_t i = 0; i < mRows; ++i){
			for (size_t j = 0; j < mCols; ++j){
				this->set(i,j,(*this)(i,j)+val);
			}
		}
	}
	void operator+=(Matrix m){
		Matrix th = (*this)+m;
		for (size_t i = 0; i < th.mRows; ++i){
			for (size_t j = 0; j < th.mCols; ++j){
				th.set(i,j,th(i,j));
			}
		}
	}

	// -= derivation from parent '-'
	void operator-=(int val){
		for (size_t i = 0; i < mRows; ++i){
			for (size_t j = 0; j < mCols; ++j){
				this->set(i,j,(*this)(i,j)-val);
			}
		}
	}
	void operator-=(double val){
		for (size_t i = 0; i < mRows; ++i){
			for (size_t j = 0; j < mCols; ++j){
				this->set(i,j,(*this)(i,j)-val);
			}
		}
	}
	void operator-=(Matrix m){
		Matrix th = (*this)+m;
		for (size_t i = 0; i < th.mRows; ++i){
			for (size_t j = 0; j < th.mCols; ++j){
				th.set(i,j,th(i,j));
			}
		}
	}

	// *= derivation from parent '*'
	void operator*=(int val){
		for (size_t i = 0; i < mRows; ++i){
			for (size_t j = 0; j < mCols; ++j){
				this->set(i,j,(*this)(i,j)*val);
			}
		}
	}
	void operator*=(double val){
		for (size_t i = 0; i < mRows; ++i){
			for (size_t j = 0; j < mCols; ++j){
				this->set(i,j,(*this)(i,j)*val);
			}
		}
	}
	void operator*=(Matrix m){
		Matrix th = (*this)*m;
		for (size_t i = 0; i < th.mRows; ++i){
			for (size_t j = 0; j < th.mCols; ++j){
				th.set(i,j,th(i,j));
			}
		}
	}

	// /= derivation from parent '/'
	void operator/=(int val){
		for (size_t i = 0; i < mRows; ++i){
			for (size_t j = 0; j < mCols; ++j){
				this->set(i,j,(*this)(i,j)/val);
			}
		}
	}
	void operator/=(double val){
		for (size_t i = 0; i < mRows; ++i){
			for (size_t j = 0; j < mCols; ++j){
				this->set(i,j,(*this)(i,j)/val);
			}
		}
	}

private:
	size_t mRows;
	size_t mCols;
	std::vector<double> mData;
	unsigned int error;
};
