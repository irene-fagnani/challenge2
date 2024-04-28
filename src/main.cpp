#include "MatrixClass.cpp"
#include <chrono>

int main(){

    algebra::MatrixClass<double,algebra::StorageOrder::row_wise> A(3,3);

    A(0,0)=1;
    A(0,1)=2;
    A(0,2)=3;
    A(1,0)=4;
    A(1,1)=5;
    A(1,2)=6;
    A(2,0)=7;
    A(2,1)=8;
    A(2,2)=9;

    std::vector<double> v = {1,2,3};

    A.compress();
    A.print_matrix();

    std::cout<<"A*v\n"<<std::endl;


    auto t0 = std::chrono::high_resolution_clock::now();

    std::vector<double> res = A*v;

    auto t1 = std::chrono::high_resolution_clock::now();
    auto delta_t = std::chrono::duration_cast<std::chrono::microseconds>(t1-t0);
    std::cout << ((A.is_compressed())?"compressed":"uncompressed") << " : " << delta_t.count() << " ms" << std::endl;
    



    algebra::MatrixClass<double, algebra::StorageOrder::row_wise> B1;

    B1.read_matrix("lnsp_131.mtx");
    B1.compress();
    std::cout<<"\nB1*v2\n"<<std::endl;

   // B.print_matrix();

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

   // B.print_matrix();
    auto t0_3 = std::chrono::high_resolution_clock::now();

    std::vector<double> res4 = B2*v2;

    auto t1_3 = std::chrono::high_resolution_clock::now();
    auto delta_t_3 = std::chrono::duration_cast<std::chrono::microseconds>(t1_3-t0_3);
    
    std::cout << ((B2.is_compressed())?"compressed":"uncompressed") << " : " << delta_t_3.count() << " ms" << std::endl;

    B2.uncompress();


    auto t0_4 = std::chrono::high_resolution_clock::now();

    std::vector<double> res5 = B2*v2;

    auto t1_4 = std::chrono::high_resolution_clock::now();
    auto delta_t_4 = std::chrono::duration_cast<std::chrono::microseconds>(t1_4-t0_4);
    
    std::cout << ((B2.is_compressed())?"compressed":"uncompressed") << " : " << delta_t_4.count() << " ms" << std::endl;

    return 0;
}