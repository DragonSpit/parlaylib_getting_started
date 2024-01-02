// Example of parlaylib integration into VisualStudio 2022 project (Windows), as well as Linux
//

#include <iostream>
#include <string>
#include <random>

#include <parlay/io.h>
#include <parlay/primitives.h>
#include <parlay/random.h>
#include <parlay/sequence.h>

#include "mergesort.h"
#include "samplesort.h"

static void benchmark_merge_sort(size_t n)
{
    parlay::random_generator gen;
    std::uniform_int_distribution<int> dis(INT32_MIN, INT32_MAX);       // generates random values in the full range of signed integers
    parlay::random r(0);

    // generate random signed integer values
    auto data = parlay::tabulate(n, [&](int i) {
        auto r = gen[i];
        //return 4; });           // to set all elements of input array to a constant value
        //return i; });           // incrementing (pre-sorted) pattern
        return dis(r); });

    parlay::internal::timer t("Time");
    parlay::sequence<int> input_data;
    parlay::sequence<int> result;
    for (int i = 0; i < 5; i++) {
        input_data = data;
        t.start();
        merge_sort(input_data);
        t.next("merge_sort");
    }

    auto first_ten = input_data.head(10);
    auto last_ten  = input_data.tail(10);
    std::cout << "merge_sort: first 10 elements: " << parlay::to_chars(first_ten) << std::endl;
    std::cout << "merge_sort: last  10 elements: " << parlay::to_chars(last_ten)  << std::endl;
}

static void benchmark_integer_sort(size_t n)
{
    parlay::random r(0);

    size_t bits = sizeof(unsigned) * 8;
    auto identity = [](unsigned a) { return a; };                // create a lambda function for array index, which returns the array element indexed without any modifications to it

    parlay::internal::timer t("Time");
    parlay::sequence<unsigned> input_data;
    parlay::sequence<unsigned> input_data_ms;
    parlay::sequence<unsigned> result;
    for (int i = 0; i < 5; i++) {
        auto S = parlay::tabulate(n, [&](size_t i) -> unsigned {
            //return 3; });                                        // to set all elements of input array to a constant value
            //return (unsigned)i; });                              // incrementing (pre-sorted) pattern
            return r.ith_rand(i); });
        input_data = S;
        t.start();
        result = parlay::internal::integer_sort(parlay::make_slice(input_data), identity, bits);
        //sample_sort(input_data);
        t.next("integer_sort");

        // Compare result to merge sort
        input_data_ms = S;
        merge_sort(input_data_ms);
        if (input_data_ms != result)
        {
            std::cout << "Integer Sort and Merge Sort results are not equal";
            exit(1);
        }
    }

    auto first_ten = result.head(10);
    auto last_ten  = result.tail(10);
    std::cout << "integer_sort: first 10 elements: " << parlay::to_chars(first_ten) << std::endl;
    std::cout << "integer_sort: last  10 elements: " << parlay::to_chars(last_ten)  << std::endl;
}

// User provides the size of the random input array as the command line argument
int main(int argc, char* argv[])
{
    auto usage = "Usage: parlay_proj <n>";
    if (argc != 2) std::cout << usage << std::endl;
    else {
        size_t n;
        try { n = std::stoll(argv[1]); }
        catch (...) { std::cout << usage << std::endl; return 1; }

        benchmark_merge_sort(  n);
        benchmark_integer_sort(n);
    }
}
