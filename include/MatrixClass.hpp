#include <map>
#include <array>
#include <iostream>
#include <vector>
#include <complex>
#include <fstream>
#include <string>
#include <sstream>

namespace algebra{
 /*
 @brief enumerator that indicates the storage ordering
 @param row_wise indicates the storage ordering by rows
 @param column_wise indicates the sorage ordering by columns
 
 */
enum StorageOrder{
    row_wise,
    column_wise
};

    /*
    @brief dynamic matrix class template
    */
    template<typename T, StorageOrder S>
    class MatrixClass{

       private:

       std::size_t _rows,_cols,_nzero=0;

       std::map<std::array<std::size_t,2>,T> _data;

       bool compressed;

       std::vector<T> values(_rows+1);
       std::vector<std::size_t> inner_indexes(_nzero);
       std::vector<std_size_t> outer_indexes(_nzero); 

       /*
       @brief check if the indexes provided are inside the dimension of the matrix or not
       @param row index
       @param column index
       @return true if the indexes provided are inside the matrix dimension, false otherwise
       */
       bool in_bound(const std::size_t r,const std::size_t c) const;
       
       /*
       @brief compute the number of non-zero elements
       */
       void compute_nzero();

       public:

       friend bool operator<(const std::array<std::size_t,2> & index1, const std::array<std::size_t,2> & index2);

       friend std::vector<T> operator*(const MatrixClass<T,S> & A,const std::vector<T> & v);

       friend std::vector<T> operator*(const std::vector<T> & v,const MatrixClass<T,S> & A);

       friend MatrixClass<T,S> read_matrix(const std::string & filename);
       
       /*
       @brief constructor for the Matrix class
       @param number of total columns
       @param number of total rows
       @param boolean variable to know if the matrix has been already compressed or not (set to false if not provided)
       */
       MatrixClass(std::size_t rows=0, std::size_t cols = 0) : _rows(rows), _cols(cols), compressed(false) {
        compute_nzero();
        resize_matrix(rows,cols);}

        /*
        @brief non const call operator
        @param row index
        @param column index
        @param value to add in the matrix in (i,j) position
        */
       T & operator()(const std::size_t i,const std::size_t j);

        /*
        @brief const call operator
        @param row index
        @param column index
        @param value to add in the matrix in (i,j) position
        */
       T operator()(const std::size_t i,const std::size_t j) const;
    
        /*
        @brief perform the compression, according the storage order defined at compile time
        */
       void compress();
    
        /*
        @brief perform the decompression, according the storage order defined at compile time
        */
       void uncompress();

        /*
        @brief allows to know if the matrix is compressed or not
        @return compressed private member of the class
        */
       bool is_compressed() const;
        
        /*
        @brief resize a given matrix, given the numbers of column and rows
        */
       void resize_matrix(std::size_ t rows, std::size_t cols);

    };



        /*
        @brief operator< overloading for column-major ordering in the case of column wise order
        @param first array of indexes to compare
        @param second array of indexes to compare with the first
        @return true if the index1 position is before index2 position in the Matrix, according to the storage order decided, zero otherwise
        */
       bool operator<(const std::array<std::size_t,2> & index1, const std::array<std::size_t,2> & index2);
        
        /*
        @brief operator* overloading for performing the multiplication between a matrix A and a vector v, A*v
        @param vector that will be multiplicated with the matrix
        @return result of the multiplication
        */
       template<typename T, StorageOrder S>
       std::vector<T> operator*(const MatrixClass<T,S> & A,const std::vector<T> & v);

       /*
        @brief operator* overloading for performing the multiplication between a vector v and a matrix A, v*A
        @param vector that will be multiplicated with the matrix
        @return result of the multiplication
        */
       template<typename T, StorageOrder S>
       std::vector<T> operator*(const std::vector<T> & v,const MatrixClass<T,S> & A);
    
    /*
    @brief given the name of the mtx file, the function write its content in a MatrixClass object
    @param name of the mtx file
    @return MatrixClass object
    */
       template<typename T, StorageOrder S>
       MatrixClass<T,S> read_matrix(const std::string & filename);
};