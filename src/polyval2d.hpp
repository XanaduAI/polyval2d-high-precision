#include <boost/multiprecision/mpfr.hpp>
#include <boost/math/special_functions/gamma.hpp>

#include <iostream>
#include <span>


namespace details {
    using boost::multiprecision::mpfr_float;
    struct scoped_mpfr_precision
    {
       unsigned p;
       scoped_mpfr_precision(unsigned new_p) : p(mpfr_float::default_precision())
       {
          mpfr_float::default_precision(new_p);
       }
       ~scoped_mpfr_precision()
       {
          mpfr_float::default_precision(p);
       }
    };
}

/**
 * c: row-major matrix
 * */
void polyval2d(const std::span<const double> c, std::pair<size_t, size_t> shape_of_c, const std::span<const double> x, const std::span<const double> y, unsigned digits10, std::span<double> res) {
    using namespace boost::multiprecision;
    details::scoped_mpfr_precision scoped(digits10);

    assert(x.size() == y.size());
    assert(x.size() == res.size());
    const size_t length = x.size();

    std::vector<mpfr_float> x_mp;
    std::vector<mpfr_float> y_mp;

    x_mp.reserve(length);
    y_mp.reserve(length);
    for(const auto val: x) {
        x_mp.emplace_back(mpfr_float{val});
    }
    for(const auto val: y) {
        y_mp.emplace_back(mpfr_float{val});
    }

    for(size_t idx = 0; idx < length; idx++){
        mpfr_float this_res{0};

        for(size_t i = 0; i < shape_of_c.first; i++) {
            for(size_t j = 0; j < shape_of_c.second; j++) {
                this_res += c[i*shape_of_c.second + j]*pow(x_mp[idx], i)*pow(y_mp[idx], j);
            }
        }

        res[idx] = static_cast<double>(this_res);
    }
}
