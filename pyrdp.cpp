#include "src/rdp.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(cxxrdp, m) {
  m.doc() = "C++ implementation of RDP (Ramer–Douglas–Peucker algorithm).";
  m.def("rdp", &rdp,
        "A function that simplify list of points using RDP algorithm.",
        py::arg("points"),
        py::arg("epsilon"),
        py::arg("max_thread") = 1,
        py::arg("max_point_per_thread") = 10000);
}
