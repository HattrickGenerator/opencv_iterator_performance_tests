# opencv_iterator_performance_tests
Project using google benchmark to evaluate hypothesis of iterators of opencv matrix class, that are not optimized away (violate zero overhead principle)

The new goal of this project is to evaluate strategies for dealing with the opencv iterators, that I couldn't make any faster. Fort his I used template meta programming to forward either pointers or the iterators to an stl algorithm, depending on the underlying matrices being continuous or not.
