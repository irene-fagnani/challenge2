
#include "MatrixClass_implementations.hpp"
#include "Utilities.hpp"
#include "GetPot"


int main(int argc, char **argv){
  GetPot command_line(argc,argv);
  std::string filename=command_line("filename","lnsp_131.mtx");
  bool print=command_line("print",false);

  std::cout<<"\nBIG MATRIX CASE: ---------------------------------------------------------\n"<<std::endl;


  algebra::MatrixClass<double,algebra::StorageOrder::row_wise> B_row;
  B_row.read_matrix(filename);

  algebra::MatrixClass<double,algebra::StorageOrder::column_wise> B_col;
  B_col.read_matrix(filename);

  //B_row.print_matrix();
  //B_col.print_matrix();

  std::cout<<"\nNorm computations: B_row\n "<<std::endl;
  std::cout<<"one_norm:       "<<B_row.compute_norm<algebra::NormMethod::one_norm>()<<std::endl;
  std::cout<<"infinity_norm:  "<<B_row.compute_norm<algebra::NormMethod::infinity_norm>()<<std::endl;    
  std::cout<<"frobenius_norm: "<<B_row.compute_norm<algebra::NormMethod::Frobenius_norm>()<<std::endl;

  std::cout<<"\nNorm computations: B_col\n "<<std::endl;
  std::cout<<"one_norm:       "<<B_col.compute_norm<algebra::NormMethod::one_norm>()<<std::endl;
  std::cout<<"infinity_norm:  "<<B_col.compute_norm<algebra::NormMethod::infinity_norm>()<<std::endl;    
  std::cout<<"frobenius_norm: "<<B_col.compute_norm<algebra::NormMethod::Frobenius_norm>()<<std::endl;

  std::vector<double> v_big(B_row.get_cols(),1);
  algebra::MatrixClass<double,algebra::StorageOrder::row_wise> v_big_m(B_row.get_cols(),1,1);

  std::cout<<"\n\nUncompress: -------------------------------------------------------------------\n";

  std::cout<<"\n\nProducts: matrix * vector\n";

  std::cout<<"\n B_row * v_big: ";

  auto delta_U_1=time_test(B_row,v_big,print);

  std::cout<<"\n B_col * v_big: ";

  auto delta_U_2=time_test(B_col,v_big,print);

  std::cout<<"\n\nProducts: matrix * matrix\n";

  std::cout<<"\n B_row * v_big_m: ";

  auto delta_U_3=time_test(B_row,v_big_m,print);

  std::cout<<"\n B_col * v_big_m: ";

  auto delta_U_4=time_test(B_col,v_big_m,print);

  std::cout<<"\n B_col * B_row: ";

  auto delta_U_5=time_test(B_col,B_row,print);

  std::cout<<"\n\nCompress: -------------------------------------------------------------------\n";

  B_row.compress();
  B_col.compress();

  std::cout<<"\n\nProducts: matrix * vector\n";

  std::cout<<"\n B_row * v_big: ";

  auto delta_C_1=time_test(B_row,v_big,print);

  std::cout<<"\n B_col * v_big: ";

  auto delta_C_2=time_test(B_col,v_big,print);

  std::cout<<"\n\nProducts: matrix * matrix\n";

  std::cout<<"\n B_row * v_big_m: ";

  auto delta_C_3=time_test(B_row,v_big_m,print);

  std::cout<<"\n B_col * v_big_m: ";

  auto delta_C_4=time_test(B_col,v_big_m,print);

  std::cout<<"\n B_col * B_row: ";

  auto delta_C_5=time_test(B_col,B_row,print);

   //std::cout<<"\n\nDecompress: --------------------------------------------------------------\n"<<std::endl;
   //B_row.uncompress();
   //B_col.uncompress();

  
   // std::cout<<"\nB_row: \n"<<std::endl;
   // B_row.print_matrix();
   // std::cout<<"\nB_col: \n"<<std::endl;
   // B_col.print_matrix();

  std::cout<<"\n\nConclusions: ---------------------------------------------------------------\n"<<std::endl;

  std::cout<<"Percentage of time decrease for the matrix-vector product:\n"<<std::endl;
  std::cout<<"B_row * v_big: "<<(delta_U_1.count()-delta_C_1.count())*100./delta_U_1.count()<<"%\n"<<std::endl;
  std::cout<<"B_col * v_big: "<<(delta_U_2.count()-delta_C_2.count())*100./delta_U_2.count()<<"%\n"<<std::endl;
  std::cout<<"Percentage of time decrease for the matrix-matrix product:\n"<<std::endl;
  std::cout<<"B_row * v_big_m: "<<(delta_U_3.count()-delta_C_3.count())*100./delta_U_3.count()<<"%\n"<<std::endl;
  std::cout<<"B_col * v_big_m: "<<(delta_U_4.count()-delta_C_4.count())*100./delta_U_4.count()<<"%\n"<<std::endl;
  std::cout<<"B_col * B_row: "<<(delta_U_5.count()-delta_C_5.count())*100./delta_U_5.count()<<"%\n"<<std::endl;
  
  
  

    std::cout<<"SMALL MATRIX CASE: ---------------------------------------------------------\n"<<std::endl;
    

    algebra::MatrixClass<std::complex<double>,algebra::StorageOrder::row_wise> A_row_complex(3,3);

    A_row_complex(0,0).imag(1);
    A_row_complex(1,1).imag(1);
    A_row_complex(2,2).imag(1); 


    algebra::MatrixClass<double,algebra::StorageOrder::column_wise> A_col_real(3,3);

    A_col_real(0,0)=1;
    A_col_real(1,1)=1;
    A_col_real(2,2)=1;
    std::cout<<"A_row: complex matrix stored row_wise\n"<<std::endl;
    A_row_complex.print_matrix();

    std::cout<<"\nNorm computations\n "<<std::endl;
    std::cout<<"one_norm:       "<< A_row_complex.compute_norm<algebra::NormMethod::one_norm>()<<std::endl;
    std::cout<<"infinity_norm:  "<< A_row_complex.compute_norm<algebra::NormMethod::infinity_norm>()<<std::endl;    
    std::cout<<"frobenius_norm: "<< A_row_complex.compute_norm<algebra::NormMethod::Frobenius_norm>()<<std::endl;
    
    std::cout<<"\nA_col: real matrix stored column_wise\n"<<std::endl;
    A_col_real.print_matrix();
    
    A_col_real.resize_matrix(2,2,0);
    std::cout << "\n A_col resized" << std::endl;
    A_col_real.print_matrix();

    std::cout<<"\nNorm computations\n "<<std::endl;
    std::cout<<"one_norm:       "<<A_col_real.compute_norm<algebra::NormMethod::one_norm>()<<std::endl;
    std::cout<<"infinity_norm:  "<<A_col_real.compute_norm<algebra::NormMethod::infinity_norm>()<<std::endl;    
    std::cout<<"frobenius_norm: "<<A_col_real.compute_norm<algebra::NormMethod::Frobenius_norm>()<<std::endl;



    std::vector<double> v_real = {1,2,3};
    std::vector<std::complex<double>> v_complex = {{0,1},{0,2},{0,3}};

    algebra::MatrixClass<double,algebra::StorageOrder::row_wise> v_real_m(3,1);
    v_real_m(0,0)=1;
    v_real_m(1,0)=2;
    v_real_m(2,0)=3;
    algebra::MatrixClass<std::complex<double>,algebra::StorageOrder::column_wise> v_complex_m(3,1);
    v_complex_m(0,0).imag(1);
    v_complex_m(1,0).imag(2);
    v_complex_m(2,0).imag(3);

    std::cout<<"\nv_real: \n"<<std::endl;
    algebra::print_vector(v_real);

    std::cout<<"\nv_complex: \n"<<std::endl;
    algebra::print_vector(v_complex);

    std::cout<<"\nv_real_m: \n"<<std::endl;
    v_real_m.print_matrix();

    std::cout<<"\nv_complex_m: \n"<<std::endl;
    v_complex_m.print_matrix();

    
    std::cout<<"\nUncompress: -------------------------------------------------------------------\n"<<std::endl;

    std::cout<<"Products: matrix * vector\n\n"<<std::endl;

    std::cout<<"\n A_row_complex * v_real\n"<<std::endl;

    time_test(A_row_complex,v_real,print);

    std::cout<<"\n A_col_real * v_complex\n"<<std::endl;

    time_test(A_col_real,v_complex,print);

    std::cout<<"Products: matrix * matrix\n\n"<<std::endl;

    std::cout<<"\n A_row_complex * v_real_m\n"<<std::endl;

    time_test(A_row_complex,v_real_m,print);

    std::cout<<"\n A_col_real * v_complex_m\n"<<std::endl;

    time_test(A_col_real,v_complex_m,print);

    std::cout<<"\n A_col_real * A_row_complex\n"<<std::endl;

    time_test(A_col_real,A_row_complex,print);

   

    std::cout<<"\nCompress: -------------------------------------------------------------------\n"<<std::endl;

    A_col_real.compress();
    A_row_complex.compress();

    std::cout<<"Products: matrix * vector\n\n"<<std::endl;

    std::cout<<"\n A_row_complex * v_real\n"<<std::endl;

    time_test(A_row_complex,v_real,print);

    std::cout<<"\n A_col_real * v_complex\n"<<std::endl;

    time_test(A_col_real,v_complex,print);

    std::cout<<"Products: matrix * matrix\n\n"<<std::endl;

    std::cout<<"\n A_row_complex * v_real_m\n"<<std::endl;

    time_test(A_row_complex,v_real_m,print);

    std::cout<<"\n A_col_real * v_complex_m\n"<<std::endl;

    time_test(A_col_real,v_complex_m,print);

    std::cout<<"\n A_col_real * A_row_complex\n"<<std::endl;

    time_test(A_col_real,A_row_complex,print);

  std::cout<<"\nDecompress: --------------------------------------------------------------\n"<<std::endl;
   A_col_real.uncompress();
   A_row_complex.uncompress();
  
   std::cout<<"\nA_col_real: \n"<<std::endl;
   A_col_real.print_matrix();
   std::cout<<"\nA_row_complex: \n"<<std::endl;
   A_row_complex.print_matrix();
    
  




    return 0;
}
