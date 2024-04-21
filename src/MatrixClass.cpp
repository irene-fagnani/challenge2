#include "MatrixClass.hpp"

namespace algebra{

template<typename T, StorageOrder S>
bool MatrixClass<T,S>::in_bound(const std::size_t r, const std::size_t c) const{

    return r<_rows && c<_cols;
}

template<typename T, StorageOrder S>
void MatrixClass<T,S>::compress(){

}

template<typename T, StorageOrder S>
T & MatrixClass<T,S>::operator()(const std::size_t i,const std::size_t j){
      /*
        if(!in_bound(i,j)){
             std::cerr("Index out of bounds.");
        }

        if constexpr(S==StorageOrder::row_wise){

             return   data[{i,j}];
            
        }
        if constexpr(S==StorageOrder::column_wise){

             return data[{j,i}];
            
    }
    return data[{i,j}];
    */
}



};

template<typename T, algebra::StorageOrder S>
bool operator<(const std::array<std::size_t,2> & index1, const std::array<std::size_t,2> & index2){

        if constexpr(S==algebra::StorageOrder::column_wise){

           return (index1[0]<index2[0] || index1[0]==index2[0] && index1[1]<index2[1]);

        }
       

       if constexpr(S==algebra::StorageOrder::row_wise){

        return  (index1[1]<index2[1] || index1[1]==index2[1] && index1[0]<index2[0]);

       }
}
