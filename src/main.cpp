
#include "MatrixClass.hpp"
#include <chrono>

int main(){

    algebra::MatrixClass<std::complex<double>,algebra::StorageOrder::row_wise> A_row(3,3);

    A_row(0,0)=1;
    A_row(0,1)=2;
    A_row(0,2)=3;
    A_row(1,0)=4;
    A_row(1,1)=5;
    A_row(1,2)=6;
    A_row(2,0)=7;
    A_row(2,1)=8;
    A_row(2,2)=9;

    algebra::MatrixClass<std::complex<double>,algebra::StorageOrder::column_wise> A_col(3,3);

    A_col(0,0)=1;
    A_col(0,1)=2;
    A_col(0,2)=3;
    A_col(1,0)=4;
    A_col(1,1)=5;
    A_col(1,2)=6;
    A_col(2,0)=7;
    A_col(2,1)=8;
    A_col(2,2)=9;

    std::cout<<"---ROW_WISE---"<<std::endl;
    std::cout<<"one_norm: "<<A_row.compute_norm<algebra::NormMethod::one_norm>()<<std::endl;
    std::cout<<"infinity_norm: "<<A_row.compute_norm<algebra::NormMethod::infinity_norm>()<<std::endl;    
    std::cout<<"frobenius_norm: "<<A_row.compute_norm<algebra::NormMethod::Frobenius_norm>()<<std::endl;

    std::cout<<"---COL_WISE---"<<std::endl;
    std::cout<<"one_norm: "<<A_col.compute_norm<algebra::NormMethod::one_norm>()<<std::endl;
    std::cout<<"infinity_norm: "<<A_col.compute_norm<algebra::NormMethod::infinity_norm>()<<std::endl;    
    std::cout<<"frobenius_norm: "<<A_col.compute_norm<algebra::NormMethod::Frobenius_norm>()<<std::endl;

    std::vector<std::complex<double>> v = {1,2,3};

   // A.compress();
   // A.print_matrix();

    std::cout<<"A*v\n"<<std::endl;



    auto t0 = std::chrono::high_resolution_clock::now();

   // std::vector<std::complex<double>> res = A*v;

    auto t1 = std::chrono::high_resolution_clock::now();
    auto delta_t = std::chrono::duration_cast<std::chrono::microseconds>(t1-t0);
  //  std::cout << ((A.is_compressed())?"compressed":"uncompressed") << " : " << delta_t.count() << " ms" << std::endl;
    



    algebra::MatrixClass<double, algebra::StorageOrder::row_wise> B1;

    B1.read_matrix("lnsp_131.mtx");
    B1.compress();
    std::cout<<"\nB1*v2\n"<<std::endl;

    B1.print_matrix();

    std::vector<double> v2(B1.get_cols(),1);

    auto t0_1 = std::chrono::high_resolution_clock::now();

    std::vector<double> res2 = B1*v2;

    auto t1_1 = std::chrono::high_resolution_clock::now();
    auto delta_t_1 = std::chrono::duration_cast<std::chrono::microseconds>(t1_1-t0_1);
    
    std::cout << ((B1.is_compressed())?"compressed":"uncompressed") << " : " << delta_t_1.count() << " ms" << std::endl;

    B1.uncompress();

    auto t0_2 = std::chrono::high_resolution_clock::now();

    std::vector<double> res3 = B1*v2;

    auto t1_2 = std::chrono::high_resolution_clock::now();
    auto delta_t_2 = std::chrono::duration_cast<std::chrono::microseconds>(t1_2-t0_2);
    
    std::cout << ((B1.is_compressed())?"compressed":"uncompressed") << " : " << delta_t_2.count() << " ms" << std::endl;

    
    
    algebra::MatrixClass<double,algebra::StorageOrder::column_wise> B2;
    B2.read_matrix("lnsp_131.mtx");
    B2.compress();
    std::cout<<"\nB2*v2\n"<<std::endl;

   B2.print_matrix();
    auto t0_3 = std::chrono::high_resolution_clock::now();

    std::vector<double> res4 = B2*v2;

    auto t1_3 = std::chrono::high_resolution_clock::now();
    auto delta_t_3 = std::chrono::duration_cast<std::chrono::microseconds>(t1_3-t0_3);
    
    std::cout << ((B2.is_compressed())?"compressed":"uncompressed") << " : " << delta_t_3.count() << " ms" << std::endl;

   // B2.uncompress();


    auto t0_4 = std::chrono::high_resolution_clock::now();

    std::vector<double> res5 = B2*v2;

    auto t1_4 = std::chrono::high_resolution_clock::now();
    auto delta_t_4 = std::chrono::duration_cast<std::chrono::microseconds>(t1_4-t0_4);
    
    std::cout << ((B2.is_compressed())?"compressed":"uncompressed") << " : " << delta_t_4.count() << " ms" << std::endl;

    std::cout<<"\nB2*B1\n"<<std::endl;

    auto t0_5=std::chrono::high_resolution_clock::now();

    algebra::MatrixClass<double,algebra::StorageOrder::row_wise> res6=B2*B1;

    auto t1_5=std::chrono::high_resolution_clock::now();
    auto delta_t_5 = std::chrono::duration_cast<std::chrono::microseconds>(t1_5-t0_5);

    std::cout<<((res6.is_compressed())?"compressed":"uncompressed")<<" : "<<delta_t_5.count()<<" ms"<<std::endl;

    std::cout<<B2.compute_norm<algebra::NormMethod::one_norm>()<<std::endl;

    

    algebra::MatrixClass<double,algebra::StorageOrder::row_wise> B_row;
    B_row.read_matrix("lnsp_131.mtx");
    B_row.compress();
    std::cout<<"---ROW_WISE---"<<std::endl;
    std::cout<<"one_norm: "<<B_row.compute_norm<algebra::NormMethod::one_norm>()<<std::endl;
    std::cout<<"infinity_norm: "<<B_row.compute_norm<algebra::NormMethod::infinity_norm>()<<std::endl;    
    std::cout<<"frobenius_norm: "<<B_row.compute_norm<algebra::NormMethod::Frobenius_norm>()<<std::endl;

    algebra::MatrixClass<double,algebra::StorageOrder::column_wise> B_col;
    B_col.read_matrix("lnsp_131.mtx");
    B_col.compress();
    std::cout<<"---COL_WISE---"<<std::endl;
    std::cout<<"one_norm: "<<B_col.compute_norm<algebra::NormMethod::one_norm>()<<std::endl;
    std::cout<<"infinity_norm: "<<B_col.compute_norm<algebra::NormMethod::infinity_norm>()<<std::endl;    
    std::cout<<"frobenius_norm: "<<B_col.compute_norm<algebra::NormMethod::Frobenius_norm>()<<std::endl;
    /*
    std::cout<<"\n\nCOLUMN WISE MATRIX\n\n"<<std::endl;
    B_col.print_matrix();

    std::cout<<"\n\nROW WISE MATRIX\n\n"<<std::endl;
    B_row.print_matrix();
    */
   






    return 0;
}