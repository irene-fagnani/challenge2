#include "MatrixClass.hpp"


namespace algebra{

    /*!
    *@brief Print a vector.
    *@tparam T type of the elements in the vector.
    *@param v vector to print.
    */

   template<typename T>
   void print_vector(std::vector<T> & v){

      for(std::size_t i=0;i<v.size();++i){

            if constexpr(is_complex<T>::value){

                std::cout<<"| "<<v[i].real()<<"+"<<v[i].imag()<<"i |"<<std::endl;

            }else{

                std::cout<<"| "<<v[i]<<" |"<<std::endl;
            }
            
      }
   };

    
    /*!
    * @brief Count the time for the multiplication between a matrix and a vector.
    * @tparam T type of the elements in the matrix.
    * @tparam S storage order of the matrix.
    * @tparam Tv type of the elements in the vector.
    * @param mat matrix to multiply.
    * @param v vector to multiply.
    * @param print boolean to print the result of the multiplication.
    * @return Time spent in the multiplication.
    */

   template<typename T, StorageOrder S, typename Tv>
   auto time_test(MatrixClass<T,S> & mat, std::vector<Tv> & v, bool print){

      auto t0=std::chrono::high_resolution_clock::now();
      auto res=mat*v;
      auto t1=std::chrono::high_resolution_clock::now();
      auto delta_t=std::chrono::duration_cast<std::chrono::microseconds>(t1-t0);
      std::cout<<"Time for the multiplication: "<<delta_t.count()<<" microseconds\n";
      if(print){
        print_vector(res);  
      }
       return delta_t; 
   }
    
    /*!
     * @brief Count the time for the multiplication between two matrices.
     * @tparam T1 Type of elements in the first matrix.
     * @tparam S1 Storage order of the first matrix.
     * @tparam T2 Type of elements in the second matrix.
     * @tparam S2 Storage order of the second matrix.
     * @param mat1 First matrix involved in the multiplication.
     * @param mat2 Second matrix involved in the multiplication.
     * @param print Boolean to print the result of the multiplication.
     * @return Time spent in the multiplication.
     */
    template<typename T1, StorageOrder S1, typename T2, StorageOrder S2>
    auto time_test(MatrixClass<T1,S1> & mat1, MatrixClass<T2,S2> & mat2, bool print){

      auto t0=std::chrono::high_resolution_clock::now();
      auto res=mat1*mat2;
      auto t1=std::chrono::high_resolution_clock::now();
      auto delta_t=std::chrono::duration_cast<std::chrono::microseconds>(t1-t0);
      std::cout<<"Time for the multiplication: "<<delta_t.count()<<" microseconds\n";
      if(print){
        res.print_matrix(); 
      }
      return delta_t;
         
   }

};