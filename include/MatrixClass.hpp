#include <map>
#include <array>
#include <iostream>
#include <vector>
#include <complex>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
namespace algebra{
    /**
     * @brief enumerator that indicates the storage ordering
     * @param row_wise indicates the storage ordering by rows
     * @param column_wise indicates the sorage ordering by columns
    */
    enum StorageOrder{
        row_wise,
        column_wise
    };
    

    /**
     * @brief enumerator that indicates the norm method
     * @param one_norm indicates the one norm
     * @param infinity_norm indicates the infinity norm
     * @param Frobenius_norm indicates the Frobenius norm
     */
    enum NormMethod{
        one_norm,
        infinity_norm,
        Frobenius_norm
    };

    /**
     * @brief dynamic matrix class template
     * @tparam T type of the elements in the matrix
     * @tparam S storage order of the matrix
    */
    template<typename T, StorageOrder S>
    class MatrixClass{

    private:

        std::size_t _rows;  /**< Number of rows. */
        std::size_t _cols;  /**< Number of columns. */
        std::size_t _nnz=0;  /**< Number of non zero elements in the matrix. */

        std::map<std::array<std::size_t,2>,T> _data;  /**< Uncompressed form. Map that store as values non zero elements, as key the vector that stores the correspondents indexes in the matrix */

        bool compressed;  /**< True if the is stored in compressed form, false otherwise */

        std::vector<T> values;  /**< Compressed form. Vector that stores the non zero elements.*/
        std::vector<std::size_t> inner_indexes; /**< Compressed form. Vector that stores inner indexes. */
        std::vector<std::size_t> outer_indexes; /**< Compressed form. Vector that stores outer indexes. */

        /**
         * @brief check if the indexes provided are inside the dimension of the matrix or not
         * @param row index
         * @param column index
         * @return true if the indexes provided are inside the matrix dimension, false otherwise
        */
        bool in_bound(const std::size_t r,const std::size_t c) const{
            return r < _rows && c < _cols;
        }

        /**
         * @brief compute the number of non-zero elements
        */
        void compute_nzero(){
            for (auto it = _data.begin(); it != _data.end(); it++) {
            if (it->second != T{}) {
                _nnz++;
            }
        }
        }

    public:

        friend bool operator<(const std::array<std::size_t,2> & index1, const std::array<std::size_t,2> & index2);

        /**
         * @brief constructor for the Matrix class
         * @param number of total columns
         * @param number of total rows
         * @param boolean variable to know if the matrix has been already compressed or not (set to false if not provided)
        */
        MatrixClass(std::size_t rows=0, std::size_t cols = 0) : _rows(rows), _cols(cols), compressed(false) {
            compute_nzero();
            resize_matrix(rows,cols);}

        /**
         * @brief return _rows
         * @return number of rows
        */
        std::size_t get_rows() const{
            return _rows;
        }

        /**
         * @brief return _cols
         * @return number of columns
        */
        std::size_t get_cols() const{
            return _cols;
        }

        /**
         * @brief non const call operator
         * @param row index
         * @param column index
         * @param value to add in the matrix in (i,j) position
        */
        T & operator()(const std::size_t i,const std::size_t j){
            if (!in_bound(i, j)) {
            std::cerr << "Index out of bounds." << std::endl;
        }
        if (!is_compressed()) {
            if constexpr (S == StorageOrder::row_wise) {
                return _data[{i, j}];
            }

            if constexpr (S == StorageOrder::column_wise) {
                return _data[{j, i}];
            }

        } else {
            if constexpr (S == StorageOrder::row_wise) {
                for (std::size_t ii = inner_indexes[i]; ii < inner_indexes[i + 1]; ++ii) {
                    if (outer_indexes[ii] == j) {
                        return values[ii];
                    }
                }
            } else if constexpr (S == StorageOrder::column_wise) {
                for (std::size_t jj = outer_indexes[j]; jj < outer_indexes[j + 1]; ++jj) {
                    if (inner_indexes[jj] == i) {
                        return values[jj];
                    }

                }
            }
        }
        std::cerr<<"In the case of compressed matrix can only change the value of existing non-zero elements.\n"<<std::endl;


        }

        /**
         * @brief const call operator
         * @param row index
         * @param column index
         * @param value to add in the matrix in (i,j) position
        */
        T operator()(const std::size_t i,const std::size_t j) const{
             if (!in_bound(i, j)) {
            std::cerr << "Index out of bounds." << std::endl;
        }
        if (!is_compressed()) {
            if constexpr (S == StorageOrder::row_wise) {
                if (_data.find({i, j}) == _data.end()) {
                    return 0;
                } else {
                    return _data.at({i, j});
                }


            }
            if constexpr (S == StorageOrder::column_wise) {
                if (_data.find({j, i}) == _data.end()) {
                    return 0;
                } else {
                    return _data.at({j, i});
                }

            }
        } else {
            if constexpr (S == StorageOrder::row_wise) {
                for (std::size_t ii = inner_indexes[i]; ii < inner_indexes[i + 1]; ++ii) {
                    if (outer_indexes[ii] == j) {
                        return values[ii];
                    }

                }
                return 0;
            } else if constexpr (S == StorageOrder::column_wise) {
                for (std::size_t jj = outer_indexes[j]; jj < outer_indexes[j + 1]; ++jj) {
                    if (inner_indexes[jj] == i) {
                        return values[jj];
                    }

                }
                return 0;
            }
        }

        }

        /**
         * @brief perform the compression, according the storage order defined at compile time
        */
        void compress();

        /**
         * @brief perform the decompression, according the storage order defined at compile time
        */
        void uncompress();

        /**
         * @brief allows to know if the matrix is compressed or not
         * @return compressed private member of the class
        */
        bool is_compressed() const;

        void set_compressed(bool value){
            compressed=value;
        }

        /**
         * @brief resize a given matrix, given the numbers of column and rows
        */
        void resize_matrix(std::size_t rows, std::size_t cols);

        /**
         * @brief given the name of the mtx file, the function write its content in a MatrixClass object
         * @param name of the mtx file
         * @return MatrixClass object
        */
        void read_matrix(const std::string & filename);

        /**
         * @brief operator* overloading for performing the multiplication between a matrix A and a vector v, A*v
         * @param vector that will be multiplicated with the matrix
         * @return result of the multiplication
        */

        std::vector<T> operator*(const std::vector<T> & v);

        /**
         * @brief operator* overloading for performing the multiplication between two matrices (general case)
         * @param matrix that will be multiplicted with this
         * @return result of the multiplication (it is of type MatrixClass)
         */
        template<StorageOrder S1>
        MatrixClass<T,StorageOrder::row_wise> operator*(MatrixClass<T,S1> const & lhs);
        

        /**
         * @brief print matrix
        */
        void print_matrix()const;
        
        /**
         * @brief compute the norm of the matrix, according to the specified norm method
         * @tparam N NormMethod specified the wanted norm method computation
         * @return double result of the norm computation
         */
        template<NormMethod N>
        decltype(auto) compute_norm()const;

    };



    /**
     * @brief operator< overloading for column-major ordering in the case of column wise order
     * @param first array of indexes to compare
     * @param second array of indexes to compare with the first
     * @return true if the index1 position is before index2 position in the Matrix, according to the storage order decided, zero otherwise
    */
    bool operator<(const std::array<std::size_t,2> & index1, const std::array<std::size_t,2> & index2);





};