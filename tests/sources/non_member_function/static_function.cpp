#include <gtest/gtest.h>
#include <cpp_reflection/cpp_reflection.hpp>

class some_class {
public:
    static unsigned int get_nb(void) { return 42; }

    static void do_something(const std::string&, int& nb, char, float, int nb2) { nb = nb2; }

    static int add(int nb1, int nb2) { return nb1 + nb2; }
};

REGISTER_CLASS_FUNCTIONS(some_class, (get_nb)(do_something)(add))

TEST(StaticFunction, ReturnGoodValue) {
    EXPECT_EQ(42, cpp_reflection::reflection_maker<unsigned int()>::invoke("some_class", "get_nb"));
}

TEST(StaticFunction, AcceptMultipleParams) {
    int nb = 0;

    cpp_reflection::reflection_maker<void(const std::string&, int&, char, float, int)>::invoke("some_class", "do_something",
                                                                                               std::string("hello"), nb, 'a', 1.0, 42);
    EXPECT_EQ(42, nb);
}

TEST(StaticFunction, DoExpectedJob) {
    EXPECT_EQ(42, cpp_reflection::reflection_maker<int(int, int)>::invoke("some_class", "add", 20, 22));
}

TEST(StaticFunction, UnregisteredFunction) {
    try {
        cpp_reflection::reflection_maker<void()>::invoke("some_class", "unregistered");
        FAIL() << "Expected cpp_reflection::reflection_exception";
    }
    catch (const cpp_reflection::reflection_exception& e) {
        EXPECT_EQ(e.what(), std::string("Function some_class::unregistered is not registered"));
    }
    catch (...) {
        FAIL() << "Expected cpp_reflection::reflection_exception";
    }
}

TEST(StaticFunction, UnregisteredClass) {
    try {
        cpp_reflection::reflection_maker<void()>::invoke("unregistered_class", "unregistered");
        FAIL() << "Expected cpp_reflection::reflection_exception";
    }
    catch (const cpp_reflection::reflection_exception& e) {
        EXPECT_EQ(e.what(), std::string("Class unregistered_class is not registered"));
    }
    catch (...) {
        FAIL() << "Expected cpp_reflection::reflection_exception";
    }
}


TEST(StaticFunction, WrongFunctionPrototype) {
    try {
        cpp_reflection::reflection_maker<void()>::invoke("some_class", "get_nb");
        FAIL() << "Expected cpp_reflection::reflection_exception";
    }
    catch (const cpp_reflection::reflection_exception& e) {
        EXPECT_EQ(e.what(), std::string("Invalid function signature for function some_class::get_nb"));
    }
    catch (...) {
        FAIL() << "Expected cpp_reflection::reflection_exception";
    }
}
