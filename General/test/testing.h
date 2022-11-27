///    @file testing.h
///    @author Etienne Malaboeuf
///    @brief Simple testing tools are only what's needed for pilgrim
///    @version 0.1
///    @date 2021-08-31
///
///    Copyright 2021 XXX XXX

#ifndef TESTING_H
#define TESTING_H

#include <cstddef>
#include <cstdint>

//////////////////////////////////////
/// @def DOXYGEN_IS_DOCUMENTING
///
/// Use this macro to test if you should define an empty macro just for the doc generation
///
//////////////////////////////////////

#if defined(DOXYGEN_IS_DOCUMENTING)
    #define DOXYGEN_IS_DOCUMENTING
#endif


//////////////////////////////////////
/// @def PILG_TEST_STRINGIFY
///
/// This macro will convert the text as argument to a string litteral.
///
/// Example usage:
///
///     PILG_TEST_STRINGIFY(THIS IS A TEST);
///
/// Expands to :
///
///     "THIS IS A TEST";
///
//////////////////////////////////////

#if defined(DOXYGEN_IS_DOCUMENTING)
    #define PILG_TEST_STRINGIFY(the_arg)
#else
    #define private_PILG_TEST_STRINGIFY(the_arg) #the_arg
    #define PILG_TEST_STRINGIFY(the_arg)         private_PILG_TEST_STRINGIFY(the_arg)
#endif


//////////////////////////////////////
/// @def PILG_TEST_FAIL_COUNT
///
/// Use this macro to increment a value if the expression is evaluated to false.
///
/// Example usage:
///
///     testing::Uint32 the_errors = 0;
///     PILG_TEST_FAIL_COUNT(MyCond1, the_errors);
///     PILG_TEST_FAIL_COUNT(MyCond2, the_errors);
///     PILG_TEST_FAIL_COUNT(MyCond3, the_errors);
///     return the_errors:
///
//////////////////////////////////////

#if defined(DOXYGEN_IS_DOCUMENTING)
    #define PILG_TEST_FAIL_COUNT(Expression, the_counter)
#else
    #define PILG_TEST_FAIL_COUNT(Expression, the_counter) ((Expression) ? 0 : ++the_counter)
#endif


//////////////////////////////////////
/// @def PILG_TEST_PRESENT_RESULTS
///
/// Use this macro to produce a string representing the state of the test cases
///
/// Example usage:
///
///     testing::Uint32 the_errors = 0;
///     PILG_TEST_FAIL_COUNT(MyCond1, the_errors);
///     PILG_TEST_FAIL_COUNT(MyCond2, the_errors);
///     PILG_TEST_FAIL_COUNT(MyCond3, the_errors);
///     std::cout << PILG_TEST_PRESENT_RESULTS(the_errors) << '\n'; // Tests passed or Tests FAILED in + line / file details
///     return the_errors:
///
//////////////////////////////////////

#define PILG_TEST_PRESENT_RESULTS(the_counter) (the_counter == 0 ? "Tests passed" : "Tests FAILED in " __FILE__ ":" PILG_TEST_STRINGIFY(__LINE__))


////////////////////////////////////////////////////////////////////////////////
/// Types
////////////////////////////////////////////////////////////////////////////////

namespace testing {

    using Uint64 = ::uint64_t;
    using Uint32 = ::uint32_t;
    using Uint16 = ::uint16_t;
    using Uint8  = ::uint8_t;

    using Int64 = ::int64_t;
    using Int32 = ::int32_t;
    using Int16 = ::int16_t;
    using Int8  = ::int8_t;

    using Flo32 = float;
    using Flo64 = double;

    /// The largest structure size
    ///
    using SizeType = ::size_t;

    /// Represents the difference between 2 ptr. You can use that for indexing too or just use IndexType
    ///
    using DifferenceType = ::ptrdiff_t;

    /// Different from SizeType !! Signed variant of SizeType using 2s complement. "Think of it like that":
    /// using SizeType = unsigned SignedSizeType;
    /// using uint     = unsigned int;
    ///
    using SignedSizeType = ::ptrdiff_t;

    /// Use that to index containers : my_array[XXX as IndexType]
    ///
    using IndexType = DifferenceType;

    // Use char for strings, not Uint8/Int8
    // Use Uint8 for manipulating bytes (most of the time) and void* if possible when passing byte array to a function.


} // namespace testing

#endif
