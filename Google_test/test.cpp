#include <gtest/gtest.h>
#include <memory>
#include "../main.cpp"

class HashTableTest : public ::testing::Test {
protected:
    HashTable* allStudentsTable;
    std::shared_ptr<Student> student;
    std::string key;

    void SetUp() override {
        allStudentsTable = new HashTable();
        student = std::make_shared<Student>(Student{ "Doe", "J.", {90, 85, 80, 95, 88} }); // Create a sample student
        key = "doej"; // Define the key for the student
    }

    void TearDown() override {
        delete allStudentsTable;
    }
};

TEST_F(HashTableTest, TestInsertAndSearch) {
    // Insert the student into the main table
    allStudentsTable->insert(key, student);

    // Search for the student using the key
    std::shared_ptr<Student> foundStudent = allStudentsTable->search(key);

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

