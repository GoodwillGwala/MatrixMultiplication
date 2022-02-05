import subprocess
import os
import numpy as np
import matplotlib.pyplot as plt
import fileinput
import sys

# function runs c++ files as a subprocess with different parameters


def run(args, threads):
    print("==Program execution BEGIN::Batch==")

# store thread and openMp list of execution times
    times = []
    thread = []
    openMP = []

    # Execute program, once for each n argument
    for n in args:

        # file descriptors data, temp for reading and writing respectively
        # this writes contents of temp into data which is passed on to the stdin buffer expected by c++ script
        data, temp = os.pipe()
#       c++ script is expecting 2 input size of matrix (n) and number of threads (threads) separated by a new line (\n)
<<<<<<< HEAD
        os.write(temp,bytes( str(n)+"\n" ,"utf-8"))
        os.write(temp,bytes( str(threads)+"\n" ,"utf-8"))
=======
        os.write(temp, bytes(str(n)+"\n", "utf-8"))
>>>>>>> 33742f7a71ff8f5993d33dbabff1659868252bc0
#       clean up and close pipe after writing to data descriptor
        os.close(temp)
#       subprocess is created that compiles, run script and pass in the data descriptor to the stdin buffer
        p = subprocess.check_output(
            "g++-10 -fopenmp -lgomp -g -Wall Lab1.cpp; ./a.out", stdin=data, shell=True)
#       print out arguements (n and thread) and results of the subprocess (p.decode) which is decoded to the standard python string (utf-8)

        print("-N = :" + str(n) + ", -Number of Threads: " + str(threads))
        print((p.decode("utf-8")))


# the output of subprocess is returned from the stdout buffer.
#       store the first line of output results from rank3TensorMultOpenMP()
        openMP.append(p.decode("utf-8").splitlines()[0])
#       store the second line of output results from rank3TensorMultPThread()
        thread.append(p.decode("utf-8").splitlines()[1])

    print("==Program execution END::Batch==")
    print("=========================================")
# store these results on another list
    # Convert to float before returning
    times.append(list(map(float, openMP)))
    times.append(list(map(float, thread)))
#   return the results of the computation time for OpenMP and MultPThread
    return times


def replace(file, searchExp, replaceExp):
    for line in fileinput.input(file, inplace=1):
        line = line.replace(searchExp, replaceExp)
        sys.stdout.write(line)

# Driver function for subprocess and plotting


def main():
    #   matrix sizes from 10 to 100 in increments of 10
    args = range(10, 100, 10)
#   number of threads where 1 corresponds to serial programming
    thread = (os.cpu_count())

    threads = [1, thread]
# Run the computation functions that make use of the subprocess to run the c++ script passing in the appropriate arguements
    first_file = "Matrix.cpp"
    first_text = "#pragma omp parallel for num_threads(2) collapse(2) private(i,j,k) shared(result)"

    second_file = "Lab1.cpp"
    second_text = "#pragma omp parallel for schedule(static) num_threads(NUM_THREADS)"

    subs = "// #pragma"


#   replace operative
    replace(first_file, subs, first_text)
    replace(second_file, subs, second_text)


#   parallel programming
    paralell_time = run(args, threads[1])


#   calling the replace method
    replace(first_file, first_text, subs)
    replace(second_file, second_text, subs)
#   serial programming
    serial_times = run(args, threads[0])
#   replace operative
    replace(first_file, subs, first_text)
    replace(second_file, subs, second_text)


################OpenMP Plot::BEGIN#########################
    # Plot it with plt
    plt.xlabel('Dimensions of Matrix')
    plt.ylabel('Time to compute rank 3 Tensor Contraction  (seconds)')
    plt.title(
        'Serial vs. Parallel Programming Time for Rank 3 Tensor Contraction (OpenMP)')

    plt.plot(args, serial_times[0], 'p--', label='Serial Programming')
    plt.plot(args, paralell_time[0], 'm--', label='Parallel Programming')

    plt.legend()

    # save the plot as a PNG image
    plt.savefig('../figures/OpenMP.png')

    # show the plt plot window
    plt.show()

################OpenMP Plot::END#########################


################Threads Plot::BEGIN#########################

    # Plot it with plt
    plt.xlabel('Dimensions of Matrix')
    plt.ylabel('Time to compute rank 3 Tensor Contraction  (seconds)')
    plt.title(
        'Serial vs. Parallel Programming Time for Rank 3 Tensor Contraction (Thread)')

    plt.plot(args, np.squeeze(serial_times[1]),
             'g--', label='Serial Programming')
    plt.plot(args, np.squeeze(
        paralell_time[1]), 'r--', label='Parallel Programming')

    plt.legend()

    # save the plot as a PNG image
    plt.savefig('../figures/Threads.png')

    # show the plt plot window
    plt.show()


################Threads Plot::END#########################
# Driver function
if __name__ == "__main__":
    main()
