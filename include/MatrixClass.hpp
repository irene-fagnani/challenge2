#include <map>
#include <iostream>
#include <complex>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>
#include "MatrixClass_utilities.hpp"


#ifndef MATRIXCLASS_HPP
#define MATRIXCLASS_HPP

namespace algebra{
    /*!
     * @brief dynamic matrix class template
     * @tparam T type of the elements in the matrix
     * @tparam S storage order of the matrix
    */
    template<typename T, StorageOrder S>
    class MatrixClass{

    private:

        std::size_t _rows;  /*!< Number of rows. */
        std::size_t _cols;  /*!< Number of columns. */
        std::size_t _nnz=0;  /*!< Number of non zero elements in the matrix. */

        std::map<std::array<std::size_t,2>,T> _data;  /*!< Uncompressed form. Map that store as values non zero elements, as key the vector that stores the correspondents indexes in the matrix */

        bool compressed;  /*!< True if the is stored in compressed form, false otherwise */

        std::vector<T> values;  /*!< Compressed form. Vector that stores the non zero elements.*/
        std::vector<std::size_t> inner_indexes; /*!< Compressed form. Vector that stores inner indexes. */
        std::vector<std::size_t> outer_indexes; /*!< Compressed form. Vector that stores outer indexes. */


    public:

        friend bool operator<(const std::array<std::size_t,2> & index1, const std::array<std::size_t,2> & index2);

        /*!
         * @brief constructor for the Matrix class
         * @param number of total columns
         * @param number of total rows
         * @param boolean variable to know if the matrix has been already compressed or not (set to false if not provided)
        */
        MatrixClass(std::size_t rows=0, std::size_t cols = 0, T value=0) : _rows(rows), _cols(cols), compressed(false) {
            compute_nzero();
            resize_matrix(rows,cols,value);}
        /*!
         * @brief check if the indexes provided are inside the dimension of the matrix or not
         * @param row index
         * @param column index
         * @return true if the indexes provided are inside the matrix dimension, false otherwise
        */
        bool in_bound(const std::size_t r,const std::size_t c) const{
            return r < _rows && c < _cols;
        }

        /*!
         * @brief compute the number of non-zero elements
        */
        void compute_nzero(){
            for (auto it = _data.begin(); it != _data.end(); it++) {
                if (it->second != T{}) {
                    _nnz++;
                }
            }
        }

        /*!
         * @brief return _rows
         * @return number of rows
        */
        std::size_t get_rows() const{
            return _rows;
        }

        /*!
         * @brief return _cols
         * @return number of columns
        */
        std::size_t get_cols() const{
            return _cols;
        }

        /*!
         * @brief non const call operator
         * @param row index
         * @param column index
         * @param value to add in the matrix in (i,j) position
        */
        T & operator()(const std::size_t i,const std::size_t j);

        /*!
         * @brief const call operator
         * @param row index
         * @param column index
         * @param value to add in the matrix in (i,j) position
        */
        T operator()(const std::size_t i,const std::size_t j) const;

        /*!
         * @brief perform the compression, according the storage order defined at compile time
        */
        void compress();

        /*!
         * @brief perform the decompression, according the storage order defined at compile time
        */
        void uncompress();

        /*!
         * @brief allows to know if the matrix is compressed or not
         * @return compressed private member of the class
        */
        bool is_compressed() const{
            return compressed;
        }

        /*!
         * @brief set the compressed private member of the class equal to the value parameter
         * @param value new value for the compressed private and bool variable
         */
        void set_compressed(bool value){
            compressed=value;
        }

        /*!
         * @brief resize a given matrix, given the numbers of column and rows (only if the matrix is uncompress)
        */
        void resize_matrix(std::size_t rows, std::size_t cols, T val);

        /*!
         * @brief given the name of the mtx file, the function write its content in a MatrixClass object
         * @param name of the mtx file
         * @return MatrixClass object
        */
        void read_matrix(const std::string & filename);

        /*!
         * @brief operator* overloading for performing the multiplication between a matrix A and a vector v, A*v
         * @param vector that will be multiplicated with the matrix
         * @return result of the multiplication
        */

        template<typename Tv>
        std::vector<decltype(T{} * Tv{})> operator*(const std::vector<Tv> & v);

        /*!
         * @brief operator* overloading for performing the multiplication between two objects of type MatrixClass (general case)
         * @param matrix that will be multiplicted with this
         * @return result of the multiplication (it is of type MatrixClass and of StorageOrder::row_wise)
         */
        template<typename T2,StorageOrder S2>
        MatrixClass<decltype(T{} * T2{}),StorageOrder::row_wise> operator*(MatrixClass<T2,S2> const & lhs);

        /*!
         * @brief compute the norm of the matrix, according to the specified norm method
         * @tparam N NormMethod specified the wanted norm method computation
         * @return double result of the norm computation
         */

        template<NormMethod N>
        decltype(auto) compute_norm()const;
        /*!
        * @brief this function print an object of type MatrixClass
        */

        void print_matrix()const;
    };

};

#endif