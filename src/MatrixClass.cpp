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
        if(!compressed){
           if constexpr(S==StorageOrder::row_wise){
               return  _data[{i,j}];
          }
             
        if constexpr(S==StorageOrder::column_wise){
             return _data[{j,i}];
          }
            
        }else{
          if constexpr(S==StorageOrder::row_wise){
               for(std::size_t ii=inner_indexes[i];ii<inner_indexes[i+1];++i){
                    if(outer_indexes[ii]==j){
                         return values[ii];
                    }
                    std::cerr<<"In compressed case, cannot update the element."<<std::endl;
               }
          }else if constexpr(S==StorageOrder::column_wise){
               for(std::size_t jj=outer_indexes[j];jj<outer_indexes[j+1];++j){
                    if(inner_indexes[jj]==i){
                         return values[jj];
                    }
                    std::cerr<<"In compressed case, cannot update the element."<<std::endl;
               }
          }
        }
       
    
}

template<typename T, StorageOrder S>
T MatrixClass<T,S>::operator()(const std::size_t i,const std::size_t j)const{
        if(!in_bound(i,j)){
             std::cerr<<"Index out of bounds."<<std::endl;
        }
        if(!compressed){
           if constexpr(S==StorageOrder::row_wise){
          if(_data.find({i,j})==_data.end()){
               return 0;
          }else{
               return   _data[{i,j}];
          }
             
            
        }
        if constexpr(S==StorageOrder::column_wise){
          if(_data.find({j,i})==data.end()){
               return 0;
          }else{
             return _data[{j,i}];
          }
            
        }
        }else{
          if constexpr(S==StorageOrder::row_wise){
               for(std::size_t ii=inner_indexes[i];ii<inner_indexes[i+1];++i){
                    if(outer_indexes[ii]==j){
                         return values[ii];
                    }
                    return 0;
               }
          }else if constexpr(S==StorageOrder::column_wise){
               for(std::size_t jj=outer_indexes[j];jj<outer_indexes[j+1];++j){
                    if(inner_indexes[jj]==i){
                         return values[jj];
                    }
                    return 0;
               }
          }
        }
       
    
    
}

template<typename T, StorageOrder S>
void MatrixClass<T,S>::compress(){
     compute_nzero();
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
     compute_nzero();
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
          else if constexpr(S==StorageOrder::column_wise){
               for(std::size_t i=0;i<_cols;++i){
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

template<typename T, StorageOrder S>
void resize_matrix(std::size_t rows, std::size_t cols){
     if(!compressed){
          for(std::size_t i=0;i<rows;++i){
               for(std::size_t j=0;j<cols;++j){
                    _data[{i,j}]=0;
               }
          }
     }
}

template<typename T, StorageOrder S>
bool operator<(const std::array<std::size_t,2> & index1, const std::array<std::size_t,2> & index2){

        if constexpr(S==StorageOrder::column_wise){

           return (index1[0]<index2[0] || index1[0]==index2[0] && index1[1]<index2[1]);

        }
        return  (index1[1]<index2[1] || index1[1]==index2[1] && index1[0]<index2[0]);

       
}

template<typename T,StorageOrder S>
std::vector<T> operator*(const MatrixClass<T,S> & A, const std::vector<T> & v){

     if(v.size()!=A._cols){
          std::cerr<<"The vector size does not match the matrix column size."<<std::endl;
     }

     std::vector<T> result(A._rows);
     if constexpr(S==StorageOrder::row_wise){
         if(!A.compressed){
               for(std::size_t i=0;i<A._rows;++i){
                    for(std::size_t j=0;j<A._cols;++j){
                         result[i]+=A._data[{i,j}]*v[j];
                    }
               }
          }else{
                for(std::size_t i=0;i<A._rows;++i){
                    for(std::size_t j=A.inner_indexes[i];j<A.inner_indexes[i+1];++j){
                         result[i]+=A.values[j]*v[A.outer_indexes[j]];
                    }
               }
          }
     }else if(S==StorageOrder::column_wise){

          if(!A.compressed){
               for(std::size_t i=0;i<A._rows;++i){
                    for(std::size_t j=0;j<A._cols;++j){
                         result[i]+=A._data[{j,i}]*v[j];
                    }
               }
          }else{
               for(std::size_t i=0;i<A._rows;++i){
                    for(std::size_t j=A.outer_indexes[i];j<A.outer_indexes[i+1];++j){
                         result[i]+=A.values[j]*v[A.inner_indexes[j]];
                    }
               }
          }
     }

     return result;

}

template<typename T,StorageOrder S>
std::vector<T> operator*( const std::vector<T> & v,const MatrixClass<T,S> & A){

     if(v.size()!=A._rows){
          std::cerr<<"The vector size does not match the matrix column size."<<std::endl;
     }

     std::vector<T> result(A._rows);
     if constexpr(S==StorageOrder::row_wise){
         if(!A.compressed){
               for(std::size_t j=0;j<A._cols;++j){
                    for(std::size_t i=0;j<A._rows;++i){
                         result[j]+=A._data[{i,j}]*v[i];
                    }
               }
          }else{
                for(std::size_t j=0;j<A._cols;++j){
                    for(std::size_t i=A.inner_indexes[j];i<A.inner_indexes[j+1];++i){
                         result[j]+=A.values[i]*v[A.outer_indexes[i]];
                    }
               }
          }
     }else if(S==StorageOrder::column_wise){

          if(!A.compressed){
               for(std::size_t j=0;j<A._cols;++j){
                    for(std::size_t i=0;j<A._rows;++i){
                         result[j]+=A._data[{j,i}]*v[i];
                    }
               }
          }else{
               for(std::size_t j=0;j<A._cols;++j){
                    for(std::size_t i=A.outer_indexes[j];i<A.outer_indexes[j+1];++i){
                         result[j]+=A.values[i]*v[A.inner_indexes[i]];
                    }
               }
          }
     }

     return result;

}

};