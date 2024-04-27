#include "MatrixClass.cpp"

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

    std::vector<double> res = A*v;
    for(auto & elem: res){
        std::cout<<elem<<std::endl;
    }
    
    algebra::MatrixClass<double, algebra::StorageOrder::row_wise> B;
    B.read_matrix("lnsp_131.mtx");
    B.print_matrix();
    return 0;
}