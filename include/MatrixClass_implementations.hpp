#include "MatrixClass.hpp"

#ifndef MATRIXCLASS_IMPLEMENTATIONS_HPP
#define MATRIXCLASS_IMPLEMENTATIONS_HPP
namespace algebra{

    template<typename T, StorageOrder S>
     T & MatrixClass<T,S>::operator()(const std::size_t i,const std::size_t j){
            if (!in_bound(i, j)) {
            std::cerr << "Index out of bounds." << std::endl;
            exit(1);
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
            exit(1);
            }

    template<typename T, StorageOrder S>
    T MatrixClass<T,S>::operator()(const std::size_t i, const std::size_t j)const{
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

    template<typename T, StorageOrder S>
    void MatrixClass<T,S>::compress(){
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

                    if (value != T{}) {
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

                    if (value != T{}) {
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

    template<typename T, StorageOrder S>
    void MatrixClass<T,S>::uncompress(){
         compute_nzero();
        if (!is_compressed()) {
            std::cout << "The matrix has already been uncompressed. If you want to uncompress it again, please compress it first." << std::endl;
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

    template<typename T, StorageOrder S>
    void MatrixClass<T,S>::resize_matrix(std::size_t rows, std::size_t cols){
        if (!is_compressed()) {
            for (std::size_t i = 0; i < rows; ++i) {
                for (std::size_t j = 0; j < cols; ++j) {
                    if constexpr(S==StorageOrder::row_wise){
                    _data[{i, j}] = 0;
                    }else if constexpr(S==StorageOrder::column_wise){
                        _data[{j,i}]=0;
                    }
               }
            }
        }
    }

    template<typename T, StorageOrder S>
    void MatrixClass<T,S>::read_matrix(const std::string & filename){
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
                if constexpr(S==StorageOrder::row_wise){
                _rows = std::stoi(tokens[0]);
                _cols = std::stoi(tokens[1]);
                }else if constexpr(S==StorageOrder::column_wise){
                _cols = std::stoi(tokens[0]);
                _rows = std::stoi(tokens[1]);  
                }
                _nnz = std::stoi(tokens[2]);

                for (std::size_t i = 0; i < _nnz; ++i) {
                    std::getline(file, line);
                    std::stringstream ss2(line);
                    std::size_t row, col;
                    T value;
                    if constexpr(S==StorageOrder::row_wise){
                    ss2 >> row >> col >> value;
                    }else if constexpr(S==StorageOrder::column_wise){
                        ss2>>col>>row>>value;
                    }
                    _data[{row - 1, col - 1}] = value;
                }
            }
        }
        file.close();
    }


    template<typename T, StorageOrder S>
    std::vector<T> MatrixClass<T,S>::operator*(const std::vector<T> & v){
         if (v.size() != _cols) {
            std::cerr << "The vector size does not match the matrix column size." << std::endl;
        }

        std::vector<T> result(_rows,T{});
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

       template<typename T, StorageOrder S1>
       template<StorageOrder S2>
        MatrixClass<T,StorageOrder::row_wise> MatrixClass<T,S1>::operator*(MatrixClass<T,S2> const & lhs){
            if(_cols!=lhs.get_rows()){
                std::cerr<<"The number of columns of the first matrix must be equal to the number of rows of the second matrix."<<std::endl;
            }
    
        MatrixClass<T,StorageOrder::row_wise> result(_rows,lhs.get_cols());
        if constexpr (S1 == StorageOrder::row_wise) {
            if (!compressed) {
                for(std::size_t k=0;k<lhs.get_cols();++k){
                    for (std::size_t i = 0; i < _rows; ++i) {
                        for (auto it = _data.lower_bound({i, 0}); it != _data.upper_bound({i, _cols - 1}); ++it) {
                            result(i,k) += (it->second) * lhs(it->first[1],k);
                        }
                    }
                }
            } else {
                for(std::size_t k=0;k<lhs.get_cols();++k){
                    for (std::size_t i = 0; i < _rows; ++i) {
                        for (std::size_t j = inner_indexes[i]; j < inner_indexes[i + 1]; ++j) {
                            result(i,k) += values[j] * lhs(outer_indexes[j],k);
                        }
                    }
                }
            }
        } else if (S1 == StorageOrder::column_wise) {
            if (!compressed){
                for(std::size_t k=0;k<lhs.get_cols();++k){
                    for (std::size_t j = 0; j < _cols; ++j) {
                        for(auto it=_data.lower_bound({j,0});it!=_data.upper_bound({j,_rows-1});++it){
                            result(it->first[1],k)+=(it->second)*lhs(j,k);
                        }
                    }
                }
            } else {
                for(std::size_t k=0;k<lhs.get_cols();++k){
                    for (std::size_t j = 0; j < _cols; ++j) {
                        for (std::size_t i = outer_indexes[j]; i < outer_indexes[j + 1]; ++i) {
                            result(inner_indexes[i],k) += values[i] * lhs(j,k);
                        }
                    }
                }
            }
        }

    
    return result;
    }


     template<typename T, StorageOrder S>
     template<NormMethod N>
     decltype(auto) MatrixClass<T,S>::compute_norm()const{
      decltype(auto) norm=0.0;
        if constexpr(N==NormMethod::infinity_norm){
            decltype(auto) sum=0.0;
                for(std::size_t i=0;i<_rows;++i){
                    sum=0.0;
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
                    sum=0.0;
                    for(std::size_t i=0;i<_rows;++i){
                        sum+=std::abs((*this)(i,j));
                    }
                    norm=std::max(norm,sum);
                }

            return norm;
        }

        if constexpr(N==NormMethod::Frobenius_norm){
                for(std::size_t i=0;i<_rows;++i){
                    for(std::size_t j=0;j<_cols;++j){
                        norm+=std::abs((*this)(i,j))*std::abs((*this)(i,j));
                    }
                }
            return std::sqrt(norm);
        }

    }

    template<typename T, StorageOrder S>
    void MatrixClass<T,S>::print_matrix()const{
        for (std::size_t i = 0; i <_rows; ++i) {
            for (std::size_t j = 0; j < _cols; ++j) {
                if constexpr(is_complex<T>{}){
                    std::cout << std::real((*this)(i, j)) << " + " << std::imag((*this)(i, j)) << "i   ";
                }else{
                    std::cout << (*this)(i, j) << " ";
                }
                
            }
            std::cout << std::endl;
        }
    }



};

#endif