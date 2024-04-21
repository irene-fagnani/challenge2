#include <map>
#include <array>
#include <iostream>
#include <vector>


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

       std::size_t _rows,_cols;

       std::map<std::array<std::size_t,2>,T> _data;

       bool compressed;

       std::vector<T> values;
       std::vector<T> inner_indexes;
       std::vector<T> outer_indexes; 

       /*
       @brief check if the indexes provided are inside the dimension of the matrix or not
       @param row index
       @param column index
       @return true if the indexes provided are inside the matrix dimension, false otherwise
       */
       bool in_bound(const std::size_t r,const std::size_t c) const;

       public:
       friend bool operator<(const std::array<std::size_t,2> & index1, const std::array<std::size_t,2> & index2);
       
       /*
       @brief constructor for the Matrix class
       @param number of total columns
       @param number of total rows
       @param boolean variable to know if the matrix has been already compressed or not (set to false if not provided)
       */
       MatrixClass(std::size_t rows = 0, std::size_t cols = 0) : _rows(rows), _cols(cols), compressed(false) {}
        /*
        @brief non const call operator
        @param row index
        @param column index
        @param value to add in the matrix in (i,j) position
        */
       T & operator()(const std::size_t i,const std::size_t j);
    
        /*
        @brief perform the compression, according the storage order defined at compile time
        */
       void compress();

        
        
    
    };


};

/*
        @brief operator< overloading for column-major ordering in the case of column wise order
        @param first array of indexes to compare
        @param second array of indexes to compare with the first
        @return true if the index1 position is before index2 position in the Matrix, according to the storage order decided, zero otherwise
        */
       bool operator<(const std::array<std::size_t,2> & index1, const std::array<std::size_t,2> & index2);