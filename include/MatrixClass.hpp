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

    /**
     * @brief operator< overloading for column-major ordering in the case of column wise order
     * @param first array of indexes to compare
     * @param second array of indexes to compare with the first
     * @return true if the index1 position is before index2 position in the Matrix, according to the storage order decided, zero otherwise
    */
    bool operator<(const std::array<std::size_t,2> & index1, const std::array<std::size_t,2> & index2){
         if constexpr (S == StorageOrder::column_wise) {

            return (index1[0] < index2[0] || index1[0] == index2[0] && index1[1] < index2[1]);

        }
        return (index1[1] < index2[1] || index1[1] == index2[1] && index1[0] < index2[0]);

    }


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
        void compress(){
             if (is_compressed()) {
            std::cout
                    << "The matrix has already been compressed. If you want to compress it again, please decompress it first."
                    << std::endl;
            return;
        } else {
            (S == StorageOrder::row_wise) ? inner_indexes.resize(_rows + 1, 0) : outer_indexes.resize(_cols+1, 0);
            compute_nzero();
            T value;
            size_t i, j;

            if constexpr (S == StorageOrder::row_wise) {
                for (auto it = _data.begin(); it != _data.end(); ++it) {
                    value = it->second;
                    i = it->first[0];
                    j = it->first[1];

                    if (value != 0) {
                        values.push_back(value);
                        outer_indexes.push_back(j);
                        inner_indexes[i + 1] =inner_indexes[i + 1] + ((inner_indexes[i + 1] == 0) ? inner_indexes[i] : 0);
                        inner_indexes[i + 1]++;
                    }
                }
            } else if constexpr (S == StorageOrder::column_wise) {
                for (auto it = _data.begin(); it != _data.end(); ++it) {
                    value = it->second;
                    j = it->first[0];
                    i = it->first[1];

                    if (value != 0) {
                        values.push_back(value);
                        inner_indexes.push_back(i);
                        outer_indexes[j + 1] =outer_indexes[j + 1] + ((outer_indexes[j + 1] == 0) ? outer_indexes[j] : 0);
                        outer_indexes[j + 1]++;
                    }
                }

            }

            _data.clear();
            set_compressed(true);
        }
        }

        /**
         * @brief perform the decompression, according the storage order defined at compile time
        */
        void uncompress(){
             compute_nzero();
        if (!is_compressed()) {
            std::cout << "The matrix has not been compressed yet." << std::endl;
            return;
        } else {
            if constexpr (S == StorageOrder::row_wise) {
                for (std::size_t i = 0; i < _rows; ++i) {
                    for (std::size_t j = inner_indexes[i]; j < inner_indexes[i + 1]; j++) {
                        _data[{i, outer_indexes[j]}] = values[j];
                    }
                }

            } else if constexpr (S == StorageOrder::column_wise) {
                for (std::size_t i = 0; i < _cols; ++i) {
                    for (std::size_t j = outer_indexes[i]; j < outer_indexes[i + 1]; j++) {
                        _data[{i,inner_indexes[j]}] = values[j];
                    }
                }
            }
        }
        set_compressed(false);
        inner_indexes.clear();
        outer_indexes.clear();
        values.clear();
        }

        /**
         * @brief allows to know if the matrix is compressed or not
         * @return compressed private member of the class
        */
        bool is_compressed() const{
            return compressed;
        }
        
        /**
         * @brief set the compressed private member of the class equal to the value parameter
         * @param value new value for the compressed private and bool variable
         */
        void set_compressed(bool value){
            compressed=value;
        }

        /**
         * @brief resize a given matrix, given the numbers of column and rows
        */
        void resize_matrix(std::size_t rows, std::size_t cols){
            if (!is_compressed()) {
            for (std::size_t i = 0; i < rows; ++i) {
                for (std::size_t j = 0; j < cols; ++j) {
                    _data[{i, j}] = 0;
                }
            }
            }
        }

        /**
         * @brief given the name of the mtx file, the function write its content in a MatrixClass object
         * @param name of the mtx file
         * @return MatrixClass object
        */
        void read_matrix(const std::string & filename){
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Cannot open the file." << std::endl;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line[0] == '%') {
                continue;
            }
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;

            while (ss >> token) {
                tokens.push_back(token);
            }

            if (tokens.size() != 3) {
                std::cerr << "The matrix file is not in the correct format." << std::endl;
            }

            if (tokens[0] == "%%MatrixMarket") {
                if (tokens[1] != "matrix" || tokens[2] != "real" || tokens[2] != "integer") {
                    std::cerr << "Only general, real, matrices are supported. " << std::endl;
                }
            } else {
                _rows = std::stoi(tokens[0]);
                _cols = std::stoi(tokens[1]);
                _nnz = std::stoi(tokens[2]);

                for (int i = 0; i < _nnz; ++i) {
                    std::getline(file, line);
                    std::stringstream ss2(line);
                    int row, col;
                    T value;
                    ss2 >> row >> col >> value;
                    _data[{row - 1, col - 1}] = value;
                }
            }
        }
        file.close();
    }

        /**
         * @brief operator* overloading for performing the multiplication between a matrix A and a vector v, A*v
         * @param vector that will be multiplicated with the matrix
         * @return result of the multiplication
        */

        std::vector<T> operator*(const std::vector<T> & v){

        if (v.size() != _cols) {
            std::cerr << "The vector size does not match the matrix column size." << std::endl;
        }

        std::vector<T> result(_rows,0);
        if constexpr (S == StorageOrder::row_wise) {
            if (!compressed) {
                for (std::size_t i = 0; i < _rows; ++i) {
                    for (auto it = _data.lower_bound({i, 0}); it != _data.upper_bound({i, _cols - 1}); ++it) {
                        result[i] += (it->second) * v[it->first[1]];
                    }
                }
            } else {
                for (std::size_t i = 0; i < _rows; ++i) {
                    for (std::size_t j = inner_indexes[i]; j < inner_indexes[i + 1]; ++j) {
                        result[i] += values[j] * v[outer_indexes[j]];
                    }
                }
            }
        } else if (S == StorageOrder::column_wise) {

            if (!compressed){
                    for (std::size_t j = 0; j < _cols; ++j) {
                        for(auto it=_data.lower_bound({j,0});it!=_data.upper_bound({j,_rows-1});++it){
                            result[it->first[1]]+=(it->second)*v[j];
                    }
                }
            } else {
                for (std::size_t j = 0; j < _cols; ++j) {
                    for (std::size_t i = outer_indexes[j]; i < outer_indexes[j + 1]; ++i) {
                        result[inner_indexes[i]] += values[i] * v[j];
                    }
                }
            }
        }

        return result;

    }


        /**
         * @brief operator* overloading for performing the multiplication between two matrices (general case)
         * @param matrix that will be multiplicted with this
         * @return result of the multiplication (it is of type MatrixClass)
         */
        template<StorageOrder S1>
        MatrixClass<T,StorageOrder::row_wise> operator*(MatrixClass<T,S1> const & lhs){
    
    MatrixClass<T,StorageOrder::row_wise> result(_rows,lhs.get_cols());
    for(std::size_t i=0;i<get_rows();++i){
        for(std::size_t j=0;j<lhs.get_cols();++j){
            for(std::size_t k=0;k<lhs.get_rows();++k){
                result(i,j)+=((*this)(i,k))*(lhs(k,j));
            }
        }
    }
    return result;
    }

        

        /**
         * @brief this function print an object of type MatrixClass
        */
        void print_matrix()const{
        for (std::size_t i = 0; i < _rows; ++i) {
            for (std::size_t j = 0; j < _cols; ++j) {
                std::cout << (*this)(i, j) << " ";
            }
            std::cout << std::endl;
        }
        }
        /**
         * @brief compute the norm of the matrix, according to the specified norm method
         * @tparam N NormMethod specified the wanted norm method computation
         * @return double result of the norm computation
         */
        template<NormMethod N>
        decltype(auto) compute_norm()const{

        decltype(auto) norm=0.0;

        if constexpr(N==NormMethod::infinity_norm){
            decltype(auto) sum=0.0;
                for(std::size_t i=0;i<_rows;++i){
                    sum=0;
                    for(std::size_t j=0;j<_cols;++j) {
                        sum += std::abs((*this)(i, j));
                    }
                    norm=std::max(norm,sum);

            }
            return norm;
        }

        if constexpr(N==NormMethod::one_norm){
            decltype(auto) sum=0.0;
                for(std::size_t j=0;j<_cols;++j){
                    sum=0;
                    for(std::size_t i=0;i<_rows;++i){
                        sum+=std::abs((*this)(i,j));
                    }
                    norm=std::max(norm,sum);
                }

            return norm;
        }

        if constexpr(N==NormMethod::Frobenius_norm){
            if(!compressed){
                for(const auto & elem: _data){
                    norm += std::abs(elem.second)*std::abs(elem.second);
                }
            }else{
                for(std::size_t i=0;i<values.size();++i){
                    norm += std::abs(values[i])*std::abs(values[i]);
                }
            }
            return std::sqrt(norm);
        }

    }

    };




};