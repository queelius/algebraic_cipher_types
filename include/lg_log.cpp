#include <iostream>

struct log_table
{
    static constexpr int pre_calc[]
    {
        8, 7, 6, 6, 5, 5, 5, 5,
        4, 4, 4, 4, 4, 4, 4, 4,
        3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1
    };

    //template <typename T>
    static int log2(int32_t x)
    {
        struct test
        {
            union
            {
                int32_t a;
                unsigned char buf[4];
            };
        };
        // convert to to byte array, put in vector<char> buf.
        test a;
        a.a = x;
        //const int len = buf.size();
        int len = 4;

        std::cout << "a.a = " << a.a << "\n";
        std::cout << "buf:\n";
        for (int i = 0; i < 4; ++i)
        {
            std::cout << "\t" << a.buf[i] << "\n";
            std::cout << "\t" << (unsigned int)a.buf[i] << "\n";
        }

        std::cout << "len * 8 = " << (len * 8) << "\n";
        std::cout << "a.buf[len - 1] = " << a.buf[len - 1] << "\n";
        std::cout << "pre_calc[...] = " << pre_calc[a.buf[len - 1]] << "\n";

        return len * 8 - pre_calc[a.buf[len - 1]] - 1;
    }
};

template <int N>
int log2(int a)
{
    struct test
    {
        union
        {
            int x;
            char bytes[N];
        };


    };

    test t;
    t.x = a;

    std::cout << "printing:\n";
    for (int i = 0; i < N; ++i)
        std::cout << (int)t.bytes[i] << "\n";
    std::cout << "done.\n";

    if (t.bytes[N-1] >= 128) return -1; // -ve bigint (invalid - cannot take log of -ve number)
    int l = 0;
    while ((t.bytes[N-1] >> l) > 0) ++l;
    return l + N*8-9;
}    

int main()
{
    std::cout << log2<4>(111) << "\n";
}