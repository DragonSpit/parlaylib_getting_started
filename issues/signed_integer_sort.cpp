static void benchmark_signed_integer_sort(size_t n)
{
    std::uniform_int_distribution<int> dis(INT32_MIN, INT32_MAX - 1);       // generates random values in the range of 0 to N-1, where N is the size of the array

    parlay::random r(0);
    auto S = parlay::tabulate(n, [&](size_t i) -> int {
        // return 3; });                                        // to set all elements of input array to a constant value
        return r.ith_rand(i); });
    auto S2 = parlay::tabulate(n, [&](size_t i) -> int {
        return r.ith_rand(i); });
    for (size_t i = 0; i < S.size(); i++) {                       // randomly negate array values
        if (S2[i] > (INT32_MAX / 2))
            S[i] = -S[i];
    }
    auto first_ten = S.head(10);
    std::cout << "first 10 elements of input: " << parlay::to_chars(first_ten) << std::endl;

    size_t bits = sizeof(unsigned) * 8;
    auto identity = [](int a) { return a; };                // create a lambda function for array index, which returns the array element indexed without any modifications to it

    parlay::internal::timer t("Time");
    parlay::sequence<int> input_data;
    parlay::sequence<int> input_data_ms;
    parlay::sequence<int> result;
    for (int i = 0; i < 5; i++) {
        input_data = S;
        t.start();
        result = parlay::internal::integer_sort(parlay::make_slice(input_data), identity, bits);
        //sample_sort(input_data);
        t.next("signed_integer_sort");

        // Compare result to merge sort
        input_data_ms = S;
        merge_sort(input_data_ms);
        if (input_data_ms != result)
        {
            std::cout << "Integer Sort and Merge Sort results are not equal";
            exit(1);
        }
    }

    auto first_ten2 = result.head(10);
    //auto first_ten = input_data.head(10);     // for sample_sort
    std::cout << "first 10 elements: " << parlay::to_chars(first_ten2) << std::endl;
}
