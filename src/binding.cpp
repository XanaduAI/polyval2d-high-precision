#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include "polyval2d.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_polyval2d_binding, m) {
    m.def("polyval2d", [](
                py::array_t<double, py::array::c_style | py::array::forcecast> x,
                py::array_t<double, py::array::c_style | py::array::forcecast> y,
                py::array_t<double, py::array::c_style | py::array::forcecast> c,
                unsigned int digits10) {
            py::buffer_info c_buf = c.request();
            py::buffer_info x_buf = x.request();
            py::buffer_info y_buf = y.request();

            if(c_buf.ndim != 2) {
            throw std::invalid_argument("c must be a two-dimensional array");
            }
            if (x_buf.size != y_buf.size) {
            throw std::invalid_argument("x and b must have the same size");
            }
            py::array_t<double> res(x_buf.size);
            py::buffer_info res_buf = res.request();

            polyval2d(
                    std::span<double>(static_cast<double*>(c_buf.ptr), static_cast<size_t>(c_buf.size)),
                    {c_buf.shape[0], c_buf.shape[1]},
                    std::span<double>(static_cast<double*>(x_buf.ptr), static_cast<size_t>(x_buf.size)),
                    std::span<double>(static_cast<double*>(y_buf.ptr), static_cast<size_t>(y_buf.size)),
                    digits10,
                    std::span<double>(static_cast<double*>(res_buf.ptr), static_cast<size_t>(res_buf.size))
                    );

            return res;
    });
}
