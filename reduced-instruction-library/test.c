
#define BOOST_PP_LIMIT_REPEAT (1024) // Configure boost header to allow up to 1024 repeats. Max allowed by header is 1024.
#include <boost/preprocessor/arithmetic/div.hpp>
#include "boost/preprocessor/repetition/repeat.hpp"
#include "boost/preprocessor/arithmetic/add.hpp"
#include "boost/preprocessor/arithmetic/sub.hpp"
#include "boost/preprocessor/arithmetic/mul.hpp"
#include "boost/preprocessor/arithmetic/mod.hpp"
#include "boost/preprocessor/arithmetic/div.hpp"

#define Fold(z, n, text)  text                       // Helper function for STRREP
#define STRREP(str, n) BOOST_PP_REPEAT(n, Fold, str) // Call 'STRREP' to repeat 'str' 'n' times

BOOST_PP_DIV(14, 5) // expands to 2
BOOST_PP_DIV(15, 5) // expands to 2
BOOST_PP_DIV(16, 5) // expands to 2





