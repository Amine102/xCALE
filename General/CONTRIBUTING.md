# How to become a contributor and submit your own code

<!-- ## Contributor License Agreements
Internship or else ? -->


## Contributing A Patch

### Workflow

1. You want to add a new feature.
2. Please don't mix more than one logical change per branch, because it
   makes the history hard to follow. If you want to make a change that doesn't
   have a fit in this feature, please create a new one.
3. Also, coordinate with team members. This ensures that work isn't being 
   duplicated and communicating your plan early also generally leads to better 
   release.
4. Create a new branch from the current development branch, develop and test 
   your code changes.
5. Ensure that your code adheres to the existing coding style in the sample to 
   which you are contributing.
6. Ensure that your code has an appropriate set of unit tests which all pass 
   (thus th code must compile).
7. Submit a merge request.

### Notes On How To Manage A Branch

Common:
* The current development branch is named `septembre_2020` but this'll change.
* If you are not comfortable with git on the command line, use tools 
  (git-fork under windows, the gitlens addon under VSCode), it's better than 
  doing something you are not sure of and that can negatively impact the group.
* Always, always, pull the branch you are going to merge to before merging on 
  it. if you merge, "ma-super-feature" in "develop", pull develop first then 
  merge, otherwise you may create problems for the rest of the group.
* Check what you are going to push, for example, don't send your IDE, VSCode, 
  Vim configuration files. Don't send files that are too big (unsure ? ask).

When developing a feature:
* A feature simply represents a change in the repo, whether it's an update, a 
  typo fix or a bug fix.
* Implement a feature in an associated branch (1-2,3 features per branch not 
  10). Of course, a feature can be implemented in a multitude of commits on this
  branch but don't forget that if you don't merge you isolate yourself from the 
  rest of the group.
* Nothing prevents you from having several feature branches at any given time.
* Merge regularly on the development branch to avoid file conflicts, that is, 
  avoid developing on branches detached for too long from the development 
  branch, remember that a "developer discusses with other developers while 
  writing code" and if you don't merge, it's like saying nothing for a long 
  time).
* When you merge a feature, if this feature is finished, delete the branch used 
  to implement it, in fact the code is now merged and the feature branch is 
  therefore a duplicate.
* You don't need to push the feature branch to the server, you can keep it 
  locally, and after merging with the development branch and deleting the 
  feature branch, push the changes made the development branch to the server.




<!-- ## Code Of Conduct
TODO(Etienne M): -->


## Style

