#include "src/rdp.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(crdp, m) {
  m.doc() = "C++ implementation of RDP (Ramer–Douglas–Peucker algorithm).";
  m.def("rdp", &rdp,
        "A function that simplify list of points using RDP algorithm.");
}
