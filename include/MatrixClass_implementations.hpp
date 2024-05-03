#include "MatrixClass.hpp"

namespace algebra{

    template<typename T, StorageOrder S>
     T & MatrixClass<T,S>::operator()(const std::size_t i,const std::size_t j){
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
}