To keep the source consistent, readable and easy to merge, a rigid 
coding style has been setup, as partially defined 
[here](https://google.github.io/styleguide/cppguide.html). All commits will be 
expected to conform to the style outlined by the previous link. Use 
[.clang-format](.clang-format) 
([Quick guide](https://leimao.github.io/blog/Clang-Format-Quick-Tutorial/)) to 
check your formatting.

<!-- ## Requirements for Contributors
TODO(Etienne M): Do we need something more to be able to build the tests/usecases ? -->


## Developing Pilgrim General

### Testing Pilgrim General

Assuming you have read the `Satisfying The Dependencies` of the 
[README.md](README.md).

To make sure your changes work as intended and don't break existing
functionality, you'll want to compile and run Pilgrim General tests. For that 
you can use CMake:
```
$ mkdir -p build
$ cd build
$ cmake ..
```

To choose between building only the tests or only the usecases, you may modify 
your cmake command to be one of each:
```
$ cmake -DPILG_BUILD_TEST=ON  -DPILG_BUILD_USECASE=OFF .. # sets up Pilgrim General tests
$ cmake -DPILG_BUILD_TEST=OFF -DPILG_BUILD_USECASE=ON  .. # sets up Pilgrim General usecase
$ cmake .. # sets up Pilgrim General usecase and Pilgrim General tests
```

Make sure you have Probt and other dependencies installed.

Next, you can build Pilgrim General and all desired tests. On Unix-like, this is
usually done by:
```
$ make
```
Or to build using all your processors (faster):
```
$ make -j 8
```

To run the tests, do:
```
$ make test
```

All tests should pass, else there is a bug in the test or the app and it must be
fixed.


## Notes

- I recommend using [ninja-build](https://ninja-build.org/manual.html) to replace 
`make`, it may help understand why there is a bug introduced via a compiler 
option and it's also faster than make and provide an alternative to using 
Visual Studio on Windows.
- On Windows, prefer using clang+ninja instead of MSVC+Visual studio.
- If you are not comfortable in C++ or in programming in general, use tools to
help you. Such tools are:
    - [Clang-format](https://clang.llvm.org/docs/ClangFormat.html), it helps you produce clean, conforming code.
    - [Clang-tidy](https://clang.llvm.org/extra/clang-tidy/), a linter to help you fix potential programming errors.
    - [Include-What-You-Use](https://github.com/include-what-you-use/include-what-you-use), make sure you import .
    everything you use
- Some C++ best practices can be found [here](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines).

## Specific C++ Tips

General:
* Prefer scoped objects, don't heap-allocate (new/malloc delete/free) unnecessarily.
* Use namespaces but never use `using <namespace here>;`. Rather, explicitly declare your code in the namespace:
```C++
/// When you are a client of a code in a given namespace ("std" here)
using namespace std;            // Bad
cout << "Hellow world" << endl; // Bad

std::cout << "Hellow world" << std::endl; // Good

/// When you are developping code in a given namespace ("PILGRIM" here)
using namespace PILGRIM;                         // Bad
class MySuperClass0 { /* Implementation... */ }; // Bad
class MySuperClass1 { /* Implementation... */ }; // Bad


namespace PILGRIM {

    class MySuperClass0 { /* Implementation... */ }; // Good
    class MySuperClass1 { /* Implementation... */ }; // Good

} // namespace PILGRIM
```
* Be consistent in the way you create code. Fix yourself a set of rules, chekc 
  online.
* Give a variable a name that makes send.
* Dont write documentation to write documentation.
* Write documentation to explain an API or to explcit a topic.
* Try to produce code that do not generate compiler warnings !
* Format your code. Lint your code.

Im not going to copy parts of the 
[official C++ guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines):

Interface rules:
- I.1: Make interfaces explicit
- I.2: Avoid non-const global variables
- I.3: Avoid singletons
- I.4: Make interfaces precisely and strongly typed
- I.5: State preconditions (if any)
- I.7: State postconditions
- I.11: Never transfer ownership by a raw pointer (T*) or reference (T&) (use 
        shared ptr/unique ptr)
- I.13: Do not pass an array as a single pointer
- I.22: Avoid complex initialization of global objects
- I.23: Keep the number of function arguments low
- I.25: Prefer empty abstract classes as interfaces to class hierarchies
- I.26: If you want a cross-compiler ABI, use a C-style subset (aka, if you 
        produce a shared lib, dont do like Probt)
- I.27: For stable library ABI, consider the Pimpl idiom
- I.30: Encapsulate rule violations

Function definition rules:
- F.1: “Package” meaningful operations as carefully named functions
- F.2: A function should perform a single logical operation
- F.3: Keep functions short and simple
- F.4: If a function might have to be evaluated at compile time, declare it 
       constexpr
- F.5: If a function is very small and time-critical, declare it inline
- F.6: If your function must not throw, declare it noexcept
- F.7: For general use, take T* or T& arguments rather than smart pointers
- F.8: Prefer pure functions
- F.9: Unused parameters should be unnamed

Parameter passing expression rules:
- F.16: For “in” parameters, pass cheaply-copied types by value and others by 
        reference to const
- F.17: For “in-out” parameters, pass by reference to non-const
- F.20: For “out” output values, prefer return values to output parameters
- F.21: To return multiple “out” values, prefer returning a struct or tuple
- F.60: Prefer T* over T& when “no argument” is a valid option

Other function rules:
- F.55: Don’t use va_arg arguments
- F.56: Avoid unnecessary condition nesting

Class rules:
- C.1: Organize related data into structures (structs or classes)
- C.2: Use class if the class has an invariant; use struct if the data members 
       can vary independently
- C.4: Make a function a member only if it needs direct access to the 
       representation of a class (vs static)
- C.5: Place helper functions in the same namespace as the class they support
- C.7: Don’t define a class or enum and declare a variable of its type in the 
       same statement
- C.8: Use class rather than struct if any member is non-public
- C.9: Minimize exposure of members

Enumeration rules:
- Enum.1: Prefer enumerations over macros
- Enum.2: Use enumerations to represent sets of related named constants
- Enum.3: Prefer enum classes over “plain” enums
- Enum.4: Define operations on enumerations for safe and simple use
- Enum.5: Don’t use ALL_CAPS for enumerators
- Enum.8: Specify enumerator values only when necessary

Ressource managment:
- R.1: Manage resources automatically using resource handles and RAII (Resource 
       Acquisition Is Initialization)
- R.5: Prefer scoped objects, don’t heap-allocate unnecessarily
- R.6: Avoid non-const global variables
- R.10: Avoid malloc() and free()
- R.11: Avoid calling new and delete explicitly
- R.20: Use unique_ptr or shared_ptr to represent ownership
- R.21: Prefer unique_ptr over shared_ptr unless you need to share ownership
- R.22: Use make_shared() to make shared_ptrs
- R.23: Use make_unique() to make unique_ptrs
- R.24: Use std::weak_ptr to break cycles of shared_ptrs
- R.30: Take smart pointers as parameters only to explicitly express lifetime semantics
- R.32: Take a unique_ptr<widget> parameter to express that a function assumes ownership of a widget
- R.33: Take a unique_ptr<widget>& parameter to express that a function reseats the widget
- R.34: Take a shared_ptr<widget> parameter to express that a function is part owner
- R.35: Take a shared_ptr<widget>& parameter to express that a function might reseat the shared pointer
- R.36: Take a const shared_ptr<widget>& parameter to express that it might retain a reference count to the object ???

Declaration rules:
- ES.3: Don’t repeat yourself, avoid redundant code
- ES.8: Avoid similar-looking names
- ES.12: Do not reuse names in nested scopes
- ES.21: Don’t introduce a variable (or constant) before you need to use it
- ES.23: Prefer the {}-initializer syntax
- ES.24: Use a unique_ptr<T> to hold pointers
- ES.25: Declare an object const unless you want to modify its value later on
- ES.41: If in doubt about operator precedence, parenthesize
- ES.42: Keep use of pointers simple and straightforward
- ES.47: Use nullptr rather than 0 or NULL
- ES.50: Don’t cast away const
- ES.49: If you must use a cast, use a named cast
- ES.70: Prefer a switch-statement to an if-statement when there is a choice
- ES.71: Prefer a range-for-statement to a for-statement when there is a choice
- ES.72: Prefer a for-statement to a while-statement when there is an obvious loop variable
- ES.73: Prefer a while-statement to a for-statement when there is no obvious loop variable
- ES.74: Prefer to declare a loop variable in the initializer part of a for-statement
- ES.75: Avoid do-statements
- ES.76: Avoid goto
- ES.77: Minimize the use of break and continue in loops
- ES.78: Don’t rely on implicit fallthrough in switch statements
- ES.79: Use default to handle common cases (only)
- ES.87: Don’t add redundant == or != to conditions
- ES.100: Don’t mix signed and unsigned arithmetic
- ES.101: Use unsigned types for bit manipulation
- ES.102: Use signed types for arithmetic
- ES.103: Don’t overflow
- ES.104: Don’t underflow
- ES.105: Don’t divide by integer zero

Error rules:
- E.6: Use RAII to prevent leaks
- E.7: State your preconditions
- E.8: State your postconditions
- E.12: Use noexcept when exiting a function because of a throw is impossible or unacceptable
- E.13: Never throw while being the direct owner of an object
- E.16: Destructors, deallocation, and swap must never fail
- E.17: Don’t try to catch every error/exception in every function

Template rules:
TLDR, if you dont use C++ regularly, avoid them, very powerfull, very complex,
very handly but when well made/used ! You may use some if know what you are doing.
