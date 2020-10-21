#include <iostream>
#include <vector>
#include <algorithm>

namespace TraitsAndPolicies
{
    namespace Step1
    {
        template <typename T>
        T accumulate(const T* begin, const T* end)
        {
            T total = T{};

            while (begin != end)
            {
                total += *begin;
                ++begin;
            }

            return total;
        }
    }

    namespace Step2
    {
        template <typename T>
        struct AccumulationTraits;

        template <>
        struct AccumulationTraits<uint8_t>
        {
            typedef unsigned int AccumulatorType;
        };

        template<>
        struct AccumulationTraits<float>
        {
            typedef double AccumulatorType;
        };

        template <typename T>
        typename AccumulationTraits<T>::AccumulatorType accumulate(const T* begin, const T* end)
        {
            using AccT = typename AccumulationTraits<T>::AccumulatorType;

            AccT total = T{};

            while (begin != end)
            {
                total += *begin;
                ++begin;
            }

            return total;
        }
    }

    namespace Step3
    {
        template<typename T> struct AccumulationTraits;

        template<>
        struct AccumulationTraits<uint8_t>
        {
        public:
            typedef int AccumulatorType;
            static constexpr AccumulatorType const zero = 0;
        };

        template<>
        struct AccumulationTraits<int>
        {
        public:
            typedef long AccumulatorType;
            static constexpr AccumulatorType zero = 0;
        };

        template <typename T>
        typename AccumulationTraits<T>::AccumulatorType accumulate (T const* begin, T const* end)
        {
            using AccT =  typename AccumulationTraits<T>::AccumulatorType;

            AccT total = AccumulationTraits<T>::zero; // wykorzystanie cechy

            while (begin != end)
            {
                total += *begin;
                ++begin;
            }

            return total;
        }
    }

    namespace Step4
    {
        template<typename T> struct AccumulationTraits;

        template<>
        struct AccumulationTraits<uint8_t>
        {
        public:
            typedef unsigned int AccumulatorType;
            static constexpr AccumulatorType const zero = 0;
        };

        template<>
        struct AccumulationTraits<int>
        {
        public:
            typedef long AccumulatorType;
            static constexpr AccumulatorType zero = 0;
        };

        template <typename T, typename Traits = AccumulationTraits<T>>
        typename Traits::AccumulatorType accumulate (T const* begin, T const* end)
        {
            using AccT =  typename Traits::AccumulatorType;

            AccT total =Traits::zero;

            while (begin != end)
            {
                total += *begin;
                ++begin;
            }

            return total;
        }

        namespace LegacyCpp03
        {
            template <typename T, typename AT = AccumulationTraits<T> >
            struct AccumulateImpl
            {
                static typename AT::AccumulatorType accumulate(const T* begin, const T* end)
                {
                    using AccT =  typename AT::AccumulatorType;

                    AccT total = AT::zero;

                    while (begin != end)
                    {
                        total += *begin;
                        ++begin;
                    }

                    return total;
                }
            };

            template <typename T>
            inline
            typename AccumulationTraits<T>::AccumulatorType accumulate(const T* begin, const T* end)
            {
                return AccumulateImpl<T>::accumulate(begin, end);
            }

            template <typename T, typename Traits>
            inline
            typename Traits::AccumulatorType accumulate(const T* begin, const T* end)
            {
                return AccumulateImpl<T, Traits>::accumulate(begin, end);
            }
        }
    }

    inline namespace Step5
    {
        template<typename T> struct AccumulationTraits;

        template<>
        struct AccumulationTraits<uint8_t>
        {
            typedef unsigned int AccumulatorType;
            static constexpr AccumulatorType const zero = 0;
        };

        template<>
        struct AccumulationTraits<int>
        {
            typedef long AccumulatorType;
            static constexpr AccumulatorType zero = 0;
        };

        struct SumPolicy
        {
            template <typename T1, typename T2>
            static void accumulate(T1& total, const T2& value)
            {
                total += value;
            }
        };

        template <typename T, typename AccumulationPolicy = SumPolicy, typename Traits = AccumulationTraits<T>>
        typename Traits::AccumulatorType accumulate (T const* begin, T const* end)
        {
            using AccT =  typename Traits::AccumulatorType;

            AccT total = Traits::zero;

            while (begin != end)
            {
                AccumulationPolicy::accumulate(total, *begin);
                ++begin;
            }

            return total;
        }
    }
}

struct MultiplyAccumulationTraits
{
    typedef unsigned int AccumulatorType;
    static constexpr AccumulatorType const zero = 1;
};

struct MultiplyPolicy
{
    template <typename T1, typename T2>
    static void accumulate(T1& total, const T2& value)
    {
        total *= value;
    }
};

int main()
{
    using namespace std;

    uint8_t data[255];

    iota(begin(data), end(data), 1);

    auto result = TraitsAndPolicies::accumulate(begin(data), end(data));

    cout << "result: " << static_cast<int>(result) << endl;
}