#include "Matrix.cpp"
#include "Utility.cpp"
#include <thread>
#include "thread_pool.cpp"

//function computes 2D x 2D multiplication
template <typename T>
void rank2TensorMultPThread(Matrix<T> &A, Matrix<T> &B)
{
    //   the multiplication operators are defined in Matrix.h and implemented in Matrix.cpp
    //   this allows for in place multiplication where the multiplicand matrices are not of importance
    A ^= B;
}

template <typename T>
void rank3TensorMultPThread(std::vector<Matrix<T>> &A, std::vector<Matrix<T>> &B, std::vector<Matrix<T>> &C, int NUM_THREADS)
{

//       creates a thread for each 2D x 2D multiplication
        if(NUM_THREADS > 1)
        {
            std::thread threads[A.size()];
            for(auto i = 0; i< static_cast<int>(A.size()); i++ )
                threads[i] = std::thread( rank2TensorMultPThread<T> , std::ref(A[i]) , std::ref(B[i]) );
//          join the threads
            for(auto i = 0; i< static_cast<int>(A.size()); i++ )
                threads[i].join();
        }
//      if the number of threads requested is 1 then do simple serial programming
        else
            for(auto i = 0; i< static_cast<int>(A.size()); i++ )
                rank2TensorMultPThread(A[i],B[i]);

    //       creates a thread for each 2D x 2D multiplication
    if (NUM_THREADS > 1)
    {
        std::thread threads[NUM_THREADS];
        for (auto i = 0; i < static_cast<int>(A.size()); i++)
            threads[i] = std::thread(rank2TensorMultPThread<T>, std::ref(A[i]), std::ref(B[i]));
        //          join the threads
        for (auto i = 0; i < static_cast<int>(A.size()); i++)
            threads[i].join();
    }
    //      if the number of threads requested is 1 then do simple serial programming
    else
        for (auto i = 0; i < static_cast<int>(A.size()); i++)
            rank2TensorMultPThread(A[i], B[i]);

}

//same implementation as above
template <typename T>
Matrix<T> &rank2TensorMultOpenMP(Matrix<T> &A, Matrix<T> &B, Matrix<T> &C)
{
    return C = A * B;
}

template <typename T>
std::vector<Matrix<T>> &rank3TensorMultOpenMP(std::vector<Matrix<T>> &A, std::vector<Matrix<T>> &B, std::vector<Matrix<T>> &C, int &NUM_THREADS)
{

    // python script replaces the statement below to allow for serial programming when requested.
#pragma omp parallel for schedule(static) num_threads(NUM_THREADS)
    for (auto i = 0; i < static_cast<int>(A.size()); i++)
        C[i] = rank2TensorMultOpenMP(A[i], B[i], C[i]);

    //     return the result matrix C to verify correctness of implementation
    return C;
}

//function that generates a 3D matrix as a stack of matrices
template <typename T>
std::vector<Matrix<T>> generateMatrix(Matrix<T> &A)
{

    auto size = static_cast<int>(A.get_rows());

    std::vector<Matrix<T>> mat(size);

    for (auto i = 0; i < size; i++)
    {
        for (auto j = 0; j < size; j++)
        {
            for (auto k = 0; k < size; k++)
                A(j, k) = randomInt(20); // this function is a random engine defined in Utility.h.
        }
        mat[i] = A;
    }

    return mat;
}

//function to print a 3D matrix. For debug perposes only!
template <typename T>
void print(std::vector<Matrix<T>> &A)
{
    auto size = static_cast<int>(A[0].get_cols());

    for (auto i = 0; i < size; i++)
    {
        auto mat = A[i];
        std::cout << std::endl;
        for (int j = 0; j < size; j++)
            for (int k = 0; k < size; k++)
                std::cout << mat(j, k) << " ";
    }
    std::cout << std::endl;
}

auto main(int argc, char *argv[], char *envp[]) -> int
{

    CTP::ThreadPool thread_pool;


    //  default values for matrix size and number of threads
    auto N = 10, NUM_THREADS = 1;
    //  accepts data descriptor arguments from python script
    std::cin >> N >> NUM_THREADS;
    //  if threads requested is more than one then provide number of threads available
    NUM_THREADS = NUM_THREADS > 1 ? (omp_get_num_procs() / 2) : 1;
    //  allow nested parallelism
    omp_set_nested(true);
    //  declare and initialise Matrix objects
    Matrix<int> a(N, N, N), b(N, N, N);
    //  populate the Matrix objects
    auto A = generateMatrix(a);
    auto B = generateMatrix(b);
    //  assignment operator is defined in Matrix.h.
    auto C = A;

    //  declare time variables and begin timing computation using omp timer
    auto begin = omp_get_wtime();
    C = rank3TensorMultOpenMP(A, B, C, NUM_THREADS);
    auto end = omp_get_wtime();
    //  compute total computation time and return to stdout buffer which is read by subprocess from python script
    std::cout << (end - begin) << std::endl;
    //  same process as above
    begin = omp_get_wtime();
    rank3TensorMultPThread(A, B, C, NUM_THREADS);
    end = omp_get_wtime();
    std::cout << (end - begin) << std::endl;

    return 0;
}
