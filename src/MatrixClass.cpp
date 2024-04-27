#include "MatrixClass.hpp"

namespace algebra{

template<typename T, StorageOrder S>
bool MatrixClass<T,S>::in_bound(const std::size_t r, const std::size_t c) const{

    return r<_rows && c<_cols;
}

template<typename T,StorageOrder S>
void MatrixClass<T,S>::compute_nzero(){
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
        if(!is_compressed()){
           if constexpr(S==StorageOrder::row_wise){
               return  _data[{i,j}];
          }
             
        if constexpr(S==StorageOrder::column_wise){
             return _data[{j,i}];
          }
            
        }else{
          if constexpr(S==StorageOrder::row_wise){
               for(std::size_t ii=inner_indexes[i];ii<inner_indexes[i+1];++ii){
                    if(outer_indexes[ii]==j){
                         return values[ii];
                    }
                    std::cerr<<"In compressed case, cannot update the element."<<std::endl;
               }
          }else if constexpr(S==StorageOrder::column_wise){
               for(std::size_t jj=outer_indexes[j];jj<outer_indexes[j+1];++jj){
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
        if(!is_compressed()){
           if constexpr(S==StorageOrder::row_wise){
          if(_data.find({i,j})==_data.end()){
               return 0;
          }else{
               return   _data[{i,j}];
          }
             
            
        }
        if constexpr(S==StorageOrder::column_wise){
          if(_data.find({j,i})==_data.end()){
               return 0;
          }else{
             return _data[{j,i}];
          }
            
        }
        }else{
          if constexpr(S==StorageOrder::row_wise){
               for(std::size_t ii=inner_indexes[i];ii<inner_indexes[i+1];++ii){
                    if(outer_indexes[ii]==j){
                         return values[ii];
                    }
                    return 0;
               }
          }else if constexpr(S==StorageOrder::column_wise){
               for(std::size_t jj=outer_indexes[j];jj<outer_indexes[j+1];++jj){
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
     if(is_compressed()){
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
     set_compressed(true);
}

template<typename T, StorageOrder S>
void MatrixClass<T,S>::uncompress(){
     compute_nzero();
     if(!is_compressed()){
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
     set_compressed(false);
     inner_indexes.clear();
     outer_indexes.clear();
     values.clear();
}

template<typename T, StorageOrder S>
bool MatrixClass<T,S>::is_compressed()const{
     return compressed;
}

template<typename T, StorageOrder S>
void MatrixClass<T,S>::resize_matrix(std::size_t rows, std::size_t cols){
     if(!is_compressed()){
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
std::vector<T> MatrixClass<T,S>::operator*(const std::vector<T> & v){

     if(v.size()!=_cols){
          std::cerr<<"The vector size does not match the matrix column size."<<std::endl;
     }

     std::vector<T> result(_rows);
     if constexpr(S==StorageOrder::row_wise){
         if(!compressed){
               for(std::size_t i=0;i<_rows;++i){
                    for(std::size_t j=0;j<_cols;++j){
                         result[i]+=_data[{i,j}]*v[j];
                    }
               }
          }else{
                for(std::size_t i=0;i<_rows;++i){
                    for(std::size_t j=inner_indexes[i];j<inner_indexes[i+1];++j){
                         result[i]+=values[j]*v[outer_indexes[j]];
                    }
               }
          }
     }else if(S==StorageOrder::column_wise){

          if(!compressed){
               for(std::size_t i=0;i<_rows;++i){
                    for(std::size_t j=0;j<_cols;++j){
                         result[i]+=_data[{j,i}]*v[j];
                    }
               }
          }else{
               for(std::size_t i=0;i<_rows;++i){
                    for(std::size_t j=outer_indexes[i];j<outer_indexes[i+1];++j){
                         result[i]+=values[j]*v[inner_indexes[j]];
                    }
               }
          }
     }

     return result;

}

/*

template<typename T,StorageOrder S>
std::vector<T> operator*( const std::vector<T> & v,const MatrixClass<T,S> & A){

     if(v.size()!=A._rows){
          std::cerr<<"The vector size does not match the matrix column size."<<std::endl;
     }

     std::vector<T> result(A._rows);
     if constexpr(S==StorageOrder::row_wise){
         if(!A.is_compressed()){
               for(std::size_t j=0;j<A._cols;++j){
                    for(std::size_t i=0;j<A._rows;++i){
                         result[j]+=A(i,j)*v[i];
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

          if(!A.is_compressed()){
               for(std::size_t j=0;j<A._cols;++j){
                    for(std::size_t i=0;j<A._rows;++i){
                         result[j]+=A(i,j)*v[i];
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

*/

template<typename T, StorageOrder S>
MatrixClass<T,S> MatrixClass<T,S>::read_matrix(const std::string & filename){
     MatrixClass<T,S> matrix;
     std::ifstream file(filename);

     if(!file.is_open()){
          std::cerr<<"Cannot open the file."<<std::endl;
          return matrix;
     }

     std::string line;
     while(std::getline(file,line)){
          if(line[0]=='%'){
               continue;
          }
          std::stringstream ss(line);
          std::string token;
          std::vector<std::string> tokens;

          while(ss>>token){
               tokens.push_back(token);
          }

          if(tokens.size()!=3){
               std::cerr<<"The matrix file is not in the correct format."<<std::endl;
               return matrix;
          }

          if(tokens[0]=="%%MatrixMarket"){
               if(tokens[1]!="matrix" || tokens[2]!="real" || tokens[2]!="integer"){
                    std::cerr<<"Only general, real, matrices are supported. "<<std::endl;
                    return matrix;
               }
          }else{
               int rows=std::stoi(tokens[0]);
               int cols=std::stoi(tokens[1]);
               int nnz=std::stoi(tokens[2]);

               for(int i=0;i<nnz;++i){
                    std::getline(file,line);
                    std::stringstream ss2(line);
                    int row,col;
                    T value;
                    ss2>>row>>col>>value;
                    matrix(row-1,col-1)=value;
               }
          }
     }
     file.close();
     return matrix;
}

};