#include <complex>
#include <iostream>
#include <array>
#ifndef MATRIXCLASS_UTILITIES_HPP
#define MATRIXCLASS_UTILITIES_HPP
namespace algebra{

    /*!
     * @brief enumerator that indicates the storage ordering
     * @param row_wise indicates the storage ordering by rows
     * @param column_wise indicates the sorage ordering by columns
    */
    enum StorageOrder{
        row_wise,
        column_wise
    };
     /*!
     * @brief enumerator that indicates the norm method
     * @param one_norm indicates the one norm
     * @param infinity_norm indicates the infinity norm
     * @param Frobenius_norm indicates the Frobenius norm
     */
    enum NormMethod{
        one_norm,
        infinity_norm,
        Frobenius_norm
    };

    /*!
     * @brief primary template
     * @tparam T value type
     */
    template<typename T>
    struct is_complex : std::false_type {};
    /*!
     * @brief patrial specialization for complex numbers
     * @tparam T value type
     */
     
    template<typename T>
    struct is_complex<std::complex<T>> : std::true_type {};

        /*!
     * @brief operator< overloading for column-major ordering in the case of column wise order
     * @param first array of indexes to compare
     * @param second array of indexes to compare with the first
     * @return true if the index1 position is before index2 position in the Matrix, according to the storage order decided, zero otherwise
    */
    template<StorageOrder S>
    bool operator<(const std::array<std::size_t,2> & index1, const std::array<std::size_t,2> & index2){
         if constexpr (S == StorageOrder::column_wise) {

            return (index1[0] < index2[0] || index1[0] == index2[0] && index1[1] < index2[1]);

        }
        return (index1[1] < index2[1] || index1[1] == index2[1] && index1[0] < index2[0]);

    }
};
#endif