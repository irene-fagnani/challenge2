#include "MatrixClass.hpp"

namespace algebra{

template<typename T, StorageOrder S>
bool MatrixClass<T,S>::in_bound(const std::size_t r, const std::size_t c) const{

    return r<_rows && c<_cols;
}

template<typename T,StorageOrder S>
void compute_nzero(){
     for(auto it=_data.begin();it!=_data.end();it++){
          if(it->second!=0){
               _nzero++;
          }
     }

}


template<typename T, StorageOrder S>
T & MatrixClass<T,S>::operator()(const std::size_t i,const std::size_t j){
      
        if(!in_bound(i,j)){
             std::cerr<<"Index out of bounds."<<std::endl;
        }

        if constexpr(S==StorageOrder::row_wise){

             return   _data[{i,j}];
            
        }
        if constexpr(S==StorageOrder::column_wise){

             return _data[{j,i}];
            
    }
    
}

template<typename T, StorageOrder S>
T MatrixClass<T,S>::operator()(const std::size_t i,const std::size_t j)const{
        compute_nzero();
        if constexpr(S==StorageOrder::row_wise){

             return  _data.at({i,j});
            
        }
        if constexpr(S==StorageOrder::column_wise){

             return _data.at({j,i});
            
    }

    
}

template<typename T, StorageOrder S>
void MatrixClass<T,S>::compress(){
     if(compressed){
          std::cout<<"The matrix has already been compressed. If you want to compress it again, please decompress it first."<<std::endl;
          return;
     }else{
          if constexpr(S==StorageOrder::row_wise){
            inner_indexes[0]=0;
            for(auto it=_data.begin();it!=_data.end();++it){
               if(it->second!=0){
                    values.push_back(it->second);
                    outer_indexes.push_back(it->first[1]);
                    inner_indexes[it->first[0]+1]++;

               }
            }
          }else if constexpr(S==StorageOrder::column_wise){
               inner_indexes[0]=0;
               for(auto it=_data.begin();it!=_data.end();++it){
                    if(it->second!=0){
                         values.push_back(it->second);
                         outer_indexes.push_back(it->first[0]);
                         inner_indexes[it->first[1]+1]++;
                    }
               }
          }
     }
     _data.clear();
     compressed=true;
}

template<typename T, StorageOrder S>
void MatrixClass<T,S>::uncompress(){
     if(!compressed){
          std::cout<<"The matrix has not been compressed yet."<<std::endl;
          return;
     }else{
          if constexpr(S==StorageOrder::row_wise){
               for(std::size_t i=0;i<_rows;++i){
                    for(std::size_t j=inner_indexes[i];j<inner_indexes[i+1];j++){
                         _data[{i,outer_indexes[j]}]=values[j];
                    }
               }
             
          }
     }
     compressed=false;
     inner_indexes.clear();
     outer_indexes.clear();
     values.clear();
}

template<typename T, StorageOrder S>
bool MatrixClass<T,S>::is_compressed()const{
     return compressed;
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
