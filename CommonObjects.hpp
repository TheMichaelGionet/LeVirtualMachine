#ifndef VM_Common_Objects
#define VM_Common_Objects

template <typename T1, typename T2>
class Tuple2
{
    public:
        T1 E1;
        T2 E2;
};

template <typename T1, typename T2, typename T3>
class Tuple3
{
    public:
        T1 E1;
        T2 E2;
        T3 E3;
};

template <typename T1, typename T2, typename T3, typename T4>
class Tuple4
{
    public:
        T1 E1;
        T2 E2;
        T3 E3;
        T4 E4;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5>
class Tuple5
{
    public:
        T1 E1;
        T2 E2;
        T3 E3;
        T4 E4;
        T5 E5;
};

















#endif
