#include "MatrixClass.hpp"

namespace algebra {


    template<typename T, StorageOrder S>
    bool MatrixClass<T, S>::is_compressed() const {
        return compressed;
    }

    template<typename T, StorageOrder S>
    void MatrixClass<T, S>::resize_matrix(std::size_t rows, std::size_t cols) {
        if (!is_compressed()) {
            for (std::size_t i = 0; i < rows; ++i) {
                for (std::size_t j = 0; j < cols; ++j) {
                    _data[{i, j}] = 0;
                }
            }
        }
    }

    template<typename T, StorageOrder S>
    bool operator<(const std::array<std::size_t, 2> &index1, const std::array<std::size_t, 2> &index2) {

        if constexpr (S == StorageOrder::column_wise) {

            return (index1[0] < index2[0] || index1[0] == index2[0] && index1[1] < index2[1]);

        }
        return (index1[1] < index2[1] || index1[1] == index2[1] && index1[0] < index2[0]);


    }

    template<typename T, StorageOrder S>
    std::vector<T> MatrixClass<T, S>::operator*(const std::vector<T> &v) {

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


    template<typename T, StorageOrder S>
    void MatrixClass<T, S>::read_matrix(const std::string &filename) {
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


    template<typename T,StorageOrder S>
    template<StorageOrder S1>
    MatrixClass<T,StorageOrder::row_wise> MatrixClass<T,S>::operator*(MatrixClass<T,S1> const & lhs){
    
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

    template<typename T, StorageOrder S>
    void MatrixClass<T, S>::print_matrix()const{
        for (std::size_t i = 0; i < _rows; ++i) {
            for (std::size_t j = 0; j < _cols; ++j) {
                std::cout << (*this)(i, j) << " ";
            }
            std::cout << std::endl;
        }

    }

    
   template<typename T, StorageOrder S>
    template<NormMethod N>
    decltype(auto) MatrixClass<T,S>::compute_norm()const{

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

