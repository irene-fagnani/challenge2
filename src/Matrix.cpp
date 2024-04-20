#include "Matrix.hpp"
namespace algebra{


template<typename T, StorageOrder S>
bool Matrix<T,S>::in_bound(const std::size_t r, const std::size_t c) const{

    return r<rows && c<cols;
}

template<typename T, StorageOrder S>
void Matrix<T,S>::compress(){

}

template<typename T, StorageOrder S>
T & Matrix<T,S>::operator()(const size_t i,const size_t j){

        if(!in_bound(i,j)){
             throw std::out_of_range("Index out of bounds.");
        }

        if constexpr(S==StorageOrder::row_wise){

             return  & data[{i,j}];
            
        }
        if constexpr(S==StorageOrder::column_wise){

             return & data[{j,i}];
            
    }
}


bool Matrix<T,S>operator<(const idx & index1, const idx & index2)const{

        if constexpr(S==StorageOrder::column_wise){

           return (index1[0]<index2[0] || index1[0]==index2[0] && index1[1]<index2[1])

        }
       }

       if constexpr(S==StorageOrder::row_wise){

        return index1<index2;

       }

};