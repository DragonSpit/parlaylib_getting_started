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
    std::uniform_int_distribution<unsigned> dis(0, n - 1);       // generates random values in the range of 0 to N-1, where N is the size of the array
    parlay::random r(0);

    // generate random unsigned values
    auto data = parlay::tabulate(n, [&](unsigned i) {
        auto r = gen[i];
        return dis(r); });

    parlay::internal::timer t("Time");
    parlay::sequence<unsigned> input_data;
    parlay::sequence<unsigned> result;
    for (int i = 0; i < 5; i++) {
        input_data = data;
        t.start();
        merge_sort(input_data);
        t.next("merge_sort");
    }

    auto first_ten = input_data.head(10);
    std::cout << "first 10 elements: " << parlay::to_chars(first_ten) << std::endl;
}

static void benchmark_integer_sort(size_t n)
{
    parlay::random_generator gen;
    std::uniform_int_distribution<unsigned> dis(0, n - 1);       // generates random values in the range of 0 to N-1, where N is the size of the array

    parlay::random r(0);
    auto S = parlay::tabulate(n, [&](size_t i) -> unsigned {
        // return 3; });                                        // to set all elements of input array to a constant value
        return r.ith_rand(i); });

    size_t bits = sizeof(unsigned) * 8;
    auto identity = [](unsigned a) { return a; };                // create a lambda function for array index, which returns the array element indexed without any modifications to it

    parlay::internal::timer t("Time");
    parlay::sequence<unsigned> input_data;
    parlay::sequence<unsigned> result;
    for (int i = 0; i < 5; i++) {
        input_data = S;
        t.start();
        result = parlay::internal::integer_sort(parlay::make_slice(input_data), identity, bits);
        //sample_sort(input_data);
        t.next("integer_sort");
    }

    auto first_ten = result.head(10);
    //auto first_ten = input_data.head(10);     // for sample_sort
    std::cout << "first 10 elements: " << parlay::to_chars(first_ten) << std::endl;
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
