#include <gtest/gtest.h>
#include "../main.cpp"

class TrieTest : public ::testing::Test {
protected:
    Trie* allStudentsTrie;
    Trie* allFivesTrie;
    Trie* anyThreesTrie;
    Trie* anyTwosTrie;
    Student* student;
    std::string key;

    void SetUp() override {
        allStudentsTrie = new Trie();
        allFivesTrie = new Trie();
        anyThreesTrie = new Trie();
        anyTwosTrie = new Trie();
        student = new Student{ "Doe", "J.", {90, 85, 80, 95, 88} }; // Create a sample student
        key = "doej"; // Define the key for the student
        allStudentsTrie->insert(key, student); // Insert the student into the main trie
    }

    void TearDown() override {
        delete allStudentsTrie;
        delete allFivesTrie;
        delete anyThreesTrie;
        delete anyTwosTrie;
        delete student;
    }
};

TEST_F(TrieTest, TestInsertAndSearch) {
    // Search for the student using the key
    Student* foundStudent = allStudentsTrie->search(key);

    // Check if the searched student is not null
    ASSERT_NE(foundStudent, nullptr);

    // Check if the found student's details match the original student
    EXPECT_EQ(foundStudent->surname, "Doe");
    EXPECT_EQ(foundStudent->initials, "J.");
    EXPECT_EQ(foundStudent->grades[0], 90);
    EXPECT_EQ(foundStudent->grades[1], 85);
    EXPECT_EQ(foundStudent->grades[2], 80);
    EXPECT_EQ(foundStudent->grades[3], 95);
    EXPECT_EQ(foundStudent->grades[4], 88);
}

TEST_F(TrieTest, TestRemove) {
    // Remove the student using the key
    allStudentsTrie->remove(key);

    // Search for the student again
    Student* foundStudent = allStudentsTrie->search(key);

    // Check if the searched student is null
    ASSERT_EQ(foundStudent, nullptr);
}

TEST_F(TrieTest, TestInsertAllFives) {
    Student* fivesStudent = new Student{ "Smith", "A.", {5, 5, 5, 5, 5} };
    std::string fivesKey = "smitha";
    allStudentsTrie->insert(fivesKey, fivesStudent);
    allFivesTrie->insert(fivesKey, fivesStudent);

    // Search in allFivesTrie
    Student* foundStudent = allFivesTrie->search(fivesKey);

    // Check if the searched student is not null
    ASSERT_NE(foundStudent, nullptr);

    // Check if the found student's details match the original student
    EXPECT_EQ(foundStudent->surname, "Smith");
    EXPECT_EQ(foundStudent->initials, "A.");
    EXPECT_EQ(foundStudent->grades[0], 5);
    EXPECT_EQ(foundStudent->grades[1], 5);
    EXPECT_EQ(foundStudent->grades[2], 5);
    EXPECT_EQ(foundStudent->grades[3], 5);
    EXPECT_EQ(foundStudent->grades[4], 5);

    delete fivesStudent; // Clean up
}

TEST_F(TrieTest, TestInsertAnyThrees) {
    Student* threesStudent = new Student{ "Brown", "B.", {3, 4, 3, 4, 3} };
    std::string threesKey = "brownb";
    allStudentsTrie->insert(threesKey, threesStudent);
    anyThreesTrie->insert(threesKey, threesStudent);

    // Search in anyThreesTrie
    Student* foundStudent = anyThreesTrie->search(threesKey);

    // Check if the searched student is not null
    ASSERT_NE(foundStudent, nullptr);

    // Check if the found student's details match the original student
    EXPECT_EQ(foundStudent->surname, "Brown");
    EXPECT_EQ(foundStudent->initials, "B.");
    EXPECT_EQ(foundStudent->grades[0], 3);
    EXPECT_EQ(foundStudent->grades[1], 4);
    EXPECT_EQ(foundStudent->grades[2], 3);
    EXPECT_EQ(foundStudent->grades[3], 4);
    EXPECT_EQ(foundStudent->grades[4], 3);

    delete threesStudent; // Clean up
}

TEST_F(TrieTest, TestInsertAnyTwos) {
    Student* twosStudent = new Student{ "White", "C.", {2, 4, 2, 4, 2} };
    std::string twosKey = "whitec";
    allStudentsTrie->insert(twosKey, twosStudent);

    // Search in allStudentsTrie
    Student* foundStudent = allStudentsTrie->search(twosKey);

    // Check if the student is excluded from the main list (should be null)
    ASSERT_EQ(foundStudent, nullptr);

    delete twosStudent; // Clean up
}
