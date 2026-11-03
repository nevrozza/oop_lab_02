#include "str/string_class.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(StringTest, DefaultIsEmpty) {
    String s;
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0u);
    EXPECT_EQ(s.capacity(), 1u);
    EXPECT_STREQ(s.c_str(), "");
}

TEST(StringTest, ConstructsFromCString) {
    String s("hello");
    EXPECT_EQ(s.size(), 5u);
    EXPECT_STREQ(s.c_str(), "hello");
}

TEST(StringTest, PushBackDoublesCapacity) {
    String s;
    s.push_back('a');
    EXPECT_EQ(s.capacity(), 1u);
    s.push_back('b');
    EXPECT_EQ(s.capacity(), 2u);
    s.push_back('c');
    EXPECT_EQ(s.capacity(), 4u);
    EXPECT_STREQ(s.c_str(), "abc");
}

TEST(StringTest, CopyIsIndependent) {
    String original("hi");
    String copy(original);
    original.push_back('!');
    EXPECT_STREQ(copy.c_str(), "hi");
}

TEST(StringTest, CopyAssignmentIsIndependent) {
    String a("old");
    String b("new");
    a = b;
    b.push_back('!');
    EXPECT_STREQ(a.c_str(), "new");
}

TEST(StringTest, SelfAssignment) {
    String s("hello");
    const String &alias = s;
    s = alias;
    EXPECT_STREQ(s.c_str(), "hello");
}

TEST(StringTest, MoveConstructorAndSourceReuse) {
    String source("hello");
    String dest(std::move(source));
    EXPECT_STREQ(dest.c_str(), "hello");
    EXPECT_TRUE(source.empty());
    EXPECT_STREQ(source.c_str(), "");
    source.append("again");
    EXPECT_STREQ(source.c_str(), "again");
}

TEST(StringTest, MoveAssignment) {
    String source("new");
    String dest("old");
    dest = std::move(source);
    EXPECT_STREQ(dest.c_str(), "new");
    EXPECT_TRUE(source.empty());
}

TEST(StringTest, AppendWithEmptyOperand) {
    String a("abc");
    String b;
    a.append(b);
    EXPECT_STREQ(a.c_str(), "abc");
    b.append(a);
    EXPECT_STREQ(b.c_str(), "abc");
}

TEST(StringTest, AppendOwnBuffer) {
    String s("abc");
    s.append(s);
    EXPECT_STREQ(s.c_str(), "abcabc");
    s.append(s.c_str() + 5);
    EXPECT_STREQ(s.c_str(), "abcabcc");
}

TEST(StringTest, FindSubstring) {
    String s("Hello, World!");
    EXPECT_EQ(s.find("World"), 7u);
    EXPECT_EQ(s.find(""), 0u);
    EXPECT_EQ(s.find("abc"), String::npos);
}

TEST(StringTest, Print) {
    std::ostringstream out;
    String("hello").print(out);
    EXPECT_EQ(out.str(), "hello");
}

TEST(StringTest, HistogramCountsInFirstAppearanceOrder) {
    String s("a bb a c a");
    String words[3];
    std::size_t counts[3];
    std::size_t size;
    s.word_histogram(words, counts, size);
    ASSERT_EQ(size, 3u);
    EXPECT_STREQ(words[0].c_str(), "a");
    EXPECT_STREQ(words[1].c_str(), "bb");
    EXPECT_STREQ(words[2].c_str(), "c");
    EXPECT_EQ(counts[0], 3u);
    EXPECT_EQ(counts[1], 1u);
    EXPECT_EQ(counts[2], 1u);
}

TEST(StringTest, HistogramSeparatesPunctuationAndWhitespace) {
    String s("  cat,\tcat!dog\nCAT.");
    String words[3];
    std::size_t counts[3];
    std::size_t size;
    s.word_histogram(words, counts, size);
    ASSERT_EQ(size, 3u);
    EXPECT_STREQ(words[0].c_str(), "cat");
    EXPECT_STREQ(words[1].c_str(), "dog");
    EXPECT_STREQ(words[2].c_str(), "CAT");
    EXPECT_EQ(counts[0], 2u);
    EXPECT_EQ(counts[1], 1u);
    EXPECT_EQ(counts[2], 1u);
}

TEST(StringTest, HistogramEmptyAndSingleWord) {
    std::size_t size = 10;
    String().word_histogram(nullptr, nullptr, size);
    EXPECT_EQ(size, 0u);
    String(" , ! ").word_histogram(nullptr, nullptr, size);
    EXPECT_EQ(size, 0u);

    String word;
    std::size_t count;
    String("a").word_histogram(&word, &count, size);
    ASSERT_EQ(size, 1u);
    EXPECT_STREQ(word.c_str(), "a");
    EXPECT_EQ(count, 1u);
}
