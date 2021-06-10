#include <vector>

template<
  typename T, // transition function of type (Q,A)->(Q,A,{L,R})
  typename I, // models the concept of the input alphabet, I subset A
  typename A, // models the concept of the tape alphabet
  typename Q  // models the concept of a set of states
>
struct tm
{
  struct tape
  {
    std::vector<A>;

    void set_input(std::vector<I> in)
    {

    }
  }
  
  using TapeAlphabet = A;
  using InputAlphabet = I;
  using TransitionFn = T;
  using TapePosition = int;

  Q q;
  T t;

  void run(I input)
  {
    Tape tape;
    TapePosition p = 0;
  };
};

